#!/bin/sh

rosbag record /aerial_robot/ground_pose /aerial_robot/pose /baro /battery_voltage_status /camera_velocity /distance /gps /zed/odom /realsense/odom /realsense/slam/status /imu /joy /motor_pwms /servo/states /uav/nav /flight_config_ack

