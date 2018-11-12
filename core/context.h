#ifndef __CONTEXT_HPP__
#define __CONTEXT_HPP__


#include <vector>
#include <assert.h>
#include "caffe/caffe.hpp"
#include "glog/logging.h"
#include "utils/utils.hpp"
using namespace caffe;
// context of mcts.hpp
// go can be seen as a special case of context
// get features 

namespace Beta{
  template<typename StateType, typename ActionType>
  class Context{
    public:
        Context(float epsilon);
        ~Context();

        // get current state
        
        //virtual void self_play() = 0;



        StateType& legal_state(int index){
          assert(index >= 0 && index < current_legal_states_.size());
          return *(current_legal_states_[index].get());
        }

        void push(){

        }

        void push_action(std::shared_ptr<ActionType> action){
           actions_.push_back(action);
        }

        virtual void get_legal_action(const StateType& state) = 0;


        size_t size_legal_action(){
          return current_legal_action_.size(); 
        }

        const ActionType& legal_action(int index){
          return *(current_legal_action_[index].get());
        }

        


        ActionType sample_from_pai();




    protected:
        std::vector<std::shared_ptr<StateType> > states_;
        std::vector<std::shared_ptr<ActionType> > actions_;

        std::vector<std::shared_ptr<ActionType> > current_legal_action_;
        std::vector<std::shared_ptr<StateType> > current_legal_states_;

        // label of ground truth
        std::vector<std::shared_ptr< Blob<float> > > pai_;
        int z_; // -1 loss, 0 draw, 1 win
        float epsilon_;


  };
 



 



}



#endif 