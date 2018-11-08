#ifndef __MCTS_HPP__
#define __MCTS_HPP__

#include "tree.hpp"
#include "proto/beta.pb.h"
#include <queue>
#include "core/parameters.hpp"
namespace Beta{
template <typename StateType, typename ActionType, typename ContextType>
//multiple thread run
class MCTS{
    typedef Node<StateType, ActionType> NodeDef;
    public:
        MCTS(){

        }
        ~MCTS(){

        }

        bool init(Parameters* parameters){
            tree_.reset(new Tree<StateType, ActionType, ContextType>(
               parameters->l(),
               parameters->num_simulation(),
               parameters->tau(),
               parameters->v_resign(),
               parameters->epsilon(),
               parameters->num_thread(),
               parameters->board_size(),
               parameters->batch_size(),
               parameters->channels() 
            ));
            context_.reset(new ContextType(parameters->epsilon()));
            return true;
        }

        void run(){
            NodeDef* root = new NodeDef("root");
            tree_->run(root);


        }
    protected:
        shared_ptr<ContextType > context_;
        shared_ptr<Tree<StateType, ActionType, ContextType> > tree_;
        




};
}
#endif