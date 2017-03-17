#include "gdt.h"

gdt_entry_t gdt_entries[GDT_LEN];
gdt_ptr_t gdt_ptr;

extern uint32_t stack;
extern void gdt_flush(uint32_t);

static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

void init_gdt() {
    gdt_ptr.limit = sizeof(gdt_entry_t) * GDT_LEN - 1;
    gdt_ptr.base = (uint32_t) &gdt_entries; // why & of array name

    // null entry
    gdt_set_gate(SEL_NULL_IDX, 0, 0, 0, 0); 
    // ref to OSDev Wiki to understand the meaning of 'access' 
    // code seg for kernel
    gdt_set_gate(SEL_KCODE_IDX, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    // data seg for kernel
    gdt_set_gate(SEL_KDATA_IDX, 0, 0xFFFFFFFF, 0x92, 0xCF);
    // code seg for uesr
    gdt_set_gate(SEL_UCODE_IDX, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    // data seg for user
    gdt_set_gate(SEL_UDATA_IDX, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    gdt_flush((uint32_t)&gdt_ptr);
}

static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_mid = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access = access;
}