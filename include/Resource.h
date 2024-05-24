#ifndef __COAP_SERVER_RESOURCE_H
#define __COAP_SERVER_RESOURCE_H

#include <iostream>
#include <string>
#include <unordered_map>

class ResourceNode {
public:
    std::string name;
    std::string data;
    std::unordered_map<std::string, ResourceNode*> children;

    ResourceNode(const std::string& nodeName) : name(nodeName) {}

    ~ResourceNode() {
        for (auto& child : children) {
            delete child.second;
        }
    }
};

class ResourceManager {
private:
    ResourceNode* root;

    // 辅助函数：递归查找资源节点
    ResourceNode* findNode(const std::string& path) {
        ResourceNode* currentNode = root;
        size_t start = 0, end = 0;
        while ((end = path.find('/', start)) != std::string::npos) {
            std::string part = path.substr(start, end - start);
            if (currentNode->children.find(part) == currentNode->children.end()) {
                return nullptr;
            }
            currentNode = currentNode->children[part];
            start = end + 1;
        }
        std::string lastPart = path.substr(start);
        if (currentNode->children.find(lastPart) == currentNode->children.end()) {
            return nullptr;
        }
        return currentNode->children[lastPart];
    }

public:
    ResourceManager() {
        root = new ResourceNode("root");
    }

    ~ResourceManager() {
        delete root;
    }

    // GET 方法，获取资源数据
    std::string getResource(const std::string& path) {
        ResourceNode* node = findNode(path);
        if (node) {
            return node->data;
        } else {
            return "Resource not found";
        }
    }

    // POST 方法，添加新资源
    void postResource(const std::string& path, const std::string& data) {
        ResourceNode* currentNode = root;
        size_t start = 0, end = 0;
        while ((end = path.find('/', start)) != std::string::npos) {
            std::string part = path.substr(start, end - start);
            if (currentNode->children.find(part) == currentNode->children.end()) {
                currentNode->children[part] = new ResourceNode(part);
            }
            currentNode = currentNode->children[part];
            start = end + 1;
        }
        std::string lastPart = path.substr(start);
        currentNode->children[lastPart] = new ResourceNode(lastPart);
        currentNode->children[lastPart]->data = data;
    }

    // PUT 方法，更新已有资源数据
    int putResource(const std::string& path, const std::string& data) {
        ResourceNode* node = findNode(path);
        if (node) {
            node->data = data;
            return 1;
        } else {
            std::cerr << "Resource not found, cannot update" << std::endl;
            return 0;
        }
    }

    // DELETE 方法，删除资源
    int deleteResource(const std::string& path) {
        ResourceNode* currentNode = root;
        size_t start = 0, end = 0;
        while ((end = path.find('/', start)) != std::string::npos) {
            std::string part = path.substr(start, end - start);
            if (currentNode->children.find(part) == currentNode->children.end()) {
                std::cerr << "Resource not found, cannot delete" << std::endl;
                return 0;
            }
            currentNode = currentNode->children[part];
            start = end + 1;
        }
        std::string lastPart = path.substr(start);
        if (currentNode->children.find(lastPart) == currentNode->children.end()) {
            std::cerr << "Resource not found, cannot delete" << std::endl;
        } else {
            delete currentNode->children[lastPart];
            currentNode->children.erase(lastPart);
        }
        return 1;
    }
};


#endif