#include <MD_AD9833.h>
#include <MD_AD9833_lib.h>
#include <Wire.h>
#include "Adafruit_DRV2605.h"
#include <MD_AD9833.h>
#include <SPI.h>

#define DATA  11      // SPI Data/MOSI pin number.  Meg 2560 uses pin 51
#define CLK   13      // SPI Clock pin number.  Mega 2560 uses pin 52
#define FSYNC 10      // SPI Load/SS pin number.  Mega 2560 uses pin 53

Adafruit_DRV2605 drv;
MD_AD9833 AD(FSYNC);  // Hardware SPI
MD_AD9833::mode_t mode;
float freq_Value = 0;  // la variable freq_Value va a almacenar el valor de frecuencia proveniente del mapeo en frecuencias de cada dato
void SetearModo(void); //para setear el modo (seno, triangular, cuadrada) - ver mas abajo en el codigo
//void ModoOFF(void); //para apagar el generador
void SetearFrecuencia(void); //para setear la frecuencia de la señal una vez que se eligió el modo
  
void setup() {
  Serial.begin(9600);
  Serial.println("DRV2605 Audio responsive test");
  drv.begin();

  drv.setMode(DRV2605_MODE_PWMANALOG); //actuador entra en modo analogico 
  drv.writeRegister8(DRV2605_REG_CONTROL3, 0xA3); //pone a 1 el bit n_pwm_analog
   Serial.begin(9600);    //inicia comunicación con Arduino
   AD.begin(); //inicializacion del modulo AD9833
   SetearModo(); //funcion para setear el modo de la señal (triangular, seno, cuadrada)

}

void loop() {
  if (Serial.available())
  {
    String data = Serial.readStringUntil('\n');
    freq_Value = data.toFloat();
    SetearFrecuencia(); 
  //delay(100);
  }
}

void SetearModo(void) //acá setea los distintos modos -- elegir uno y comentar los otros
{
  //mode = MD_AD9833::MODE_SINE; 
  //mode = MD_AD9833::MODE_TRIANGLE;
  mode = MD_AD9833::MODE_SQUARE1; //en este caso, se eligió el modo de onda cuadrada
  //mode = MD_AD9833::MODE_OFF;
  AD.setMode(mode);
}
//  void ModoOFF(void)
//{
  //mode = MD_AD9833::MODE_OFF;
  //AD.setMode(mode);
//}

void SetearFrecuencia(void) //se  setea la/s frecuencia/s elegida/s
{  
  if (freq_Value != 0 && freq_Value != 1000)
  {
    if (freq_Value > 12500000) freq_Value = 12500000;
    AD.setFrequency(0, freq_Value);
  }
  else{
    if (freq_Value == 0){
      mode = MD_AD9833::MODE_OFF;
      AD.setMode(mode);
    }
    if (freq_Value == 1000){
      mode = MD_AD9833::MODE_SQUARE1;
      AD.setMode(mode);
    }
   
  }
}