#include "core/state.h"
#include "core/renju_context.h"
#include "core/renju_action.h"
#include "mcts/mcts.h"

namespace Beta{

    template<typename StateType, typename ActionType, typename ContextType>
    MCTS::MCTS(){

    }

    template<typename StateType, typename ActionType, typename ContextType>    
    MCTS::~MCTS(){

    } 

    template<typename StateType, typename ActionType, typename ContextType>
    bool MCTS::init(Parameters* parameters){
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

    template<typename StateType, typename ActionType, typename ContextType>
    void MCTS::run(){
        NodeDef* root = new NodeDef("root");
        tree_->run(root);
    }

    




}