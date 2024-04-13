#include "CoAPServer.h"

int main() {
    COAPServer cs = COAPServer(10);
    cs.run();
}