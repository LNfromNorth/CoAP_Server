#ifndef __COAP_SERVER_ARGS_H__
#define __COAP_SERVER_ARGS_H__

#include <iostream>


class Args {
public:
    Args(int argc, char** args);
    ~Args() {}
    int get_init_flag() {return init_flag; }
    int get_tp_num() {return tp_num;}
private:
    int parse(int argc, char** argv);
private:
    int tp_num = 5;
    int init_flag = 0;
};

#endif
