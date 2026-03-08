void clear_screen(void);
void print(const char*);
void shell(void);
void idt_init(void);

void kernel_main(void)
{
    clear_screen();

    print("MyOS kernel started\n");
    print("Memory OK\n");

    idt_init();
    __asm__ volatile ("sti");

    print("Interrupts ready\n");
    print("Keyboard IRQ ready\n\n");

    shell();
}
