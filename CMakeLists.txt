cmake_minimum_required(VERSION 3.5)
project(Beta)
option(test "Build all tests." ON)
SET(CMAKE_C_COMPILER g++)
add_compile_options(-std=c++11)

#set(CAFFE_LIBS caffe2 caffe2_protos)
set(CAFFE_LIBS caffe)

link_directories(/usr/local/lib)
link_directories(/usr/lib/graphviz /usr/lib/)


include_directories(/usr/include/graphviz)
include_directories(/usr/local/Cellar/graphviz/2.40.1/include/graphviz/)
include_directories(./)
include_directories(caffe/distribute/include)
include_directories(/usr/local/cuda/include/)
include_directories(caffe/distribute/include/caffe/proto)

link_directories(/usr/local/cuda/lib64/)
link_directories(caffe/distribute/lib/)

add_executable(test_network test/test_network.cc network/network.cpp core/state.cpp network/NetServer.cpp proto/server.pb.cc proto/server.grpc.pb.cc)
target_link_libraries(test_network ${CAFFE_LIBS} glog grpc grpc++ protobuf boost_system boost_filesystem gtest gtest_main)

#add_executable(test )


add_executable(simple_queue_test utils/simple_queue_test.cc)
target_link_libraries(simple_queue_test gtest gtest_main glog pthread)

add_executable(run_client run_client.cpp mcts/mcts_client.cpp mcts/mcts.cpp  mcts/tree.cpp mcts/node.cpp proto/beta.pb.cc proto/server.grpc.pb.cc proto/server.pb.cc
core/renju_context.cpp core/context.cpp core/renju_action.cpp core/state.cpp )
target_link_libraries(run_client ${CAFFE_LIBS} glog grpc grpc++ protobuf opencv_core opencv_imgproc opencv_imgcodecs boost_system boost_filesystem gflags gvc cgraph)



add_executable(run_server run_server.cpp network/NetServer.cpp core/state.cpp proto/server.pb.cc proto/server.grpc.pb.cc network/network.cpp)
target_link_libraries(run_server ${CAFFE_LIBS} glog grpc grpc++ protobuf boost_system boost_filesystem gtest gtest_main gflags)





#if (test)
#enable_testing()
#add_executable(test_tree  test/test_tree.cc proto/beta.pb.cc)
#target_link_libraries(test_tree gtest gtest_main glog protobuf ${CAFFE_LIBS} pthread gvc cgraph)
#add_test(NAME test_tree COMMAND test_tree )

#add_executable(test_example  test/test_example.cc )
#target_link_libraries(test_example gtest gtest_main glog pthread)
#add_test(NAME test_example COMMAND test_example )


#add_executable(test_random  test/test_random.cc )
#target_link_libraries(test_random gtest gtest_main glog pthread)
#add_test(NAME test_random COMMAND test_random)


#add_executable(test_network  test/test_network.cc network/net.cc)
#target_link_libraries(test_network gtest gtest_main glog ${CAFFE_LIBS} protobuf pthread)
#add_test(NAME test_network COMMAND test_network)
#endif()

#include_directories(./)
#include_directories(/usr/local/include)


#add_executable(test_graphviz test/test_graphviz.cc)
#target_link_libraries(test_graphviz gvc cgraph glog)

#add_library(libproto proto/beta.pb.cc)
#add_executable(toy test/toy.cc )
#target_link_libraries(toy  ${CAFFE_LIBS} protobuf)

#add_executable(test_mcts test/test_mcts.cc proto/beta.pb.cc network/net.cc)
#target_link_libraries(test_mcts ${CAFFE_LIBS} protobuf glog pthread gvc cgraph)