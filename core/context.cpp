#include "core/context.h"
#include "core/state.h"
#include "core/renju_action.h"
namespace Beta{
    template<typename StateType, typename ActionType>
    Context<StateType, ActionType>::Context(float epsilon):epsilon_(epsilon){

    }

    template<typename StateType, typename ActionType>
    Context<StateType, ActionType>::~Context(){

    }

    template class Context<State, RenJuAction>;

    
    





}
