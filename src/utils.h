#pragma once

#include <iostream>
#include <type_traits>

namespace chipp8 {

namespace utils {

constexpr inline void pp_array(const auto &arr) {
    for (auto i = 0u; i < arr.size(); ++i) {
        using elm_type = typename std::remove_cv_t<typename std::remove_reference_t<decltype(arr[0])>>;
        constexpr elm_type bit_cnt = sizeof(elm_type) * 8u;
        elm_type mask = 1u << (bit_cnt - 1);
        for (auto j = 0u; j < bit_cnt; ++j) {
            std::cout << ((arr[i] & mask) ? "#" : " ");
            mask >>= 1;
        }
        std::cout << "\n";
    }
}

constexpr inline void pp_display(const auto &disp, size_t w, size_t h) {
    for (auto i = 0u; i < w; ++i) {
        for (auto j = 0u; j < h; ++j) {
            std::cout << (disp[i*j] == 1 ? "#" : " ");
        }
        std::cout << "\n";
    }
}

}

} // namespace chipp8
