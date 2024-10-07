cmake_minimum_required(VERSION 3.5)
project(scan_filter)

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create the executable
add_executable(scan_filter_node src/lab2.cpp)

# Link libraries
ament_target_dependencies(scan_filter_node rclcpp sensor_msgs std_msgs)

# Install targets
install(TARGETS
  scan_filter_node
  DESTINATION lib/${PROJECT_NAME}
)

# Install launch files (if any)
install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}/
)

# Export package dependencies
ament_package()
