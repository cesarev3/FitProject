/***************************************\
 **   Projeto Termometro Automotivo    **
 *                                      *
 **     Sensor Crepuscular e XBee      **
 \***************************************/

#include <IRremote.h>
int RECV_PIN = 11;
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
const int brilho = 9;
const int LDR = A3;
const int farol = 2;
const int LM351 = A1;
const int LM352 = A2;

// termometro
float adclido1 = 0;
float temperaturac1 = 0;
int lertemperatura1 = 1;
float adclido2 = 0;
float temperaturac2 = 0;
int lertemperatura2 = 1;
float temperaturas1 [32];
float temperaturas2 [32];
byte temperatura = 0;

// Controle Display
byte unidade = 0;
byte dezena = 0;
byte ldrvalor = 0;
byte pwm = 255;
byte ledpwm = 5;
int digito[] = {
  0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1};
byte indice = 0;
byte tela = 1;

void setup(){

  // definindo comunicacao serial
  Serial.begin(9600);
  irrecv.enableIRIn();
  // especificando tensao de referencia das portas analogicas para o LM35
  //  analogReference(INTERNAL); //Se estiver usando Arduino Mega, use INTERNAL1V1

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

  /* 
   // Indo para rotina do termometro 1
   termometro1();  
   
   // impressao dos dados no display
   impressao();
   
   for (int i=0; i < 30000; i++) {
   // indo para rotina do sensor luz
   sensorluz(); 
   */
  if (irrecv.decode(&results)) {

    if (results.value == 0xE12440BF){
      tela = 1;
    }
    if (results.value == 0xE12428D7){
      tela = 2;  
    }

    if (results.value == 0xE12448B7){
      digitalWrite(ledg, HIGH);
    }
    if (results.value == 0xE124609F){
      digitalWrite (ledg, LOW);
    }

    irrecv.resume();
  }

  if (tela == 1) {
    // Indo para rotina do termometro 1
    termometro1();  

    // impressao dos dados no display
    impressao();   
  }

  if (tela == 2) {
    // Indo para rotina do termometro 1
    termometro2();  

    // impressao dos dados no display
    impressao();   
  }    



  /*  
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   }
   
   // Indo para rotina do termometro 2
   termometro2(); 
   
   // impressao dos dados no display
   impressao();
   
   for (int i=0; i < 30000; i++) {
   // indo para rotina do sensor luz
   sensorluz(); 
   }
   
   //Serial.print(" temperatura: ");
   //Serial.print(temperaturac1);
   //Serial.print(" temperatura: ");
   //Serial.print(temperaturac2);
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
   */
}

void termometro1() { // Leitura do termometro

  for (lertemperatura1 = 0; lertemperatura1 < 30; lertemperatura1++) {

    adclido1 = analogRead(LM351);
    temperaturas1[lertemperatura1] = (adclido1 * 0.4887585532746823) - 0.5;

    temperaturas1[31]  = ( temperaturas1[0]   + temperaturas1[1]  + temperaturas1[2]
      + temperaturas1[3]  + temperaturas1[4]  + temperaturas1[5]  + temperaturas1[6]
      + temperaturas1[7]  + temperaturas1[8]  + temperaturas1[9]  + temperaturas1[10]
      + temperaturas1[11] + temperaturas1[12] + temperaturas1[13] + temperaturas1[14]
      + temperaturas1[15] + temperaturas1[16] + temperaturas1[17] + temperaturas1[18]
      + temperaturas1[19] + temperaturas1[20] + temperaturas1[21] + temperaturas1[22]
      + temperaturas1[23] + temperaturas1[24] + temperaturas1[25] + temperaturas1[26]
      + temperaturas1[27] + temperaturas1[28] + temperaturas1[29]) / 30;
  }
  temperaturac1 = temperaturas1[31];

  temperatura = temperaturac1;
  analogWrite(ledb, ledpwm);
  analogWrite(ledr, LOW);
}
void termometro2() { // Leitura do termometro

  for (lertemperatura2 = 0; lertemperatura2 < 30; lertemperatura2++) {

    adclido2 = analogRead(LM352);
    temperaturas2[lertemperatura2] = (adclido2 * 0.4887585532746823) - 0.5;

    temperaturas2[31]  = ( temperaturas2[0]   + temperaturas2[1]  + temperaturas2[2]
      + temperaturas2[3]  + temperaturas2[4]  + temperaturas2[5]  + temperaturas2[6]
      + temperaturas2[7]  + temperaturas2[8]  + temperaturas2[9]  + temperaturas2[10]
      + temperaturas2[11] + temperaturas2[12] + temperaturas2[13] + temperaturas2[14]
      + temperaturas2[15] + temperaturas2[16] + temperaturas2[17] + temperaturas2[18]
      + temperaturas2[19] + temperaturas2[20] + temperaturas2[21] + temperaturas2[22]
      + temperaturas2[23] + temperaturas2[24] + temperaturas2[25] + temperaturas2[26]
      + temperaturas2[27] + temperaturas2[28] + temperaturas2[29]) / 30;
  }
  temperaturac2 = temperaturas2[31];

  temperatura = temperaturac2; 
  analogWrite(ledr, ledpwm);
  analogWrite(ledb, LOW);
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
  if (ldrvalor < 90) {
    pwm = 210;
    ledpwm = 1;
    analogWrite(brilho, pwm); 
    digitalWrite(farol, HIGH); 
  }
  else {
    pwm = 0;
    ledpwm = 30;
    analogWrite(brilho, pwm); 
    digitalWrite(farol, LOW); 
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




