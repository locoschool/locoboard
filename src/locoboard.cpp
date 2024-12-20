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

#ifdef USE_MOTORS
void rotate_motor(unsigned char motor_ind, unsigned char direction, unsigned char speed)
{
  if(motor_ind == MOTOR_L)
  {
    digitalWrite(PIN_MOTOR_L_DIR, direction);
    digitalWrite(PIN_MOTOR_L_PWM, speed);
  }
  else if(motor_ind == MOTOR_R)
  {
    digitalWrite(PIN_MOTOR_R_DIR, direction);
    digitalWrite(PIN_MOTOR_R_PWM, speed);
  }
  else
  {
    Serial.println("ERROR: wrong motor index.");
  }
}

void stop_motor(unsigned char motor_ind)
{
  if(motor_ind == MOTOR_L)
  {
    digitalWrite(PIN_MOTOR_L_DIR, LOW);
    digitalWrite(PIN_MOTOR_L_PWM, LOW);
  }
  else if(motor_ind == MOTOR_R)
  {
    digitalWrite(PIN_MOTOR_R_DIR, LOW);
    digitalWrite(PIN_MOTOR_R_PWM, LOW);
  }
  else
  {
    Serial.println("ERROR: wrong motor index.");
  }
}

void setup_motor_pins()
{
  pinMode(PIN_MOTOR_L_DIR, OUTPUT);
  pinMode(PIN_MOTOR_L_PWM, OUTPUT);
  pinMode(PIN_MOTOR_R_DIR, OUTPUT);
  pinMode(PIN_MOTOR_R_PWM, OUTPUT);

  digitalWrite(PIN_MOTOR_L_DIR, LOW);
  digitalWrite(PIN_MOTOR_L_PWM, LOW);
  digitalWrite(PIN_MOTOR_R_DIR, LOW);
  digitalWrite(PIN_MOTOR_R_PWM, LOW);
}
#endif

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
  unsigned long now = millis();
  if(TinyReceiverDecode())
  {
    remote.last = now;

    if(remote.held && remote.button == TinyIRReceiverData.Command)
      return false;

    remote.button = TinyIRReceiverData.Command;
    remote.held = true;
    return true;
  }
  else if((now - remote.last) > REMOTE_INTERVAL_MS)
    remote.held = false;

  return false;
}

unsigned char get_ir_button()
{
  return remote.button;
}

bool get_ir_held()
{
  return remote.held;
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

#ifdef USE_LED_MATRIX
byte led_matrix_buffer[8];

void i2c_write(byte device_addr, byte cmd, byte data)
{
  Wire.beginTransmission(device_addr);
  Wire.write(cmd);
  Wire.write(data);
  Wire.endTransmission();
}

void clear_led_matrix()
{
  for(int i=0; i<8; i++) led_matrix_buffer[i] = 0;
}

void set_led_matrix_pixel(unsigned char row, unsigned char col, unsigned char value)
{
  if(value) led_matrix_buffer[row] |= 1 << (col+7)%8;
  else led_matrix_buffer[row] &= ~(1 << (col+7)%8);
}

void show_led_matrix()
{
  // AS1115 is in shutdown on power-up. Wake up & reset feature register.
  i2c_write(0x00, 0x0C, 0x01);
  // // Default global intensity is minimal. Set to 50 %.
  i2c_write(0x00, 0x0A, 0x01);
  // // Set scan limit to display all digits.
  i2c_write(0x00, 0x0B, 0x07);
  i2c_write(0x00, 0x09, 0x00);
  for(int i=0; i<8; i++) i2c_write(0x00, i+1, led_matrix_buffer[i]);
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