#ifndef __NETWORK_HPP__
#define __NETWORK_HPP__
#include "network/net.h"
#include "glog/logging.h"
#include <memory>
#include <string>
#include <vector>
namespace Beta{
    
    template<typename State, typename Action, typename T>
    class Network{
        public:
            Network(int board_size, int batch_size, int channels): 
            board_size_(board_size), batch_size_(batch_size), channels_(channels), output_dim_(board_size*board_size){
                

            }
            ~Network(){


            }

        public:
            
            void init(NetDef& init_model, NetDef& predict_model, NetDef& update_model, bool use_gpu){
                
                init_model_ = init_model;
                predict_model_ = predict_model;
                update_model_ = update_model;
                init_net_.reset(new Net(init_model_));
                predict_net_.reset(new Net(predict_model_));
                update_net_.reset(new Net(update_model_)); 
                if(use_gpu){
                    init_net_->SetDeviceCUDA();
                    predict_net_->SetDeviceCUDA();
                    update_net_->SetDeviceCUDA();
                }
                workspace_.reset(new Workspace("workspace"));

                // create input data
                Blob* data = workspace_->CreateBlob("data");
                T* input = data->GetMutable<T>();
                input->Resize(vector<int>{batch_size_,channels_,board_size_,board_size_});
                input->template mutable_data<float>();

                // create label pai, action prediction
                Blob* pai = workspace_->CreateBlob("pai");
                T* label_pai = pai->GetMutable<T>();
                label_pai->Resize(vector<int>{batch_size_, board_size_* board_size_});
                label_pai->template mutable_data<float>();
                LOG(INFO)<<"label pai dim: "<<label_pai->dims();
                // create label z, game winner
                Blob* z = workspace_->CreateBlob("z");
                T* label_z = z->GetMutable<T>();
                label_z->Resize(vector<int>{batch_size_, 1});
                label_z->template mutable_data<float>();

            }




            void allocate(){
                init_ = CreateNet( init_model_,workspace_.get());
                LOG(INFO)<<"create init network finish";
                predict_ = CreateNet(predict_model_, workspace_.get());
                LOG(INFO)<<"create predict network finish";
                update_ = CreateNet(update_model_, workspace_.get());
                LOG(INFO)<<"create update network finish";
            }



            void create_network(bool training){
                create_lenet(training);


            }



            void create_lenet(bool training){
                update_net_->AddInput("data");
                predict_net_->AddInput("data");

                update_net_->AddInput("pai");

                update_net_->AddConvOp("data", "conv1_w", "conv1_b", "conv1", 1, 0, 5);
                predict_net_->AddConvOp("data", "conv1_w", "conv1_b", "conv1", 1, 0, 5);
                
                update_net_->AddInput("conv1_w");
                predict_net_->AddInput("conv1_w");
               
                update_net_->AddInput("conv1_b");
                predict_net_->AddInput("conv1_b");

                if (training) {
                    init_net_->AddXavierFillOp({20, 3, 5, 5}, "conv1_w");
                    init_net_->AddConstantFillOp({20}, "conv1_b");
                }

                update_net_->AddMaxPoolOp("conv1", "pool1", 2, 0, 2);
                update_net_->AddConvOp("pool1", "conv2_w", "conv2_b", "conv2", 1, 0, 5);
                update_net_->AddInput("conv2_w");
                update_net_->AddInput("conv2_b");

                predict_net_->AddMaxPoolOp("conv1", "pool1", 2, 0, 2);
                predict_net_->AddConvOp("pool1", "conv2_w", "conv2_b", "conv2", 1, 0, 5);
                predict_net_->AddInput("conv2_w");
                predict_net_->AddInput("conv2_b");

                if (training) {
                    init_net_->AddXavierFillOp({50, 20, 5, 5}, "conv2_w");
                    init_net_->AddConstantFillOp({50}, "conv2_b");
                }



                update_net_->AddMaxPoolOp("conv2", "pool2", 2, 0, 2);
                update_net_->AddFcOp("pool2", "fc3_w", "fc3_b", "fc3");
                update_net_->AddInput("fc3_w");
                update_net_->AddInput("fc3_b");

                predict_net_->AddMaxPoolOp("conv2", "pool2", 2, 0, 2);
                predict_net_->AddFcOp("pool2", "fc3_w", "fc3_b", "fc3");
                predict_net_->AddInput("fc3_w");
                predict_net_->AddInput("fc3_b");


                if (training) {
                    init_net_->AddXavierFillOp({500, 50}, "fc3_w");
                    init_net_->AddConstantFillOp({500}, "fc3_b");
                }
                update_net_->AddReluOp("fc3", "fc3");
                update_net_->AddFcOp("fc3", "pred_w", "pred_b", "pred");
                update_net_->AddInput("pred_w");
                update_net_->AddInput("pred_b");

                predict_net_->AddReluOp("fc3", "fc3");
                predict_net_->AddFcOp("fc3", "pred_w", "pred_b", "pred");
                predict_net_->AddInput("pred_w");
                predict_net_->AddInput("pred_b");

                if (training) {
                    init_net_->AddXavierFillOp({output_dim_, 500}, "pred_w");
                    init_net_->AddConstantFillOp({output_dim_}, "pred_b");
                }

                update_net_->AddSoftmaxOp("pred", "softmax");
                predict_net_->AddSoftmaxOp("pred", "softmax");




                LOG(INFO)<<predict_model_.external_input().size();
                for(auto x: predict_model_.external_input()){
                    LOG(INFO)<<x;
                }
                LOG(INFO)<<init_model_.external_input().size();
                for(auto x: init_model_.external_input()){
                    LOG(INFO)<<x;
                }
                for(auto op: init_model_.op()){
                    LOG(INFO)<<op.type()<<" "<<op.name();
                }
                for(auto op: predict_model_.op()){
                    LOG(INFO)<<op.type()<<" "<<op.name();
                }
            }

