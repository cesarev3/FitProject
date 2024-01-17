/***************************************\
 **   Projeto Termometro Automotivo    **
 *                                      *
 **      com Sensor Crepuscular        **
 \***************************************/

#include <IRremote.h>
int RECV_PIN = 4;
IRrecv irrecv(RECV_PIN);
decode_results results;

// enderecamento de pinos
const int a = 3;
const int b = A0;
const int c = 7;
const int d = 8;
const int a1 = A4;
const int b1 = A5;
const int c1 = 12;
const int d1 = 13;
const int ledb = 5;
const int ledr = 10;
const int ledg = 6;
const int ledr2 = 11;
const int brilho = 9;
const int LDR = A3;
const int farol = 2;
const int LM351 = A1;
const int LM352 = A2;
const int lanterna = A6;
const int acc = A7;

// termometro 1
float adclido1 = 0;
int lertemperatura1 = 1;
float temperaturas1 [102];
byte temperatura1 = 0;

// termometro 2
float adclido2 = 0;
int lertemperatura2 = 1;
float temperaturas2 [102];
byte temperatura2 = 0;

// termometro para impressao
byte temperatura = 0;

// Numeracao do Display
byte unidade = 0;
byte dezena = 0;
int digito[] = {
  0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1};
byte indice = 0;


// Iluminacao do Display
int ldrvalor = 0;
byte pwm = 255;
byte ledpwm = 4;
byte senscont = 8;
int sensvalor = 800;
boolean sensmenu = 0;
byte menu = 0;
boolean hideon = 0;

// Troca de tela temporizada
byte tela = 3;

// controle de tempo
unsigned int segundo1;
unsigned int segundo2;
unsigned int segundo3;
unsigned int segundo = 0;
unsigned long UtlTime;

// acionamento automatico do farol
boolean farolauto = 1;
boolean ledfarol = 0;
int valorlanterna = 0;
int valoracc = 0;

// Tempos em segundos
byte tempotela1 = 10;
byte tempotela2 = 20; //este tempo deve ser somado ao tempotela1. Para 10 segundos deve ser colocado tempotela1 + 10
byte delayaceso = 15; // tempo que o circuito mantem o farol baixo aceso mesmo com iluminacao no LDR

