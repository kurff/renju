#include "core/state.h"
#include "network/NetServer.h"
#include "glog/logging.h"
namespace Beta{
    template<typename StateType>
    NetServer<StateType>::NetServer() : Evaluation::Service(), Network<StateType>(){
        state_.reset(new StateType());
    }

    template<typename StateType>
    NetServer<StateType>::~NetServer(){

    }


    template<typename StateType>
    Status NetServer<StateType>::Forward_test(ServerContext* context, const Request* request, Reply* response){
        //response->set_loss(2.0);
        //request->s()
        int size = request->s().shape().dim_size();
        LOG(INFO)<< size <<" ";
        int total_size = 1;
        int number = request->s().shape().dim(0);
        int channel = request->s().shape().dim(1);
        int height = request->s().shape().dim(2);
        int width = request->s().shape().dim(3);
        for(int i = 0; i < size; ++ i){
            LOG(INFO)<< request->s().shape().dim(i);
            total_size *= request->s().shape().dim(i);
        }

        state_->init_input(number, channel, height, width);

        LOG(INFO)<< "init input";
        const float* input = request->s().data().data();

        LOG(INFO)<<"set input";
        state_->set_input(number*channel*height*width, input);

        LOG(INFO)<<"forward ";
        this->forward_test(state_.get());

        return Status::OK;
    }

    template<typename StateType>
    Status NetServer<StateType>::Forward_train(ServerContext* context, const Request* request, Reply* response){


        return Status::OK;
    }

    template class NetServer<State>;









}