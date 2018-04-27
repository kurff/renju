#ifndef __NETWORK_HPP__
#define __NETWORK_HPP__
#include "network/net.h"
#include "glog/logging.h"
#include <memory>
#include <string>
#include <vector>
#include "caffe2/core/tensor.h"
namespace Beta{
    
    template<typename State, typename Action, typename DataContext>
    class Network{
        typedef typename caffe2::Tensor<DataContext> T;
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


            }

            void allocate_inputs(const std::vector<string>& inputs, const std::vector<string>& label){
                Blob* data = workspace_->CreateBlob(inputs[0]);
                T* input = data->GetMutable<T>();
                input->Resize(vector<int>{batch_size_,channels_,board_size_,board_size_});
                input->template mutable_data<float>();

                // create label pai, action prediction
                Blob* pai = workspace_->CreateBlob(label[0]);
                T* label_pai = pai->GetMutable<T>();
                label_pai->Resize(vector<int>{batch_size_, 1});
                label_pai->template mutable_data<int>();
                LOG(INFO)<<"label pai dim: "<<label_pai->ndim()<<" "<<label_pai->dim32(1);
                // create label z, game winner
                Blob* z = workspace_->CreateBlob(label[1]);
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
                //create_lenet(training);
                LOG(INFO)<<"allocate_inputs";
                allocate_inputs({"data"},{"pai","z"});
                vector<string> base_net_outputs;
                add_feed_data("data");
                LOG(INFO)<<"add_feed_data";
                add_label_data({"pai","z"});
                LOG(INFO)<<"add_label_data";
                create_base_network({"data"},base_net_outputs, training);
                LOG(INFO)<<"create_base_network";
                create_head(base_net_outputs, {"p","r"}, training);
                LOG(INFO)<<"create_head";
                create_multi_task_loss({"p","r"},{"pai","z"});
                LOG(INFO)<<"create_multi_task_loss";
            }

            void add_batch_normalization(const vector<string>& inputs, 
            vector<string>& outputs, bool training){

            }

            void add_convolutional_block(const vector<string>& inputs, 
            vector<string>& outputs, vector<int> shapes,bool training, int index, bool pooling = false){
                string w = "conv_w_"+std::to_string(index);
                string b = "conv_b_"+std::to_string(index);
                string o = "conv_o_"+std::to_string(index);
                string m = "conv_m_"+std::to_string(index);
                outputs.clear();
                
                //string r = "relu_"+std::to_string(index);
                if(inputs.size() == 1){
                    LOG(INFO)<<"size of inputs is 1";
                    update_net_->AddConvOp(inputs[0],w,b,m,1,0,shapes[2]);
                    predict_net_->AddConvOp(inputs[0],w,b,m,1,0,shapes[2]);

                    update_net_->AddInput(w);
                    update_net_->AddInput(b);

                    predict_net_->AddInput(w);
                    predict_net_->AddInput(b);

                    update_net_->AddReluOp(m,m);
                    predict_net_->AddReluOp(m,m);

                    if(pooling){
                        update_net_->AddMaxPoolOp(m,o,2,0,2);
                        predict_net_->AddMaxPoolOp(m,o,2,0,2);
                        outputs.push_back(o);
                    }else{
                        outputs.push_back(m);
                    }
                    if(training){
                        init_net_->AddXavierFillOp(shapes, w);
                        init_net_->AddConstantFillOp({shapes[0]}, b);
                    }
                }else{
                    LOG(INFO)<<"size of input is "<< inputs.size();
                    return;
                }
            }

            void add_fc_block(const vector<string>& inputs, 
            const vector<string>& outputs, vector<int> shapes,bool training,
                int index, bool add_relu = true, bool add_softmax = false
                , bool add_tanh = false){
                LOG(INFO)<<"add fc block with const output";
                string w = "fc_w_"+std::to_string(index);
                string b = "fc_b_"+std::to_string(index);
                string o = "fc_o_"+std::to_string(index);
                if(inputs.size() == 1){
                    update_net_->AddFcOp(inputs[0],w,b,o);
                    predict_net_->AddFcOp(inputs[0],w,b,o);
                    update_net_->AddInput(w);
                    update_net_->AddInput(b);
                    predict_net_->AddInput(w);
                    predict_net_->AddInput(b);
                    if(add_relu){
                        update_net_->AddReluOp(o,outputs[0]);
                        predict_net_->AddReluOp(o,outputs[0]);
                    }
                    if(add_softmax){
                        update_net_->AddSoftmaxOp(o,outputs[0]);
                        predict_net_->AddSoftmaxOp(o,outputs[0]);
                    }
                    if(add_tanh){
                        update_net_->AddTanh(o,outputs[0]);
                        predict_net_->AddTanh(o,outputs[0]);
                    }
                    if(training){
                        init_net_->AddXavierFillOp(shapes, w);
                        init_net_->AddConstantFillOp({shapes[0]}, b);
                    }
                }else{
                    LOG(INFO)<<"input size is "<< inputs.size();
                    return;
                }


            }


