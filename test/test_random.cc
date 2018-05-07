#include "gtest/gtest.h"
#include "glog/logging.h"
#include "utils/utils.hpp"
#include "mcts/tree.hpp"
#include "core/state.hpp"
#include "core/action.hpp"
#include <vector>
#include <random>
using namespace std;
using namespace Beta;
TEST(random, generator){
Dirichlet dirichlet(0.2);
vector<float> prior;
for(int i = 0; i < 2; ++ i){
    dirichlet.run(prior,5);
    LOG(INFO)<<"run: prior"<< prior.size();
    for(int j = 0; j < 5; ++ j){
        cout<< prior[j]<<" ";
    }
    cout<<endl;
}

// float counter = 0;
// float* pattern = new float [10]();
// vector<Node<State, ChessAction> * > acts;
// for(int i = 0; i < 10; ++ i){
//     Node<State, ChessAction>* ptr = new Node<State, ChessAction>(std::to_string(i));
//     acts.push_back(ptr);
// }




// Sample<Node<State, ChessAction> > sample;
// for(int i = 0; i < 100; ++ i){
//     //int x = sample.run(actions, generator);
//     int x = sample.run(acts);
//     pattern[x] ++; 
// }


// for(int i = 0; i < 10; ++ i){
//     LOG(INFO)<< pattern[i];
// }

}