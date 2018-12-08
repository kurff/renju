#include <string>
#include "state.h"

namespace Beta{
    State::State(){
        input_.reset(new Blob<float>());
        prob_.reset(new Blob<float>());
        z_.reset(new Blob<float>());
    }

    State::~State(){

    }

    State::State(const State& state){

    }

    void State::init_input(int num, int channel, int height, int width){
        input_->Reshape(num, channel, height, width);

    }
    void State::init_prob(int num, int channel, int height, int width){
        prob_->Reshape(num, channel, height, width);

    }
    void State::init_z(int num, int channel, int height, int width){
        z_->Reshape(num, channel, height, width);
    }

    void State::set_input(int n, int c, int y, int x, float v){
        int o = input_->offset(n,c,y,x);
        input_->mutable_cpu_data()[o] = v;
    }

    void State::set_input(int size, const float* src){
        std::memcpy(input_->mutable_cpu_data(), src, sizeof(float)*size);
    }










}