            void add_fc_block(const vector<string>& inputs, 
            vector<string>& outputs, vector<int> shapes,bool training,
                int index, bool add_relu = true, bool add_softmax = false
                , bool add_tanh = false){
                LOG(INFO)<<"add fc block with non-const output";
                string w = "fc_w_"+std::to_string(index);
                string b = "fc_b_"+std::to_string(index);
                string o = "fc_o_"+std::to_string(index);
                string t = "t_"+std::to_string(index);
                outputs.clear();
                outputs.push_back(o);
                if(inputs.size() == 1){
                    update_net_->AddFcOp(inputs[0],w,b,t);
                    predict_net_->AddFcOp(inputs[0],w,b,t);
                    update_net_->AddInput(w);
                    update_net_->AddInput(b);
                    predict_net_->AddInput(w);
                    predict_net_->AddInput(b);
                    if(add_relu){
                        update_net_->AddReluOp(t,o);
                        predict_net_->AddReluOp(t,o);
                    }
                    if(add_softmax){
                        update_net_->AddSoftmaxOp(t,o);
                        predict_net_->AddSoftmaxOp(t,o);
                    }
                    if(add_tanh){
                        update_net_->AddTanh(t,o);
                        predict_net_->AddTanh(t,o);
                    }
                    if(training){
                        init_net_->AddXavierFillOp(shapes, w);
                        init_net_->AddConstantFillOp({shapes[0]}, b);
                    }
                }else{
                    LOG(INFO)<<"input size is "<< inputs.size();
                    return;
                }
            }


            void add_residual_block(const vector<string>& inputs, 
            vector<string>& outputs, bool training){


            }

            void add_feed_data(const string& data){
                update_net_->AddInput(data);
                predict_net_->AddInput(data);
            }

            void add_label_data(const vector<std::string>& labels){
                update_net_->AddInput(labels[0]);
                update_net_->AddInput(labels[1]);
            }

            void create_base_network(const vector<string>& inputs, 
            vector<string>& outputs, bool training){
                vector<string> output_block1;
                vector<string> output_block2;
                add_convolutional_block(inputs, output_block1,{32,3,3,3},training,0);
                add_convolutional_block(output_block1, output_block2,{32,32,3,3},training,1);
                add_convolutional_block(output_block2, outputs,{32,32,3,3},training,2);
                //vector<string> fc_block1;
                //vector<string> fc_block2;
                //add_fc_block(output_block3,fc_block1,{100,},1,0,1);
                //add_fc_block(fc_block1,fc_block2,{},1,1,0);
            }

            void create_head(const vector<string>& inputs, 
            const vector<string>& outputs, bool training){
                vector<string> output_block1;
                
                add_convolutional_block(inputs, output_block1, {2,32,1,1}, training,3);
                //vector<string> fc_block1;
                add_fc_block(output_block1, {outputs[0]},{ output_dim_,162},training,0,0,0,1);
                vector<string> output_block2;
                add_convolutional_block(inputs,output_block2,{1,32,1,1},training,4);
                //vector<string> fc_block2;
                add_fc_block(output_block2, {outputs[1]},{1,81},training,1,0,1,0);

            }

