#include "coap.h"
#include "monitor.h"

static Logger& logger = Logger::get_instance();

class MyServer: public COAPServer {
public:
    MyServer():COAPServer(8888) {
        printf("init myserver\n");
    }

    virtual void data_handler(void* arg) {
        logger.log(DEBUG, "into data_handler");
        COAPMessage* msg = (COAPMessage*)arg;
        msg->print();
        sensor_t* data = (sensor_t*)msg->get_data();
        printf("temperture is %d, humidity is %d\n", 
            data->temperature, data->humidity);
    }
};

int main(int argc, char** argv) {
    MyServer* ms = new MyServer();
    ms->run();
    return 0;
}
