#include <pigpiod_if2.h>
#include <ros/ros.h>

int main(int argc, char **argv) {
  ros::init(argc, argv, "led");
  ros::NodeHandle n;
  int pi = pigpio_start(0, 0);
  set_mode(pi, 17, PI_OUTPUT);
  bool led_prev = false;
  while (1) {
    if (led_prev == false) {
      gpio_write(pi, 17, 1);
      time_sleep(2);
      led_prev = true;
    } else {
      gpio_write(pi, 17, 0);
      time_sleep(2);
      led_prev = false;
    }
  }
}
