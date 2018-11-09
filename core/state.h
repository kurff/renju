#ifndef CORE_STATE_HPP__
#define CORE_STATE_HPP__

#include "caffe/caffe.hpp"
#include <memory>
using namespace caffe;

namespace Beta{
    // state should be in search space
    class State{
        public:
            State();

            ~State();

            State(const State& state);

            void init(int num, int channel, int height, int width);

            void init_input(int num, int channel, int height, int width);

            void init_prob(int num, int channel, int height, int width);

            void init_z(int num, int channel, int height, int width);

            void set_input(int n, int c, int y, int x , float v );

            

            

            Blob* input() const { return input_.get();}
            Blob* prob(){return prob_.get();}
            Blob* z(){return z_.get();}
        protected:
            std::shared_ptr<Blob> input_;
            std::shared_ptr<Blob> prob_;
            std::shared_ptr<Blob> z_;


            

    };
    


}





#endif