            void create_multi_task_loss(const vector<string>& predict, const vector<string>& label ){
                //update_net_->AddLabelCrossEntropyOp();
                update_net_->AddInput("ITER");
                init_net_->AddConstantFillOp({1}, 1.f, "ONE");
                init_net_->AddConstantFillOp({1}, (int64_t)0, "ITER")->mutable_device_option()->set_device_type(CPU);
                update_net_->AddInput("ONE");
                update_net_->AddIterOp("ITER");
                LOG(INFO)<<"add input and iter";
                update_net_->AddConstantFillOp({1}, 0.5f,"weight1");
                update_net_->AddConstantFillOp({1}, 0.5f,"weight2");



                update_net_->AddLabelCrossEntropyOp(predict[0], label[0], "xent");
                update_net_->AddAveragedLossOp("xent","loss1");
                update_net_->AddSquaredL2DistanceOp({predict[1],label[1]},"l2_loss");
                update_net_->AddAveragedLossOp("l2_loss","loss2");
                //update_net_->AddInput("weight1");
                //update_net_->AddInput("weight2");

                update_net_->AddWeightedSumOp({"loss1","weight1","loss2","weight2"},"loss");
                LOG(INFO)<<"add loss op";
                update_net_->AddConstantFillWithOp(1.f, "loss", "loss_grad");
                update_net_->AddConstantFillWithOp(1.f, "loss1", "loss1_grad");
                update_net_->AddConstantFillWithOp(1.f, "loss2", "loss2_grad");
                LOG(INFO)<<"AddConstantFillWithOp";
                update_net_->AddGradientOps();
                LOG(INFO)<<"AddGradientOps";
                update_net_->AddLearningRateOp("ITER", "LR", 0.1);
                LOG(INFO)<<"add params";
                params_.clear();
                for(auto op : init_model_.op()){
                    for(auto out: op.output()){
                        if(!(out.compare("ITER")==0 || out.compare("ONE")==0)){
                            params_.push_back(out);
                        }
                    }
                }
                for(auto param : params_){
                    LOG(INFO)<<"init model output: "<<param;
                    update_net_->AddWeightedSumOp({param,"ONE", param+"_grad","LR"},param);
                }

            }


            void create_lenet(bool training){
                update_net_->AddInput("data");
                predict_net_->AddInput("data");

                update_net_->AddInput("pai");
                update_net_->AddConvOp("data", "conv1_w", "conv1_b", "conv1", 1, 0, 5);
                predict_net_->AddConvOp("data", "conv1_w", "conv1_b", "conv1", 1, 0, 5);

                update_net_->AddReluOp("conv1", "conv1");
                predict_net_->AddReluOp("conv1", "conv1");
                
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
                //for(auto op: init_model_.op()){
                //    LOG(INFO)<<op.type()<<" "<<op.name();
                //}
                //for(auto op: predict_model_.op()){
                //    LOG(INFO)<<op.type()<<" "<<op.name();
                //}
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

                params_.clear();
                for(auto op : init_model_.op()){
                    for(auto out: op.output()){
                        if(!(out.compare("ITER")==0 || out.compare("ONE")==0)){
                            params_.push_back(out);
                        }
                    }
                }
                for(auto param : params_){
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
                //print_shape();

            }

            void update_parameters(){
                update_->Run();
                //print_shape();

            }
            void forward(State& state){
                Blob* data = workspace_->GetBlob("data");
                T* d = data->GetMutable<T>();

                Blob* input = state.input();
                TensorCPU* in = input->GetMutable<TensorCPU>();
                d->template CopyFrom<CPUContext>(*in);
                predict_->Run();
                Blob* prob = workspace_->GetBlob("p");
                T* tp = prob->GetMutable<T>();
                Blob* sp = state.prob();
                sp->GetMutable<TensorCPU>()->template CopyFrom<DataContext>(*tp);
                Blob* z = workspace_->GetBlob("z");
                T* tz = z->GetMutable<T>();
                Blob* sz = state.z();
                sz->GetMutable<TensorCPU>()->template CopyFrom<DataContext>(*tz);
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


            void save_model(string init_pb){
                auto op = init_net_->AddSaveOp(params_,1,"minidb",init_pb,"minidb",{});
                workspace_->RunOperatorOnce(*op);
                //for(auto x: params_){
                //    auto b = workspace_->GetBlob(x);
                //    TensorPrinter tensor_print(x,"save"+x);
                //    tensor_print.Print<float>((b->template Get<TensorCPU>()));
                    //tensor_print.PrintMeta(b->Get<>);
                //}
            }

            void load_model(string init_pb){
                auto op = init_net_->AddLoadOp(params_,1,"",{},init_pb,{},"minidb",1,1,1,{});
                workspace_->RunOperatorOnce(*op);
                
                //for(auto x : ws.Blobs()){
                //    LOG(INFO)<<"blob names "<<x;
                //    auto b = ws.GetBlob(x);
                //    TensorPrinter tensor_print(x,"load"+x);
                //    tensor_print.Print<float>(b->template Get<TensorCPU>());
                //}
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
            
            std::vector<std::string> params_;

    };




}







#endif