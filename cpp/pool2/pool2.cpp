//
// Created by angel on 23.05.2019.
//

#include "pool2.h"
#include <cstdlib>
#include <cstdint>
#include <cstring>

pool::Pool2::Pool2(std::size_t objSize, std::size_t pageSize) :
        m_objSize(objSize),
        m_pageSize(pageSize),
        m_topNode(nullptr),
        m_pages(),
        m_pages_allocated(0),
        m_items_provided(0) {
    if (m_objSize < sizeof(uintptr_t)) {
        m_objSize = sizeof(uintptr_t);
    }
}

pool::Pool2::~Pool2() {
    for (auto page : m_pages) {
        free(page);
    }
    m_pages.clear(); //?
}

void *pool::Pool2::get() {
    if (m_topNode == nullptr) {
        m_topNode = grow();
    }

    auto result = m_topNode;
    auto *prev_value_ptr = (uintptr_t *) m_topNode;
    auto prev_ptr_view = *prev_value_ptr;
    m_topNode = (void *) prev_ptr_view;
    ++m_items_provided;
    return result;
}

void pool::Pool2::release(void *obj) {
    void *prev = m_topNode;
    m_topNode = obj;
    memcpy(m_topNode, &prev, sizeof(uintptr_t));
    --m_items_provided;
}

void *pool::Pool2::grow() {
    auto page = malloc(m_objSize * m_pageSize);
    m_pages.emplace_back(page);
    ++m_pages_allocated;
    return fillPage(page);
}

void *pool::Pool2::fillPage(void *page) {
    void *position = page;
    void *null_value = nullptr;
    memcpy(position, &null_value, sizeof(uintptr_t));
    for (size_t i = 1; i < m_pageSize; ++i) {
        void *nextPosition = static_cast<uintptr_t *>(position) + m_objSize;
        memcpy(nextPosition, &position, sizeof(uintptr_t));
        position = nextPosition;
    }
    return position;
}

