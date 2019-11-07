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
      ROS_INFO("OK");
      gpio_write(pi, 17, 1);
      gpio_write(pi, 27, 1);
      gpio_write(pi, 22, 1);
      gpio_write(pi, 10, 1);
      gpio_write(pi, 9, 1);
      gpio_write(pi, 11, 1);
      gpio_write(pi, 26, 1);
      gpio_write(pi, 13, 1);
      gpio_write(pi, 19, 1);
      gpio_write(pi, 25, 1);
      gpio_write(pi, 8, 1);
      gpio_write(pi, 7, 1);
      time_sleep(2);
      led_prev = true;
    } else {
      ROS_INFO("NO");
      gpio_write(pi, 17, 0);
      gpio_write(pi, 27, 0);
      gpio_write(pi, 22, 0);
      gpio_write(pi, 10, 0);
      gpio_write(pi, 9, 0);
      gpio_write(pi, 11, 0);
      gpio_write(pi, 26, 0);
      gpio_write(pi, 13, 0);
      gpio_write(pi, 19, 0);
      gpio_write(pi, 25, 0);
      gpio_write(pi, 8, 0);
      gpio_write(pi, 7, 0);
      time_sleep(2);
      led_prev = false;
    }
  }
}
