#ifndef NETWORK_NETWORK_H_
#define NETWORK_NETWORK_H_
#include <string>
#include <memory>
#include "caffe/caffe.hpp"

using namespace std;
using namespace caffe;
namespace Beta{
    template<typename StateType, typename ActionType>
    class Network{
        public:
            Network();
            ~Network();

            void init(const SolverParameter& param);
            void init(const std::string& prototxt, const std::string& model_file);
            void update();
            void forward_train(const StateType& state);
            void forward_test(const StateType& state);
        
        protected:
        // solver file for training
            std::shared_ptr<Solver<float> > solver_;
        // net for evaluation
            std::shared_ptr<Net<float> > net_;




    };






}



#endif