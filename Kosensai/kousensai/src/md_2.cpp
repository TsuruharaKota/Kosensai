#include <iostream>
#include <pigpiod_if2.h>
#include <ros/ros.h>
#include <std_msgs/Int16MultiArray.h>
#include <cstdlib>

using std::abs;
using std::cout;
using std::endl;

constexpr int wheel_0_pin[3] = {17, 27, 22};
constexpr int wheel_1_pin[3] = {10, 9, 11};
constexpr int wheel_2_pin[3] = {5, 6, 13};
constexpr int wheel_3_pin[3] = {25, 8, 7};

int gpio_handle, pwm_right_front, pwm_left_front, pwm_right_back, pwm_left_back;

void front(int pwm, int pwm_pin, int change_pin1, int change_pin2) {
  gpio_write(gpio_handle, change_pin1, 1);
  gpio_write(gpio_handle, change_pin2, 0);
  set_PWM_dutycycle(gpio_handle, pwm_pin, pwm);
}

void back(int pwm, int pwm_pin, int change_pin1, int change_pin2) {
  gpio_write(gpio_handle, change_pin1, 0);
  gpio_write(gpio_handle, change_pin2, 1);
  set_PWM_dutycycle(gpio_handle, pwm_pin, pwm);
}

void set(int pwm, int pwm_pin, int change_pin1, int change_pin2) {
  pwm >= 0 ? front(abs(pwm), pwm_pin, change_pin1, change_pin2) : back(abs(pwm), pwm_pin, change_pin1, change_pin2);
}

void mdCallback(const std_msgs::Int16MultiArray &msg) {
  pwm_right_front = msg.data[0];
  pwm_left_front = msg.data[1];
  pwm_right_back = msg.data[2];
  pwm_left_back = msg.data[3];
}
int main(int argc, char **argv) {
  gpio_handle = pigpio_start(0, 0);
  cout << "start" << endl;
  ros::init(argc, argv, "md_info");
  ros::NodeHandle n;
  ros::Subscriber md_sub = n.subscribe("wheel_info", 10, mdCallback);
  cout << "create object" << endl;
  ros::Rate loop_rate(1000);
  while (ros::ok()) {
    cout << "in loop" << endl;
    set(pwm_right_front, wheel_0_pin[0], wheel_0_pin[1], wheel_0_pin[2]);
    set(pwm_left_front, wheel_1_pin[0], wheel_1_pin[1], wheel_1_pin[2]);
    set(pwm_left_back, wheel_2_pin[0], wheel_2_pin[1], wheel_2_pin[2]);
    set(pwm_right_back, wheel_3_pin[0], wheel_3_pin[1], wheel_3_pin[2]);
    ros::spinOnce();
    loop_rate.sleep();
  }
}
