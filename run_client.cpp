#include "gflags/gflags.h"
#include "glog/logging.h"

#include "core/state.h"
#include "core/renju_action.h"
#include "core/renju_context.h"
#include "mcts/mcts_client.h"

DEFINE_string(channel, "192.168.0.107:5342", "channel of client");
DEFINE_string(param, "lenet_solver.prototxt", "parameter file of ");
DEFINE_string(image_name, "mnist_png/testing/0/3.png", "image name");

using namespace Beta;
int main(int argc, char** argv){

    ::google::InitGoogleLogging(argv[0]);
  // Print output to stderr (while still logging)
    FLAGS_alsologtostderr = 1;

    gflags::SetUsageMessage("run_client to run mcts search.\n"
        "Usage:\n"
        "    run_client --channel --param "
        "channel for distribute learning\n"
        "    http://www.image-net.org/download-images\n");
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    
    gflags::ShowUsageWithFlagsRestrict(argv[0], "run client");
  
    std::shared_ptr<grpc::Channel> channel(grpc::CreateChannel(
      FLAGS_channel, grpc::InsecureChannelCredentials()));
    std::shared_ptr<MCTSClient<State,RenJuAction, RenJuContext<State, RenJuAction>> > mcts(
        new MCTSClient<State, RenJuAction, RenJuContext<State, RenJuAction> >( channel)
    );
    std::string file = FLAGS_image_name;
    mcts->load_example(file);









}


