#include "gtest/gtest.h"
#include "glog/logging.h"
#include <memory>
#include "network/network.hpp"
using namespace Beta;
TEST(NETWORK, build){
    NetDef init_model;
    NetDef predict_model;
    NetDef update_model;
    Network<int , int, TensorCPU> net(15,1,3);

    net.init(init_model, predict_model,update_model, 0);
    net.create_lenet(1);
    net.add_loss();
    net.allocate();
    net.save("lenet");
    //net.train();


}