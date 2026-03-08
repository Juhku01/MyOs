#include "idt.h"
#include "ports.h"

extern void idt_load(uint32_t);
extern void irq1_stub(void);

static struct idt_entry idt[256];
static struct idt_ptr idtp;

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

static void pic_remap(void)
{
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    outb(0x21, 0x20); /* master PIC offset 32 */
    outb(0xA1, 0x28); /* slave PIC offset 40 */

    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    /* salli vain IRQ1 (keyboard) master PIC:llä */
    outb(0x21, 0xFD);
    outb(0xA1, 0xFF);
}

void idt_init(void)
{
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base = (uint32_t)&idt;

    for (int i = 0; i < 256; i++)
        idt_set_gate(i, 0, 0, 0);

    pic_remap();

    /* IRQ1 = interrupt vector 33 */
    idt_set_gate(33, (uint32_t)irq1_stub, 0x08, 0x8E);

    idt_load((uint32_t)&idtp);
}
