#include <curses.h>
#include <stdint.h>

char get_hex( uint8_t num )
{
    switch( num ) {
    case 0x0:
        return '0';
    case 0x1:
        return '1';
    }
}


int main( void ) {
    initscr();
    refresh();
    noecho();
    curs_set(0);

    addstr("  0 1 2 3 4 5 6 7 8 9 A B C D E F\n");

    for (uint8_t i = 0; i < 0x100; i++) {
        mvaddch(i+1, 0, get_hex(i>>4));
        mvaddch(i+1, 0, get_hex(i & 0x0F));
    }
}
