#include <cmath>
#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int16MultiArray.h>
std_msgs::Int16MultiArray motor;
double Vx = 0, Vy = 0, resolutional_speed, gyro_angle;

namespace kousensai {
double map(double x, double in_min, double in_max, double out_min,
           double out_max) {
  return (double)(x - in_min) * (out_max - out_min) / (in_max - in_min) +
         out_min;
}
}

void joyCallback(const sensor_msgs::Joy &msg) {
  double speed = 0, angle = 0;
  double turn_right = 0, turn_left = 0;
  speed = hypot(msg.axes[1], -msg.axes[0]) * 255;
  angle = atan2(msg.axes[1], -msg.axes[0]);
  Vx = speed * cos(angle + gyro_angle);
  Vy = speed * sin(angle + gyro_angle);
  turn_right = kousensai::map(msg.axes[5], 1, -1, 0, 255);
  turn_left =  kousensai::map(msg.axes[2], 1, -1, 0, 255);
  resolutional_speed = ((turn_right - turn_left) / 2) * 255;
}

void gyroCallback(const std_msgs::Float64 &msg) { gyro_angle = msg.data; }
int main(int argc, char **argv) {
  ros::init(argc, argv, "omuni");
  ros::NodeHandle n;
  ros::Subscriber controller_sub = n.subscribe("joy", 10, joyCallback);
  ros::Subscriber gyro_sub = n.subscribe("gyro_yaw", 10, gyroCallback);
  ros::Publisher wheel_pub =
      n.advertise<std_msgs::Int16MultiArray>("wheel_info", 100);
  motor.data.resize(4);
  ros::Rate loop_rate(1000);

  while (ros::ok()) {
    motor.data[0] =
        -Vx * cos(M_PI / 4) + Vy * sin(M_PI / 4) + resolutional_speed;
    ;
    motor.data[1] =
        -Vx * cos(M_PI / 4) - Vy * sin(M_PI / 4) + resolutional_speed;
    motor.data[2] =
        Vx * cos(M_PI / 4) - Vy * sin(M_PI / 4) + resolutional_speed;
    motor.data[3] =
        Vx * cos(M_PI / 4) + Vy * sin(M_PI / 4) + resolutional_speed;
    motor.data[0] > 255 ? motor.data[0] = 255 : motor.data[0] = motor.data[0];
    motor.data[1] > 255 ? motor.data[1] = 255 : motor.data[1] = motor.data[1];
    motor.data[2] > 255 ? motor.data[2] = 255 : motor.data[2] = motor.data[2];
    motor.data[3] > 255 ? motor.data[3] = 255 : motor.data[3] = motor.data[3];
    wheel_pub.publish(motor);
    ros::spinOnce();
    loop_rate.sleep();
  }
}
