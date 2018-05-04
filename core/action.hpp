#ifndef __ACTION_HPP__
#define __ACTION_HPP__

#include <vector>

//#include "caffe2/core/tensor.h"
//using namespace caffe2;
namespace Beta{
class Action{
    public:
        Action(){

        }

        ~Action(){

        }

        //virtual void play(Tensor<CPUContext>& probability) = 0;
        
        bool & legal(){return legal_;}

    protected:
        bool legal_;



};




class GoAction: public Action{
    public:
        GoAction():x_(0),y_(0){

        }
        ~GoAction(){

        }

        //template<>
        // choose action 

        //void play(Tensor<CPUContext>& probability){
            

            
       // }



        int x(){return x_;}
        int y(){return y_;}
        
    protected:
        int x_;
        int y_;
        //std::vector<>
};






//class ChessAction: public Action{
class ChessAction: public Action{
    public:
        ChessAction():epsilon_(0.25f),x_(0),y_(0)
        ,c_(0),prior_(0.0f),confidence_(0.0f), noise_(0.0f){

        }
        ~ChessAction(){

        }
        const int x() const {return x_;}
        const int y() const {return y_;}
        const int c() const {return c_;}
        const float prior() const {return prior_;}
        const float confidence() {
            return (1-epsilon_)*prior_ + epsilon_*noise_;
        }
        const float noise(){return noise_;}

        void set_x(int x){x_ = x;}
        void set_y(int y){y_ = y;}
        void set_c(int c){c_ = c;}


        void set_prior(float prior){prior_=prior;}
        void set_noise(float noise){noise_ = noise;}

    protected:  
        int x_; // the 
        int y_;
        int c_;
        float prior_;
        float confidence_;  // confidence_ = (1-epsilon)* w + epsilon * noise_; 
        float noise_;
        float epsilon_;        

};


}

#endif 