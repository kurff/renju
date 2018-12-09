#include "core/state.h"
#include "core/renju_context.h"
#include "core/renju_action.h"
#include "mcts/mcts_client.h"
#include "opencv2/opencv.hpp"
namespace Beta{
    template<typename StateType, typename ActionType, typename ContextType>
    MCTSClient<StateType, ActionType, ContextType>::MCTSClient(std::shared_ptr<grpc::Channel> channel) 
    : MCTS<StateType, ActionType, ContextType>(), stub_(Evaluation::NewStub(channel)), queue_(new SimpleQueue<Request>() ){
        

    }
    
    template<typename StateType, typename ActionType, typename ContextType>
    MCTSClient<StateType, ActionType, ContextType>::~MCTSClient(){


    }

    template<typename StateType, typename ActionType, typename ContextType>
    void MCTSClient<StateType, ActionType, ContextType>::upload(){
        


    }


    template<typename StateType, typename ActionType, typename ContextType>
    string MCTSClient<StateType, ActionType, ContextType>::evaluate_train(){
        grpc::ClientContext grpc_context;
        Request request;
        queue_->Pop(&request);
        Reply reply;
        Status status = stub_->Forward_train(&grpc_context, request, &reply);
        if (status.ok()) {
            return "success";
        }else {
            std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
            return "RPC failed";
        }
    }


    template<typename StateType, typename ActionType, typename ContextType>
    string MCTSClient<StateType, ActionType, ContextType>::evaluate_test(){
        grpc::ClientContext grpc_context;
        Request request;
        queue_->Pop(&request);
        Reply reply;
        Status status = stub_->Forward_test(&grpc_context, request, &reply);
        if (status.ok()) {
            return "success";
        }else {
            std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
            return "RPC failed";
        }
    }

    template<typename StateType, typename ActionType, typename ContextType>
    string MCTSClient<StateType, ActionType, ContextType>::fake_example(){
        grpc::ClientContext grpc_context;
        Request request;
        
        Reply reply;
        Status status = stub_->Forward_test(&grpc_context, request, &reply);
        if (status.ok()) {
            LOG(INFO)<< reply.loss();
            return "success";
        }else {
            std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
            return "RPC failed";
        }
    }

    template<typename StateType, typename ActionType, typename ContextType>
    string MCTSClient<StateType, ActionType, ContextType>::load_example(const string& file){
        grpc::ClientContext grpc_context;
        Request request;
        cv::Mat image = cv::imread(file, -1);
        image.convertTo(image, CV_32FC1, 1.0);
        if(!image.empty()){
            request.mutable_s()->mutable_shape()->add_dim(1);
            request.mutable_s()->mutable_shape()->add_dim(image.channels());
            request.mutable_s()->mutable_shape()->add_dim(image.rows);
            request.mutable_s()->mutable_shape()->add_dim(image.cols);

            //request.mutable_s()->set_data(image.data);
            int number_element = image.rows *  image.cols;
            request.mutable_s()->mutable_data()->Resize( number_element, 0 );
            float* dst = request.mutable_s()->mutable_data()->mutable_data();
            std::memcpy(dst, image.data, sizeof(float)* number_element);

        }






        Reply reply;
        Status status = stub_->Forward_test(&grpc_context, request, &reply);
        if (status.ok()) {
            LOG(INFO)<< reply.loss();
            return "success";
        }else {
            std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
            return "RPC failed";
        }

    }

    template<typename StateType, typename ActionType, typename ContextType>
    string MCTSClient<StateType, ActionType, ContextType>::load_several_examples(const string& file_list){
        

    }


    template class MCTSClient<State, RenJuAction, RenJuContext<State, RenJuAction> >;

}