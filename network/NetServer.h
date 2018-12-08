#ifndef NETWORK_NETSERVER_H_
#define NETWORK_NETSERVER_H_
#include <string>
#include <memory>
#include "network/network.h"
#include "proto/server.pb.h"
#include "proto/server.grpc.pb.h"


using namespace google::protobuf;
using namespace grpc;

namespace Beta{
    template<typename StateType>
    class NetServer : public Beta::Evaluation::Service, public Network<StateType> {
        public:
            NetServer();


            Status Forward_test(ServerContext* context, const Request* request, Reply* response);

            Status Forward_train(ServerContext* context, const Request* request, Reply* response);

            ~NetServer();

        protected:
            std::shared_ptr<StateType> state_;
            


    };



}





#endif