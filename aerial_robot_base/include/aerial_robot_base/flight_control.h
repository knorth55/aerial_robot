// -*- mode: c++ -*-
/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2017, JSK Lab
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/o2r other materials provided
 *     with the distribution.
 *   * Neither the name of the JSK Lab nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

#ifndef FLIGHT_CONTROLLER_BASE_PLUGIN_H
#define FLIGHT_CONTROLLER_BASE_PLUGIN_H

/* ros */
#include <ros/ros.h>

/* basic instance */
#include <aerial_robot_base/basic_state_estimation.h>
#include <aerial_robot_base/control_input_array.h>
#include <aerial_robot_base/flight_navigation.h>

/* ros msg */
#include <aerial_robot_base/MotorInfo.h>

/* util */
#include <boost/algorithm/clamp.hpp>
#include <tf/transform_datatypes.h>

using namespace std;

namespace control_plugin
{
  class ControlBase
  {
  public:
    ControlBase():
      motor_num_(1),
      control_timestamp_(-1)
    {}

    virtual ~ControlBase(){}
    void virtual initialize(ros::NodeHandle nh,
               ros::NodeHandle nhp,
               BasicEstimator* estimator,
               Navigator* navigator,
               double ctrl_loop_rate)
    {
      nh_ = ros::NodeHandle(nh, "controller");
      nhp_ = ros::NodeHandle(nhp,  "controller");

      estimator_ = estimator;
      navigator_ = navigator;

      ctrl_loop_rate_ = ctrl_loop_rate;

      estimate_mode_ = estimator_->getEstimateMode();

      nhp_.param("verbose", verbose_, true);

      ros::NodeHandle motor_info_node("motor_info");
      std::string ns = motor_info_node.getNamespace();

      motor_info_node.param("min_pwm", min_pwm_, 0.0);
      if(verbose_) cout << ns  << ": min_pwm_ is "  <<  min_pwm_ << endl;
      motor_info_node.param("max_pwm", max_pwm_, 0.0);
      if(verbose_) cout << ns  << ": max_pwm_ is "  <<  max_pwm_ << endl;
      motor_info_node.param("f_pwm_rate", f_pwm_rate_, 1.0);
      if(verbose_) cout << ns  << ": f_pwm_rate_ is "  <<  f_pwm_rate_ << endl;
      motor_info_node.param("m_f_rate", m_f_rate_, 0.0);
      if(verbose_) cout << ns  << ": m_f_rate_ is %.3f\n"  <<  m_f_rate_ << endl;
      motor_info_node.param("f_pwm_offset", f_pwm_offset_, 0.0);
      if(verbose_) cout << ns  << ": f_pwm_offset_ is "  <<  f_pwm_offset_ << endl;
      motor_info_node.param("pwm_rate", pwm_rate_, 1.0);
      if(verbose_) cout << ns  << ": pwm_rate_ is "  <<  pwm_rate_ << endl;
      motor_info_node.param("force_landing_pwm", force_landing_pwm_, 0.5);
      if(verbose_) cout << ns  << ": force_landing_pwm_ is "  <<  force_landing_pwm_ << endl;
    }

    virtual void update() {}

    virtual void activate() {}
    virtual void reset() {}

  protected:
    ros::NodeHandle nh_;
    ros::NodeHandle nhp_;

    Navigator* navigator_;
    BasicEstimator* estimator_;
    FlightCtrlInput* flight_ctrl_input_;

    double ctrl_loop_rate_;
    double control_timestamp_;
    int motor_num_;

    /* new param */
    double min_pwm_, max_pwm_;
    double f_pwm_rate_; //gain which convert f to pwm and also take the bit shift into account
    double f_pwm_offset_;
    double m_f_rate_;
    double pwm_rate_; //percentage
    double force_landing_pwm_; //pwm
    int estimate_mode_;

    bool verbose_;
  };

};
#endif