            void add_loss(){

                update_net_->AddInput("ITER");
                init_net_->AddConstantFillOp({1}, 1.f, "ONE");
                init_net_->AddConstantFillOp({1}, (int64_t)0, "ITER")->mutable_device_option()->set_device_type(CPU);
                update_net_->AddInput("ONE");
                update_net_->AddIterOp("ITER");


                update_net_->AddLabelCrossEntropyOp("softmax", "pai", "xent");
                update_net_->AddAveragedLossOp("xent","loss");
                update_net_->AddConstantFillWithOp(1.f, "loss", "loss_grad");
                update_net_->AddGradientOps();
                update_net_->AddLearningRateOp("ITER", "LR", 0.1);

                std::vector<string> params;
                for(auto op : init_model_.op()){
                    for(auto out: op.output()){
                        if(!(out.compare("ITER")==0 || out.compare("ONE")==0)){
                            params.push_back(out);
                        }
                    }
                }
                for(auto param : params){
                    LOG(INFO)<<"init model output: "<<param;
                    update_net_->AddWeightedSumOp({param,"ONE", param+"_grad","LR"},param);
                }
            }

            void print_shape(){
                for(auto x: workspace_->Blobs()){
                    LOG(INFO)<<"workspace: "<<x;
                    auto y = workspace_->GetBlob(x);
                    LOG(INFO)<<y->Get<TensorCPU>().dims();
                }
            }

            void init_parameters(){
                init_->Run();
                print_shape();

            }

            void update_parameters(){
                update_->Run();
                print_shape();

            }
            void forward(Blob* input, Blob* prob, float& v){
                Blob* data = workspace_->GetBlob("data");
                T* d = data->GetMutable<T>();
                TensorCPU* in = input->GetMutable<TensorCPU>();
                d->template CopyFrom<CPUContext>(in);
                predict_->Run();



            }



            void train(){
                



                init_->Run();
                LOG(INFO)<<"init RUN";
                predict_->Run();
                LOG(INFO)<<"predict RUN";
                update_->Run();
                LOG(INFO)<<"update RUN";
                print_shape();
                
                for(int i =0; i< 1; ++ i){
                    predict_->Run();
                    auto o = workspace_->GetBlob("softmax");
                    for(int j = 0; j < 10; ++ j){
                        LOG(INFO)<<o->Get<TensorCPU>().data<float>()[j];
                    }
                }
                //predict_->Run();



            }


            void inference(const State& state, Action& action, float& v){


            }

            void save(string proto_name){
                WriteProtoToTextFile(predict_model_,proto_name+"_pred.pbtxt");
                WriteProtoToTextFile(init_model_, proto_name+"_init.pbtxt");
                WriteProtoToTextFile(update_model_, proto_name+"_update.pbtxt");
            }

        protected:
            int board_size_;
            int batch_size_;
            int channels_;

            int output_dim_;

            std::shared_ptr<Net> init_net_;
            std::shared_ptr<Net> predict_net_;
            std::shared_ptr<Net> update_net_;


            std::shared_ptr<Workspace> workspace_;

            NetDef init_model_;
            NetDef predict_model_;
            NetDef update_model_;


            unique_ptr<NetBase> init_;
            unique_ptr<NetBase> predict_;
            unique_ptr<NetBase> update_;


    };




}







#endif