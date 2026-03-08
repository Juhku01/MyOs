#include <stdint.h>
#include "ports.h"

char keyboard_map[128] =
{
    0,27,'1','2','3','4','5','6','7','8','9','0','-','=',8,
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\','z',
    'x','c','v','b','n','m',',','.','/',0,'*',0,' ',0
};

char keyboard_shift_map[128] =
{
    0,27,'!','@','#','$','%','^','&','*','(',')','_','+',8,
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',0,
    'A','S','D','F','G','H','J','K','L',':','"','~',0,'|','Z',
    'X','C','V','B','N','M','<','>','?',0,'*',0,' ',0
};

#define KEY_NONE  0
#define KEY_UP    1
#define KEY_DOWN  2

#define KB_BUFFER_SIZE 256

static char kb_buffer[KB_BUFFER_SIZE];
static int kb_head = 0;
static int kb_tail = 0;

static int shift_pressed = 0;
static int caps_lock = 0;

static void kb_push(char c)
{
    int next = (kb_head + 1) % KB_BUFFER_SIZE;
    if (next != kb_tail)
    {
        kb_buffer[kb_head] = c;
        kb_head = next;
    }
}

char get_key(void)
{
    if (kb_head == kb_tail)
        return 0;

    char c = kb_buffer[kb_tail];
    kb_tail = (kb_tail + 1) % KB_BUFFER_SIZE;
    return c;
}

void keyboard_handler(void)
{
    static int extended = 0;

    uint8_t scancode = inb(0x60);

    if (scancode == 0xE0)
    {
        extended = 1;
        outb(0x20, 0x20);
        return;
    }

    /* shift press */
    if (scancode == 0x2A || scancode == 0x36)
    {
        shift_pressed = 1;
        outb(0x20, 0x20);
        return;
    }

    /* shift release */
    if (scancode == 0xAA || scancode == 0xB6)
    {
        shift_pressed = 0;
        outb(0x20, 0x20);
        return;
    }

    /* caps lock */
    if (scancode == 0x3A)
    {
        caps_lock = !caps_lock;
        outb(0x20, 0x20);
        return;
    }

    /* release code */
    if (scancode & 0x80)
    {
        extended = 0;
        outb(0x20, 0x20);
        return;
    }

    if (extended)
    {
        extended = 0;

        if (scancode == 0x48)
            kb_push(KEY_UP);
        else if (scancode == 0x50)
            kb_push(KEY_DOWN);

        outb(0x20, 0x20);
        return;
    }

    if (scancode > 58)
    {
        outb(0x20, 0x20);
        return;
    }

    char c = shift_pressed ? keyboard_shift_map[scancode]
                           : keyboard_map[scancode];

    if (caps_lock && c >= 'a' && c <= 'z')
        c = c - 32;

    if (caps_lock && shift_pressed && c >= 'A' && c <= 'Z')
        c = c + 32;

    if (c)
        kb_push(c);

    /* EOI master PIC */
    outb(0x20, 0x20);
}
