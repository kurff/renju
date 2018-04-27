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

            void init(int batch_size, int board_size, int channel){
                TensorCPU* input = input_->GetMutable<TensorCPU>();
                input->Resize(vector<TIndex>{batch_size, channel, board_size, board_size});
                input->mutable_data<float>();
                TensorCPU* prob = prob_->GetMutable<TensorCPU>();
                prob->Resize(vector<TIndex>{batch_size, 1, board_size, board_size});
                prob->mutable_data<float>();
                TensorCPU* z = z_->GetMutable<TensorCPU>();
                z->Resize(vector<TIndex>{1});
                z->mutable_data<float>();
            }

            Blob* input(){ return input_.get();}
            Blob* prob(){return prob_.get();}
            Blob* z(){return z_.get();}





        protected:
            std::shared_ptr<Blob> input_;
            std::shared_ptr<Blob> prob_;
            std::shared_ptr<Blob> z_;

            

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