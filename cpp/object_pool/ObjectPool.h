//
// Created by angel on 03.02.2019.
//

#ifndef CPP_OBJECTPOOL_H
#define CPP_OBJECTPOOL_H

namespace pool {

    template<typename T>
    class Node {
    private:
        void *m_start;
        T **m_link;
        T *m_body;
    public:
        explicit Node(void *start) : m_start(start) {
            m_link = (T **) start;
            T **link = m_link;
            ++link;
            m_body = (T *) link;
        }

        T *getBody() {
            return m_body;
        }

        T *getPrevBody() {
            return *m_link;
        }

        void makeLinkToBody(T *body) {
            *m_link = body;
        }

        Node pointerToNextInPage() {
            T *body = m_body;
            ++body;
            return Node((void *) body);
        }
    };

    template<typename T>
    static Node<T> nodeByBody(T *body) {

        T **link = (T **) body;
        --link;
        return Node<T>((void *) link);
    }


    template<typename T>
    class Page {
    private :
        void *m_page_ptr;
    public:
        explicit Page(size_t capacity) {
            T example = T();
            size_t item_size = sizeof(example);
            size_t itemptr_szie = sizeof(&example);
            size_t mem_size = capacity * (itemptr_szie + item_size);
            m_page_ptr = malloc(mem_size);
        }

        ~Page() {
            free(m_page_ptr);
        }

        T *fillPage(size_t capacity) {
            Node<T> node = Node<T>(m_page_ptr);
            node.makeLinkToBody(nullptr);
            for (size_t i = 1; i < capacity; ++i) {
                Node<T> next = node.pointerToNextInPage();
                T *body = node.getBody();
                next.makeLinkToBody(body);
                node = next;
            }
            return node.getBody();
        }
    };

    template<typename T>
    class ObjectPool {
    private:
        size_t m_page_size;
        size_t m_size;
        T *m_top_item;
        std::vector <std::shared_ptr<Page<T>>> m_pages;
    public:
        explicit ObjectPool<T>(size_t page_size) :
                m_page_size(page_size),
                m_size(0),
                m_top_item(nullptr) {
        }

        ~ObjectPool() {
            size_t total_size = m_pages.size() * m_page_size;
            if (total_size != m_size) {
                throw std::invalid_argument("not all objects back in pool");
            }

            m_top_item = nullptr;
            m_pages.clear();
        }

        size_t getSize() {
            return m_size;
        };

        void freeItem(T *ptr) {
            Node<T> node = nodeByBody(ptr);
            node.makeLinkToBody(m_top_item);
            m_top_item = ptr;
            ++m_size;
        }

        T *getItem() {
            // create new page if there is no items left
            if (m_top_item == nullptr) {
                m_top_item = grow();
            }

            T *result = m_top_item;

            Node<T> node = nodeByBody(m_top_item);
            m_top_item = node.getPrevBody();

            --m_size;

            return result;
        }

    private:
        T *grow() {
            auto page = std::make_shared<Page<T>>(m_page_size);
            m_pages.emplace_back(page);
            T *top_node = page->fillPage(m_page_size);
            m_size += m_page_size;
            return top_node;
        }

    };

} //namespace pool


#endif //CPP_OBJECTPOOL_H
