//
//  packer.h
//  pair_packer
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 05.07.2023.
//  Copyright © 2023 Dmitrii Torkhov. All rights reserved.
//

#pragma once

#include <string>

#include "common.h"

namespace oo {

    template<class C, class S = uint16_t>
    class packer {
    public:

        explicit packer(C &container) : m_container(container) {}

        void add_header() {
            m_container.insert(m_container.end(), std::begin(packer_header), std::end(packer_header));
        }

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
        inline void set(size_t index, P &value) {
            const auto ptr = reinterpret_cast<const typename C::value_type *>(&value);
            std::copy_n(ptr, sizeof(P), m_container.data() + index);
        }

    };

}
