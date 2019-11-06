#include <iostream>
#include <pigpiod_if2.h>
#include <ros/ros.h>
#include <std_msgs/Int16MultiArray.h>

using std::abs;
using std::cout;
using std::endl;

constexpr int wheel_0_pin[3] = {17, 27, 22};
constexpr int wheel_1_pin[3] = {10, 9, 11};
constexpr int wheel_2_pin[3] = {5, 6, 13};
constexpr int wheel_3_pin[3] = {25, 8, 7};

int gpio_handle, pwm_right_front, pwm_left_front, pwm_right_back, pwm_left_back;

class l298n {
private:
  int pwm_pin, change_pin1, change_pin2;
  void front(int pwm);
  void back(int pwm);

public:
  l298n(int user_pwm_pin, int user_change_pin1, int user_change_pin2);
  void set(int pwm);
};

l298n::l298n(int user_pwm_pin, int user_change_pin1, int user_change_pin2) {
  pwm_pin = user_pwm_pin;
  change_pin1 = user_change_pin1;
  change_pin2 = user_change_pin2;
  set_mode(gpio_handle, change_pin1, PI_OUTPUT);
  set_mode(gpio_handle, change_pin2, PI_OUTPUT);
  set_mode(gpio_handle, pwm_pin, PI_OUTPUT);
}

void l298n::set(int pwm) {
  pwm >= 0 ? this->front(abs(pwm)) : this->back(abs(pwm));
}

void l298n::front(int pwm) {
  gpio_write(gpio_handle, change_pin1, 1);
  gpio_write(gpio_handle, change_pin2, 0);
  set_PWM_dutycycle(gpio_handle, pwm_pin, pwm);
}

void l298n::back(int pwm) {
  gpio_write(gpio_handle, change_pin1, 0);
  gpio_write(gpio_handle, change_pin2, 1);
  set_PWM_dutycycle(gpio_handle, pwm_pin, pwm);
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
  l298n right_front_wheel(wheel_0_pin[0], wheel_0_pin[1], wheel_0_pin[2]);
  l298n left_front_wheel(wheel_1_pin[0], wheel_1_pin[1], wheel_1_pin[2]);
  l298n right_back_wheel(wheel_2_pin[0], wheel_2_pin[1], wheel_2_pin[2]);
  l298n left_back_wheel(wheel_3_pin[0], wheel_3_pin[1], wheel_3_pin[2]);
  cout << "create object" << endl;
  ros::Rate loop_rate(1000);
  while (ros::ok()) {
    cout << "in loop" << endl;
    right_front_wheel.set(pwm_right_front);
    left_front_wheel.set(pwm_left_front);
    right_back_wheel.set(pwm_right_back);
    left_back_wheel.set(pwm_left_back);
    ros::spinOnce();
    loop_rate.sleep();
  }
}
