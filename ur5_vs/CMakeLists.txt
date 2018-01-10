cmake_minimum_required(VERSION 2.8.3)
project(ur5_vs)

find_package(catkin REQUIRED COMPONENTS
    roscpp
    rospy
    std_msgs
    roslib
    geometry_msgs
    image_transport
    cv_bridge
    visualization_msgs
    sensor_msgs
    tf
    actionlib
    control_msgs
    pcl_ros
    pcl_conversions
    joint_state_publisher
    moveit_core
    moveit_msgs
    moveit_planners_ompl
    moveit_ros_move_group
    moveit_ros_planning_interface

    moveit_ros_visualization
    robot_state_publisher
    ur_description
    xacro
  )

find_package(OpenCV 2.4.8 EXACT REQUIRED)
#find_package(PCL REQUIRED)
find_package(gazebo REQUIRED)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ur5_ik
  CATKIN_DEPENDS roscpp rospy std_msgs
  DEPENDS system_lib
    geometry_msgs
    visualization_msgs
    message_runtime
)


include_directories(
${GAZEBO_INCLUDE_DIRS}
${catkin_INCLUDE_DIRS}
${OpenCV_INCLUDE_DIR}
${PCL_INCLUDE_DIR}
include)

link_directories(${GAZEBO_LIBRARY_DIRS} ${PCL_LIBRARY_DIRS})
list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")

set(SOURCES
    ${PROJECT_SOURCE_DIR}/fk_test.cpp
    ${PROJECT_SOURCE_DIR}/gnuplot_ci.cpp
    ${PROJECT_SOURCE_DIR}/vsdemo.cpp
    ${PROJECT_SOURCE_DIR}/test_opencv.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ur5model.cpp
    ${PROJECT_SOURCE_DIR}/deepQ.cpp
    ${PROJECT_SOURCE_DIR}/eyeinhand.cpp
    ${PROJECT_SOURCE_DIR}/eyeinhand_v2.cpp
    ${PROJECT_SOURCE_DIR}/eyeinhand_6dof_tracking.cpp
    ${PROJECT_SOURCE_DIR}/eyeinhand_6dof_depthtracking.cpp
    ${PROJECT_SOURCE_DIR}/eyeinhand_6dof_4points.cpp
    ${PROJECT_SOURCE_DIR}/eyeinhand_v3.cpp
    ${PROJECT_SOURCE_DIR}/eyeinhand_point_vs.cpp
    ${PROJECT_SOURCE_DIR}/calibration_transform_setup.cpp
#    ${PROJECT_SOURCE_DIR}/detect_blob.cpp


)

#add_executable(ur5_ik src/fk_test.cpp src/ur5model.cpp include/ur5model.h src/gnuplot_ci.cpp include/gnuplot_ci.h)
#add_executable(visual_servoing src/vsdemo.cpp src/ur5model.cpp include/ur5model.h src/gnuplot_ci.cpp include/gnuplot_ci.h)
#add_executable(ur5_result src/ur5result.cpp src/ur5model.cpp include/ur5model.h src/gnuplot_ci.cpp include/gnuplot_ci.h)
#add_executable(test_opencv src/test_opencv.cpp)
#add_executable(vs_som src/deepQ.cpp src/ur5model.cpp include/ur5model.h src/gnuplot_ci.cpp include/gnuplot_ci.h)
#add_executable(eyeInHand src/eyeinhand.cpp src/MyFilledCircle.cpp src/ur5model.cpp include/ur5model.h src/gnuplot_ci.cpp include/gnuplot_ci.h)
#add_executable(eyeInHand_v2 src/eyeinhand_v2.cpp src/MyFilledCircle.cpp src/image_processing.cpp src/compute_image_jacobian src/matrix_for_ee_to_camera.cpp src/ur5model.cpp include/ur5model.h src/gnuplot_ci.cpp include/gnuplot_ci.h)
add_executable(eyeinhand_6dof_tracking src/eyeinhand_6dof_tracking.cpp src/MyFilledCircle.cpp src/image_processing.cpp src/compute_image_jacobian src/matrix_for_ee_to_camera.cpp include/my_funcs.h include/ur5_geo_jacobian.h src/compute_new_geo_jacobian.cpp)

#add_executable(eyeinhand_6dof_depthtracking src/eyeinhand_6dof_depthtracking.cpp src/MyFilledCircle.cpp src/image_processing.cpp src/compute_image_jacobian src/compute_image_jacobian_depth src/matrix_for_ee_to_camera.cpp)

