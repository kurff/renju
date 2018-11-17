#ifndef MCTS_CLIENT_H_
#define MCTS_CLIENT_H_


#include "mcts/mcts.h"
#include "proto/server.pb.h"
#include "proto/server.grpc.pb.h"

using namespace grpc;




namespace Beta{
    template<typename StateType, typename ActionType, typename ContextType>
    class MCTSClient : public MCTS<StateType, ActionType, ContextType>, {
        public:
            MCTSClient(std::shared_ptr<grpc::Channel> channel);
            ~MCTSClient();

            void upload();
        protected:
            std::unique_ptr<Evaluation::Stub> stub_;
            
            



    };




}
#endif