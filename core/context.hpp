#ifndef __CONTEXT_HPP__
#define __CONTEXT_HPP__


#include <vector>
#include <assert.h>
#include "caffe2/core/tensor.h"
#include "glog/logging.h"
#include "utils/utils.hpp"
// context of mcts.hpp
// go can be seen as a special case of context
// get features 

namespace Beta{
  



  template<typename StateType, typename ActionType>
  class Context{
    public:
        Context(float epsilon): epsilon_(epsilon){

        }
        ~Context(){

        }

        // get current state
        
        //virtual void self_play() = 0;



        StateType& legal_state(int index){
          assert(index >= 0 && index < current_legal_states_.size());
          return *(current_legal_states_[index].get());
        }

        void push(TensorCPU* pai){
            pai_.push_back(pai);
        }

        void push_action(std::shared_ptr<ActionType> action){
           actions_.push_back(action);
        }


        //virtual void calc_next_state() = 0;

        //virtual void get_legal_action(const Tensor<CPUContext>& current) = 0;

        virtual void get_legal_action(const StateType& state) = 0;


        size_t size_legal_action(){
          return current_legal_action_.size(); 
        }
        const ActionType& legal_action(int index){
          return *(current_legal_action_[index].get());
        }

        


        ActionType sample_from_pai(){



        }




    protected:
        std::vector<std::shared_ptr<StateType> > states_;
        std::vector<std::shared_ptr<ActionType> > actions_;

        std::vector<std::shared_ptr<ActionType> > current_legal_action_;
        std::vector<std::shared_ptr<StateType> > current_legal_states_;

        // label of ground truth
        std::vector<TensorCPU* > pai_;
        int z_; // -1 loss, 0 draw, 1 win
        float epsilon_;


  };
 

  template<typename StateType, typename ActionType>
  class RenjuContext: public Context<StateType, ActionType>{
    public:
      RenjuContext(float epsilon): Context<StateType,ActionType>(epsilon){

      }

      ~RenjuContext(){

      }

      void get_legal_action(const StateType& state){

      }

      protected:
        

  };

 //
  template<typename StateType, typename ActionType>
  class GoContext: public Context<StateType, ActionType>{
    public:
      GoContext(float epsilon):Context<StateType,ActionType>(epsilon){}
      ~GoContext(){}
      void get_current_state() {

      }
      void get_current_features(){

      }


      void calc_next_state(){
        this->current_legal_states_.resize(this->current_legal_action_.size());
        for(int i = 0; i < this->current_legal_action_.size(); ++ i){


        }

      }
      void get_legal_action(const StateType& state){

      }
    protected:
      
      

  };

  template<typename StateType, typename ActionType>
  class ChessContext: public Context<StateType, ActionType>{
    public:
      ChessContext(float epsilon):Context<StateType,ActionType>(epsilon){}
      ~ChessContext(){}
      void get_current_state() {

      }
      void get_current_features(){

      }


      void calc_next_state(){


      }

      void get_legal_action(const StateType& state){
        const Blob* input = state.input();
        const TensorCPU t = input->Get<TensorCPU>();
        
        int batch_size = state.batch_size();
        int channel = state.channel();
        int board_size = state.board_size();
        assert(batch_size != 1);
        for(int i = 0; i < batch_size; ++ i){
          for(int j = 0; j < channel; ++ j){
            for(int p = 0; p < board_size; ++ p){
              for(int q = 0; q < board_size; ++ q){
                  int index = i * channel* board_size * board_size
                  + j * board_size * board_size + p * board_size + q;
                  if( t.data<float>()[index] <= 0.00001f ){
                    std::shared_ptr<StateType> legal(new StateType());
                    legal->ReshapeLike(state);
                    legal->set_input(i,j,p,q,1.0);
                    std::shared_ptr<ActionType> action(new ActionType(j,p,q));
                    this->current_legal_states_.push_back(legal);
                    this->current_legal_action_.push_back(action);
                  }
              }
            }
          }
        }
      }




    protected:
      

  };




}



#endif 