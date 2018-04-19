#include "gtest/gtest.h"
#include "glog/logging.h"
#include <memory>
#include "network/network.hpp"
using namespace Beta;
TEST(NETWORK, build){
    NetDef init_model;
    NetDef predict_model;
    NetDef update_model;
    Network<int , int, TensorCPU> net(15,16,3);

    net.init(init_model, predict_model,update_model, 0);
    net.create_lenet(1);
    net.add_loss();
    net.allocate();
    net.save("lenet");
    net.init_parameters();
    LOG(INFO)<<"update";
    for(int i =0; i< 100; ++ i)
        net.update_parameters();


}