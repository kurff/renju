#ifndef __MCTS_HPP__
#define __MCTS_HPP__

#include "tree.hpp"
#include "proto/beta.pb.h"
#include <queue>
#include "core/parameters.hpp"
namespace Beta{
template <typename State, typename Action, typename Context, typename DataContext>
//multiple thread run
class MCTS{
    public:
        MCTS(){

        }
        ~MCTS(){

        }

        bool init(Parameters* parameters){

            tree_.reset(new Tree<State, Action, Context, DataContext>(
               parameters->L(),
               parameters->num_simulation(),
               parameters->tau(),
               parameters->v_resign(),
               parameters->epsilon(),
               parameters->num_thread(),
               parameters->board_size(),
               parameters->batch_size(),
               parameters->channels() 
            ));
            context_ .reset(new Context());
            return true;
        }

        void run(State root){
            tree_->run();

        }
    protected:
        shared_ptr<Context > context_;
        shared_ptr<Tree<State, Action, Context, DataContext> > tree_;
        




};
}
#endif