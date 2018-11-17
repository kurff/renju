#include "mcts/mcts_client.h"
namespace Beta{
    template<typename StateType, typename ActionType, typename ContextType>
    MCTSClient<StateType, ActionType, ContextType>::MCTSClient(std::shared_ptr<grpc::Channel> channel) : stub_(Evaluation::NewStub(channel)){
        

    }
    
    template<typename StateType, typename ActionType, typename ContextType>
    MCTSClient<StateType, ActionType, ContextType>::~MCTSClient(){


    }

    template<typename StateType, typename ActionType, typename ContextType>
    void MCTSClient<StateType, ActionType, ContextType>::upload(){
        



    }







}