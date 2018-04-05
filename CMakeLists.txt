cmake_minimum_required(VERSION 3.5)
project(Beta)
option(test "Build all tests." ON)
SET(CMAKE_C_COMPILER g++)
add_compile_options(-std=c++11)

link_directories(/usr/local/lib)

if (test)
enable_testing()
#add_executable(test_tree  test/test_tree.cc proto/beta.pb.cc)
#target_link_libraries(test_tree gtest gtest_main glog protobuf)
#add_test(NAME test_tree COMMAND test_tree)

add_executable(test_example  test/test_example.cc )
target_link_libraries(test_example gtest gtest_main glog )
add_test(NAME test_example COMMAND test_example)


add_executable(test_random  test/test_random.cc )
target_link_libraries(test_random gtest gtest_main glog )
add_test(NAME test_random COMMAND test_random)

endif()

include_directories(./)
include_directories(/usr/local/include)


#add_library(libproto proto/beta.pb.cc)
#add_executable(Demo main.cpp )

#target_link_libraries(Demo  libproto glog protobuf )

