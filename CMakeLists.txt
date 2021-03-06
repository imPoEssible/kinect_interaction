cmake_minimum_required(VERSION 2.8.3)

project(poe_kinect)

find_package(orocos_kdl REQUIRED)
find_package(catkin REQUIRED COMPONENTS geometry_msgs
					roscpp
					roslib
					tf
					rospy
					std_msgs
					sensor_msgs
					cv_bridge
					image_transport
					message_generation
					)

# Find OpenNI
find_package(PkgConfig)
pkg_check_modules(OpenNI REQUIRED libopenni)

#Find GLUT
find_package(GLUT REQUIRED)

add_message_files(
	FILES
	pointerpos.msg
	gestures.msg
)

generate_messages(
	DEPENDENCIES
	#pointerpos
	geometry_msgs
	std_msgs
	sensor_msgs
)

catkin_package(
	CATKIN_DEPENDS message_runtime std_msgs sensor_msgs)

include_directories(${catkin_INCLUDEDIR}
		    ${OpenNI_INCLUDEDIR}
		    ${orocos_kdl_INCLUDE_DIRS}
		    ${OpenCV_INCLUDE_DIRS}
		    ${OpenGL_INCLUDE_DIRS}
		    ${GLUT_LIBRARIES})


include_directories("nite/Include")
include_directories("nite/Source/XnCommon")
include_directories("${PROJECT_SOURCE_DIR}/include/poe_kinect")

link_directories(${catkin_LIBRARY_DIRS})
link_directories(${orocos_kdl_LIBRARY_DIRS}
		 ${GLUT_LIBRARIES}
)


add_executable(detectfinger src/SingleControl/main.cpp)
add_dependencies(detectfinger geometry_msgs_gencpp ${PROJECT_NAME}_generate_messages_cpp)
target_link_libraries(detectfinger ${catkin_LIBRARIES}
				   ${OpenNI_LIBRARIES}
				   ${orocos_kdl_LIBRARIES}
				   ${OpenGL_INCLUDE_DIRS} -lXnVNite_1_5_2)


add_executable(detectpeople src/detectpeople.cpp)
add_dependencies(detectpeople geometry_msgs_gencpp ${PROJECT_NAME}_generate_messages_cpp)
target_link_libraries(detectpeople ${catkin_LIBRARIES}
				   ${OpenNI_LIBRARIES}
				   ${orocos_kdl_LIBRARIES}
				   ${OpenGL_INCLUDE_DIRS} -lXnVNite_1_5_2)

#add_executable(skeleton src/Skeleton/openni_tracker.cpp)
#add_dependencies(skeleton geometry_msgs_gencpp)
#target_link_libraries(skeleton ${catkin_LIBRARIES}
#			       ${OpenNI_LIBRARIES}
#			       ${orocos_kdl_LIBRARIES})


#add_executable(viewer src/PointViewer/main.cpp
#			src/PointViewer/kbhit.cpp
#			src/PointViewer/opengles.cpp
#			src/PointViewer/signal_catch.cpp)

#add_dependencies(viewer geometry_msgs_gencpp)
#target_link_libraries(viewer ${catkin_LIBRARIES}
#				   ${OpenNI_LIBRARIES}
#				   ${orocos_kdl_LIBRARIES}
#				   ${OpenGL_INCLUDE_DIRS}
#				   ${GLUT_LIBRARIES}
#				   -lXnVNite_1_5_2)

install(TARGETS detectfinger detectpeople
	RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

install(DIRECTORY include/${PROJECT_NAME}/
	DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})

install(DIRECTORY launch/
	DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
	PATTERN ".svn" EXCLUDE)

install(FILES package.xml
	DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
