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
float counter = 0;
float* pattern = new float [10]();
for(auto& x: actions){
    x.set_confidence(1.0f*counter);
    ++ counter;
}

std::random_device device;
std::mt19937 generator(device());


Sample<ChessAction> sample;
for(int i = 0; i < 100; ++ i){
    int x = sample.run(actions, generator);
    pattern[x] ++; 
}
for(int i = 0; i < 10; ++ i){
    LOG(INFO)<< pattern[i];
}

}