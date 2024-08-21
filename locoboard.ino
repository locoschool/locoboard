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

void setup()
{
  Serial.begin(115200);
  
  setup_display();
  setup_ir();
  setup_distance_sensors();
}

void loop()
{
  process_remote_button();
}
