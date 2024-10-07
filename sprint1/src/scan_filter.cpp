#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>
#include <std_msgs/msg/string.hpp>

class ScanFilterNode : public rclcpp::Node
{
public:
    ScanFilterNode() : Node("scan_filter_node")
    {
        // Declare parameters
        this->declare_parameter("nth", 5);
        this->get_parameter("nth", nth_);

        // Create a publisher
        pub_ = this->create_publisher<sensor_msgs::msg::LaserScan>("filtered_scan", 10);

        // Create a subscriber
        sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
            "scan", 10, std::bind(&ScanFilterNode::scan_callback, this, std::placeholders::_1));
    }

private:
    void scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "Original scan reads %zu", msg->ranges.size());

        // Create a new LaserScan message based on the original message
        auto filtered_msg = std::make_shared<sensor_msgs::msg::LaserScan>(*msg);

        // Clear the ranges and intensities (if used)
        filtered_msg->ranges.clear();
        if (!msg->intensities.empty())
        {
            filtered_msg->intensities.clear();
        }

        // Filter the scan readings
        for (size_t i = 0; i < msg->ranges.size(); i += nth_)
        {
            filtered_msg->ranges.push_back(msg->ranges[i]);
            if (!msg->intensities.empty())
            {
                filtered_msg->intensities.push_back(msg->intensities[i]);
            }
        }

        // Update the angle_increment, angle_min, and angle_max to reflect the filtered scan
        filtered_msg->angle_increment = msg->angle_increment * nth_;
        filtered_msg->angle_max = msg->angle_min + (filtered_msg->ranges.size() - 1) * filtered_msg->angle_increment;

        RCLCPP_INFO(this->get_logger(), "Filtered scan reads %zu", filtered_msg->ranges.size());

        // Publish the filtered scan
        pub_->publish(*filtered_msg);
    }

    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr sub_;
    rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr pub_;
    size_t nth_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ScanFilterNode>());
    rclcpp::shutdown();
    return 0;
}
