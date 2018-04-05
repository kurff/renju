#include "gtest/gtest.h"
#include "glog/logging.h"
#include <memory>

using namespace std;
template<typename State, typename Action>

class Node{
    public:
      Node(){

      }
      ~Node(){

      }
      
      void print(){
          LOG(INFO)<<"example ";
      }
    protected: 
      
};


template<typename State, typename Action>
class Example{
    typedef Node<State, Action> X;
    public: 
        Example(){

        }
        ~Example(){

        }
        void run(){
            x_->print();
        }

    protected:
        shared_ptr<X> x_ ;
    




};

TEST(Example,test){
    Example<int, int>* example = new Example<int, int>();
    example->run();





}