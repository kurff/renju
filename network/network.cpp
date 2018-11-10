#include "core/state.h"
#include "core/action.h"
#include "core/renju_action.h"
#include "network/network.h"

namespace Beta{
    template<typename StateType, typename ActionType>
    Network<StateType, ActionType>::Network(){

    }
    template<typename StateType, typename ActionType>
    Network<StateType, ActionType>::~Network(){


    }

    template<typename StateType, typename ActionType>
    void Network<StateType, ActionType>::init(const SolverParameter& param){
        solver_.reset(SolverRegistry<float>::CreateSolver(param));
        

    }

    template<typename StateType, typename ActionType>
    void Network<StateType, ActionType>::update(){

    }

    template<typename StateType, typename ActionType>
    void Network<StateType, ActionType>::forward_train(const StateType& state){
        

    }
    template<typename StateType, typename ActionType>
    void Network<StateType, ActionType>::forward_test(const StateType& state){

    }
    template<typename StateType, typename ActionType>
    void Network<StateType, ActionType>::init(const std::string& prototxt, const std::string& model_file){
        net_.reset(new Net<float>(prototxt, TEST));
        net_->CopyTrainedLayersFrom(model_file);


    }
    template class Network<State, RenJuAction>;
    template class Network<int , int>;
    //template class Network<State, RenJuAction>;







}