void setup(){

  // definindo comunicacao serial
  Serial.begin(9600);
  irrecv.enableIRIn();
  //  analogWrite(ledg, ledpwm);


  // especificando tensao de referencia das portas analogicas para o LM35
  analogReference(INTERNAL); //Se estiver usando Arduino Mega, use INTERNAL1V1

  // definindo os pinos digitais como saida
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(a1, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(c1, OUTPUT);
  pinMode(d1, OUTPUT);
  pinMode(ledr, OUTPUT);
  pinMode(ledb, OUTPUT);
  pinMode(ledr2, OUTPUT);
  pinMode(ledg, OUTPUT);
  pinMode(brilho, OUTPUT);
  pinMode(farol, OUTPUT);

  // definindo o numero de teste ao ligar
  unidade = 8;
  dezena = 8;

  // escrevendo a unidade
  indice = unidade * 4;
  digitalWrite(a, digito [indice + 0]);  
  digitalWrite(b, digito [indice + 1]);  
  digitalWrite(c, digito [indice + 2]);  
  digitalWrite(d, digito [indice + 3]); 

  // escrevendo a dezena
  indice = dezena * 4;
  digitalWrite(a1, digito [indice + 0]);  
  digitalWrite(b1, digito [indice + 1]);  
  digitalWrite(c1, digito [indice + 2]);  
  digitalWrite(d1, digito [indice + 3]); 

  analogWrite(ledr2, 255);
  delay(1000);
  analogWrite(ledg, 100);
  analogWrite(ledr2, 0); 
  delay(1000);
  analogWrite(ledb, 100);
  analogWrite(ledg, 0); 
  delay(1000);
  analogWrite(ledr, 100);
  analogWrite(ledb, 0); 
  delay(1000);
  analogWrite(ledr, 0); 
}

void loop(){

  // contando os segundos
  if(millis() - UtlTime < 0) {     
    UtlTime=millis(); 
  }   
  else {     
    segundo=int((millis() - UtlTime) / 1000);
  } 
  
  // verifica se na entrada ha sinal nas lanternas
  valorlanterna = analogRead(lanterna); 
 if (valorlanterna < 100) {
   farolauto = 1;
 } else
   if (valorlanterna > 500) {
   farolauto = 0;
   }
   
    // verifica se na entrada ha sinal no ACC
//  valoracc = analogRead(acc); 
// if (valoracc < 100) {
//   hideon = 1;
// } else
//   if (valoracc > 500) {
//   hideon = 0;
//   }
   
  if (hideon == 0) {  
    
    // vai para a rotina de sensor de iluminacao
    sensorluz();

    // vai para a rotina do termometro 1
    termometro1();

    // vai para a rotina do termometro 2
    termometro2();

    // Acende o Led Vermelho (Farol Auto) conforme opcao de auto ou off
    if (farolauto == 1) {
      analogWrite(ledr2, 255);
    }
    if (farolauto == 0) {
      analogWrite(ledr2, 0);
    }

    // Acende o Led Verde (Farol Ligado) conforme opcao de auto ou off
    if (ledfarol == 1) {
      analogWrite(ledg, ledpwm);
    }

    if (ledfarol == 0) {
      digitalWrite(ledg, LOW); 
    }

  } 
  else {
    digitalWrite(ledr, LOW);
    digitalWrite(ledb, LOW);
    digitalWrite(ledg, LOW);
    digitalWrite(ledr2, LOW);
    digitalWrite(farol, LOW);
    digitalWrite(brilho, HIGH);
  }

  // Decdificacao do IR
  if (irrecv.decode(&results)) { 

    if (results.value == 0xE124C837){ // Tecla DISP para ativar a rotina Hide mode
      hideon++;
      delay(50);
      if (hideon > 1) {
        hideon = 0;
      }
    }

    if (hideon == 0) {

      if (results.value == 0xE12418E7){ // Tecla SEL para sensibilidade
        menu++;
        delay(50);
        if (menu > 2) {
          menu = 1;
        }
      }
      if (results.value == 0xE124E817){ // Tecla Number para sair do menu
        menu = 0;
        delay(50);
      }

      if (menu == 0) { // le os dados se estiver na rotina normal

        if (results.value == 0xE12440BF){ // Tecla 1 para temperatura interna
          tela = 1;
          analogWrite(ledr2, 255);
        }
        if (results.value == 0xE12428D7){ // Tecla 2 para temperatura externa
          tela = 2; 
          analogWrite(ledr2, 255);
        }
        if (results.value == 0xE1246897){ // Tecla 3 para temperatura interna interna e externa
          tela = 3;
          segundo1 = segundo + tempotela1;
          segundo2 = segundo + tempotela2; 
          analogWrite(ledr2, 255);
        }

        if (results.value == 0xE12448B7){ // Tecla ON para ligar o automatico do farol
          farolauto = 1;
          analogWrite(ledr2, 255);
        }
        if (results.value == 0xE124609F){ // Tecla MUTE para desligar o automatico do farol
          farolauto = 0;
          analogWrite(ledr2, 255);
        }

      } 
      if (menu == 1) {  // le os dados se estiver na rotina sensibilidade do sensor

        if (results.value == 0xE12440BF){ // Tecla 1 para sensibilidade
          senscont = 1;
          sensvalor = 100;
        }
        if (results.value == 0xE12428D7){ // Tecla 2 para sensibilidade
          senscont = 2;
          sensvalor = 200;
        }  
        if (results.value == 0xE1246897){ // Tecla 3 para sensibilidade
          senscont = 3;
          sensvalor = 300;
        }  
        if (results.value == 0xE124B847){ // Tecla 4 para sensibilidade
          senscont = 4;
          sensvalor = 400;
        }  
        if (results.value == 0xE124F00F){ // Tecla 5 para sensibilidade
          senscont = 5;
          sensvalor = 500;
        }  
        if (results.value == 0xE1249867){ // Tecla 6 para sensibilidade
          senscont = 6;
          sensvalor = 600;
        }  
        if (results.value == 0xE1247887){ // Tecla 7 para sensibilidade
          senscont = 7;
          sensvalor = 700;
        }  
        if (results.value == 0xE124B04F){ // Tecla 8 para sensibilidade
          senscont = 8;
          sensvalor = 800;
        }  
        if (results.value == 0xE124F807){ // Tecla 9 para sensibilidade
          senscont = 9;
          sensvalor = 900;
        }
      }

      if (menu == 2) {  // le os dados se estiver na rotina delay do farol

        if (results.value == 0xE1246897){ // Tecla 3 para os mesmos segundos de delay
          delayaceso = 3;
        } 
        if (results.value == 0xE124B847){ // Tecla 4 para os mesmos segundos de delay
          delayaceso = 4;
        }  
        if (results.value == 0xE124F00F){ // Tecla 5 para os mesmos segundos de delay
          delayaceso = 5;
        }  
        if (results.value == 0xE1249867){ // Tecla 6 para os mesmos segundos de delay
          delayaceso = 6;
        }  
        if (results.value == 0xE1247887){ // Tecla 7 para os mesmos segundos de delay
          delayaceso = 7;
        }  
        if (results.value == 0xE124B04F){ // Tecla 8 para os mesmos segundos de delay
          delayaceso = 8;
        }  
        if (results.value == 0xE124F807){ // Tecla 9 para os mesmos segundos de delay
          delayaceso = 9;
        }
      } 
    }
    irrecv.resume();
  }

  if (hideon == 0) {

    if (tela == 1) { // Indo para rotina do termometro 1
      analogWrite(ledb, ledpwm + 25);
      analogWrite(ledr, LOW);
      //    adclido = analogRead(LM351);
      //    termometro();  
      temperatura = temperatura1;
      impressao(); // impressao dos dados no display
    }

    if (tela == 2) { // Indo para rotina do termometro 2
      analogWrite(ledr, ledpwm + 7);
      analogWrite(ledb, ledpwm + 25);
      //    adclido = analogRead(LM352);
      //    termometro();  
      temperatura = temperatura2;
      impressao(); // impressao dos dados no display  
    }  

    if (tela == 3 && segundo < segundo1 && segundo < segundo2) { // Indo para rotina do termometro 1
      analogWrite(ledb, ledpwm + 25);
      analogWrite(ledr, LOW);
      //    adclido = analogRead(LM351);
      //    termometro();  
      temperatura = temperatura1;
      impressao(); // impressao dos dados no display  
    }  
    if (tela == 3 && segundo > segundo1 && segundo < segundo2) { // Indo para rotina do termometro 2
      analogWrite(ledr, ledpwm + 7);
      analogWrite(ledb, ledpwm + 25);
      //    adclido = analogRead(LM352);
      //    termometro();  
      temperatura = temperatura2;    
      impressao(); // impressao dos dados no display  
    }  

    // rotina de contagem de mudanca de tela
    if (tela == 3 && segundo > segundo1 && segundo > segundo2) { 
      segundo1 = segundo + tempotela1;
      segundo2 = segundo + tempotela2;
    } 

    // incrementa o indice da leitura de temperatura
    lertemperatura1++;
    lertemperatura2++;

    if (menu == 0) { // apaga o Led vermelho do IR se estiver na rotina normal
      digitalWrite(ledr2, LOW); 
    }
  }

  //Serial.print(segundo);
  //Serial.print(" ");
  //Serial.print(" temperatura c1: ");
  //Serial.print(temperaturac1);
  //Serial.print(" temperatura: ");
  //Serial.print(temperatura);
  //Serial.print(" unidade: ");
  //Serial.print(unidade);
  //Serial.print(" dezena: ");
  //Serial.print(dezena);
  //Serial.print(" tela: ");
  //Serial.print(tela);
  //Serial.print(" contador tela: ");
  //Serial.print(contadortela);
Serial.print(" LDR: ");
Serial.print(ldrvalor);
  //Serial.print(" Valor Sens: ");
  //Serial.print(sensvalor);
  //Serial.print(" sensibilidade: ");
  //Serial.print(senscont);
  //Serial.print(" Valor Delay: ");
  //Serial.print(delayaceso);
  //Serial.print(" Menu: ");
  //Serial.print(menu);
  //Serial.print(" Hide: ");
  //Serial.print(hideon);
  //Serial.print(" IR: ");
  //Serial.print(results.value);
//Serial.print(" lanternas: ");
//Serial.print(valorlanterna);
//Serial.print(" ACC: ");
//Serial.print(valoracc);
Serial.println("  ");
delay(150);
}

void termometro1() { // Leitura do termometro

  adclido1 = analogRead(LM351);

  if (lertemperatura1 > 99) lertemperatura1 = 0;

  //    temperaturas1[lertemperatura1] = (adclido1 * 0.4887585532746823) - 0.5;
  temperaturas1[lertemperatura1] = (adclido1 * ((1.1/1023)*100)) - 0.5;

  temperaturas1[101]  = ( temperaturas1[0]   + temperaturas1[1]  + temperaturas1[2]
    + temperaturas1[3]  + temperaturas1[4]  + temperaturas1[5]  + temperaturas1[6]
    + temperaturas1[7]  + temperaturas1[8]  + temperaturas1[9]  + temperaturas1[10]
    + temperaturas1[11] + temperaturas1[12] + temperaturas1[13] + temperaturas1[14]
    + temperaturas1[15] + temperaturas1[16] + temperaturas1[17] + temperaturas1[18]
    + temperaturas1[19] + temperaturas1[20] + temperaturas1[21] + temperaturas1[22]
    + temperaturas1[23] + temperaturas1[24] + temperaturas1[25] + temperaturas1[26]
    + temperaturas1[27] + temperaturas1[28] + temperaturas1[29] + temperaturas1[30]
    + temperaturas1[31] + temperaturas1[32] + temperaturas1[33] + temperaturas1[34]      
    + temperaturas1[35] + temperaturas1[36] + temperaturas1[37] + temperaturas1[38]      
    + temperaturas1[39] + temperaturas1[40] + temperaturas1[41] + temperaturas1[42]
    + temperaturas1[43] + temperaturas1[44] + temperaturas1[45] + temperaturas1[46]
    + temperaturas1[47] + temperaturas1[48] + temperaturas1[49] + temperaturas1[50]
    + temperaturas1[51] + temperaturas1[52] + temperaturas1[53] + temperaturas1[54]      
    + temperaturas1[55] + temperaturas1[56] + temperaturas1[57] + temperaturas1[58]      
    + temperaturas1[59] + temperaturas1[60] + temperaturas1[61] + temperaturas1[62]
    + temperaturas1[63] + temperaturas1[64] + temperaturas1[65] + temperaturas1[66]
    + temperaturas1[67] + temperaturas1[68] + temperaturas1[69] + temperaturas1[70]
    + temperaturas1[71] + temperaturas1[72] + temperaturas1[73] + temperaturas1[74]
    + temperaturas1[75] + temperaturas1[76] + temperaturas1[77] + temperaturas1[78]
    + temperaturas1[79] + temperaturas1[80] + temperaturas1[81] + temperaturas1[82]
    + temperaturas1[83] + temperaturas1[84] + temperaturas1[85] + temperaturas1[86]
    + temperaturas1[87] + temperaturas1[88] + temperaturas1[89] + temperaturas1[90]
    + temperaturas1[91] + temperaturas1[92] + temperaturas1[93] + temperaturas1[94]      
    + temperaturas1[95] + temperaturas1[96] + temperaturas1[97] + temperaturas1[98]      
    + temperaturas1[99]) / 100;

  temperatura1 = temperaturas1[101];
}

void termometro2() { // Leitura do termometro

  adclido2 = analogRead(LM352);

  if (lertemperatura2 > 99) lertemperatura2 = 0;

  //    temperaturas2[lertemperatura2] = (adclido2 * 0.4887585532746823) - 0.5;
  temperaturas2[lertemperatura2] = (adclido2 * ((1.1/1023)*100)) - 0.5;

  temperaturas2[101]  = ( temperaturas2[0]   + temperaturas2[1]  + temperaturas2[2]
    + temperaturas2[3]  + temperaturas2[4]  + temperaturas2[5]  + temperaturas2[6]
    + temperaturas2[7]  + temperaturas2[8]  + temperaturas2[9]  + temperaturas2[10]
    + temperaturas2[11] + temperaturas2[12] + temperaturas2[13] + temperaturas2[14]
    + temperaturas2[15] + temperaturas2[16] + temperaturas2[17] + temperaturas2[18]
    + temperaturas2[19] + temperaturas2[20] + temperaturas2[21] + temperaturas2[22]
    + temperaturas2[23] + temperaturas2[24] + temperaturas2[25] + temperaturas2[26]
    + temperaturas2[27] + temperaturas2[28] + temperaturas2[29] + temperaturas2[30]
    + temperaturas2[31] + temperaturas2[32] + temperaturas2[33] + temperaturas2[34]      
    + temperaturas2[35] + temperaturas2[36] + temperaturas2[37] + temperaturas2[38]      
    + temperaturas2[39] + temperaturas2[40] + temperaturas2[41] + temperaturas2[42]
    + temperaturas2[43] + temperaturas2[44] + temperaturas2[45] + temperaturas2[46]
    + temperaturas2[47] + temperaturas2[48] + temperaturas2[49] + temperaturas2[50]
    + temperaturas2[51] + temperaturas2[52] + temperaturas2[53] + temperaturas2[54]      
    + temperaturas2[55] + temperaturas2[56] + temperaturas2[57] + temperaturas2[58]      
    + temperaturas2[59] + temperaturas2[60] + temperaturas2[61] + temperaturas2[62]
    + temperaturas2[63] + temperaturas2[64] + temperaturas2[65] + temperaturas2[66]
    + temperaturas2[67] + temperaturas2[68] + temperaturas2[69] + temperaturas2[70]
    + temperaturas2[71] + temperaturas2[72] + temperaturas2[73] + temperaturas2[74]
    + temperaturas2[75] + temperaturas2[76] + temperaturas2[77] + temperaturas2[78]
    + temperaturas2[79] + temperaturas2[80] + temperaturas2[81] + temperaturas2[82]
    + temperaturas2[83] + temperaturas2[84] + temperaturas2[85] + temperaturas2[86]
    + temperaturas2[87] + temperaturas2[88] + temperaturas2[89] + temperaturas2[90]
    + temperaturas2[91] + temperaturas2[92] + temperaturas2[93] + temperaturas2[94]      
    + temperaturas2[95] + temperaturas2[96] + temperaturas2[97] + temperaturas2[98]      
    + temperaturas2[99]) / 100;

  temperatura2 = temperaturas2[101];
}

void impressao() { // impressao dos dados no Display

  if (menu == 0) {
    if (temperatura > 19 && temperatura < 30) {
      unidade = temperatura - 20;
      dezena = 2;
    }
    if (temperatura > 29 && temperatura < 40) {
      unidade = temperatura - 30;
      dezena = 3;
    }
    if (temperatura > 39 && temperatura < 50) {
      unidade = temperatura - 40;
      dezena = 4;
    }
    if (temperatura > 49 && temperatura < 60) {
      unidade = temperatura - 50;
      dezena = 5;
    }
    if (temperatura > 59 && temperatura < 70) {
      unidade = temperatura - 60;
      dezena = 6;
    }
    if (temperatura > 9 && temperatura < 20) {
      unidade = temperatura - 10;
      dezena = 1;
    }
    if (temperatura > -1 && temperatura < 10) {
      unidade = temperatura;
      dezena = 10;
    }
  } 
  else {
    if (menu == 1) {
      unidade = senscont;
      dezena = 5;
    }
    else {
      if (menu == 2) {
        unidade = delayaceso;
        dezena = 7;
      }
    }
  }
  indice = unidade * 4;
  digitalWrite(a, digito [indice + 0]);  
  digitalWrite(b, digito [indice + 1]);  
  digitalWrite(c, digito [indice + 2]);  
  digitalWrite(d, digito [indice + 3]); 

  indice = dezena * 4;
  digitalWrite(a1, digito [indice + 0]);  
  digitalWrite(b1, digito [indice + 1]);  
  digitalWrite(c1, digito [indice + 2]);  
  digitalWrite(d1, digito [indice + 3]); 
}

void sensorluz() { // Rotina do sensor de luz

    ldrvalor = analogRead(LDR);
  if (ldrvalor < sensvalor) {
    segundo3 = segundo + delayaceso;
    pwm = 210;
    ledpwm = 1;
    analogWrite(brilho, pwm); // escreve brilho do display
    if (farolauto == 1) {
      digitalWrite(farol, HIGH); 
      ledfarol = 1;
    }
    if (farolauto == 0) {
      digitalWrite(farol, LOW); 
      ledfarol = 0;
    }
  }

  // if (ldrvalor > sensvalor + 50 && segundo > segundo3) {
  if (ldrvalor == 1023 && segundo > segundo3) {
    pwm = 0;
    ledpwm = 4;
    analogWrite(brilho, pwm); 
    if (farolauto == 1) {
      digitalWrite(farol, LOW);
      ledfarol = 0; 
    }
  }
}

/*
Controle remoto do Kit Avancado
 Power = E12448B7
 Mute = E124609F
 Sel = E12418E7
 Vol Up = E12458A7
 Vol Down = E124A05F
 Number = E124E817
 1 = E12440BF
 2 = E12428D7
 3 = E1246897
 4 = E124B847
 5 = E124F00F
 6 = E1249867
 7 = E1247887
 8 = E124B04F
 9 = E124F807
 0 = E12430CF
 Band = E124D827
 Disp = E124C837
 */
























