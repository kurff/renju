#ifndef __STATE_HPP__
#define __STATE_HPP__

#include "caffe/caffe.hpp"
#include <memory>
using namespace caffe2;

namespace Beta{
    // state should be in search space
    class State{
        public:
            State();

            ~State(){


            }

            State(const State& state);




            void init(int batch_size, int board_size, int channel)
            void init_input(int batch_size, int board_size, int channel);

            void init_prob(int batch_size, int board_size, int channel);

            void init_z(int batch_size, int board_size, int channel);

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
    


}





#endif