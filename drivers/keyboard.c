#include "keyboard.h"
#include "kernel/io.h"
#include "scancodes.h"
#include "interrupts.h"

uint8_t get_keycode()
{
    uint8_t scancode = 0;
    if (interrupts_enabled())
        scancode = get_key_intr();
    else
        scancode = get_key_nointr();
    
    if (scancode < KEY_DOWN_END)
    {
        // Key Pressed
        pressed_keys |= scancode;
        return scancode;
    }
    else if (scancode < KEY_UP_END)
    {
        // Key Released
        pressed_keys &= ~(scancode - KEY_DOWN_END);
        // Don't return a keycode if a key is released
        return 0;
        //return scancode - KEY_DOWN_END;
    }
    return 0;
}

char get_char(uint8_t keycode)
{
    if (keycode >= KEY_1 && keycode <= KEY_9)
        return keycode + 47;
    switch (keycode)
    {
        case KEY_ENTER:
            return '\n';
        case KEY_0:
            return '0';
        case KEY_MINUS:
            return '-';
        case KEY_EQUALS:
            return '=';
        case KEY_TAB:
            return '\t';
        case KEY_LBRACKET:
            return '[';
        case KEY_RBRACKET:
            return ']';
        case KEY_SEMICOLON:
            return ';';
        case KEY_QUOTE:
            return '\'';
        case KEY_BACKTICK:
            return '`';
        case KEY_BACKSLASH:
            return '\\';
        case KEY_COMMA:
            return ',';
        case KEY_PERIOD:
            return '.';
        case KEY_SLASH:
            return '/';
        case KEY_NUM_STAR:
            return '*';
        case KEY_SPACE:
            return ' ';
        case KEY_NUM_0:
            return '0';
        case KEY_NUM_1:
            return '1';
        case KEY_NUM_2:
            return '2';
        case KEY_NUM_3:
            return '3';
        case KEY_NUM_4:
            return '4';
        case KEY_NUM_5:
            return '5';
        case KEY_NUM_6:
            return '6';
        case KEY_NUM_7:
            return '7';
        case KEY_NUM_8:
            return '8';
        case KEY_NUM_9:
            return '9';
        case KEY_NUM_MINUS:
            return '-';
        case KEY_NUM_PLUS:
            return '+';
        case KEY_NUM_PERIOD:
            return '.';
		case KEY_A:
			return 'A';
		case KEY_B:
			return 'B';
		case KEY_C:
			return 'C';
		case KEY_D:
			return 'D';
		case KEY_E:
			return 'E';
		case KEY_F:
			return 'F';
		case KEY_G:
			return 'G';
		case KEY_H:
			return 'H';
		case KEY_I:
			return 'I';
		case KEY_J:
			return 'J';
		case KEY_K:
			return 'K';
		case KEY_L:
			return 'L';
		case KEY_M:
			return 'M';
		case KEY_N:
			return 'N';
		case KEY_O:
			return 'O';
		case KEY_P:
			return 'P';
		case KEY_Q:
			return 'Q';
		case KEY_R:
			return 'R';
		case KEY_S:
			return 'S';
		case KEY_T:
			return 'T';
		case KEY_U:
			return 'U';
		case KEY_V:
			return 'V';
		case KEY_W:
			return 'W';
		case KEY_X:
			return 'X';
		case KEY_Y:
			return 'Y';
		case KEY_Z:
			return 'Z';
    }
    return '\0';
}

uint8_t get_key_nointr()
{
    uint8_t flags;
    while (true)
    {
        flags = inb(0x64);
        if (flags & 1) // A new byte is available to read
        {
            if (flags & 0x20)
            {
                // A mouse event.
                // Eat it out of port 0x60 and moveon.
                inb(0x60);
                continue;
            }
            // Must not be a mouse value if we're here.
            return inb(0x60);
        }
    }
}

uint8_t get_key_intr()
{
    // TODO
    return 0;
}

