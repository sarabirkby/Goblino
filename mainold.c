#include "main.h"


/* checks to see if a map pointer exists, returns true of success */


map_t * create_map(uint8_t width, uint8_t height)
{
    // Allocate space for the struct
    map_t * map = (map_t *) calloc(1, sizeof(map_t));

    map->width = width;
    map->height = height;

    // Create the rows
    map->tiles = (tile_t **) calloc(map->height, sizeof(tile_t *));

    // Creates an array of tiles for every row
    for (int w = 0; w < map->width; w++) {
        map->tiles[w] = (tile_t *) calloc(map->width, sizeof(tile_t));
    }

    return map;
}


void destroy_map(map_t * source)
{

    // Free every column in a row
    for(int h = 0; h < source->height; h++) {
        free(source->tiles[h]);
    }

    // Free the rows
    free(source->tiles);

    // Free the struct
    free(source);
}


void copy_map(map_t *dest, const map_t *source)
{

    // Create a new empty map based on old map's dimensions
    dest = create_map(source->width, source->height);

    // Copy the underlying data into the new map (row by row copy)
    for (int h = 0; h < source->height; h++) {
        memcpy(dest->tiles[h], source->tiles[h], source->width * sizeof(tile_t));
    }

    dest->x_buffer = source->x_buffer;
    dest->y_buffer = source->y_buffer;

}


map_t *create_map_copy(map_t *source)
{
    check_map_ptr(source);

    // Create a new empty map based on old map's dimensions
    map_t *dest = create_map(source->width, source->height);

    for (int h = 0; h < source->height; h++) {
        memcpy(dest->tiles[h], source->tiles[h], source->width * sizeof(tile_t));
    }

    dest->x_buffer = source->x_buffer;
    dest->y_buffer = source->y_buffer;

    return dest;
}


void clear_tile(map_t * layer, coord_t del_y, coord_t del_x)
{
    layer->tiles[del_y][del_x].type = empty_tile;
    layer->tiles[del_y][del_x].can_enter = true;
}



void generate_grass(map_t *dest)
{
    check_map_ptr(dest);

    tile_t g_tile;
    g_tile.can_enter = true;


    for (int h = 0; h < dest->height; h++) {
        for (int w = 0; w < dest->width; w++) {
            g_tile.x = w;
            g_tile.y = h;

            switch(rand() % 2) {
            case 0:
                g_tile.type = grass1_tile;
                break;
            case 1:
                g_tile.type = grass2_tile;
                break;
            case 2:
            default:
                fprintf(stderr, "rand mod out of range");
            }


            dest->tiles[h][w] = g_tile;
        }
    }
}

void generate_mud(map_t *dest)
{
    tile_t m_tile;
    m_tile.can_enter = true;
    m_tile.type = mud_tile;

    for (int h = 0; h < dest->height; h++) {
        for (int w = 0; w < dest->width; w++) {
            m_tile.x = w;
            m_tile.y = h;

            //each grassy tile has a 1/5th chance of being mud
            if (rand() % 7 == 0) {
                dest->tiles[h][w] = m_tile;
            }
        }
    }
}

void generate_borders(map_t *dest)
{
    check_map_ptr(dest);

    // Creates a template to copy for border tiles
    tile_t b_tile;
    b_tile.type = border_tile;
    b_tile.can_enter = false;
    for (int h = 0; h < dest->height; h++) {
        for (int w = 0; w < dest->width; w++) {
            b_tile.x = w;
            b_tile.y = h;

            if (h == 0 || w == 0 || h == (dest->height)-1 || w == (dest->width)-1) {
                dest->tiles[h][w] = b_tile;
            }
        }
    }

}

void generate_grasslands(map_t *dest)
{
    check_map_ptr(dest);

    generate_grass(dest);
    generate_mud(dest);
    generate_borders(dest);

    dest->player_x = NULL;
    dest->player_y = NULL;

    dest->look_mode = false;
}


void generate_player_tile(map_t * layer, coord_t gen_y, coord_t gen_x)
{
    check_map_ptr(layer);

    tile_t p_tile;
    p_tile.type = player_type;
    p_tile.y = player.y;
    p_tile.x = player.x;

    layer->tiles[player.y][player.x] = p_tile;
}


void make_looking_cursor(map_t * layer, cursor_t cursor)
{
    check_map_ptr(dest);

    tile_t c_tile;
    c_tile.type = looking_cursor_tile;
    c_tile.x = cursor.x;
    c_tile.y = cursor.y;

    dest->tiles[cursor.y][cursor.x] = c_tile;
}


{
    bool can_move = false;
    switch(dir) {
    case down_dir:
        if (dest->tiles[dest->])
            break;
    }
    if (can_move)
        move_tile(dest, terrain, dir, make_func);
}*

void move_player(map_t * layer, player_t player, move_dir_t dir)
{
    coord_t x = player.x, y = player.y;
    clear_tile(layer, y, x);
    switch(dir) {
    case down_dir:
        if (y < layer->width-1 && layer->tiles[y+1][x].can_enter) {
            generate_player_tile(layer, y+1, x);
            player.y++;
        }
        break;
    }
    case up_dir:
        if (y > 0 && layer->tiles[y-1][x].canlayer->tiles[])
}


void move_looking_cursor(map_t * layer);


char get_char(const tile_type_t type)
{
    switch(type) {
    case player_tile:
        attron(COLOR_PAIR(player_pair));
        return '@';
    case looking_cursor_tile:
        attron(COLOR_PAIR(looking_cursor_pair));
        return 'X';
    case border_tile:
        attron(COLOR_PAIR(border_pair));
        return '#';
    case grass1_tile:
        attron(COLOR_PAIR(grass1_pair));
        return '.';
    case grass2_tile:
        attron(COLOR_PAIR(grass2_pair));
        return ',';
    case mud_tile:
        attron(COLOR_PAIR(mud_pair));
        return '~';
    default:
        fprintf(stderr, "tile type does not exist: %d\n", type);
        return '\0';
    }
}


