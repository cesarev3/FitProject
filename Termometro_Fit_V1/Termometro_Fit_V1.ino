/***************************************\
 ** ROBOCORE ARDUINO KIT INICIANTE **
 * *
 ** Display 7 Segmentos 2 **
 \***************************************/
const int a = 4;
const int b = A0;
const int c = 7;
const int d = 8;
const int a1 = A4;
const int b1 = A5;
const int c1 = 12;
const int d1 = 13;
const int ledb = 3;
const int ledr = 10;
const int brilho = 9;
const int LDR = A3;
const int farol = 2;

int digito[] = {
  0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1};
int indice = 0;

// termometro
float adclido1 = 0;
float temperaturac1 = 0;
int lertemperatura1 = 1;
float adclido2 = 0;
float temperaturac2 = 0;
int lertemperatura2 = 1;
float temperaturas1 [32];
float temperaturas2 [32];
const int LM351 = A1;
const int LM352 = A2;
int temperatura = 0;
int unidade = 0;
int dezena = 0;
float decimal = 0;
int tela = 0;
int contadortela = 0;
int ldrvalor = 0;
int pwm = 255;
float tempanterior1 = 0;
float tempanterior2 = 0;
int tempload = 0;
float tempteste = 0;

void setup(){

  // definindo comunicacao serial
Serial.begin(9600);

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
  pinMode(brilho, OUTPUT);
  pinMode(farol, OUTPUT);

}
void loop(){

  ldrvalor = analogRead(LDR);
  if (ldrvalor < 120) {
    pwm = 210;
    analogWrite(brilho, pwm); 
    digitalWrite(farol, HIGH); 
  }
  else {
    pwm = 0;
    analogWrite(brilho, pwm); 
    digitalWrite(farol, LOW); 
  }

  // Indo para rotina do termometro
  termometro1();  
  termometro2();  

  if (tela == 0) {
    tempteste = temperaturac1 - tempanterior1;
    if (tempteste < 0.99 || tempteste > 0.99) {
      temperatura = temperaturac1; 
      decimal = temperaturac1 - temperatura;
      if (decimal > 0.7) {
        temperatura += 1;
      }
    }
    else {
      temperatura = tempanterior1;
      decimal = tempanterior1 - temperatura;
      if (decimal > 0.7) {
        temperatura += 1;
      }
    }
    analogWrite(ledb, 10);
    analogWrite(ledr, LOW);
  }
  if (tela == 1) {
    tempteste = temperaturac2 - tempanterior2;
    if (tempteste < 0.99 || tempteste > 0.99) {
      temperatura = temperaturac2; 
      decimal = temperaturac2 - temperatura;
      if (decimal > 0.7) {
        temperatura += 1;
      }
    }
    else {
      temperatura = tempanterior2;
      decimal = tempanterior2 - temperatura;
      if (decimal > 0.7) {
        temperatura += 1;
      }
    }
    analogWrite(ledr, 10);
    analogWrite(ledb, LOW);
  }

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

  // incrementa o valor do indice da matriz em 1
  lertemperatura1++;
  lertemperatura2++;

  // incrementa o valor do contador da tela em 1
  contadortela++;
  if (contadortela > 100) {
    tela++;
    contadortela = 0;
  }
  if(tela > 1) {
    tela = 0;
  }


Serial.print(" temperatura: ");
Serial.print(temperaturac1);
Serial.print(" temperatura: ");
Serial.print(temperaturac2);
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
Serial.println("  ");

}

void termometro1() { // Leitura do termometro

  if(lertemperatura1 > 29) {
    lertemperatura1 = 0;
  }

  adclido1 = analogRead(LM351);
  // temperaturas1[lertemperatura1] = (adclido1 * ((1.1/1023)*100)) - 0.5; // equiparar multimetro com prototipo
  temperaturas1[lertemperatura1] = (adclido1 * 0.4887585532746823) - 0.5;

  temperaturas1[31]  = ( temperaturas1[0]   + temperaturas1[1]  + temperaturas1[2]
    + temperaturas1[3]  + temperaturas1[4]  + temperaturas1[5]  + temperaturas1[6]
    + temperaturas1[7]  + temperaturas1[8]  + temperaturas1[9]  + temperaturas1[10]
    + temperaturas1[11] + temperaturas1[12] + temperaturas1[13] + temperaturas1[14]
    + temperaturas1[15] + temperaturas1[16] + temperaturas1[17] + temperaturas1[18]
    + temperaturas1[19] + temperaturas1[20] + temperaturas1[21] + temperaturas1[22]
    + temperaturas1[23] + temperaturas1[24] + temperaturas1[25] + temperaturas1[26]
    + temperaturas1[27] + temperaturas1[28] + temperaturas1[29]) / 30;

  temperaturac1 = temperaturas1[31];
}

void termometro2() { // Leitura do termometro

  if(lertemperatura2 > 29) {
    lertemperatura2 = 0;
  }

  adclido2 = analogRead(LM352);
  // temperaturas2[lertemperatura2] = (adclido2 * ((1.1/1023)*100)) - 0.5; // equiparar multimetro com prototipo
  temperaturas2[lertemperatura2] = (adclido2 * 0.4887585532746823) - 0.5;

  temperaturas2[31]  = ( temperaturas2[0]   + temperaturas2[1]  + temperaturas2[2]
    + temperaturas2[3]  + temperaturas2[4]  + temperaturas2[5]  + temperaturas2[6]
    + temperaturas2[7]  + temperaturas2[8]  + temperaturas2[9]  + temperaturas2[10]
    + temperaturas2[11] + temperaturas2[12] + temperaturas2[13] + temperaturas2[14]
    + temperaturas2[15] + temperaturas2[16] + temperaturas2[17] + temperaturas2[18]
    + temperaturas2[19] + temperaturas2[20] + temperaturas2[21] + temperaturas2[22]
    + temperaturas2[23] + temperaturas2[24] + temperaturas2[25] + temperaturas2[26]
    + temperaturas2[27] + temperaturas2[28] + temperaturas2[29]) / 30;

  temperaturac2 = temperaturas2[31];
}