#add_executable(eyeinhand_6dof_4points src/eyeinhand_6dof_4points.cpp src/MyFilledCircle.cpp src/image_processing.cpp src/compute_image_jacobian src/compute_image_jacobian_depth src/compute_image_jacobian_four_points src/matrix_for_ee_to_camera.cpp)
#add_executable(eyeInHand_v3 src/eyeinhand_v3.cpp src/MyFilledCircle.cpp src/ur5model.cpp include/ur5model.h src/image_processing.cpp src/matrix_for_ee_to_camera.cpp src/gnuplot_ci.cpp include/gnuplot_ci.h src/compute_image_jacobian_area.cpp)
#add_executable(eyeInHand_point_vs src/eyeinhand_point_vs.cpp src/MyFilledCircle.cpp src/ur5model.cpp include/ur5model.h src/gnuplot_ci.cpp include/gnuplot_ci.h)
#add_executable(plotting src/plotting.cpp src/gnuplot_ci.cpp include/gnuplot_ci.h)
add_executable(calibration_transform_setup src/calibration_transform_setup.cpp)
#add_executable(eyeinhand_6dof_area_tracking src/eyeinhand_6dof_area_tracking.cpp src/MyFilledCircle.cpp src/image_processing.cpp src/compute_image_jacobian src/compute_image_jacobian_area src/matrix_for_ee_to_camera.cpp)
#add_executable(robot_jacobian_test src/robot_jacobian_test.cpp src/MyFilledCircle.cpp src/image_processing.cpp src/compute_image_jacobian src/matrix_for_ee_to_camera.cpp src/ur5model.cpp include/ur5model.h src/gnuplot_ci.cpp include/gnuplot_ci.h)
#add_executable(deepak_jacob_test src/deepak_jacob_test.cpp src/MyFilledCircle.cpp src/image_processing.cpp src/compute_image_jacobian src/matrix_for_ee_to_camera.cpp src/ur5model.cpp include/ur5model.h src/gnuplot_ci.cpp include/gnuplot_ci.h)
#add_executable(deepak_lin_ang_ee src/deepak_lin_ang_ee.cpp src/MyFilledCircle.cpp src/image_processing.cpp src/compute_image_jacobian src/matrix_for_ee_to_camera.cpp src/ur5model.cpp include/ur5model.h src/gnuplot_ci.cpp include/gnuplot_ci.h)
#add_executable(kinematics_ur5 src/kinematics_ur5.cpp)

#add_executable(detect_blob src/detect_blob.cpp)



# Build Executable
#add_executable(gp_test src/gp_test.cpp)

add_definitions(-std=c++11)

# Linking GSL libraries
find_package(PkgConfig REQUIRED)
pkg_check_modules(gsl REQUIRED gsl)

# Linking GNUplot Library
#target_link_libraries(test_opencv ${OpenCV_LIBS})


#target_link_libraries(kinematics_ur5
#  ${catkin_LIBRARIES}
#  ${Boost_LIBRARIES}
#)

#target_link_libraries(ur5_ik
#  ${catkin_LIBRARIES}
#  ${OpenCV_LIBS}
#  ${PROJECT_SOURCE_DIR}/lib/libgnuplot_ci.a
#)

#target_link_libraries(visual_servoing
#  ${catkin_LIBRARIES}
#  ${OpenCV_LIBS}
#  ${PROJECT_SOURCE_DIR}/lib/libgnuplot_ci.a
#)

#target_link_libraries(ur5_result
#  ${catkin_LIBRARIES}
#  ${OpenCV_LIBS}
#  ${PROJECT_SOURCE_DIR}/lib/libgnuplot_ci.a
#)

#target_link_libraries(vs_som
#    ${catkin_LIBRARIES}
#    ${OpenCV_LIBS}
#    ${PCL_LIBRARIES}
#   )

#target_link_libraries(eyeInHand
#    ${catkin_LIBRARIES}
#    ${OpenCV_LIBS}
#    ${PCL_LIBRARIES}
#   )

#target_link_libraries(eyeInHand_v2
#    ${catkin_LIBRARIES}
#    ${OpenCV_LIBS}
#    ${PCL_LIBRARIES}
#   )
target_link_libraries(eyeinhand_6dof_tracking
    ${catkin_LIBRARIES}
    ${OpenCV_LIBS}
    ${PCL_LIBRARIES}
   )

#target_link_libraries(robot_jacobian_test
#    ${catkin_LIBRARIES}
#    ${OpenCV_LIBS}
#    ${PCL_LIBRARIES}
#   )

#target_link_libraries(deepak_jacob_test
#    ${catkin_LIBRARIES}
#    ${OpenCV_LIBS}
#    ${PCL_LIBRARIES}
#   )

#target_link_libraries(deepak_lin_ang_ee
#    ${catkin_LIBRARIES}
#    ${OpenCV_LIBS}
#    ${PCL_LIBRARIES}
#   )

#target_link_libraries(eyeinhand_6dof_depthtracking
#    ${catkin_LIBRARIES}
#    ${OpenCV_LIBS}
#    ${PCL_LIBRARIES}
#   )
#target_link_libraries(eyeinhand_6dof_4points
#    ${catkin_LIBRARIES}
#    ${OpenCV_LIBS}
#    ${PCL_LIBRARIES}
#   )

#target_link_libraries(eyeInHand_v3
#    ${catkin_LIBRARIES}
#    ${OpenCV_LIBS}
#    ${PCL_LIBRARIES}
#   )

#target_link_libraries(plotting
#    ${catkin_LIBRARIES}
#    ${OpenCV_LIBS}
#    ${PCL_LIBRARIES}
#   )

#target_link_libraries(eyeInHand_point_vs
#    ${catkin_LIBRARIES}
#    ${OpenCV_LIBS}
#    ${PCL_LIBRARIES}
#   )

target_link_libraries(calibration_transform_setup
    ${catkin_LIBRARIES}
    ${OpenCV_LIBS}
    ${PCL_LIBRARIES}
   )
#target_link_libraries(eyeinhand_6dof_area_tracking
#    ${catkin_LIBRARIES}
#    ${OpenCV_LIBS}
#    ${PCL_LIBRARIES}
#   )

#target_link_libraries(detect_blob
#    ${catkin_LIBRARIES}
#    ${OpenCV_LIBS}
#    ${PCL_LIBRARIES}
#   )

#target_link_libraries(gp_test ${PROJECT_NAME})
#install(TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_SOURCE_DIR}/lib)
