
#include "core/state.h"
#include "core/renju_action.h"
#include "core/renju_context.h"
namespace Beta{
    template<typename StateType, typename ActionType>
    RenJuContext<StateType, ActionType>::RenJuContext(float epsilon): Context<StateType,ActionType>(epsilon){

    }

    template<typename StateType, typename ActionType>
    RenJuContext<StateType, ActionType>::~RenJuContext(){

    }

    template<typename StateType, typename ActionType>
    void RenJuContext<StateType, ActionType>::get_legal_action(const StateType& state){

    }

    template class RenJuContext<State, RenJuAction>;











}