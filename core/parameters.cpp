#include "core/parameters.h"
namespace Beta{
void read_from_text(string file, Parameters* parameters){
    int fd = open(file.c_str(), O_RDONLY);            
    FileInputStream* input = new FileInputStream(fd);
    bool success = google::protobuf::TextFormat::Parse(input, (Message*)(parameters));
    delete input;
    const google::protobuf::Descriptor* desc = 
    static_cast<Message*>(parameters)->GetDescriptor();
    for(int i = 0; i < desc->field_count(); ++ i){
        const google::protobuf::FieldDescriptor* field = desc->field(i);
        LOG(INFO)<< field->name()<<" ("<<field->type_name()<<") ";
    }
    //LOG(INFO)<<"max_depth: "<< parameters->depth();
    close(fd);
}

static Parameters* global_parameters(){
    static Parameters* param = new Parameters();
    return param;
}

}