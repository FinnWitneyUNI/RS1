#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>
#include <std_msgs/msg/string.hpp>

class ScanFilterNode : public rclcpp::Node
{
public:
    ScanFilterNode() : Node("scan_filter_node")
    {

        // Create a publisher
        pub1_ = this->create_publisher<sensor_msgs::msg::LaserScan>("filtered_scan_1", 10);

        pub2_ = this->create_publisher<sensor_msgs::msg::LaserScan>("filtered_scan_2", 10);

        // Create a subscriber
        sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
            "scan", 10, std::bind(&ScanFilterNode::scan_callback, this, std::placeholders::_1));
    }

private:
    void scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
    {
        // Create a new LaserScan message
        auto filtered_msg1 = std::make_shared<sensor_msgs::msg::LaserScan>(*msg);
        auto filtered_msg2 = std::make_shared<sensor_msgs::msg::LaserScan>(*msg);
        
        int start_index_2 = static_cast<int>(((2*M_PI)-(M_PI / 4)) / msg->angle_increment);
        int end_index_2 = static_cast<int>(msg->angle_max/msg->angle_increment);
        int start_index_1 = static_cast<int>(msg->angle_min/msg->angle_increment);
        int end_index_1 = static_cast<int>((M_PI / 4 - msg->angle_min) / msg->angle_increment);


        filtered_msg1->ranges.clear();
        filtered_msg2->ranges.clear();

        filtered_msg1->angle_min = msg->angle_min;
        filtered_msg1->angle_max = M_PI / 4;
        filtered_msg2->angle_max = msg->angle_max;
        filtered_msg2->angle_min = (2*M_PI) - (M_PI / 4);

        for (int i = start_index_1; i <=  end_index_1; i++)
        {
            filtered_msg1->ranges.push_back(msg->ranges[i]);
        }

        for (int j = start_index_2; j <= end_index_2; j++)
        {
            filtered_msg2->ranges.push_back(msg->ranges[j]);
        }

        
        // Adjust the scan time and time increment as needed
        filtered_msg1->scan_time = msg->scan_time;
        filtered_msg1->time_increment = msg->time_increment;
        filtered_msg2->scan_time = msg->scan_time;
        filtered_msg2->time_increment = msg->time_increment;

        // Publish the filtered scan
        pub1_->publish(*filtered_msg1);
        pub2_->publish(*filtered_msg2);
    }

    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr sub_;
    rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr pub1_;
    rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr pub2_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ScanFilterNode>());
    rclcpp::shutdown();
    return 0;
}
