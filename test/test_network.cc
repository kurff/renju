#include "gtest/gtest.h"
#include "glog/logging.h"
#include <memory>
#include "network/network.h"
#include "core/state.h"
using namespace Beta;
TEST(NETWORK, build){
    Network<int, int> network;
    network.init("lenet_solver.prototxt");


}