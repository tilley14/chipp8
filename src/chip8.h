#pragma once

#include <array>
#include <bitset>
#include <stdint.h>

#include "sprites.h"

/* https://en.wikipedia.org/wiki/CHIP-8#Virtual_machine_description
   https://chip8.gulrak.net/
*/

namespace chipp8 {

// 0x000 to 0x1FF
constexpr const uint16_t FONT_START_ADDR = 0x050u;
constexpr const uint16_t PROGRAM_START_ADDR = 0x0200u;
constexpr const uint16_t ETI_660_PROGRAM_START_ADDR = 0x0600u;

// Memory MAP
// 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
// 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
// 0x200-0xFFF - Program ROM and work RAM
struct chip8 {
    uint16_t keys;

    std::bitset<64u /*width*/ * 32u /*height*/ > pixels;

    // Interpreter occupies the first 512 bytes so most
    // programs begin at memory location 512 (0x200).
    //
    // The uppermost 256 bytes (0xF00-0xFFF) are reserved
    // for display refresh
    //
    // The 96 bytes below that (0xEA0-0xEFF) were reserved for
    // the call stack, internal use, and other variables
    //
    // In modern CHIP-8 impls, where the interpreter is running
    // outside the 4k memory, there is no need to avoid the lower 512 bytes
    // of memory and it is common to store font data there
    std::array<uint8_t, 4096u> mem;

    // v0-vFs
    // The VF register doubles as a flag for some instructions
    // and this, it should be avoided
    //
    // In an addition operation, VF is the carry flag, while in
    // subtraction, it is the "no borrow" flag
    //
    // In the draw instruction VF is set upon pixel collision
    std::array<uint8_t, 16u> v;

    // The address register is 12 bits wide
    // This register is generally used to store memory addresses, so only the lowest (rightmost) 12 bits are usually used.
    uint16_t i;

    // Timers Decrement at a rate of 60 Hz
    // Delay Timer
    uint8_t d_timer;

    // Sound Timer
    uint8_t s_timer;

    // program counter
    uint16_t pc;

    // stack pointer
    uint8_t sp;

