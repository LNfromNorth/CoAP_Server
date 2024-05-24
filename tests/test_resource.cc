#include <iostream>
#include "Resource.h"

int main() {
    ResourceManager manager;

    // 添加新资源
    manager.postResource("devices/device1/sensors/temperature", "23.5");

    // 获取资源数据
    std::string resourceData = manager.getResource("devices/device1/sensors/temperature");
    std::cout << "GET Resource: " << resourceData << std::endl;

    // 更新资源数据
    manager.putResource("devices/device1/sensors/temperature", "24.0");

    // 获取更新后的资源数据
    resourceData = manager.getResource("devices/device1/sensors/temperature");
    std::cout << "GET Updated Resource: " << resourceData << std::endl;

    // 删除资源
    manager.deleteResource("devices/device1/sensors/temperature");

    // 尝试获取已删除的资源数据
    resourceData = manager.getResource("devices/device1/sensors/temperature");
    std::cout << "GET Deleted Resource: " << resourceData << std::endl;

    return 0;
}