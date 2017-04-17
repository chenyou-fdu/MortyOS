#include "libs/common.h"

inline void outb(uint16_t port, uint8_t value) {
    asm volatile("outb %1, %0" : : "dN" (port), "a" (value));
}

inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

inline uint16_t inw(uint16_t port) {
    uint16_t ret;
    asm volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}


inline void enable_interrupt() {
    asm volatile ("sti");
}

inline void disable_interrupt() {
    asm volatile ("cli");
}
inline uint32_t read_eflags() {
    uint32_t eflags;
    __asm__ volatile ("pushfl; popl %0" : "=r" (eflags));
    return eflags;
}

int32_t save_interrupt() {
    int32_t interrupt_state;
    if(read_eflags() | FL_IF) {
        disable_interrupt();
        interrupt_state = TRUE;
    } else interrupt_state = FALSE;
    return interrupt_state;
}

void recover_interrupt(int32_t interrupt_state) {
    if(interrupt_state) 
        enable_interrupt();
}