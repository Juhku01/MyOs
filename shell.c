void print(const char*);
void clear_screen();
char get_key();
void backspace();

#define BUFFER_SIZE 128
#define HISTORY_SIZE 10

#define KEY_UP   1
#define KEY_DOWN 2

#define PROMPT "myos> "

char buffer[BUFFER_SIZE];
int buffer_index = 0;

/* history */
char history[HISTORY_SIZE][BUFFER_SIZE];
int history_count = 0;

/* -1 = ei selata historiaa */
int history_index = -1;

int strcmp(const char* a, const char* b)
{
    int i = 0;

    while (a[i] && b[i])
    {
        if (a[i] != b[i])
            return 0;
        i++;
    }

    return a[i] == b[i];
}

int startswith(const char* str, const char* prefix)
{
    int i = 0;

    while (prefix[i])
    {
        if (str[i] != prefix[i])
            return 0;
        i++;
    }

    return 1;
}

void strcpy(char* dest, const char* src)
{
    int i = 0;

    while (src[i])
    {
        dest[i] = src[i];
        i++;
    }

    dest[i] = 0;
}

void show_prompt()
{
    print(PROMPT);
}

void clear_input_line()
{
    while (buffer_index > 0)
    {
        buffer_index--;
        buffer[buffer_index] = 0;
        backspace();
    }
}

void load_history_entry(int index)
{
    clear_input_line();

    if (index < 0 || index >= history_count)
        return;

    strcpy(buffer, history[index]);

    int i = 0;
    while (buffer[i])
    {
        char s[2];
        s[0] = buffer[i];
        s[1] = 0;
        print(s);
        i++;
    }

    buffer_index = i;
}

void save_history()
{
    if (buffer_index == 0)
        return;

    if (history_count > 0 && strcmp(history[history_count - 1], buffer))
        return;

    if (history_count < HISTORY_SIZE)
    {
        strcpy(history[history_count], buffer);
        history_count++;
    }
    else
    {
        for (int i = 1; i < HISTORY_SIZE; i++)
            strcpy(history[i - 1], history[i]);

        strcpy(history[HISTORY_SIZE - 1], buffer);
    }
}

void run_command()
{
    buffer[buffer_index] = 0;

    if (buffer_index == 0)
    {
        print("\n");
        history_index = -1;
        show_prompt();
        return;
    }

    save_history();

    if (strcmp(buffer, "help"))
    {
        print("\nCommands:\n");
        print("help\n");
        print("clear / cls\n");
        print("version\n");
        print("about\n");
        print("echo <text>\n");
    }
    else if (strcmp(buffer, "clear") || strcmp(buffer, "cls"))
    {
        clear_screen();
    }
    else if (strcmp(buffer, "version"))
    {
        print("\nMyOS v0.1\n");
    }
    else if (strcmp(buffer, "about"))
    {
        print("\nMyOS hobby operating system\n");
        print("Written in C and Assembly\n");
    }
    else if (startswith(buffer, "echo "))
    {
        print("\n");
        print(buffer + 5);
        print("\n");
    }
    else
    {
        print("\nUnknown command: ");
        print(buffer);
        print("\n");
    }

    buffer_index = 0;
    buffer[0] = 0;
    history_index = -1;
    show_prompt();
}

void history_up()
{
    if (history_count == 0)
        return;

    if (history_index == -1)
        history_index = history_count - 1;
    else if (history_index > 0)
        history_index--;

    load_history_entry(history_index);
}

void history_down()
{
    if (history_count == 0)
        return;

    if (history_index == -1)
        return;

    if (history_index < history_count - 1)
    {
        history_index++;
        load_history_entry(history_index);
    }
    else
    {
        history_index = -1;
        clear_input_line();
        buffer[0] = 0;
    }
}

void shell()
{
    show_prompt();

    while (1)
    {
        char c = get_key();

        if (!c)
            continue;

        if (c == KEY_UP)
        {
            history_up();
        }
        else if (c == KEY_DOWN)
        {
            history_down();
        }
        else if (c == '\n')
        {
            run_command();
        }
        else if (c == 8 || c == 127)
        {
            if (buffer_index > 0)
            {
                buffer_index--;
                buffer[buffer_index] = 0;
                backspace();
            }
        }
        else
        {
            if (buffer_index < BUFFER_SIZE - 1)
            {
                char s[2];
                s[0] = c;
                s[1] = 0;

                print(s);
                buffer[buffer_index++] = c;
                buffer[buffer_index] = 0;
            }
        }
    }
}
