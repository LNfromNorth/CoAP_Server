#include <string.h>
#include <iostream>
#include <stdlib.h>

#include "Args.h"
#include "Log.h"

static Logger& logger = Logger::get_instance();

typedef int (*doing_handler)(char* arg);
static int s_tp_num = 5;
static int is_help = 0;

int doing_help(char* arg);
int doing_threadpool(char* arg);


struct item{
    char name;
    char* lname;
    char* detil;
    doing_handler handler; 
} args_items[] {
    'h', (char*)"help",        (char*)"get the help for this program",    doing_help,
    't', (char*)"threadpool",  (char*)"set the num of threadpool",        doing_threadpool
};

#define SIZE_ITEM 2

int doing_help(char* arg) {
    std::cout << "Usage: coap_server [options] [target] ..." << std::endl;
    std::cout << "Options:" << std::endl;
    for(int i = 0; i < SIZE_ITEM; i++) {
        std::cout << "    -"
                  << args_items[i].name << ", "
                  << args_items[i].lname << ", "
                  << args_items[i].detil << std::endl;
    }
    is_help = 1;
    return 0;
}

int doing_threadpool(char* arg) {
    int v = atoi(arg);
    if(v == 0) {
        logger.log(INFO, "get the wrong num of threadpool, set as default");
    } else {
        s_tp_num = v;
    }
    return 0;
}

int Args::parse(int argc, char** argv) {
    is_help = 0;
    for(int i = 1; i < argc; i++) {
        int j = 0;
        int ret = 0;
        if(argv[i][0] == '-') {                             // -X
            if(argv[i][1] == '-') {                         // --XXX
                char* cmp = argv[i] + 2;
                for(; j < SIZE_ITEM; j++) {
                    if(!strcmp(cmp, args_items[j].lname)) {
                        ret = args_items[j].handler(argv[i + 1]);
                        break;
                    }
                }
            } else {                                        // -X
                for(; j < SIZE_ITEM; j++) {
                    if(args_items[j].name == argv[i][1]) {
                        ret = args_items[j].handler(argv[i + 1]);
                        break;
                    }
                }
            }
        }
        if(is_help == 1) return 0;
        if(j == SIZE_ITEM) return -1;
        if(ret == -1) return 0;
        i++;
    }
    return 0;
}

Args::Args(int argc, char** argv) {
    int ret = parse(argc, argv);
    if(ret == -1) {
        std::cout << "Wrong Commond!" << std::endl;
        doing_help(NULL);
        init_flag = 0;
    } else if(ret == 0) {
        init_flag = 0;
    } else {
        init_flag = 1;
        tp_num = s_tp_num;
    }
}

