#ifndef __STATE_HPP__
#define __STATE_HPP__

#include "caffe2/core/tensor.h"
#include "caffe2/core/blob.h"
#include <memory>
using namespace caffe2;

namespace Beta{


    // state should be in search space
    class State{
        public:
            State(){
                input_.reset(new Blob());
                prob_.reset(new Blob());
                z_.reset(new Blob());
            }

            ~State(){


            }

            State(State& state){
                ReshapeLike(state);
                TensorCPU* t = input_->GetMutable<TensorCPU>();
                TensorCPU* ts = state.input()->GetMutable<TensorCPU>();
                CPUContext context;
                t->CopyFrom<CPUContext, CPUContext>(*ts, &context);

                TensorCPU* p = prob_->GetMutable<TensorCPU>();
                TensorCPU* ps = state.input()->GetMutable<TensorCPU>();
                p->CopyFrom<CPUContext, CPUContext>(*ps, &context);

            }

            void ReshapeLike(const State& state){
                init(state.batch_size(), state.board_size(), state.channel());
            }


            void init(int batch_size, int board_size, int channel){
                batch_size_ = batch_size;
                board_size_ = board_size;
                channel_ = channel;
                init_input(batch_size, board_size, channel);
                init_prob(batch_size, board_size, channel);
                init_z(batch_size, board_size, channel);
            }

            void init_input(int batch_size, int board_size, int channel){

                TensorCPU* input = input_->GetMutable<TensorCPU>();
                input->Resize(vector<TIndex>{batch_size, channel, board_size, board_size});
                input->mutable_data<float>();
            }

            void init_prob(int batch_size, int board_size, int channel){
                TensorCPU* prob = prob_->GetMutable<TensorCPU>();
                prob->Resize(vector<TIndex>{batch_size, 1, board_size, board_size});
                prob->mutable_data<float>();
            }

            void init_z(int batch_size, int board_size, int channel){
                TensorCPU* z = z_->GetMutable<TensorCPU>();
                z->Resize(vector<TIndex>{1});
                z->mutable_data<float>();
            }

            const int batch_size() const {return batch_size_;}
            const int board_size() const {return board_size_;}
            const int channel() const {return channel_;}

            void set_input(int n, int c, int y, int x , float v ){
                TensorCPU* input = input_->GetMutable<TensorCPU>();
                float* ptr = input->mutable_data<float>();
                ptr[n*board_size_*board_size_*channel_ + c * board_size_* board_size_
                 + y* board_size_ + x] = v;
            }

            

            Blob* input() const { return input_.get();}
            Blob* prob(){return prob_.get();}
            Blob* z(){return z_.get();}
        protected:
            std::shared_ptr<Blob> input_;
            std::shared_ptr<Blob> prob_;
            std::shared_ptr<Blob> z_;
            int batch_size_;
            int board_size_;
            int channel_;

            

    };


    class GoState: public State{
        public:
            GoState(){}
            ~GoState(){}

            

        protected:
        // 19 x 19
            
            
            



    };

    class ChessState: public State{
        public:
            ChessState(){}
            ~ChessState(){}

        protected:
            
            


    };




}





#endif