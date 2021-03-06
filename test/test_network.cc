#include "gtest/gtest.h"
#include "glog/logging.h"
#include <memory>
#include "network/network.h"
#include "core/state.h"

#include "network/NetServer.h"
using namespace Beta;
TEST(NETWORK, build){
    Network<State> network;
    network.init("lenet_solver.prototxt");
    State* state = new State();
    state->init_input(64,1,28,28);
    state->init_prob(64,1,1,1);
    int x = 0;
    network.forward_train(state);


}

TEST(SERVER, build){
    NetServer<State>* server = new NetServer<State>();
    server->init("lenet_solver.prototxt");




}

TEST(NETWORK, test){
    



}

