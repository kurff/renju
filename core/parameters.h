#ifndef __PARAMETERS_HPP__
#define __PARAMETERS_HPP__

#include "proto/beta.pb.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <stdint.h>
using google::protobuf::Message;
using google::protobuf::io::FileInputStream;
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include "glog/logging.h"
using namespace std;


namespace Beta{
void read_from_text(string file, Parameters* parameters);

static Parameters* global_parameters();



}


#endif