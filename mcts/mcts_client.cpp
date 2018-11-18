
#include "mcts/mcts_client.h"
namespace Beta{
    template<typename StateType, typename ActionType, typename ContextType>
    MCTSClient<StateType, ActionType, ContextType>::MCTSClient(std::shared_ptr<grpc::Channel> channel) 
    : MCTS<StateType, ActionType, ContextType>(), stub_(Evaluation::NewStub(channel)), queue_(new SimpleQueue<std::shared_ptr< Request> >() ){
        

    }
    
    template<typename StateType, typename ActionType, typename ContextType>
    MCTSClient<StateType, ActionType, ContextType>::~MCTSClient(){


    }

    template<typename StateType, typename ActionType, typename ContextType>
    void MCTSClient<StateType, ActionType, ContextType>::upload(){
        


    }


    template<typename StateType, typename ActionType, typename ContextType>
    string MCTSClient<StateType, ActionType, ContextType>::evaluate_train(){
        grpc::ClientContext grpc_context;
        std::shared_ptr<Request> request;
        queue_.pop(request);
        Reply reply;
        Status status = stub_->Forward_train(&grpc_context, request.get(), &reply);
        if (status.ok()) {
            return "success";
        }else {
            std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
            return "RPC failed";
        }
    }


    template<typename StateType, typename ActionType, typename ContextType>
    string MCTSClient<StateType, ActionType, ContextType>::evaluate_test(){
        grpc::ClientContext grpc_context;
        std::shared_ptr<Request> request;
        queue_.pop(request);
        Reply reply;
        Status status = stub_->Forward_test(&grpc_context, request.get(), &reply);
        if (status.ok()) {
            return "success";
        }else {
            std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
            return "RPC failed";
        }
    }

    template<typename StateType, typename ActionType, typename ContextType>
    string MCTSClient<StateType, ActionType, ContextType>::fake_example(){
        grpc::ClientContext grpc_context;
        std::shared_ptr<Request> request(new Request());
        
        Reply reply;
        Status status = stub_->Forward_test(&grpc_context, request.get(), &reply);
        if (status.ok()) {
            LOG(INFO)<< reply.loss();
            return "success";
        }else {
            std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
            return "RPC failed";
        }
    } 

}