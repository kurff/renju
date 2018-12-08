#include "core/state.h"
#include "core/renju_context.h"
#include "core/renju_action.h"
#include "mcts/mcts_client.h"
namespace Beta{
    template<typename StateType, typename ActionType, typename ContextType>
    MCTSClient<StateType, ActionType, ContextType>::MCTSClient(std::shared_ptr<grpc::Channel> channel) 
    : MCTS<StateType, ActionType, ContextType>(), stub_(Evaluation::NewStub(channel)), queue_(new SimpleQueue<Request>() ){
        

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
        Request request;
        queue_->Pop(&request);
        Reply reply;
        Status status = stub_->Forward_train(&grpc_context, request, &reply);
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
        Request request;
        queue_->Pop(&request);
        Reply reply;
        Status status = stub_->Forward_test(&grpc_context, request, &reply);
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
        Request request;
        
        Reply reply;
        Status status = stub_->Forward_test(&grpc_context, request, &reply);
        if (status.ok()) {
            LOG(INFO)<< reply.loss();
            return "success";
        }else {
            std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
            return "RPC failed";
        }
    }

    template<typename StateType, typename ActionType, typename ContextType>
    string MCTSClient<StateType, ActionType, ContextType>::load_example(const string& file){


    }

    template<typename StateType, typename ActionType, typename ContextType>
    string MCTSClient<StateType, ActionType, ContextType>::load_several_examples(const string& file_list){
        

    }


    template class MCTSClient<State, RenJuAction, RenJuContext<State, RenJuAction> >;

}