//
// Created by angel on 23.05.2019.
//

#ifndef CPP_POOL_TYPES_H
#define CPP_POOL_TYPES_H

#include "pool2.h"


namespace pool {
    template<typename T>
    class PoolType {
    public:
        explicit PoolType<T>(size_t pageSize) {
            size_t size = sizeof(T);
            m_pool = std::make_shared<Pool2>(size, pageSize);
        }

        T *get() {
            return static_cast<T *> (m_pool->get());
        }

        void release(T *obj) {
            m_pool->release(obj);
        };

    private:
        std::shared_ptr<Pool2> m_pool;
    };
}

#endif //CPP_POOL_TYPES_H
