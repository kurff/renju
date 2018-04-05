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



class RenjuAction: public Action{
    public:
        RenjuAction(){

        }
        ~RenjuAction(){

        }

        
    protected:
        int x_;
        int y_;

};


//class ChessAction: public Action{
class ChessAction: public Action{
    public:
        ChessAction(){

        }
        ~ChessAction(){

        }

        // select action from probability
        //void play(Tensor<CPUContext>& probability){




        //}

        const int x() const {return x_;}
        const int y() const {return y_;}
        const int c() const {return c_;}
        const float w() const {return w_;}
        const float confidence() const {return confidence_;}
        const float noise(){return noise_;}

        void set_x(int x){x_ = x;}
        void set_y(int y){y_ = y;}
        void set_c(int c){c_ = c;}

        void set_w(float w){w_=w;}
        void set_noise(float noise){noise_ = noise;}
        void set_confidence(float confidence){confidence_ = confidence;}


    protected:  
        int x_; // the 
        int y_;
        int c_;
        float w_;
        float confidence_;  // confidence_ = (1-epsilon)* w + epsilon * noise_; 
        float noise_;        

};


}

#endif 