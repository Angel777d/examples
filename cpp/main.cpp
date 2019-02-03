#include <iostream>
#include <memory>
#include <vector>
#include "object_pool/ObjectPool.h"

struct Node {
    int first;
};

#define NodePool std::shared_ptr<pool::ObjectPool<Node>>


void test_one(const NodePool &pool_obj) {
    int len = 3;
    std::cout << std::endl << "Test 1 started:" << std::endl;
    std::vector<Node *> tmp_nodes;
    for (int i = 0; i < len; ++i) {
        Node *item = pool_obj->getItem();
        item->first = i;
        tmp_nodes.push_back(item);
    }

    Node *innerNode = pool_obj->getItem();
    innerNode->first = 100500;
    pool_obj->freeItem(innerNode);

    for (int i = 0; i < len; ++i) {
        Node *item = tmp_nodes[i];
        pool_obj->freeItem(item);
    }
    tmp_nodes.clear();

    len *= 2;
    for (int i = 0; i < len; ++i) {
        Node *item = pool_obj->getItem();
        tmp_nodes.push_back(item);
        std::cout << "  read first:" << item->first << std::endl;
    }

    for (int i = 0; i < len; ++i) {
        Node *item = tmp_nodes[i];
        pool_obj->freeItem(item);
    }
    tmp_nodes.clear();

    std::cout << "Test 1 ended:" << std::endl << std::endl;
}

void test_two(const NodePool &pool_obj) {
    std::cout << std::endl << "Test 2 started:" << std::endl;

    std::vector<Node *> tmp_nodes;

    int count = 150;
    for (int i = 0; i < count; ++i) {
        tmp_nodes.push_back(pool_obj->getItem());
        tmp_nodes[i]->first = i;
    }
    std::cout << "  get " << count << " items. pool size is: " << pool_obj->getSize() << std::endl;

    for (int i = 0; i < count; ++i) {
        pool_obj->freeItem(tmp_nodes[i]);
    }
    tmp_nodes.clear();
    std::cout << "  push " << count << " items back. pool size is: " << pool_obj->getSize() << std::endl;
    std::cout << "Test 2 ended:" << std::endl << std::endl;
}

int main() {
    NodePool pool_obj = std::make_shared<pool::ObjectPool<Node>>(100);
    std::cout << "create pool obj. pool size is: " << pool_obj->getSize() << std::endl;

    test_one(pool_obj);

    std::cout << "after test_one one pool size is: " << pool_obj->getSize() << std::endl;

    test_two(pool_obj);

    std::cout << "End\n";

    return 0;
}
