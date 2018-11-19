#include "core/state.h"
#include "core/renju_context.h"
#include "core/renju_action.h"
#include "mcts/mcts.h"

namespace Beta{

    template<typename StateType, typename ActionType, typename ContextType>
    MCTS<StateType, ActionType, ContextType>::MCTS(){

    }

    template<typename StateType, typename ActionType, typename ContextType>    
    MCTS<StateType, ActionType, ContextType>::~MCTS(){

    } 

    template<typename StateType, typename ActionType, typename ContextType>
    bool MCTS<StateType, ActionType, ContextType>::init(Parameters* parameters){
        tree_.reset(new Tree<StateType, ActionType>(
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
    void MCTS<StateType, ActionType, ContextType>::run(){
        NodeDef* root = new NodeDef("root");
        tree_->run(root);
    }

    template class MCTS<State, RenJuAction, RenJuContext<State, RenJuAction> > ;

    




}