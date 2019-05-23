#include <iostream>
#include <memory>
#include "pool2/pool_types.h"

struct Node {
    int first;
    int last;
    int base;
    int some;
    int other;
};

void test_three(pool::PoolType<Node> *pool) {
    std::cout << "Test 3 started:" << std::endl << std::endl;

    auto node1 = pool->get();
    std::cout << "get node1: " << node1->first << std::endl;

    auto node2 = pool->get();
    std::cout << "get node2: " << node2->first << std::endl;

    auto node3 = pool->get();
    std::cout << "get node3: " << node3->first << std::endl;

    node1->first = node1->other = 1;
    node2->first = node2->other = 2;
    node3->first = node3->other = 3;

    std::cout << "node1: " << node1->first << " " << node1->other << std::endl;
    std::cout << "node2: " << node2->first << " " << node2->other << std::endl;
    std::cout << "node3: " << node3->first << " " << node3->other << std::endl;

    pool->release(node1);
    pool->release(node2);

    std::cout << "free node1: " << node1->first << " " << node1->other << std::endl;
    std::cout << "free node2: " << node2->first << " " << node2->other << std::endl;

    node1 = pool->get();
    node2 = pool->get();

    std::cout << "back node1: " << node1->first << " " << node1->other << std::endl;
    std::cout << "back node2: " << node2->first << " " << node2->other << std::endl;

    node1->first = 7;
    node2->first = 8;

    std::cout << "node1: " << node1->first << " " << node1->other << std::endl;
    std::cout << "node2: " << node2->first << " " << node2->other << std::endl;
    std::cout << "node3: " << node3->first << " " << node3->other << std::endl;

    std::cout << "Test 3 ended:" << std::endl << std::endl;
}

int main() {
    auto pool2 = std::make_shared<pool::PoolType<Node>>(5);
    test_three(pool2.get());
    std::cout << "End\n";

    return 0;
}
