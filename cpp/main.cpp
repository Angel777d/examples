#include <iostream>
#include <memory>
#include <type_traits>
#include <vector>

template<class T>
class ObjectPool {
private:
    uint m_block_size;
    uint m_size;

    std::vector<T> m_storage;
    std::vector<T *> m_pointers;

public:
    ObjectPool(uint block_size = 1000, uint blocks_count = 1) :
            m_block_size(block_size),
            m_size(0),
            m_storage(0),
            m_pointers(0) {
    }

    void freeItem(T *ptr) {
        m_pointers.emplace_back(ptr);
    }

    T *getItem() {
        if (m_pointers.empty()) {
            grow(1);
        }

        T *result = m_pointers.back();
        m_pointers.pop_back();

        return result;
    }

    uint getSize() {
        return m_size;
    }

private:
    void grow(uint blocks) {
        uint delta = blocks * m_block_size;
        uint new_size = m_size + delta;

        // m_storage.reserve(new_size);
        // std::fill(m_storage.begin() + m_size, m_storage.begin() + new_size, T());
        m_storage.resize(new_size);

        m_pointers.reserve(m_pointers.size() + delta);
        for (int i = m_size; i < new_size; ++i) {
            m_pointers.emplace_back(&*m_storage.begin() + i);
        }

        m_size = new_size;
    }
};

struct Node {
    int fist = 0;
};

int main() {
    auto pool_obj = std::make_shared<ObjectPool<Node>>(35, 3);
    std::cout << "create pool obj. pool size is: " << pool_obj->getSize() << "\n";

    std::vector<Node *> tmp_nodes;

    int count = 150;
    for (int i = 0; i < count; ++i) {
        tmp_nodes.push_back(pool_obj->getItem());
        tmp_nodes[i]->fist = i;
    }
    std::cout << "get " << count << " items. pool size is: " << pool_obj->getSize() << "\n";

    for (int i = 0; i < count; ++i) {
        pool_obj->freeItem(tmp_nodes[i]);
    }
    tmp_nodes.clear();
    std::cout << "push " << count << " items back. pool size is: " << pool_obj->getSize() << "\n";

    std::cout << "End\n";
    return 0;
}
