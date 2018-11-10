#include "core/renju_action.h"

namespace Beta{
    RenJuAction::RenJuAction(){

    }
    RenJuAction::~RenJuAction(){

    }
    RenJuAction::RenJuAction(int c, int y, int x){

    }
    RenJuAction::RenJuAction(const RenJuAction& action){

    }

    void RenJuAction::calc_confidence(){
        confidence_ = (1.0f - epsilon_)*prior_ + epsilon_* noise_;
    }

    const float RenJuAction::prior(){
        return prior_;
    }
    const float RenJuAction::confidence(){
        return confidence_;
    }
    const float RenJuAction::epsilon(){
        return epsilon_;
    }
    const float RenJuAction::noise(){
        return noise_;
    }

    const int RenJuAction::x(){
        return x_;
    }
    const int RenJuAction::y(){
        return y_;
    }
    const int RenJuAction::c(){
        return c_;
    }

    int* RenJuAction::mutable_x(){
        return & x_;
    }
    int* RenJuAction::mutable_y(){
        return & y_;
    }
    int* RenJuAction::mutable_c(){
        return & c_;
    }

    float* RenJuAction::mutable_prior(){
        return & prior_;
    }

    float* RenJuAction::mutable_confidence(){
        return & confidence_;
    }
    float* RenJuAction::mutable_noise(){
        return & noise_;
    }
    float* RenJuAction::mutable_epsilon(){
        return & epsilon_;
    }




}

