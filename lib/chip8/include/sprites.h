#pragma once

#include <array>
#include <cstdint>

namespace chipp8 {

namespace sprites {

constexpr const uint16_t SPRITE_WIDTH  = 8u;
constexpr const uint16_t MAX_SPRITE_SIZE = 15u;
constexpr const uint16_t FONT_SIZE = 5u;

constexpr const std::array<uint8_t, 5u> F_0 {0xF0u, 0x90u, 0x90u, 0x90u, 0xF0u};
constexpr const std::array<uint8_t, 5u> F_1 {0x20u, 0x60u, 0x20u, 0x20u, 0x70u};
constexpr const std::array<uint8_t, 5u> F_2 {0xF0u, 0x10u, 0xF0u, 0x80u, 0xF0u};
constexpr const std::array<uint8_t, 5u> F_3 {0xF0u, 0x10u, 0xF0u, 0x10u, 0xF0u};
constexpr const std::array<uint8_t, 5u> F_4 {0x90u, 0x90u, 0xF0u, 0x10u, 0x10u};
constexpr const std::array<uint8_t, 5u> F_5 {0xF0u, 0x80u, 0xF0u, 0x10u, 0xF0u};
constexpr const std::array<uint8_t, 5u> F_6 {0xF0u, 0x80u, 0xF0u, 0x90u, 0xF0u};
constexpr const std::array<uint8_t, 5u> F_7 {0xF0u, 0x10u, 0x20u, 0x40u, 0x40u};
constexpr const std::array<uint8_t, 5u> F_8 {0xF0u, 0x90u, 0xF0u, 0x90u, 0xF0u};
constexpr const std::array<uint8_t, 5u> F_9 {0xF0u, 0x90u, 0xF0u, 0x10u, 0xF0u};
constexpr const std::array<uint8_t, 5u> F_A {0xF0u, 0x90u, 0xF0u, 0x90u, 0x90u};
constexpr const std::array<uint8_t, 5u> F_B {0xE0u, 0x90u, 0xE0u, 0x90u, 0xE0u};
constexpr const std::array<uint8_t, 5u> F_C {0xF0u, 0x80u, 0x80u, 0x80u, 0xF0u};
constexpr const std::array<uint8_t, 5u> F_D {0xE0u, 0x90u, 0x90u, 0x90u, 0xE0u};
constexpr const std::array<uint8_t, 5u> F_E {0xF0u, 0x80u, 0xF0u, 0x80u, 0xF0u};
constexpr const std::array<uint8_t, 5u> F_F {0xF0u, 0x80u, 0xF0u, 0x80u, 0x80u};

constexpr auto all_font_sprites() {
    return std::array{F_0, F_1, F_2, F_3, F_4, F_5, F_6, F_7, F_8, F_9, F_A, F_B, F_C, F_D, F_E, F_F};
}

} // namespace sprites

} // namespace chipp8
