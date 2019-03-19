#ifndef IIWA_CONTROL_CUSTOM_EFFORT_CONTROLLER_H
#define IIWA_CONTROL_CUSTOM_EFFORT_CONTROLLER_H

// ROS headers
#include <ros/node_handle.h>
// ros control
#include <controller_interface/controller.h>
#include <hardware_interface/joint_command_interface.h>

// realtime tools
#include <realtime_tools/realtime_buffer.h>
#include <realtime_tools/realtime_publisher.h>

// msgs
#include <std_msgs/Float64MultiArray.h>

// URDF
#include <urdf/model.h>

// iiwa_tools
// #include <iiwa_tools/GetGravity.h>
#include <iiwa_tools/GetJacobian.h>

// Control stack headers
#include <robot_controllers/low/PassiveDS.hpp>

namespace iiwa_control {
    class CustomEffortController : public controller_interface::Controller<hardware_interface::EffortJointInterface> {
    public:
        CustomEffortController();
        ~CustomEffortController();

        bool init(hardware_interface::EffortJointInterface* hw, ros::NodeHandle& n);

        void update(const ros::Time& /*time*/, const ros::Duration& /*period*/);

        std::vector<hardware_interface::JointHandle> joints_;

        realtime_tools::RealtimeBuffer<std::vector<double>> commands_buffer_;

        unsigned int n_joints_;

        std::vector<std::string> joint_names_;

    protected:
        ros::Subscriber sub_command_;

        // Controller
        robot_controllers::low::PassiveDS passive_ds_;

        // Controller's settings
        unsigned int space_dim_;
        std::string operation_space_, gravity_comp_;

        // iiwa_tools services
        ros::ServiceClient iiwa_client_gravity_, iiwa_client_jacobian_;
        // iiwa_tools::GetGravity gravity_srv_;
        iiwa_tools::GetJacobian jacobian_srv_;

        // URDF
        std::vector<urdf::JointConstSharedPtr> joint_urdfs_;

        // Command callback
        void commandCB(const std_msgs::Float64MultiArrayConstPtr& msg);

        // Enforce effort limits
        void enforceJointLimits(double& command, unsigned int index);
    };
} // namespace iiwa_control

#endif