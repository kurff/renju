
#ifndef CORE_RENJU_CONTEXT_H_
#define CORE_RENJU_CONTEXT_H_
#include "core/context.h"
namespace Beta{

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

}




#endif