    std::array<uint16_t, 16u> stack;
};

constexpr inline void init(chip8& cpu) {
    cpu.keys = 0u;
    cpu.pixels.reset();
    cpu.mem.fill(0u);
    cpu.v.fill(0u);
    cpu.i = 0u;
    cpu.d_timer = 0u;
    cpu.s_timer = 0u;
    cpu.pc = 0u;
    cpu.sp = 0u;
    cpu.stack.fill(0u);
}

constexpr inline void load_font_sprites(chip8& cpu) {
    constexpr auto sprite_list = sprites::all_font_sprites();
    auto i = 0u;
    for (const auto& sprite: sprite_list) {
        for (const auto& byte: sprite) {
            cpu.mem[FONT_START_ADDR + i] = byte;
            ++i;
        }
    }
}

constexpr inline uint16_t pop_stack(chip8& cpu) {
    // Take and then decrement
    return cpu.stack[cpu.sp--];
}

constexpr inline void push_stack(chip8& cpu, uint16_t val) {
    // Increment then add
    cpu.stack[++cpu.sp] = val;
}

constexpr inline uint8_t rand_byte() {
    // static_assert(false, "rand_byte Not implemented yet");
    return 0x00;
}

// 00E0  - clear the screen
constexpr inline void CLS(chip8& cpu) {
    cpu.pixels.reset();
}

// 00EE - return from subroutine to address pulled from stack
constexpr inline void RET(chip8& cpu) {
    cpu.pc = pop_stack(cpu);
}

// 0NNN - jump to native assembler subroutine at 0xNNN
constexpr inline void SYS(chip8& cpu, uint16_t addr) {
    cpu.pc = addr;
}

// 1NNN - jump to address NNN
constexpr inline void JP(chip8& cpu, uint16_t addr) {
    cpu.pc = addr;
}

// 2NNN - push return address onto stack and call subroutine at address NNN
constexpr inline void CALL(chip8& cpu, uint16_t addr) {
    push_stack(cpu, cpu.pc);
    cpu.pc = addr;
}

// 3XNN - skip next opcode if vX == NN
constexpr inline void SE(chip8& cpu, uint8_t /*V*/x, uint8_t nn) {
    if (cpu.v[x] == nn) {
        cpu.pc += 2;
    }
}

// 4XNN - skip next opcode if vX != NN
constexpr inline void SNE(chip8& cpu, uint8_t /*V*/x, uint8_t nn) {
    if (cpu.v[x] != nn) {
        cpu.pc += 2u;
    }
}

// 5XY0 - skip next opcode if vX == vY
constexpr inline void SE_REG(chip8& cpu, uint8_t /*V*/x, uint8_t /*V*/y) {
    if (cpu.v[x] == cpu.v[y]) {
        cpu.pc += 2u;
    }
}

// 6XNN - set vX to NN
constexpr inline void LD(chip8& cpu, uint8_t /*V*/x, uint8_t nn) {
    cpu.v[x] = nn;
}

// 7XNN - add NN to vX
// NOTE: The carry flag is not changed
constexpr inline void ADD(chip8& cpu, uint8_t /*V*/x, uint8_t nn) {
    cpu.v[x] += nn;
}

// 8XY0 - set vX to the value of vY
constexpr inline void LD_REG(chip8& cpu, uint8_t /*V*/x, uint8_t /*V*/y) {
    cpu.v[x] = cpu.v[y];
}

// 8XY1 - set vX to the result of bitwise vX OR vY
constexpr inline void OR_REG(chip8& cpu, uint8_t /*V*/x, uint8_t /*V*/y) {
    cpu.v[x] = (cpu.v[x] | cpu.v[y]);
}

// 8XY2 - set vX to the result of bitwise vX AND vY
constexpr inline void AND_REG(chip8& cpu, uint8_t /*V*/x, uint8_t /*V*/y) {
    cpu.v[x] = (cpu.v[x] & cpu.v[y]);
}

// 8XY3 - set vX to the result of bitwise vX XOR vY
constexpr inline void XOR_REG(chip8& cpu, uint8_t /*V*/x, uint8_t /*V*/y) {
    cpu.v[x] = (cpu.v[x] ^ cpu.v[y]);
}

// 8XY4 - add vY to vX, vF is set to 1 if an overflow happened, to 0 if not, even if X=F!
constexpr inline void ADD_REG(chip8& cpu, uint8_t /*V*/x, uint8_t /*V*/y) {
    uint16_t sum = cpu.v[x] + cpu.v[y];
    if (sum > 255u) {
        cpu.v[0xFu] = 1u;
    } else {
        cpu.v[0xFu] = 0u;
    }
    cpu.v[x] = static_cast<uint8_t>(sum & 0x00FFu);
}

// 8XY5 - subtract vY from vX, vF is set to 0 if an underflow happened, to 1 if not, even if X=F!
constexpr inline void SUB_REG(chip8& cpu, uint8_t /*V*/x, uint8_t /*V*/y) {
    if (cpu.v[x] > cpu.v[y]) {
        cpu.v[0xFu] = 1u;
    } else {
        cpu.v[0xFu] = 0u;
    }
    cpu.v[x] = cpu.v[x] - cpu.v[y];
}

// 8XY6 - If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
constexpr inline void SHR(chip8& cpu, uint8_t /*V*/x) {
    cpu.v[0xFu] = cpu.v[x] & 0x01u;
    cpu.v[x] >>= 1u;
}

// 8XY7 - set vX to the result of subtracting vX from vY, vF is set to 0 if an underflow happened, to 1 if not, even if X=F!
constexpr inline void SUBN_REG(chip8& cpu, uint8_t /*V*/x, uint8_t /*V*/y) {
    if (cpu.v[y] > cpu.v[x]) {
        cpu.v[0xFu] = 1u;
    } else {
        cpu.v[0xFu] = 0u;
    }
    cpu.v[x] = cpu.v[y] - cpu.v[x];
}

// 8XYE - set vX to vY and shift vX one bit to the left, set vF to the bit shifted out, even if X=F!
constexpr inline void SHL(chip8& cpu, uint8_t /*V*/x) {
    cpu.v[0xFu] = cpu.v[x] & 0x80u;
    cpu.v[x] <<= 1u;
}

// 9XY0 - skip next opcode if vX != vY
constexpr inline void SNE_REG(chip8& cpu, uint8_t /*V*/x, uint8_t /*V*/y) {
    if (cpu.v[x] != cpu.v[y]) {
        cpu.pc += 2u;
    }
}

// ANNN - set I to NNN
constexpr inline void LD_I(chip8& cpu, uint16_t addr) {
    cpu.i = addr;
}

// BNNN - jump to address NNN + v0
constexpr inline void JP_V0(chip8& cpu, uint16_t nnn) {
    cpu.pc = cpu.v[0x0u] + nnn;
}

// CXNN - set vx to a random value masked (bitwise AND) (Typically: 0 to 255) with NN
constexpr inline void RND(chip8& cpu, uint8_t /*V*/x, uint8_t nn) {
    cpu.v[x] = rand_byte() & nn;
}

// DXYN - draw 8xN pixel sprite at position vX, vY with data
// starting at the address in I, I is not changed
// Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and
// a height of N pixels. Each row of 8 pixels is read as bit-coded starting from
// memory location I; I value does not change after the execution of this instruction.
// As described above, VF is set to 1 if any screen pixels are flipped from
// set to unset when the sprite is drawn, and to 0 if that does not happen.
constexpr inline void DRW(chip8& cpu, uint8_t /*V*/x, uint8_t /*V*/y, uint8_t n) {
    auto start_x = cpu.v[x];
    auto start_y = cpu.v[y];
    cpu.v[0xFu] = 0u;

    constexpr auto bit_width = sizeof(decltype(cpu.mem[0])) * 8u;
    std::bitset<bit_width> buf;
    for (auto i = 0u; i < n; ++i) {
        // const auto buf = cpu.mem[cpu.i + i];
        buf = cpu.mem[cpu.i + i];
        const auto idx_y = (start_y + i);
        const auto draw_y = (idx_y % 32u); // Wrap y
        for (auto j = 0; j < bit_width; ++j) {
            const auto idx_x = (start_x + j);
            const auto draw_x = (idx_x % 64u); // Wrap x
            // const bool sprite_pixel_active = static_cast<bool>((buf & (1u << (bit_width - j - 1u))));
            const bool sprite_pixel_active = buf[bit_width - j - 1u];
            const auto draw_index = draw_x + (draw_y * 64u);
            // Collision!
            if (sprite_pixel_active && cpu.pixels[draw_index]) {
                cpu.v[0xFu] = 1u;
            }
            cpu.pixels[draw_index] = (cpu.pixels[draw_index] != sprite_pixel_active);
        }
    }
}

// EX9E - Skip next instruction if key with the value of Vx is pressed.
constexpr inline void SKP(chip8& cpu, uint8_t /*V*/x) {
    if (cpu.keys & (1u << cpu.v[x])) {
        cpu.pc += 2u;
    }
}

// EXA1 - Skip next instruction if key with the value of Vx is not pressed.
constexpr inline void SKNP(chip8& cpu, uint8_t /*V*/x) {
    if (!(cpu.keys & (1u << cpu.v[x]))) {
        cpu.pc += 2u;
    }
}

// FX07 - set vX to the value of the delay timer
constexpr inline void LD_REG_DT(chip8& cpu, uint8_t /*V*/x) {
    cpu.v[x] = cpu.d_timer;
}

// FX0A - Wait for a key press, store the value of the key in Vx
constexpr inline void WAIT_KP(chip8& cpu, uint8_t /*V*/x) {
    if (cpu.keys & 0xFFFF) {
        cpu.v[x] = cpu.keys;
    } else {
        // The idea here is that by moving the pc back the cpu will
        //  re-run this instruction
        cpu.pc -= 2;
    }
}

// FX15 - set delay timer to vX
constexpr inline void LD_DT_REG(chip8& cpu, uint8_t /*V*/x) {
    cpu.d_timer = cpu.v[x];
}

// FX18 - set sound timer to vX, sound is played as long as the sound timer reaches zero
constexpr inline void LD_ST_REG(chip8& cpu, uint8_t /*V*/x) {
    cpu.s_timer = cpu.v[x];
}

// FX1E - add vX to I
constexpr inline void ADD_I_REG(chip8& cpu, uint8_t /*V*/x) {
    cpu.i += cpu.v[x];
}

// FX29 - Set I = location of sprite for digit Vx
constexpr inline void LD_FONT(chip8& cpu, uint8_t /*V*/x) {
    cpu.i = FONT_START_ADDR + (sprites::FONT_SIZE * cpu.v[x]);
}

// FX33 - Store BCD representation of Vx in memory locations I, I+1, and I+2. (hundreds, tens, ones)
constexpr inline void LD_BCD(chip8& cpu, uint8_t /*V*/x) {
    auto val = cpu.v[x];

    // Ones
    cpu.mem[cpu.i + 2u] = val % 10u;
    val /= 10u;

    // Tens
    cpu.mem[cpu.i + 1u] = val % 10u;
    val /= 10u;

    // Hundreds
    cpu.mem[cpu.i] = val % 10u;
}

// FX55 - Store registers V0 through Vx in memory starting at location I.
constexpr inline void LD_I_V0X(chip8& cpu, uint8_t /*V*/x) {
    for (auto i = 0u; i <= x; ++i) {
        cpu.mem[cpu.i + i] = cpu.v[i];
    }
}

// FX65- Read registers V0 through Vx from memory starting at location I.
constexpr inline void LD_V0X_I(chip8& cpu, uint8_t /*V*/x) {
    for (auto i = 0u; i <= x; ++i) {
        cpu.v[i] = cpu.mem[cpu.i + i];
    }
}


constexpr bool parse_op(chip8& cpu, uint16_t instruct) {
    switch (instruct & 0xF000u) {
        case 0x0000u: {
            switch (instruct & 0x0FFFu) {
                case 0x00E0u: {
                    CLS(cpu);
                } break;

                case 0x00EEu: {
                    RET(cpu);
                } break;

                default: {
                    SYS(cpu, instruct & 0x0FFFu);
                } break;
            }
        } break;

        case 0x1000u: {
            JP(cpu, instruct & 0x0FFFu);
        } break;

        case 0x2000u: {
            CALL(cpu, instruct & 0x0FFFu);
        } break;

        case 0x3000u: {
            SE(cpu,
                static_cast<uint8_t>((instruct & 0x0F00u) >> 8u),
                static_cast<uint8_t>(instruct & 0x00FFu));
        } break;

        case 0x4000u: {
            SNE(cpu,
                static_cast<uint8_t>((instruct & 0x0F00u) >> 8u),
                static_cast<uint8_t>(instruct & 0x00FFu));
        } break;

        case 0x5000u: {
            SE_REG(cpu,
                static_cast<uint8_t>((instruct & 0x0F00u) >> 8u),
                static_cast<uint8_t>((instruct & 0x00F0u) >> 4u));
        } break;

        case 0x6000u: {
            LD(cpu,
                static_cast<uint8_t>((instruct & 0x0F00u) >> 8u),
                static_cast<uint8_t>(instruct & 0x00FFu));
        } break;

        case 0x7000u: {
            ADD(cpu,
                static_cast<uint8_t>((instruct & 0x0F00u) >> 8u),
                static_cast<uint8_t>(instruct & 0x00FFu));
        } break;

        case 0x8000u: {
            switch (instruct & 0x000Fu) {
                case 0x0000u: {
                    LD_REG(cpu,
                        static_cast<uint8_t>((instruct & 0x0F00u) >> 8u),
                        static_cast<uint8_t>((instruct & 0x00F0u) >> 4u));
                } break;

                case 0x0001u: {
                    OR_REG(cpu,
                        static_cast<uint8_t>((instruct & 0x0F00u) >> 8u),
                        static_cast<uint8_t>((instruct & 0x00F0u) >> 4u));
                } break;

                case 0x0002u: {
                    AND_REG(cpu,
                        static_cast<uint8_t>((instruct & 0x0F00u) >> 8u),
                        static_cast<uint8_t>((instruct & 0x00F0u) >> 4u));
                } break;

                case 0x0003u: {
                    XOR_REG(cpu,
                        static_cast<uint8_t>((instruct & 0x0F00) >> 8u),
                        static_cast<uint8_t>((instruct & 0x00F0) >> 4u));
                } break;

                case 0x0004u: {
                    ADD_REG(cpu,
                        static_cast<uint8_t>((instruct & 0x0F00u) >> 8u),
                        static_cast<uint8_t>((instruct & 0x00F0u) >> 4u));
                } break;

                case 0x0005u: {
                    SUB_REG(cpu,
                        static_cast<uint8_t>((instruct & 0x0F00u) >> 8u),
                        static_cast<uint8_t>((instruct & 0x00F0u) >> 4u));
                } break;

                case 0x0006u: {
                    SHR(cpu, static_cast<uint8_t>((instruct & 0x0F00u) >> 8u));
                } break;

                case 0x0007u: {
                    SUBN_REG(cpu,
                        static_cast<uint8_t>((instruct & 0x0F00u) >> 8u),
                        static_cast<uint8_t>((instruct & 0x00F0u) >> 4u));
                } break;

                case 0x000Eu: {
                    SHL(cpu, static_cast<uint8_t>((instruct & 0x0F00u) >> 8u));
                } break;
            }
        } break;

        case 0x9000u: {
            SNE_REG(cpu,
                static_cast<uint8_t>((instruct & 0x0F00u) >> 8u),
                static_cast<uint8_t>((instruct & 0x00F0u) >> 4u));
        } break;

        case 0xA000u: {
            LD_I(cpu, instruct & 0x0FFFu);
        } break;

        case 0xB000u: {
            JP_V0(cpu, instruct & 0x0FFFu);
        } break;

        case 0xC000u: {
            RND(cpu,
                static_cast<uint8_t>((instruct & 0x0F00u) >> 8u),
                static_cast<uint8_t>((instruct & 0x00FFu)));
        } break;

        case 0xD000u: {
            DRW(cpu,
                static_cast<uint8_t>((instruct & 0x0F00u) >> 8u),
                static_cast<uint8_t>((instruct & 0x00F0u) >> 4u),
                static_cast<uint8_t>((instruct & 0x000Fu)));
        } break;

        case 0xE000u: {
            switch (instruct & 0x00FFu) {
                case 0x009Eu: {
                    SKP(cpu, static_cast<uint8_t>((instruct & 0x0F00u) >> 8u));
                } break;

                case 0x00A1u: {
                    SKNP(cpu, static_cast<uint8_t>((instruct & 0x0F00u) >> 8u));
                } break;
            }
        } break;

        case 0xF000u: {
            switch (instruct & 0x00FFu) {
                case 0x0007u: {
                    LD_REG_DT(cpu, static_cast<uint8_t>((instruct & 0x0F00u) >> 8u));
                } break;

                case 0x000Au: {
                    WAIT_KP(cpu, static_cast<uint8_t>((instruct & 0x0F00u) >> 8u));
                } break;

                case 0x0015u: {
                    LD_DT_REG(cpu, static_cast<uint8_t>((instruct & 0x0F00u) >> 8u));
                } break;

                case 0x0018u: {
                    LD_ST_REG(cpu, static_cast<uint8_t>((instruct & 0x0F00u) >> 8u));
                } break;

                case 0x001Eu: {
                    ADD_I_REG(cpu, static_cast<uint8_t>((instruct & 0x0F00u) >> 8u));
                } break;

                case 0x0029u: {
                    LD_FONT(cpu, static_cast<uint8_t>((instruct & 0x0F00u) >> 8u));
                } break;

                case 0x0033u: {
                    LD_BCD(cpu, static_cast<uint8_t>((instruct & 0x0F00u) >> 8u));
                } break;

                case 0x0055u: {
                    LD_I_V0X(cpu, static_cast<uint8_t>((instruct & 0x0F00u) >> 8u));
                } break;

                case 0x0065u: {
                    LD_V0X_I(cpu, static_cast<uint8_t>((instruct & 0x0F00u) >> 8u));
                } break;
            }
        } break;

        default: {

        } break;
    }

    return false;
}

} // namespace chipp8
