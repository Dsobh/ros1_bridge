// Copyright 2015 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <iostream>
#include <memory>
#include <utility>

// include ROS 1
#ifdef __clang__
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunused-parameter"
#endif
#include "ros/ros.h"
#include "tf2_msgs/TFMessage.h"

#ifdef __clang__
# pragma clang diagnostic pop
#endif

// include ROS 2
#include "rclcpp/rclcpp.hpp"
#include "tf2_msgs/msg/tf_message.hpp"



rclcpp::Publisher<tf2_msgs::msg::TFMessage>::SharedPtr pub, pub2;

void chatterCallback(const tf2_msgs::TFMessage::ConstPtr & ros1_msg)
{
  auto ros2_msg = std::make_unique<tf2_msgs::msg::TFMessage>();


  int num_transforms = (int)ros1_msg->transforms.size();
  std::vector<geometry_msgs::msg::TransformStamped> transform_vector;
  for (int i = 0; i < num_transforms; i++){
    geometry_msgs::msg::TransformStamped transform;
  
    transform.header.stamp.sec = ros1_msg->transforms[i].header.stamp.sec;
    transform.header.stamp.nanosec = ros1_msg->transforms[i].header.stamp.nsec;
    transform.header.frame_id = ros1_msg->transforms[i].header.frame_id;
    transform.child_frame_id = ros1_msg->transforms[i].child_frame_id;
    
    
    transform.transform.translation.x = ros1_msg->transforms[i].transform.translation.x;
    transform.transform.translation.y = ros1_msg->transforms[i].transform.translation.y;
    transform.transform.translation.z = ros1_msg->transforms[i].transform.translation.z;
    transform.transform.rotation.x = ros1_msg->transforms[i].transform.rotation.x;
    transform.transform.rotation.y = ros1_msg->transforms[i].transform.rotation.y;
    transform.transform.rotation.z = ros1_msg->transforms[i].transform.rotation.z;
    transform.transform.rotation.w = ros1_msg->transforms[i].transform.rotation.w;

    transform_vector.push_back(transform);

  }

  ros2_msg->transforms = transform_vector;
  pub->publish(std::move(ros2_msg));
}

void chatterCallback2(const tf2_msgs::TFMessage::ConstPtr & ros1_msg)
{
  auto ros2_msg = std::make_unique<tf2_msgs::msg::TFMessage>();


  int num_transforms = (int)ros1_msg->transforms.size();
  std::vector<geometry_msgs::msg::TransformStamped> transform_vector;
  for (int i = 0; i < num_transforms; i++){
    geometry_msgs::msg::TransformStamped transform;
  
    transform.header.stamp.sec = ros1_msg->transforms[i].header.stamp.sec;
    transform.header.stamp.nanosec = ros1_msg->transforms[i].header.stamp.nsec;
    transform.header.frame_id = ros1_msg->transforms[i].header.frame_id;
    transform.child_frame_id = ros1_msg->transforms[i].child_frame_id;
    
    
    transform.transform.translation.x = ros1_msg->transforms[i].transform.translation.x;
    transform.transform.translation.y = ros1_msg->transforms[i].transform.translation.y;
    transform.transform.translation.z = ros1_msg->transforms[i].transform.translation.z;
    transform.transform.rotation.x = ros1_msg->transforms[i].transform.rotation.x;
    transform.transform.rotation.y = ros1_msg->transforms[i].transform.rotation.y;
    transform.transform.rotation.z = ros1_msg->transforms[i].transform.rotation.z;
    transform.transform.rotation.w = ros1_msg->transforms[i].transform.rotation.w;

    transform_vector.push_back(transform);

  }

  ros2_msg->transforms = transform_vector;
  pub2->publish(std::move(ros2_msg));
}

int main(int argc, char * argv[])
{

    static const rmw_qos_profile_t rmw_qos_profile_default =
  {
    RMW_QOS_POLICY_HISTORY_KEEP_LAST,
    30,
    RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT,
    RMW_QOS_POLICY_DURABILITY_VOLATILE,
    RMW_QOS_DEADLINE_DEFAULT,
    RMW_QOS_LIFESPAN_DEFAULT,
    RMW_QOS_POLICY_LIVELINESS_SYSTEM_DEFAULT,
    RMW_QOS_LIVELINESS_LEASE_DURATION_DEFAULT,
    false
  };

  auto qos = rclcpp::QoS(
    rclcpp::QoSInitialization(
      rmw_qos_profile_default.history,
      rmw_qos_profile_default.depth
    ),
    rmw_qos_profile_default);

  // ROS 2 node and publisher
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("bridge_talker_tf");
  pub = node->create_publisher<tf2_msgs::msg::TFMessage>("/tf", qos);
  pub2 = node->create_publisher<tf2_msgs::msg::TFMessage>("/tf_static", qos);

  // ROS 1 node and subscriber
  ros::init(argc, argv, "bridge_listener_tf");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("/tf", 10, chatterCallback);
  ros::Subscriber sub2 = n.subscribe("/tf_static", 10, chatterCallback2);



  ros::spin();

  return 0;
}
