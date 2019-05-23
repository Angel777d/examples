//
// Created by angel on 23.05.2019.
//

#ifndef CPP_POOL2_H
#define CPP_POOL2_H

#include <vector>

namespace pool {
    class Pool2 {
    public:
        Pool2(std::size_t objSize, std::size_t pageSize);

        ~Pool2();

        void *get();

        void release(void *obj);

    private:
        void *grow();

        void *fillPage(void *page);

    private:
        std::size_t m_objSize;
        std::size_t m_pageSize;

        void *m_topNode;
        std::vector<void *> m_pages;

        std::size_t m_pages_allocated;
        std::size_t m_items_provided;
    };
}
#endif //CPP_POOL2_H
