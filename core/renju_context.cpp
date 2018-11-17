
#include "core/state.h"
#include "core/renju_action.h"
#include "core/renju_context.h"
namespace Beta{
    template<typename StateType, typename ActionType>
    RenjuContext<StateType, ActionType>::RenjuContext(){

    }

    template<typename StateType, typename ActionType>
    RenjuContext<StateType, ActionType>::~RenjuContext(){

    }

    template<typename StateType, typename ActionType>
    void RenjuContext::get_legal_action(const StateType& state){

    }

    template class RenjuContext<State, RenJuAction>;











}