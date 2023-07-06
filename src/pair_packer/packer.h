//
//  packer.h
//  pair_packer
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 05.07.2023.
//  Copyright © 2023 Dmitrii Torkhov. All rights reserved.
//

#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace oo {

    template<class C, class S = uint16_t>
    class packer {
    public:

        explicit packer(C &container) : m_container(container) {}

        template<class K, class V>
        void add(const K &key, const V &value) {
            add(key);
            add(value);
        }

        void begin_block() {
            m_block_begin = m_container.size();
            add(S(0));
        }

        void end_block() {
            const auto size = S(m_container.size() - m_block_begin);
            set(m_block_begin, size);
        }

        template<class K, class V>
        void fill(std::vector<std::pair<K, V>> &container) {
            S size;
            auto index = get(m_block_begin, size);

            while (index < size) {
                K key;
                index += get(m_block_begin + index, key);

                V value;
                index += get(m_block_begin + index, value);

                container.emplace_back(key, value);
            }

            m_block_begin += index;
        }

        template<class K, class V>
        void fill(std::unordered_map<K, V> &container) {
            S size;
            auto index = get(m_block_begin, size);

            while (index < size) {
                K key;
                index += get(m_block_begin + index, key);

                V value;
                index += get(m_block_begin + index, value);

                container.emplace(key, value);
            }

            m_block_begin += index;
        }

    private:

        C &m_container;
        size_t m_block_begin{};

    private:

        template<class P>
        inline void add(const P &value) {
            const auto ptr = reinterpret_cast<const typename C::value_type *>(&value);
            m_container.insert(m_container.end(), ptr, ptr + sizeof(P));
        }

        inline void add(const std::string &value) {
            add(S(value.size()));
            m_container.insert(m_container.end(), value.data(), value.data() + value.size());
        }

        template<class P>
        inline size_t get(size_t index, P &value) const {
            const auto ptr = m_container.data() + index;
            value = *reinterpret_cast<const P *>(ptr);
            return sizeof(P);
        }

        inline size_t get(size_t index, std::string &value) const {
            S size;
            index += get(index, size);
            value.insert(value.end(), m_container.data() + index, m_container.data() + index + size);
            return sizeof(S) + size;
        }

        template<class P>
        inline void set(size_t index, P &value) {
            const auto ptr = reinterpret_cast<const typename C::value_type *>(&value);
            std::copy_n(ptr, sizeof(P), m_container.data() + index);
        }

    };

}
