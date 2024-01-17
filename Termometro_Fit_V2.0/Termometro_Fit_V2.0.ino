/***************************************\
 **   Projeto Termometro Automotivo    **
 *                                      *
 **     Sensor Crepuscular e XBee      **
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
const int ledb2 = 11;
const int brilho = 9;
const int LDR = A3;
const int farol = 2;
const int LM351 = A1;
const int LM352 = A2;

// termometro
float adclido = 0;
int lertemperatura = 1;
float temperaturas [102];
byte temperatura = 0;
int tempoleitura = -1;

// Controle Display
byte unidade = 0;
byte dezena = 0;
byte ldrvalor = 0;
byte pwm = 255;
byte ledpwm = 5;
int digito[] = {
  0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1};
byte indice = 0;
byte tela = 3;
unsigned int segundo1 = 10;
unsigned int segundo2 = 10;
unsigned int segundo = 0;
unsigned long UtlTime;
boolean farolauto = 1;

void setup(){

  // definindo comunicacao serial
  Serial.begin(9600);
  irrecv.enableIRIn();
  analogWrite(ledg, 10);


  // especificando tensao de referencia das portas analogicas para o LM35
  analogReference(INTERNAL); //Se estiver usando Arduino Mega, use INTERNAL1V1

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
  pinMode(ledb2, OUTPUT);
  pinMode(ledg, OUTPUT);
  pinMode(brilho, OUTPUT);
  pinMode(farol, OUTPUT);

  unidade = 8;
  dezena = 8;

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
  delay(3000);

}
void loop(){

  if(millis()-UtlTime<0) {     
    UtlTime=millis(); 
  }   
  else {     
    segundo=int((millis()-UtlTime)/1000);
  } 


  sensorluz();
  if (farolauto == 1) {
    analogWrite(ledg, ledpwm);
  }
  if (farolauto == 0) {
    analogWrite(ledg, 0);
  }
              analogWrite(ledb2, 0);
              
  if (irrecv.decode(&results)) {

    if (results.value == 0xE12440BF){
      tela = 1;
                analogWrite(ledb2, 255);
    }
    if (results.value == 0xE12428D7){
      tela = 2;  
                analogWrite(ledb2, 255);
    }
    if (results.value == 0xE1246897){
      tela = 3; 
      segundo1 = segundo + 10;
      segundo2 = segundo + 20; 
                analogWrite(ledb2, 255);
    }

    if (results.value == 0xE12448B7){
      farolauto = 1;
                analogWrite(ledb2, 255);
    }
    if (results.value == 0xE124609F){
      farolauto = 0;
                analogWrite(ledb2, 255);
    }

    irrecv.resume();

  }

  if (tela == 1) {
    // Indo para rotina do termometro 1
    analogWrite(ledb, ledpwm + 13);
    analogWrite(ledr, LOW);
    adclido = analogRead(LM351);
    termometro();  

    // impressao dos dados no display
    impressao();   
  }

  if (tela == 2) {
    // Indo para rotina do termometro 2
    analogWrite(ledr, ledpwm + 10);
    analogWrite(ledb, LOW);
    adclido = analogRead(LM352);
    termometro();  

    // impressao dos dados no display
    impressao();   
  }  


  if (tela == 3 && segundo < segundo1 && segundo < segundo2) {
    // Indo para rotina do termometro 1
    analogWrite(ledb, ledpwm + 13);
    analogWrite(ledr, LOW);
    adclido = analogRead(LM351);
    termometro();  

    // impressao dos dados no display
    impressao();   
  }  
  if (tela == 3 && segundo > segundo1 && segundo < segundo2) {
    // Indo para rotina do termometro 2
    analogWrite(ledr, ledpwm + 10);
    analogWrite(ledb, LOW);
    adclido = analogRead(LM352);
    termometro();  

    // impressao dos dados no display
    impressao();   
  }  

  if (tela == 3 && segundo > segundo1 && segundo > segundo2) {
    segundo1 = segundo + 10;
    segundo2 = segundo + 20;
  } 
 
lertemperatura++;
  
//Serial.print(segundo);
//Serial.print(" ");
//Serial.print(tempoleitura);
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
//Serial.print(" LDR: ");
//Serial.print(ldrvalor);
//Serial.println("  ");
//delay(200);
}

void termometro() { // Leitura do termometro
  
    if (lertemperatura > 99) lertemperatura = 0;

//    temperaturas[lertemperatura] = (adclido * 0.4887585532746823) - 0.5;
    temperaturas[lertemperatura] = (adclido * ((1.1/1023)*100)) - 0.5;

    temperaturas[101]  = ( temperaturas[0]   + temperaturas[1]  + temperaturas[2]
      + temperaturas[3]  + temperaturas[4]  + temperaturas[5]  + temperaturas[6]
      + temperaturas[7]  + temperaturas[8]  + temperaturas[9]  + temperaturas[10]
      + temperaturas[11] + temperaturas[12] + temperaturas[13] + temperaturas[14]
      + temperaturas[15] + temperaturas[16] + temperaturas[17] + temperaturas[18]
      + temperaturas[19] + temperaturas[20] + temperaturas[21] + temperaturas[22]
      + temperaturas[23] + temperaturas[24] + temperaturas[25] + temperaturas[26]
      + temperaturas[27] + temperaturas[28] + temperaturas[29] + temperaturas[30]
      + temperaturas[31] + temperaturas[32] + temperaturas[33] + temperaturas[34]      
      + temperaturas[35] + temperaturas[36] + temperaturas[37] + temperaturas[38]      
      + temperaturas[39] + temperaturas[40] + temperaturas[41] + temperaturas[42]
      + temperaturas[43] + temperaturas[44] + temperaturas[45] + temperaturas[46]
      + temperaturas[47] + temperaturas[48] + temperaturas[49] + temperaturas[50]
      + temperaturas[51] + temperaturas[52] + temperaturas[53] + temperaturas[54]      
      + temperaturas[55] + temperaturas[56] + temperaturas[57] + temperaturas[58]      
      + temperaturas[59] + temperaturas[60] + temperaturas[61] + temperaturas[62]
      + temperaturas[63] + temperaturas[64] + temperaturas[65] + temperaturas[66]
      + temperaturas[67] + temperaturas[68] + temperaturas[69] 
      + temperaturas[70]
      + temperaturas[71] + temperaturas[72] + temperaturas[73] + temperaturas[74]
      + temperaturas[75] + temperaturas[76] + temperaturas[77] + temperaturas[78]
      + temperaturas[79] + temperaturas[80] + temperaturas[81] + temperaturas[82]
      + temperaturas[83] + temperaturas[84] + temperaturas[85] + temperaturas[86]
      + temperaturas[87] + temperaturas[88] + temperaturas[89] + temperaturas[90]
      + temperaturas[91] + temperaturas[92] + temperaturas[93] + temperaturas[94]      
      + temperaturas[95] + temperaturas[96] + temperaturas[97] + temperaturas[98]      
      + temperaturas[99]) / 100;

  temperatura = temperaturas[101];
}

void impressao() { // impressao dos dados no Display

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
  if (ldrvalor < 190) {
    pwm = 210;
    ledpwm = 1;
    analogWrite(brilho, pwm); 
    if (farolauto == 1) {
      digitalWrite(farol, HIGH); 
    }
    if (farolauto == 0) {
      digitalWrite(farol, LOW); 
    }
  }
  else {
    pwm = 0;
    ledpwm = 30;
    analogWrite(brilho, pwm); 
    if (farolauto == 1) {
      digitalWrite(farol, LOW); 
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











