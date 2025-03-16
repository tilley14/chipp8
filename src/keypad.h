#pragma once

#include "chip8.h"
#include <stdint.h>

namespace chipp8 {

namespace keypad {

enum class Key : uint16_t {
    Key_0 = (1u << 0u),
    Key_1 = (1u << 1u),
    Key_2 = (1u << 2u),
    Key_3 = (1u << 3u),
    Key_4 = (1u << 4u),
    Key_5 = (1u << 5u),
    Key_6 = (1u << 6u),
    Key_7 = (1u << 7u),
    Key_8 = (1u << 8u),
    Key_9 = (1u << 9u),
    Key_A = (1u << 10u),
    Key_B = (1u << 11u),
    Key_C = (1u << 12u),
    Key_D = (1u << 13u),
    Key_E = (1u << 14u),
    Key_F = (1u << 15u),
};

constexpr inline void KEY_0_PRESSED(chip8& cpu) {
    cpu.keys |= static_cast<uint16_t>(Key::Key_0);
}

constexpr inline void KEY_0_RELEASED(chip8& cpu) {
    cpu.keys &= ~(static_cast<uint16_t>(Key::Key_0));
}

constexpr inline void KEY_1_PRESSED(chip8& cpu) {
    cpu.keys |= static_cast<uint16_t>(Key::Key_1);
}

constexpr inline void KEY_1_RELEASED(chip8& cpu) {
    cpu.keys &= ~(static_cast<uint16_t>(Key::Key_1));
}

constexpr inline void KEY_2_PRESSED(chip8& cpu) {
    cpu.keys |= static_cast<uint16_t>(Key::Key_2);
}

constexpr inline void KEY_2_RELEASED(chip8& cpu) {
    cpu.keys &= ~(static_cast<uint16_t>(Key::Key_2));
}

constexpr inline void KEY_3_PRESSED(chip8& cpu) {
    cpu.keys |= static_cast<uint16_t>(Key::Key_3);
}

constexpr inline void KEY_3_RELEASED(chip8& cpu) {
    cpu.keys &= ~(static_cast<uint16_t>(Key::Key_3));
}

constexpr inline void KEY_4_PRESSED(chip8& cpu) {
    cpu.keys |= static_cast<uint16_t>(Key::Key_4);
}

constexpr inline void KEY_4_RELEASED(chip8& cpu) {
    cpu.keys &= ~(static_cast<uint16_t>(Key::Key_4));
}

constexpr inline void KEY_5_PRESSED(chip8& cpu) {
    cpu.keys |= static_cast<uint16_t>(Key::Key_5);
}

constexpr inline void KEY_5_RELEASED(chip8& cpu) {
    cpu.keys &= ~(static_cast<uint16_t>(Key::Key_5));
}

constexpr inline void KEY_6_PRESSED(chip8& cpu) {
    cpu.keys |= static_cast<uint16_t>(Key::Key_6);
}

constexpr inline void KEY_6_RELEASED(chip8& cpu) {
    cpu.keys &= ~(static_cast<uint16_t>(Key::Key_6));
}

constexpr inline void KEY_7_PRESSED(chip8& cpu) {
    cpu.keys |= static_cast<uint16_t>(Key::Key_7);
}

constexpr inline void KEY_7_RELEASED(chip8& cpu) {
    cpu.keys &= ~(static_cast<uint16_t>(Key::Key_7));
}

constexpr inline void KEY_8_PRESSED(chip8& cpu) {
    cpu.keys |= static_cast<uint16_t>(Key::Key_8);
}

constexpr inline void KEY_8_RELEASED(chip8& cpu) {
    cpu.keys &= ~(static_cast<uint16_t>(Key::Key_8));
}

constexpr inline void KEY_9_PRESSED(chip8& cpu) {
    cpu.keys |= static_cast<uint16_t>(Key::Key_9);
}

constexpr inline void KEY_9_RELEASED(chip8& cpu) {
    cpu.keys &= ~(static_cast<uint16_t>(Key::Key_9));
}

constexpr inline void KEY_A_PRESSED(chip8& cpu) {
    cpu.keys |= static_cast<uint16_t>(Key::Key_A);
}

constexpr inline void KEY_A_RELEASED(chip8& cpu) {
    cpu.keys &= ~(static_cast<uint16_t>(Key::Key_A));
}

constexpr inline void KEY_B_PRESSED(chip8& cpu) {
    cpu.keys |= static_cast<uint16_t>(Key::Key_B);
}

constexpr inline void KEY_B_RELEASED(chip8& cpu) {
    cpu.keys &= ~(static_cast<uint16_t>(Key::Key_B));
}

constexpr inline void KEY_C_PRESSED(chip8& cpu) {
    cpu.keys |= static_cast<uint16_t>(Key::Key_C);
}

constexpr inline void KEY_C_RELEASED(chip8& cpu) {
    cpu.keys &= ~(static_cast<uint16_t>(Key::Key_C));
}

constexpr inline void KEY_D_PRESSED(chip8& cpu) {
    cpu.keys |= static_cast<uint16_t>(Key::Key_D);
}

constexpr inline void KEY_D_RELEASED(chip8& cpu) {
    cpu.keys &= ~(static_cast<uint16_t>(Key::Key_D));
}

constexpr inline void KEY_E_PRESSED(chip8& cpu) {
    cpu.keys |= static_cast<uint16_t>(Key::Key_E);
}

constexpr inline void KEY_E_RELEASED(chip8& cpu) {
    cpu.keys &= ~(static_cast<uint16_t>(Key::Key_E));
}

constexpr inline void KEY_F_PRESSED(chip8& cpu) {
    cpu.keys |= static_cast<uint16_t>(Key::Key_F);
}

constexpr inline void KEY_F_RELEASED(chip8& cpu) {
    cpu.keys &= ~(static_cast<uint16_t>(Key::Key_F));
}

} // namespace keypad

} // namespace chipp8
