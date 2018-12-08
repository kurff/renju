#include "core/state.h"
#include "network/NetServer.h"

namespace Beta{
    template<typename StateType>
    NetServer<StateType>::NetServer(){
        state_.reset(new StateType());
    }

    template<typename StateType>
    NetServer<StateType>::~NetServer(){

    }

    template<typename StateType>
    Status NetServer<StateType>::Forward_test(ServerContext* context, const Request* request, Reply* response){
        //response->set_loss(2.0);
        //request->s()





        return Status::OK;
    }

    template<typename StateType>
    Status NetServer<StateType>::Forward_train(ServerContext* context, const Request* request, Reply* response){


        return Status::OK;
    }

    template class NetServer<State>;









}