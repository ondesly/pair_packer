//
//  unpacker.h
//  pair_packer
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 06.07.2023.
//  Copyright © 2023 Dmitrii Torkhov. All rights reserved.
//

#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "common.h"

namespace oo {

    template<class C, class S = uint16_t>
    class unpacker {
    public:

        explicit unpacker(const C &container) : m_container(container) {
            if (m_container.size() >= sizeof(packer_header) && is_header_valid()) {
                m_block_begin += sizeof(packer_header);
            }
        }

        [[nodiscard]] bool is_header_valid() const {
            return std::memcmp(m_container.data(), packer_header, sizeof(packer_header)) == 0;
        }

        [[nodiscard]] bool can_fill() const {
            return m_block_begin < m_container.size();
        }

        template<class K, class V>
        void fill(std::vector<std::pair<K, V>> &container) {
            S size;
            auto index = get(m_block_begin, size);
            container.reserve(size / (sizeof(K) + sizeof(V)));

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
            container.reserve(size / (sizeof(K) + sizeof(V)));

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

        const C &m_container;
        size_t m_block_begin{};

    private:

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

    };

}
