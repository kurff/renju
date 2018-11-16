#ifndef NETWORK_SERVER_H_
#define NETWORK_SERVER_H_
#include <string>
#include "network/network.h"
#include "proto/server.pb.h"
#include "proto/server.grpc.pb.h"


using namespace google::protobuf;
using namespace grpc;

namespace Beta{
    template<typename StateType>
    class Server : public Beta::Evaluation::Service {
        public:
            Server();


            void init_train(const std::string& solver_file);

            void init_test(const std::string& prototxt, const std::string& caffemodel);

            Status Forward_test(ServerContext* context, const Request* request, Reply* response);

            Status Forward_train(ServerContext* context, const Request* request, Reply* response);
            
            ~Server();
            void run_train();
            void run_test();
        protected:
            
            std::shared_ptr<Network<StateType> > network_;


    };



}





#endif