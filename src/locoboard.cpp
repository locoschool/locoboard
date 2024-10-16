#include "locoboard.h"

#include <Arduino.h>

#ifdef USE_SERVO
#include <Servo.h>
#endif

#ifdef USE_DISTANCE_SENSOR
#include <VL53L0X.h>
#define DISTANCE_SW_ADDR 0x73
// #define USE_CALLBACK_FOR_TINY_RECEIVER
#endif

#include <Wire.h>

#ifdef USE_ADDR_LEDS
#include <FastLED.h>
#endif

#ifdef USE_REMOTE
#include <TinyIRReceiver.hpp>
#endif

#ifdef USE_DISPLAY
#include <ss_oled.h>
#endif

#ifdef USE_DISPLAY
SSOLED ssoled;
uint8_t ucBackBuffer[1024];
#endif

// Motor motor[2];
#ifdef USE_SERVO
Servo servo[3];
#endif

#ifdef USE_DISTANCE_SENSOR
VL53L0X distance_sensor[3];
#endif

#ifdef USE_REMOTE
Remote remote;
#endif

#ifdef USE_ADDR_LEDS
CRGB leds[4];
#endif

/*
void rotate_motor(unsigned char motor_ind, unsigned char direction, unsigned char speed)
{
  unsigned char v1, v2;
  switch(direction)
  {
    case 0:
    {
      v1 = LOW;
      v2 = speed;
      break;
    }
    case 1:
    {
      v1 = HIGH;
      v2 = 255 - speed;
      break;
    }
  }
  digitalWrite(motor[motor_ind].pin_a, v1);
  analogWrite(motor[motor_ind].pin_b, v2);
}

void stop_motor(unsigned char motor_ind)
{
  digitalWrite(motor[motor_ind].pin_a, LOW);
  digitalWrite(motor[motor_ind].pin_b, LOW);
}

void setup_motor_pins(unsigned char motor_ind, unsigned char pin_a, unsigned char pin_b)
{
  motor[motor_ind].pin_a = pin_a;
  motor[motor_ind].pin_b = pin_b;

  pinMode(motor[motor_ind].pin_a, OUTPUT);
  pinMode(motor[motor_ind].pin_b, OUTPUT);

  digitalWrite(motor[motor_ind].pin_a, LOW);
  digitalWrite(motor[motor_ind].pin_b, LOW);
}*/

#ifdef USE_SERVO
void rotate_servo(unsigned char servo_ind, unsigned char degrees)
{
  servo[servo_ind].write(degrees);
}

void setup_servo_pins()
{
  servo[0].attach(PIN_SERVO_1);
  servo[1].attach(PIN_SERVO_2);
  servo[2].attach(PIN_SERVO_3);
}
#endif

#ifdef USE_DISTANCE_SENSOR
void select_distance_sensor(uint8_t i) {
  if (i > 3) return;

  Wire.beginTransmission(DISTANCE_SW_ADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

void setup_distance_sensors()
{
  for(int i=0; i<3; i++)
  {
    select_distance_sensor(i);
    distance_sensor[i].setTimeout(500);
    distance_sensor[i].init();
    distance_sensor[i].setMeasurementTimingBudget(200000);
  }
}

int measure_distance_mm(unsigned char sensor_id)
{
  select_distance_sensor(sensor_id);
  int measurement = distance_sensor[sensor_id].readRangeSingleMillimeters();
  if(distance_sensor[sensor_id].timeoutOccurred()) return -1;
  else return measurement;
}
#endif

#ifdef USE_REMOTE
void setup_ir()
{
  if (!initPCIInterruptForTinyReceiver())
  {
    Serial.println("No interrupt available for defined pin.");
  }
}

bool check_ir_button_pressed()
{
  if(TinyReceiverDecode())
  {
    remote.button = TinyIRReceiverData.Command;
    return true;
  }
  else return false;
}

unsigned char get_ir_button()
{
  return remote.button;
}
#endif

#ifdef USE_POTENTIOMETER
int read_potentiometer_value()
{
  return analogRead(PIN_POTENTIOMETER);
}
#endif

#ifdef USE_ADDR_LEDS
void setup_addr_leds()
{
  FastLED.addLeds<WS2812, PIN_ADDR_LED, RGB>(leds, 4);
}

void set_led_color(unsigned char led_index, unsigned char r, unsigned char g, unsigned char b)
{
  leds[led_index] = CRGB(r, g, b);
  FastLED.show();
}
#endif

#ifdef USE_USER_BUTTON
void setup_user_button()
{
  pinMode(PIN_USER_BUTTON, INPUT);
}

bool check_user_button_pressed()
{
  return digitalRead(PIN_USER_BUTTON);
}
#endif

#ifdef USE_DISPLAY
void setup_display()
{
  int rc;
  rc = oledInit(&ssoled, OLED_128x64, -1, 0, 0, 1, -1, -1, -1, 400000L);// Standard HW I2C bus at 400Khz

  if (rc != OLED_NOT_FOUND)
  {
    oledFill(&ssoled, 0, 1);
    delay(2000);
  }
  else
  {
    Serial.println("ERROR: display not initialized.");
  }
  oledSetBackBuffer(&ssoled, ucBackBuffer);
}

void draw_line(int x1, int y1, int x2, int y2)
{
  oledDrawLine(&ssoled, x1, y1, x2, y2, 1);
}

void draw_rectangle(int x1, int y1, int x2, int y2, bool filled)
{
  oledRectangle(&ssoled, x1, y1, x2, y2, 1, filled);
}

void draw_circle(int x, int y, int radius, bool filled)
{
  oledEllipse(&ssoled, x, y, radius, radius, 1, filled);
}

void draw_pixel(int x, int y)
{
  oledSetPixel(&ssoled, x, y, 1, 1);
}

void draw_text(int x, int y, char* text, unsigned char size)
{
  oledWriteString(&ssoled, 0, x, y, text, size, 0, 1);
}

void show()
{
  oledDumpBuffer(&ssoled, NULL);
}

void clear_display()
{
  oledFill(&ssoled, 0, 1);
}
#endif