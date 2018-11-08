#include "network/network.h"

namespace Beta{
    template<typename StateType, typename ActionType>
    Network::Network(){

    }
    template<typename StateType, typename ActionType>
    Network::~Network(){


    }

    template<typename StateType, typename ActionType>
    void Network::init(const std::string& solver_file){
        solver_.reset(new SGDSolver<float>(solver_file));
        

    }

    template<typename StateType, typename ActionType>
    void Network::update(){

    }

    template<typename StateType, typename ActionType>
    void Network::forward(){

        
    }
    template<typename StateType, typename ActionType>
    void Network::init(const std::string& prototxt, const std::string& model_file){
        net_.reset(new Net<float>(prototxt, TEST));
        net_->CopyTrainedLayersFrom(model_file);


    }







}