
#include "core/state.h"
#include "core/action.h"

#include "core/renju_action.h"
#include "network/network.h"
#include "glog/logging.h"

namespace Beta{
    template<typename StateType>
    Network<StateType>::Network():step_(0){

    }
    template<typename StateType>
    Network<StateType>::~Network(){


    }

    template<typename StateType>
    void Network<StateType>::init(const string& solver_file){
        caffe::ReadSolverParamsFromTextFileOrDie(solver_file, &solver_param_);
        init(solver_param_);
    }

    template<typename StateType>
    void Network<StateType>::init(const SolverParameter& param){
        solver_.reset(SolverRegistry<float>::CreateSolver(param));
    }

    template<typename StateType>
    void Network<StateType>::update(StateType* state){
        set_train(state);
        solver_->Step(step_);
    }

    template<typename StateType>
    void Network<StateType>::set_train(StateType* state){
        auto test_nets = solver_->test_nets();
        const int num_nets = test_nets.size();
        CHECK_EQ(num_nets, 1);
         int num_inputs = test_nets[0]->num_inputs();
        CHECK_EQ(num_inputs,2);
        vector<Blob<float>* > input_blobs = test_nets[0]->input_blobs();
        Blob<float>* data = input_blobs[0];
        Blob<float>* label = input_blobs[1];
        Blob<float>* input = state->input();
        Blob<float>* prob = state->prob();
        data->CopyFrom(*input);
        label->CopyFrom(*prob);
        LOG(INFO)<<"data " << data->shape_string();
        LOG(INFO)<<"label "<< label->shape_string();
    }

    template<typename StateType>
    void Network<StateType>::set_test(StateType* state){
         int num_inputs = net_->num_inputs();
        CHECK_EQ(num_inputs, 1);
        auto input_blobs = net_->input_blobs();
        Blob<float>* data = input_blobs[0];
        data->CopyFrom(*(state->input()));
        LOG(INFO)<<"data: "<< data->shape_string();
    }

    template<typename StateType>
    void Network<StateType>::forward_train(StateType* state){
        set_train(state);
        auto test_nets = solver_->test_nets();
        const int num_nets = test_nets.size();
        CHECK_EQ(num_nets, 1);
        test_nets[0]->Forward();
    }
    template<typename StateType>
    void Network<StateType>::forward_test(StateType* state){
        set_test(state);
        net_->Forward();
        auto output_blobs = net_->output_blobs();
        // get the outputs 
        state->prob()->CopyFrom(*output_blobs[0]);
        state->z()->CopyFrom(*output_blobs[1]);

    }
    template<typename StateType>
    void Network<StateType>::init(const std::string& prototxt, const std::string& model_file){
        net_.reset(new Net<float>(prototxt, TEST));
        net_->CopyTrainedLayersFrom(model_file);
    }
    template class Network<State>;








}