#include "CoAPServer.h"

int main() {
    COAPServer cs = COAPServer(10, 8888);
    cs.run();
}