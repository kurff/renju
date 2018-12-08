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

            void init_input(int num, int channel, int height, int width);

            void init_prob(int num, int channel, int height, int width);

            void init_z(int num, int channel, int height, int width);

            void set_input(int n, int c, int y, int x , float v );

            void set_input(int size, const float* src);

            

            Blob<float>* input(){ return input_.get();}
            Blob<float>* prob(){return prob_.get();}
            Blob<float>* z(){return z_.get();}
        protected:
            std::shared_ptr<Blob<float> > input_;
            std::shared_ptr<Blob<float> > prob_;
            std::shared_ptr<Blob<float> > z_;


            

    };
    


}





#endif