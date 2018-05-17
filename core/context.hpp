#ifndef __CONTEXT_HPP__
#define __CONTEXT_HPP__


#include <vector>
#include "caffe2/core/tensor.h"
#include "glog/logging.h"
#include "utils/utils.hpp"
// context of mcts.hpp
// go can be seen as a special case of context
// get features 

namespace Beta{
  



  template<typename State, typename Action>
  class Context{
    public:
        Context(float epsilon): epsilon_(epsilon){

        }
        ~Context(){

        }

        // get current state
        
        //virtual void self_play() = 0;



        State& get_next_state(int index){
          if(index<0 || index >= current_legal_states_.size()){
            LOG(INFO)<<"out of range";
          }
          return current_legal_states_[index];
        }

        void push(Tensor<TensorCPU>* pai){
            pai_.push_back(pai);
        }

        void push_action( Action action){
           actions_.push_back(action);
        }


        //virtual void calc_next_state() = 0;

        //virtual void get_legal_action(const Tensor<CPUContext>& current) = 0;

        virtual void get_legal_action(const State& state) = 0;


        size_t size_legal_action(){
          return current_legal_action_.size(); 
        }
        const Action& legal_action(int index){
          return current_legal_action_[index];
        }

        


        Action sample_from_pai(){



        }




    protected:
        std::vector<State> states_;
        std::vector<Action> actions_;

        std::vector<Action> current_legal_action_;
        std::vector<State> current_legal_states_;

        // label of ground truth
        std::vector<Tensor<TensorCPU>* > pai_;
        int z_; // -1 loss, 0 draw, 1 win
        float epsilon_;


  };
 

  template<typename State, typename Action>
  class RenjuContext: public Context<State, Action>{
    public:
      RenjuContext(float epsilon): Context<State,Action>(epsilon){

      }

      ~RenjuContext(){

      }

      void get_legal_action(State& state){

      }

      protected:
        

  };

 //
  template<typename State, typename Action>
  class GoContext: public Context<State, Action>{
    public:
      GoContext(float epsilon):Context<State,Action>(epsilon){}
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
      void get_legal_action(const State& state){

      }
    protected:
      
      

  };

  template<typename State, typename Action>
  class ChessContext: public Context<State, Action>{
    public:
      ChessContext(float epsilon):Context<State,Action>(epsilon){}
      ~ChessContext(){}
      void get_current_state() {

      }
      void get_current_features(){

      }


      void calc_next_state(){


      }

      void get_legal_action(const State& state){
        Blob* input = state.input();
        TensorCPU* t = input->GetMutable<TensorCPU>();
        assert(state.batch_size() != 1);
        for(int i = 0; i < state.batch_size(); ++ i){
          for(int j = 0; j < state.channel(); ++ j){
            for(int p = 0; p < state.board_size(); ++ p){
              for(int q = 0; q < state.board_size(); ++ q){
                  if( t->data<float>()[] <= 0.00001f ){
                    State legal;
                    legal.ReshapeLike(state);
                    legal.set_input(i,j,p,q,1.0);
                    Action action(j,p,q);
                    this->current_legal_states_->push_back(legal);
                    this->current_legal_action_->push_back(action);
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