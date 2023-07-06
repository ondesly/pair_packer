//
//  test_packer.cpp
//  pair_packer
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 05.07.2023.
//  Copyright © 2023 Dmitrii Torkhov. All rights reserved.
//

#include <string>

#include <pair_packer/packer.h>

int main() {

    // String to Vector

    {
        std::string data;
        oo::packer<std::string> packer{data};

        packer.begin_block(); // + 2
        packer.add(uint8_t(1), uint32_t(2)); // + 5
        packer.end_block();

        packer.begin_block(); // + 2
        packer.add(int32_t(3), int8_t(4)); // + 5
        packer.end_block();

        packer.begin_block(); // + 2
        packer.add(std::string("5"), std::string("6")); // + 6
        packer.end_block();

        assert(data.size() == 22);

        //

        oo::packer<std::string> unpacker{data};

        std::vector<std::pair<uint8_t, uint32_t>> b1;
        unpacker.fill(b1);
        assert(b1.size() == 1);
        assert(b1.front().first == 1);
        assert(b1.front().second == 2);

        std::vector<std::pair<int32_t, int8_t>> b2;
        unpacker.fill(b2);
        assert(b2.size() == 1);
        assert(b2.front().first == 3);
        assert(b2.front().second == 4);

        std::vector<std::pair<std::string, std::string>> b3;
        unpacker.fill(b3);
        assert(b3.size() == 1);
        assert(b3.front().first == "5");
        assert(b3.front().second == "6");
    }

    // Vector to Map

    {
        {
            std::vector<uint8_t> data;
            oo::packer<std::vector<uint8_t>> packer{data};

            packer.begin_block(); // + 2
            packer.add(uint8_t(1), std::string("1")); // + 4
            packer.add(uint8_t(2), std::string("2")); // + 4
            packer.add(uint8_t(3), std::string("3")); // + 4
            packer.end_block();

            packer.begin_block(); // + 2
            packer.add(uint8_t(4), std::string("4")); // + 4
            packer.add(uint8_t(5), std::string("5")); // + 4
            packer.add(uint8_t(6), std::string("6")); // + 4
            packer.end_block();

            assert(data.size() == 28);

            //

            oo::packer<std::vector<uint8_t>> unpacker{data};

            std::unordered_map<uint8_t, std::string> b1;
            unpacker.fill(b1);
            assert(b1.size() == 3);
            assert(b1.begin()->first == 1 || b1.begin()->first == 2 || b1.begin()->first == 3);
            assert(b1.begin()->second == "1" || b1.begin()->second == "2" || b1.begin()->second == "3");

            std::unordered_map<uint8_t, std::string> b2;
            unpacker.fill(b2);
            assert(b2.size() == 3);
            assert(b2.begin()->first == 4 || b2.begin()->first == 5 || b2.begin()->first == 6);
            assert(b2.begin()->second == "4" || b2.begin()->second == "5" || b2.begin()->second == "6");
        }
    }

    // Unknown block count

    {
        std::string data;
        oo::packer<std::string> packer{data};

        for (int32_t i = 0; i < 1000; ++i) {
            packer.begin_block(); // + 2
            packer.add(i, -i); // + 8
            packer.end_block();
        }

        assert(data.size() == 10000);

        //

        oo::packer<std::string> unpacker{data};

        std::vector<std::vector<std::pair<int32_t, int32_t>>> v;
        while (unpacker.can_fill()) {
            v.emplace_back();
            unpacker.fill(v.back());
            assert(v.back().size() == 1);
        }
        assert(v.size() == 1000);
        assert(v.front().front().first == 0);
        assert(v.front().front().second == 0);
        assert(v.back().front().first == 999);
        assert(v.back().front().second == -999);
    }

    return 0;
}
