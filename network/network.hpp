#ifndef __NETWORK_HPP__
#define __NETWORK_HPP__
#include "network/net.h"
#include <memory>
namespace Beta{
    
    template<typename State, typename Action>
    class Network{
        public:
            Network(){


            }
            ~Network(){


            }

        public:



            void create_network(){


            }



            void run(const State& state, Action& action, float& v){


            }

        protected:
            std::shared_ptr<Net> init_net_;
            std::shared_ptr<Net> predict_net_;



    };




}







#endif