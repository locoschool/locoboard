#include "src/locoboard.h"

void process_remote_button()
{
  #ifdef USE_REMOTE
  if(check_ir_button_pressed())
  {
    switch(get_ir_button())
    {
      #ifdef USE_DISPLAY
      case BTN_0:
      {
        Serial.println("Testing display");
        test_display();
        break;
      }
      #endif
      #ifdef USE_DISTANCE_SENSOR
      case BTN_1:
      {
        Serial.println("Testing distance sensor");
        test_distance_sensors();
        break;
      }
      #endif
      #ifdef USE_POTENTIOMETER
      case BTN_2:
      {
        Serial.println("Testing potentiometer");
        test_potentiometer();
        break;
      }
      #endif
      #ifdef USE_SERVO
      case BTN_3:
      {
        Serial.println("Testing servo");
        test_servo();
        break;
      }
      #endif
      #ifdef USE_ADDR_LEDS
      case BTN_4:
      {
        Serial.println("Testing address LEDs");
        test_addr_leds();
        break;
      }
      #endif
      #ifdef USE_USER_BUTTON
      case BTN_5:
      {
        Serial.println("Testing user button");
        test_user_button();
        break;
      }
      #endif
    }
  }
  #endif
}

#ifdef USE_DISTANCE_SENSOR
void test_distance_sensors()
{
  Serial.print("L1: ");
  Serial.println(measure_distance_mm(DISTANCE_L1));
  Serial.print("L2: ");
  Serial.println(measure_distance_mm(DISTANCE_L2));
  Serial.print("L3: ");
  Serial.println(measure_distance_mm(DISTANCE_L3));
}
#endif

#ifdef USE_DISPLAY
void test_display()
{
  clear_display();
  draw_circle(30, 30, 10, true);
  draw_line(0, 0, 10, 10);
  draw_pixel(64, 32);
  draw_rectangle(0, 0, 10, 20, false);
  draw_text(32, 4, "Hello, world!", 0);
  show();
}
#endif

#ifdef USE_POTENTIOMETER
void test_potentiometer()
{
  int val = 0;
  for (int i = 0; i < 1000; i++)
  {
    val = read_potentiometer_value();
    Serial.println(val);
    delay(10);
  }
}
#endif

#ifdef USE_SERVO
void test_servo()
{
  for(int i=0; i<=180; i+=5)
  {
    rotate_servo(SERVO_S1, i);
    rotate_servo(SERVO_S2, i);
    rotate_servo(SERVO_S3, i);
    delay(50);
  }
}
#endif

#ifdef USE_ADDR_LEDS
void test_addr_leds()
{
  for(int i=0; i<255; i+=20)
  {
    for(int j=0; j<255; j+=20)
    {
      for(int k=0; k<255; k+=20)
      {
        set_led_color(LED_1, i, j, k);
        set_led_color(LED_2, i, k, j);
        set_led_color(LED_3, j, i, k);
        set_led_color(LED_4, j, k, i);
      }
    }
  }
  set_led_color(LED_1, 0, 0, 0);
  set_led_color(LED_2, 0, 0, 0);
  set_led_color(LED_3, 0, 0, 0);
  set_led_color(LED_4, 0, 0, 0);
}
#endif

#ifdef USE_USER_BUTTON
void test_user_button()
{
  for (int i=0; i<1000; i++)
  {
    if(check_user_button_pressed()) digitalWrite(13, HIGH);
    else digitalWrite(13, LOW);
    delay(10);
  } 
}
#endif

void setup()
{
  Serial.begin(115200);
  #ifdef USE_DISPLAY
  setup_display();
  #endif
  #ifdef USE_REMOTE
  setup_ir();
  #endif
  #ifdef USE_DISTANCE_SENSOR
  setup_distance_sensors();
  #endif
  #ifdef USE_SERVO
  setup_servo_pins();
  #endif
  #ifdef USE_ADDR_LEDS
  setup_addr_leds();
  #endif
  #ifdef USE_USER_BUTTON
  setup_user_button();
  #endif
}

void loop()
{
  process_remote_button();
}
