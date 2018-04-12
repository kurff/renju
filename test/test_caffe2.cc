#include "caffe2/core/operator.h"
#include "caffe2/core/init.h"
#include "network/net.h"
#include <vector>
#include <iostream>
using namespace std;
using namespace caffe2;
using namespace Beta;
int main(){
    Workspace workspace;
    Blob* blob = workspace.CreateBlob("loss");
    blob->GetMutable<TensorCPU>()->Resize(vector<int>{1, 2});
    float* data = blob->GetMutable<TensorCPU>()->mutable_data<float>();
    NetDef net_def;
    Net predict_net(net_def);
    predict_net.AddConstantFillWithOp(2.f, "loss", "loss_grad");
    
    unique_ptr<NetBase> x = CreateNet(net_def,&workspace);
    x->Run();
    TensorCPU tensor = workspace.GetBlob("loss_grad")->Get<TensorCPU>();
    cout<<tensor.dims()<<endl;
    cout<<tensor.mutable_data<float>()[1]<<endl;    

}