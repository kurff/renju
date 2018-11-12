#ifndef NETWORK_NETWORK_H_
#define NETWORK_NETWORK_H_
#include <string>
#include <memory>
#include "caffe/caffe.hpp"

using namespace std;
using namespace caffe;
namespace Beta{
    template<typename StateType>
    class Network{
        public:
            Network();
            ~Network();

            void init(const SolverParameter& param);
            void init(const string& solver_file);
            void init(const std::string& prototxt, const std::string& model_file);
            void update(StateType* state);
            void set_train(StateType* state);
            void set_test(StateType* state);
            void forward_train(StateType* state);
            void forward_test(StateType* state);
        
        protected:
        // solver file for training
            std::shared_ptr<Solver<float> > solver_;
        // net for evaluation
            std::shared_ptr<Net<float> > net_;
            caffe::SolverParameter solver_param_;
            int step_;




    };






}



#endif