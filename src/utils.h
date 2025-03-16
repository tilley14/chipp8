#pragma once

#include <iostream>
#include <string>
#include <type_traits>

namespace chipp8 {

namespace utils {

constexpr inline void pp_array(const auto &arr) {
    std::string buf;
    for (auto i = 0u; i < arr.size(); ++i) {
        using elm_type = typename std::remove_cv_t<typename std::remove_reference_t<decltype(arr[0])>>;
        constexpr elm_type bit_cnt = sizeof(elm_type) * 8u;
        elm_type mask = 1u << (bit_cnt - 1);
        for (auto j = 0u; j < bit_cnt; ++j) {
            buf += ((arr[i] & mask) ? "#" : " ");
            mask >>= 1;
        }
        buf += "\n";
    }

    std::cout << buf << std::endl;
}

constexpr inline void pp_display(const auto &disp, size_t w, size_t h) {
    std::string buf;

    buf += " ";
    for (auto j = 0u; j < w; ++j) {
        buf += std::to_string((j%10));
    }
    buf += "\n";

    for (auto y = 0u; y < h; ++y) {
        buf += std::to_string((y%10));
        for (auto x = 0u; x < w; ++x) {
            buf += (disp[x+(y*w)] ? "#" : " ");
        }
        buf += "\n";
    }
    std::cout << buf << std::endl;
}

} // namespace utils

} // namespace chipp8
