#ifndef MCTS_CLIENT_H_
#define MCTS_CLIENT_H_
#include <string>
#include <grpc++/grpc++.h>
#include "utils/simple_queue.h"
#include "mcts/mcts.h"
#include "proto/server.pb.h"
#include "proto/server.grpc.pb.h"
#include "mcts/thread_pool.h"
using namespace std;
using namespace grpc;





namespace Beta{
    template<typename StateType, typename ActionType, typename ContextType>
    class MCTSClient : public MCTS<StateType, ActionType, ContextType> {
        public:
            MCTSClient(std::shared_ptr<grpc::Channel> channel);
            ~MCTSClient();

            string evaluate_train();
            string evaluate_test();
            void upload();

            // for debug
            string fake_example();


        protected:
            std::unique_ptr<Evaluation::Stub> stub_;
            std::shared_ptr<SimpleQueue< Request > > queue_;
            std::shared_ptr<TaskThreadPool> thread_pool_;
    };

    




}
#endif