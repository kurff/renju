#include "network/server.h"

namespace Beta{
    template<typename StateType>
    Server<StateType>::Server(){
        network_.reset(new Network<StateType>());
    }

    template<typename StateType>
    Server<StateType>::init_train(const std::string& solver_file){
        network_->init(solver_file);
    }

    template<typename StateType>
    Server<StateType>::init_test(const std::string& prototxt, const std::string& caffemodel){
        network_->init(prototxt, caffemodel);
    }


    template<typename StateType>
    Server<StateType>::~Server(){

    }

    template<typename StateType>
    Status Server<StateType>::Forward_test(ServerContext* context, const Request* request, Reply* response){
        

        return Status::OK;
    }

    template<typename StateType>
    Status Server<StateType>::Forward_train(ServerContext* context, const Request* request, Reply* response){


        return Status::OK;
    }











}