void init_tile_colors( void )
{

    start_color();
    init_color(COLOR_BROWN, 165, 42, 42);
    init_color(COLOR_TAN, 218, 165, 32);
    init_color(COLOR_LTGREEN, 124, 252,0);
    init_color(COLOR_DKRED, 178, 34, 34);

    init_pair(empty_pair, COLOR_WHITE, COLOR_BLACK);
    init_pair(player_pair, COLOR_WHITE, COLOR_RED);
    init_pair(looking_cursor_pair, COLOR_WHITE, COLOR_YELLOW);
    init_pair(border_pair, COLOR_RED, COLOR_DKRED);
    init_pair(grass1_pair, COLOR_GREEN, COLOR_LTGREEN);
    init_pair(grass2_pair, COLOR_LTGREEN, COLOR_GREEN);
    init_pair(mud_pair, COLOR_BROWN, COLOR_TAN);
}


void print_chars_raw(map_t * source)
{
    for (int h = 0; h < source->height; h++) {
        for (int w = 0; w < source->width; w++) {
            // Moves to a new cell and crashes if it fails
            // Cycles through all cells
            tile_t current_tile = source->tiles[h][w];
            char character = get_char(current_tile.type);
            mvaddch(h, w, character);
        }
    }
}


void print_chars_buffered(WINDOW *win, map_t * source)
{
    werase(win);
    source->y_buffer = (win->_maxy - source->height) / 2;
    source->x_buffer = (win->_maxx - source->width) / 2;

    for (int h = 0; h < source->height; h++) {
        for (int w = 0; w < source->width; w++) {
            // Moves to a new cell and crashes if it fails
            // Cycles through all cells
            tile_t current_tile = source->tiles[h][w];
            char character = get_char(current_tile.type);
            mvaddch( h + source->y_buffer, w + source->x_buffer, character );
        }
    }
}


void reprint_moved_cells(WINDOW * win, map_t * display_map,
                        tile_t * dest_tile, tile_t * source_tile) {
    mvaddch(source_tile->y + display_map->y_buffer,
            source_tile->x + display_map->x_buffer,
            get_char(source_tile->type));
    mvaddch(dest_tile->y + display_map->y_buffer,
            dest_tile->x + display_map->x_buffer,
            get_char(dest_tile->type));
}

void reprint_cell(WINDOW * win, map_t * display_map, tile_t * tile)
{
    mvaddch(tile->y + display_map->y_buffer,
            tile->x + display_map->x_buffer,
            get_char(tile->type));
}


void cursor_move_buffered(WINDOW * win, map_t * source)
{
    wmove(win, source->cursor_y+source->y_buffer,
          source->cursor_x+source->x_buffer);
}




int main( void )
{
    srand(time(NULL));
    initscr();
    refresh();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, true);
    init_tile_colors();

    /*
     * Layer 1: Terrain
     *
     * This is the 'background' map. It represents the terrain,
     * any walls or other constructions, etc.
     * These tiles can be replaced with things that move,
     * but once they leave, it returns to its original state.
     *
     */

    map_t * terrain_layer = create_map(MAPWIDTH, MAPHEIGHT);
    map_t * player_layer = create_map(terrain_map->width, terrain_map->height);
    map_t * cursor_layer = create_map(terrain_map->width, terrain_map->height);

    //map_t * display_map = create_map_copy(terrain_map);
    //make_player(display_map, 1, 1);

    player_t player generate_player_struct(1, 1);

    generate_grasslands(terrain_layer);
    generate_player_tile(player_layer, player);
    generate_cursor_layer(cursor_layer, 0, 0);

    print_chars_buffered(stdscr, terrain_layer);
    print_chars_buffered(stdscr, player_layer);
    while (true) {
        wrefresh(stdscr);
        // If theres been a character input, some do things
        switch (getch()) {
        case 'q':
            endwin();
            return 0;

        case KEY_DOWN:
        case 's':
            if (display_map->look_mode)
                move_looking_cursor(cursor_map, display_map, down_dir);
            else {
                move_player(display_map, terrain_map, down_dir);
            break;

        case KEY_UP:
        case 'w':
            if (display_map->look_mode)
                move_looking_cursor(cursor_map, display_map, up_dir);
            else
                move_player(cursor_map, terrain_map, up_dir);
            break;

        case KEY_LEFT:
        case 'a':
            if (display_map->look_mode)
                move_looking_cursor(cursor_map, display_map, left_dir);
            else
                move_player(display_map, terrain_map, left_dir);
            break;

        case KEY_RIGHT:
        case 'd':
            if (display_map->look_mode)
                move_looking_cursor(cursor_map, display_map, right_dir);
            else
                move_player(display_map, terrain_map, right_dir);
            break;

        case 'k':
            if (!display_map->look_mode) {
                display_map->look_mode = true;
                cursor_map = create_map_copy(display_map);
                make_looking_cursor(cursor_map, display_map->player_y, display_map->player_x);
                reprint_cell(stdscr, cursor_map, &cursor_map->tiles[cursor_map->cursor_y][cursor_map->cursor_x]);
                curs_set(2);

            }
            else {
                display_map->look_mode = false;
                destroy_map(cursor_map);
                print_chars_buffered(stdscr, display_map);
                curs_set(FALSE);
            }

            break;
        }


    }

    endwin();

    return 0;
}
