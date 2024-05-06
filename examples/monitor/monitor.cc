#include "coap.h"
#include "monitor.h"

static int times = 0;

class MyServer: public COAPServer {
public:
    MyServer():COAPServer(8888) {
        printf("init myserver\n");
    }

    virtual void data_handler(void* arg) {
        COAPMessage* msg = (COAPMessage*)arg;
        // msg->print();
        sensor_t* data = (sensor_t*)msg->get_data();
        printf("temperture is %d, humidity is %d\n", 
            data->temperature, data->humidity);
        printf("times = %d\n", times++);
    }
};

int main(int argc, char** argv) {
    MyServer* ms = new MyServer();
    ms->run();
    return 0;
}
