#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

unsigned short* video = (unsigned short*)0xB8000;

int cursor_x = 0;
int cursor_y = 0;

void scroll();
void putchar(char c);

void clear_screen()
{
    for(int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        video[i] = 0x0F20;

    cursor_x = 0;
    cursor_y = 0;
}

void scroll()
{
    if(cursor_y < VGA_HEIGHT)
        return;

    for(int y = 1; y < VGA_HEIGHT; y++)
    {
        for(int x = 0; x < VGA_WIDTH; x++)
        {
            video[(y-1)*VGA_WIDTH + x] =
            video[y*VGA_WIDTH + x];
        }
    }

    for(int x = 0; x < VGA_WIDTH; x++)
        video[(VGA_HEIGHT-1)*VGA_WIDTH + x] = 0x0F20;

    cursor_y = VGA_HEIGHT - 1;
}

void putchar(char c)
{
    if(c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }
    else
    {
        video[cursor_y * VGA_WIDTH + cursor_x] =
            (0x0F << 8) | c;

        cursor_x++;
    }

    if(cursor_x >= VGA_WIDTH)
    {
        cursor_x = 0;
        cursor_y++;
    }

    scroll();
}

void print(const char* str)
{
    int i = 0;

    while(str[i])
    {
        putchar(str[i]);
        i++;
    }
}
void backspace()
{
    if (cursor_x == 0 && cursor_y == 0)
        return;

    if (cursor_x == 0)
    {
        cursor_y--;
        cursor_x = VGA_WIDTH - 1;
    }
    else
    {
        cursor_x--;
    }

    video[cursor_y * VGA_WIDTH + cursor_x] = 0x0F20;
}
