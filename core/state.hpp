#ifndef __STATE_HPP__
#define __STATE_HPP__

#include "caffe2/core/tensor.h"

using namespace caffe2;

namespace Beta{

    class State{
        public:
            State(){

            }

            ~State(){

            }

            State(const State & state){
                state_ = state.get_state();

            }

            Tensor<TensorCPU> & get_state(){
                return state_;
            }


        protected:
            Tensor<TensorCPU> state_;

            Tensor<TensorCPU> result_;
            

    }


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