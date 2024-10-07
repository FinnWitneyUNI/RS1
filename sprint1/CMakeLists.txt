cmake_minimum_required(VERSION 3.8)
project(sprint1)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable
add_executable(scan_filter_node src/scan_filter.cpp)

# Link dependencies
ament_target_dependencies(scan_filter_node
  rclcpp
  sensor_msgs
)

# Install the executable
install(TARGETS
  scan_filter_node
  DESTINATION lib/${PROJECT_NAME}
)

# Install launch files
#install(DIRECTORY launch/
#  DESTINATION share/${PROJECT_NAME}/launch
#)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
