#include <Arduino.h>
#include <Ticker.h>

// relation of the connector:
//
// GND (NTC)
// +5V (330R)
// GND (MOC3020)
// Sensor Out (NTC out)
// Optocoupler Input (MOC3020)
//
//
//

// ******* defines ******* //
#define TRIAC 13
#define NTC A0
#define Ntemp 20

// ******* flags ******* //
// bool ZC_flag = false;
// bool timer_flag = false;
bool onoff = false;

// ******* counters ******* //
uint8_t on_count = 0;
uint8_t off_count = 0;
uint8_t mean_count = 0;
uint8_t count = 0;

// ******* PID control vars ******* //
float set_point = 20.0; // em ºC
float erro = 0;
float integrator = 0;
float derivator = 0;
float Kp = 0.1;
float Ki = 0.01;
float Kd = 0;
float percent = 0.2;

// ******* signal vars ******* //
uint16_t AC_freq = 50;
float AC_period = 20; // millisseconds
uint8_t cycles = 100;
uint64_t time = 0, last_time = 0;

// ******* NTC sensor vars ******* //
uint16_t B = 3000;
float temp[Ntemp];
float mean = 0;
float analog_signal = 0;
float AD_value = 0;
float NTC_Res = 0;

// ******* Functions ******* //
void NCC_timer();
void PID_calc();

// ******* Timers ******* //
Ticker timer1(NCC_timer, AC_period, 0, MILLIS);
Ticker timer2(PID_calc, AC_period * 10, 0, MILLIS);

void NCC_timer()
{
  if (!onoff)
  {
    if (off_count >= (cycles - cycles * percent))
    {
      on_count = 0;
      onoff = !onoff;
      digitalWrite(TRIAC, HIGH);
      count++;
    }
    off_count++;
  }
  if (onoff)
  {
    if (on_count >= cycles * percent)
    {
      off_count = 0;
      onoff = !onoff;
      digitalWrite(TRIAC, LOW);
      count++;
    }
    on_count++;
  }
}

void PID_calc()
{
  erro = set_point - mean;
  integrator += erro;
  if (integrator > 10)
  {
    integrator = 10;
  }
  else if(integrator < -10){
    integrator = -10;
  }
  percent = Kp * erro + Ki * integrator;
  if(percent < 0)
    percent = 0;
}

void setup()
{
  pinMode(TRIAC, OUTPUT);
  pinMode(NTC, INPUT);
  Serial.begin(9600);

  timer1.start();
  timer2.start();

  digitalWrite(TRIAC, LOW);
}

void loop()
{
  if (Serial.available() > 0)
  {                                  
    // set_point = Serial.parseFloat(); 
    String dadosRecebidos = Serial.readStringUntil('\n');

    set_point = dadosRecebidos.substring(0, dadosRecebidos.indexOf(';')).toFloat();
    if(set_point > 30){
      set_point = 30;
    }

    dadosRecebidos.remove(0, dadosRecebidos.indexOf(';') + 1);

    Kp = dadosRecebidos.substring(0, dadosRecebidos.indexOf(',')).toFloat();
    dadosRecebidos.remove(0, dadosRecebidos.indexOf(';') + 1);

    Ki = dadosRecebidos.toFloat();
    
    integrator = 0;
  }

  if (mean_count > Ntemp)
  {
    Serial.print(mean / Ntemp);
    Serial.print(";");
    Serial.print(set_point);
    Serial.print(";");
    Serial.print(percent);
    Serial.print(";");
    Serial.print(integrator);
    Serial.print(";");
    Serial.println(onoff);
    mean_count = 0;
    mean = 0;
  }

  AD_value = analogRead(NTC);
  analog_signal = (AD_value / 1023) * 5;
  NTC_Res = analog_signal / (5 - analog_signal) * 330;
  temp[mean_count] = 1 / (log(NTC_Res / 10) / 3000 + 1 / (25 + 273.15)) - 273.15;
  mean += temp[mean_count];
  mean_count++;

  timer1.update();
  timer2.update();
}
