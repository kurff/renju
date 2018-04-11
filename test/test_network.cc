#include "gtest/gtest.h"
#include "glog/logging.h"
#include <memory>
#include "network/network.hpp"
using namespace Beta;
TEST(NETWORK, build){
    NetDef init_model;
    NetDef predict_model;
    Network<int , int> net(init_model, predict_model);
    net.init(0);
    net.create_lenet(1);
    //net.add_loss();
    net.create();
    net.save("lenet");
    net.train();


}