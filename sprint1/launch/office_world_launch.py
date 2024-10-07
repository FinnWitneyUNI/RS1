from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    return LaunchDescription([
        DeclareLaunchArgument(
            'world',
            default_value='office_world.world',
            description='Path to the world file to load'),

        # Launch the TurtleBot3 world in Gazebo
        Node(
            package='turtlebot3_gazebo',
            executable='turtlebot3_world.launch.py',
            name='turtlebot3_world',
            output='screen',
            parameters=[{
                'use_sim_time': True
            }],
            arguments=[LaunchConfiguration('world')]
        ),

        # Launch the ScanFilterNode
        Node(
            package='sprint1',
            executable='scan_filter_node',
            name='scan_filter_node',
            output='screen'
        ),
    ])
