#ifndef __COAP_SERVER_FMANAGER__
#define __COAP_SERVER_FMANAGER__

#include <chrono>
#include <memory>
#include <atomic>
#include <stdint.h>
#include <stdio.h>

#define FLAG_NUM 10

class COAPACKF {
public:
    COAPACKF() 
    :flag(true),
    msg_id(0) {};

    ~COAPACKF() {}

    bool is_set() const {
        return flag.load();
    }

    void set(bool value) {
        flag.store(value);
    }

    uint16_t get_msgid() {
        return msg_id;
    }

    void set_msgid(uint16_t value) {
        msg_id = value;
    }

private:
    std::atomic<bool> flag;
    uint16_t msg_id;
};

class FManager {
public:
    FManager() {
        for(int i = 0; i < FLAG_NUM; i++) {
            is_alloc[i] = 0;
        }
    }

    ~FManager() {
    }

    int alloc_f(uint16_t msg_id) {
        for(int i = 0; i < FLAG_NUM; i++) {
            if(is_alloc[i]==0) {
                is_alloc[i] = 1;
                flags[i].set(true);
                flags[i].set_msgid(msg_id);
                return i;
            }
        }
        return -1;
    }

    void delete_f(int index) {
        is_alloc[index] = 0;
    }

    bool check_f(int index) {
        return flags[index].is_set();
    }

    void set_f(uint16_t msg_id) {
        for(int i = 0; i < FLAG_NUM; i++) {
            if(is_alloc[i] == 1) {
                if(flags[i].get_msgid() == msg_id) {
                    flags[i].set(false);
                }
            }
        }
    }

private:
    COAPACKF flags[FLAG_NUM];
    int is_alloc[FLAG_NUM];
};

#endif
