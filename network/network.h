#ifndef NETWORK_NETWORK_H_
#define NETWORK_NETWORK_H_
#include <string>
#include "caffe/distribute/caffe.hpp"

using namespace std;
namespace Beta{
    template<typename StateType, typename ActionType>
    class Network{
        public:
            Network();
            ~Network();

            void init(const std::string& solver_file);
            void update();
            void forward();
        protected:



    };






}



#endif