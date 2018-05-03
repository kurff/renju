#include "gtest/gtest.h"
#include "glog/logging.h"
#include "utils/utils.hpp"
#include "core/action.hpp"
#include <vector>
#include <random>
using namespace std;
using namespace Beta;
TEST(random, generator){
vector<ChessAction> actions;
actions.resize(10);

Dirichlet dirichlet(0.1);
dirichlet.push(0.1);
dirichlet.push(0.9);
LOG(INFO)<<"dirichilet: " <<dirichlet.run();


float counter = 0;
float* pattern = new float [10]();
for(auto& x: actions){
    x.set_confidence(1.0f*counter);
    ++ counter;
}

vector<ChessAction* > acts;
for(int i = 0; i < 10; ++ i){
    ChessAction* ptr = new ChessAction();
    ptr->set_confidence(1.0f*i);
    acts.push_back(ptr);
}

std::random_device device;
std::mt19937 generator(device());


Sample<ChessAction> sample;
for(int i = 0; i < 100; ++ i){
    //int x = sample.run(actions, generator);
    int x = sample.run(acts, generator);
    pattern[x] ++; 
}


for(int i = 0; i < 10; ++ i){
    LOG(INFO)<< pattern[i];
}

}