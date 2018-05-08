cmake_minimum_required(VERSION 3.5)
project(Beta)
option(test "Build all tests." ON)
SET(CMAKE_C_COMPILER g++)
add_compile_options(-std=c++11)

link_directories(/usr/local/lib)
link_directories()

if (test)
enable_testing()
add_executable(test_tree  test/test_tree.cc proto/beta.pb.cc)
target_link_libraries(test_tree gtest gtest_main glog protobuf caffe2)
add_test(NAME test_tree COMMAND test_tree)

add_executable(test_example  test/test_example.cc )
target_link_libraries(test_example gtest gtest_main glog pthread)
add_test(NAME test_example COMMAND test_example )


add_executable(test_random  test/test_random.cc )
target_link_libraries(test_random gtest gtest_main glog pthread)
add_test(NAME test_random COMMAND test_random)


add_executable(test_network  test/test_network.cc network/net.cc)
target_link_libraries(test_network gtest gtest_main glog caffe2  protobuf pthread)
add_test(NAME test_network COMMAND test_network)
endif()

include_directories(./)
include_directories(/usr/local/include)


#add_library(libproto proto/beta.pb.cc)
add_executable(toy test/toy.cc )
target_link_libraries(toy  caffe2 protobuf)
add_executable(test_caffe2 test/test_caffe2.cc network/net.cc)
target_link_libraries(test_caffe2 caffe2 protobuf glog)
