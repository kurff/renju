
#include "gflags/gflags.h"
#include "glog/logging.h"
#include <grpc++/grpc++.h>
#include "core/state.h"
#include "proto/server.pb.h"
#include "proto/server.grpc.pb.h"
#include "network/NetServer.h"

DEFINE_string(solver_file, "lenet_solver.protottxt", "solver file of network");
DEFINE_string(prototxt, "*.prototxt", "prototxt file of caffe model");
DEFINE_string(caffemodel, "*.caffemodel", "weight file of caffe model");
DEFINE_string(server_address, "server address", "server address");

using namespace Beta;

int main(int argc, char** argv){

    ::google::InitGoogleLogging(argv[0]);
    // Print output to stderr (while still logging)
    FLAGS_alsologtostderr = 1;

    gflags::SetUsageMessage("run_client to run mcts search.\n"
        "Usage:\n"
        "    run_client --server_address"
        "The ImageNet dataset for the training demo is at\n"
        "    http://www.image-net.org/download-images\n");
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    //if (argc != 3 ) {
    gflags::ShowUsageWithFlagsRestrict(argv[0], "run_server");


    std::shared_ptr<NetServer<State> > service(new NetServer<State>());
    grpc::ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(FLAGS_server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(service);
    // Finally assemble the server.
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << FLAGS_server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
    server->Wait();

    //server->


}

