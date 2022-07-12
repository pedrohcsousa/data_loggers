#include <OneWire.h>
#include <DallasTemperature.h>

const int azul = 11;
const int verde = 8;
const int vermelho = 10;

const int azul2 = 7;
const int verde2 = 6;
const int vermelho2 = 5;

float cor;
float temperatura;
float PH;

namespace pin {
const byte tds_sensor = A1;
const byte one_wire_bus = A0; // Dallas Temperature Sensor
}

namespace device {
float aref = 4.3;
}
 
namespace sensor {
float ec = 0;
unsigned int tds = 0;
float waterTemp = 0;
float ecCalibration = 1;
}
 
OneWire oneWire(pin::one_wire_bus);
DallasTemperature dallasTemperature(&oneWire);
 
 
void setup() {
  Serial.begin(115200); // Dubugging on hardware Serial 0
  dallasTemperature.begin();
  pinMode(azul, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(vermelho, OUTPUT);
  pinMode(azul2, OUTPUT);
  pinMode(verde2, OUTPUT);
  pinMode(vermelho2, OUTPUT);
}

void loop() {
  readTdsQuick();
  if(cor <= 100){
    azulFuncao();
  }
  if(cor > 100 && cor <= 250 ){
    verdeFuncao();
  }
  if(cor > 250 && cor <= 400){
    amareloFuncao();
  }
  if(cor > 400){
    vermelhoFuncao();
  }
  
  if(temperatura < 25){
    PH = -1.509*0.00001*cor + 7.148;
  }
  else if(temperatura < 30){
    PH = 0.2216*0.001*cor + 7.145;
  }
  else{
    PH = 0.0010357*cor + 4.753676262072547;
  }

  if(PH < 7){
    roxoFuncao();
  }
  else{
    brancoFuncao();
  }
  delay(1000);
}
void readTdsQuick() {
  dallasTemperature.requestTemperatures();
  sensor::waterTemp = dallasTemperature.getTempCByIndex(0);
  float rawEc = analogRead(pin::tds_sensor) * device::aref / 1024.0; // leia o valor analógico mais estável pelo algoritmo de filtragem mediana e converta em valor de tensão
  float temperatureCoefficient = 1.0 + 0.02 * (sensor::waterTemp - 25.0); // fórmula de compensação de temperatura: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
  sensor::ec = (rawEc / temperatureCoefficient) * sensor::ecCalibration; // compensação de temperatura e calibração
  sensor::tds = (133.42 * pow(sensor::ec, 3) - 255.86 * sensor::ec * sensor::ec + 857.39 * sensor::ec) * 0.5; //converter valor de tensão para valor tds
  cor = sensor::tds;
  temperatura = sensor::ec;
}

void vermelhoFuncao(){
  digitalWrite(azul, LOW);
  digitalWrite(verde, LOW);
  digitalWrite(vermelho, HIGH);
}
void azulFuncao(){
  digitalWrite(azul, HIGH);
  digitalWrite(verde, LOW);
  digitalWrite(vermelho, LOW);
}
void verdeFuncao(){
  digitalWrite(azul, LOW);
  digitalWrite(verde, HIGH);
  digitalWrite(vermelho, LOW);
}
void amareloFuncao(){
  digitalWrite(azul, LOW);
  digitalWrite(verde, HIGH);
  digitalWrite(vermelho, HIGH);
}
void roxoFuncao(){
  analogWrite(azul2, 128);
  analogWrite(verde2, 0);
  analogWrite(vermelho2, 128);
}
void brancoFuncao(){
  digitalWrite(azul2, HIGH);
  digitalWrite(verde2, HIGH);
  digitalWrite(vermelho2, HIGH);
}

// cor = sensor::tds;
// temperatura = sensor::ec;
