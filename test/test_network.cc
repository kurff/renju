#include "gtest/gtest.h"
#include "glog/logging.h"
#include <memory>
#include "network/network.hpp"
#include "core/state.hpp"
using namespace Beta;
TEST(NETWORK, build){
    NetDef init_model;
    NetDef predict_model;
    NetDef update_model;
    Network<State , int, CPUContext> net(15,16,3);

    net.init(init_model, predict_model,update_model, 0);
    //net.create_lenet(1);
    LOG(INFO)<<"create network";
    net.create_network(1);
    //net.add_loss();
    
    net.allocate();
    net.save("lenet");
    net.init_parameters();
    LOG(INFO)<<"update";
    State state;
    state.init(16,15,3);

    net.forward(state);

    for(int i =0; i< 1; ++ i)
        net.update_parameters();
    net.save_model("model");
    net.load_model("model");

}