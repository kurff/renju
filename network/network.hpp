#ifndef __NETWORK_HPP__
#define __NETWORK_HPP__
#include "network/net.h"
#include <memory>
#include <string>
namespace Beta{
    
    template<typename State, typename Action>
    class Network{
        public:
            Network(NetDef& init_model, NetDef& predict_model):init_model_(init_model), predict_model_(predict_model){
                init_net_.reset(new Net(init_model_));
                predict_net_.reset(new Net(predict_model_));
                workspace_.reset(new Workspace("workspace"));
            }
            ~Network(){


            }

        public:
            
            void init(bool use_gpu){
                if(use_gpu){
                    init_net_->SetDeviceCUDA();
                    predict_net_->SetDeviceCUDA();
                }

            }


            void create_network(bool training){
 

            }

            void create_lenet(bool training){
                predict_net_->AddConvOp("data", "conv1_w", "conv1_b", "conv1", 1, 0, 5);
                predict_net_->AddInput("conv1_w");
                predict_net_->AddInput("conv1_b");
                if (training) {
                    init_net_->AddXavierFillOp({20, 1, 5, 5}, "conv1_w");
                    init_net_->AddConstantFillOp({20}, "conv1_b");
                }
                predict_net_->AddMaxPoolOp("conv1", "pool1", 2, 0, 2);

                predict_net_->AddConvOp("pool1", "conv2_w", "conv2_b", "conv2", 1, 0, 5);
                predict_net_->AddInput("conv2_w");
                predict_net_->AddInput("conv2_b");
                if (training) {
                    init_net_->AddXavierFillOp({50, 20, 5, 5}, "conv2_w");
                    init_net_->AddConstantFillOp({50}, "conv2_b");
                }
                predict_net_->AddMaxPoolOp("conv2", "pool2", 2, 0, 2);
                predict_net_->AddFcOp("pool2", "fc3_w", "fc3_b", "fc3");
                predict_net_->AddInput("fc3_w");
                predict_net_->AddInput("fc3_b");
                if (training) {
                    init_net_->AddXavierFillOp({500, 800}, "fc3_w");
                    init_net_->AddConstantFillOp({500}, "fc3_b");
                }
                predict_net_->AddReluOp("fc3", "fc3");
                predict_net_->AddFcOp("fc3", "pred_w", "pred_b", "pred");
                predict_net_->AddInput("pred_w");
                predict_net_->AddInput("pred_b");
                if (training) {
                    init_net_->AddXavierFillOp({10, 500}, "pred_w");
                    init_net_->AddConstantFillOp({10}, "pred_b");
                }
                predict_net_->AddSoftmaxOp("pred", "softmax");


            }


            void train(){

            }


            void inference(const State& state, Action& action, float& v){


            }

            void save(string proto_name){
                WriteProtoToTextFile(predict_model_,proto_name+"_pred.pbtxt");
                WriteProtoToTextFile(init_model_, proto_name+"_init.pbtxt");
            }

        protected:
            std::shared_ptr<Net> init_net_;
            std::shared_ptr<Net> predict_net_;
            std::shared_ptr<Workspace> workspace_;
            NetDef init_model_;
            NetDef predict_model_;


    };




}







#endif