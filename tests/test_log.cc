#include "Log.h"

int main() {
    Logger& logger = Logger::get_instance();

    logger.log(INFO, "Program started.");
    logger.log(DEBUG, "Debugging infomation.");
    logger.log(ERROR, "An error occurred.");

    return 0;
}