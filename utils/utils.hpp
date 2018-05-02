#ifndef __UTILS_HPP__
#define __UTILS_HPP__

// sample from 
#include "glog/logging.h"
#include <vector>
#include <cmath>
#include <random>
using namespace std;


namespace Beta{
    class Dirichlet{
        public:
            Dirichlet(float alpha): alpha_(alpha){
            }
            ~Dirichlet(){

            }

            void push(float v){
                x_.push_back(v);
            }

            float run(){
                float prodr = pow(x_[0], (alpha_ - 1.0));
                /// sum of the alphas for the bottom of the B normalization function
                float alpha_sum = alpha_;
                float gamma_prod = tgamma(alpha_); 
                for(int j = 1; j < x_.size(); j++){
                    gamma_prod *= tgamma(alpha_);
                    alpha_sum  += alpha_;
                    prodr      *= pow(x_[j], (alpha_ - 1));
                }
  /// Normalization constant
                float B = gamma_prod / tgamma(alpha_sum);

                float pdf = (1.0/B) * prodr; 
                return pdf;
            }

            const float & alpha(){return alpha_;}
        protected:
            vector<float>  x_;
            vector<float> pdf_;
            float alpha_;



    };

    template<typename Element>
    class Sample{

        public: 
          Sample(){
              
            
          }
          ~Sample(){}
          

           int run(const vector<Element>& elements,  std::mt19937& gen){
                vector<float> distribution;
                for(auto& element : elements){
                   distribution.push_back(element.confidence());
                }
                
                std::discrete_distribution<float > distri(distribution.begin(), distribution.end());

                return distri(gen);
           }

            int run(const vector<Element*>& elements, std::mt19937& gen ){
                vector<float> distribution;
                for(auto & element: elements){
                    distribution.push_back(element->confidence());
                }
                std::discrete_distribution<float > distri(distribution.begin(), distribution.end());
                return distri(gen);    
           }
        protected:
            


            



         
          





    };








}
#endif