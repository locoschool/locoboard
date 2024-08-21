#include "src/locoboard.h"

void process_remote_button()
{
  if(check_ir_button_pressed())
  {
    switch(get_ir_button())
    {
      case BTN_0:
      {
        Serial.println("Testing display");
        test_display();
        break;
      }
      case BTN_1:
      {
        Serial.println("Testing distance sensor");
        test_distance_sensors();
        break;
      }
      case BTN_2:
      {
        Serial.println("Testing potentiometer");
        test_potentiometer();
        break;
      }
      case BTN_3:
      {
        Serial.println("Testing servo");
        test_servo();
        break;
      }
    }
  }
}

void test_distance_sensors()
{
  Serial.print("L1: ");
  Serial.println(measure_distance_mm(DISTANCE_L1));
  Serial.print("L2: ");
  Serial.println(measure_distance_mm(DISTANCE_L2));
  Serial.print("L3: ");
  Serial.println(measure_distance_mm(DISTANCE_L3));
}

void test_display()
{
  clear_display();
  draw_line(0, 0, 10, 10);
  show();
}

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

void setup()
{
  Serial.begin(115200);
  
  setup_display();
  setup_ir();
  setup_distance_sensors();
  setup_servo_pins();
}

void loop()
{
  process_remote_button();
}
