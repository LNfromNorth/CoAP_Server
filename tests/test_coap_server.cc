#include "CoAPServer.h"

int main() {
    COAPServer cs = COAPServer(8888);
    cs.run();
}
