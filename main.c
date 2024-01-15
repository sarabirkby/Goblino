#include "main.h"

game_info_t Game_data;

/* checks to see if a map pointer exists, returns true of success */



map_t * create_map(WINDOW * win, uint8_t height, uint8_t width)
{
    map_t * new_map = (map_t *) calloc(1, sizeof(map_t));

    new_map->height = height;
    new_map->width = width;

    new_map->y_buffer = (win->_maxy - height) / 2;
    new_map->x_buffer = (win->_maxx - width) / 2;

    new_map->layers = (layer_t **) calloc(NUM_LAYER_TYPES, sizeof(layer_t *));
    get_layers(new_map);

    return new_map;

}

void get_layers(map_t * parent_ptr)
{
    //layer addresses start at terrain
    parent_ptr->layers[terrain_layer] = create_terrain_layer(parent_ptr);
    // player layer
    parent_ptr->layers[player_layer] = create_player_layer(parent_ptr);
}

layer_t * create_layer(layer_id_t id, map_t * parent_ptr)
{
    layer_t * layer;

    layer->id = id;

    // Create the rows
    layer->tiles = (tile_t **) calloc(parent_ptr->height, sizeof(tile_t *));

    // Creates an array of tiles for every row
    for (int h = 0; h < parent_ptr->height; h++) {
        layer->tiles[h] = (tile_t *) calloc(parent_ptr->width, sizeof(tile_t));
    }

    tile_t e_tile;
    e_tile.can_enter = true;
    e_tile.type = empty_tile;

    for (int h = 0; h < parent_ptr->height; h++) {
        for (int w = 0; w < parent_ptr->width; w++) {
            e_tile.y_pos = h, e_tile.x_pos = w;
            layer->tiles[h][w] = e_tile;
        }
    }

    return layer;
}


layer_t * create_terrain_layer(map_t * parent_ptr)
{
    layer_t * terr_layer = create_layer(terrain_layer, parent_ptr);

    create_grasslands(terr_layer, parent_ptr);

    return terr_layer;

}

layer_t * create_player_layer(map_t * parent_ptr)
{
    layer_t * p_layer = create_layer(player_layer, parent_ptr);

    Game_data.player_x = 3, Game_data.player_y = 3;
    create_player(p_layer, parent_ptr);

    return p_layer;
}

void create_player(layer_t * layer, map_t * parent_ptr)
{
    tile_t p_tile;
    p_tile.can_enter = false;
    p_tile.type = player_tile;
    p_tile.y_pos = Game_data.player_y;
    p_tile.x_pos = Game_data.player_x;


    layer->tiles[Game_data.player_y][Game_data.player_x] = p_tile;
}

void create_grasslands(layer_t * layer, map_t * parent_ptr)
{
    create_grass(layer, parent_ptr);
    create_mud(layer, parent_ptr);
    create_borders(layer, parent_ptr);
}


void create_grass(layer_t * layer, map_t * parent_ptr)
{
    tile_t g_tile;
    g_tile.can_enter = true;

    for (int h = 0; h < parent_ptr->height; h++) {
        for (int w = 0; w < parent_ptr->width; w++) {
            g_tile.x_pos = w;
            g_tile.y_pos = h;

            switch(rand() % 2) {
            case 0:
                g_tile.type = grass1_tile;
                break;
            case 1:
                g_tile.type = grass2_tile;
                break;
            default:
                fprintf(stderr, "rand mod out of range");
            }

            layer->tiles[h][w] = g_tile;
        }
    }
}


void create_mud(layer_t * layer, map_t * parent_ptr)
{
    tile_t m_tile;
    m_tile.can_enter = true;
    m_tile.type = mud_tile;

    for (int h = 0; h < parent_ptr->height; h++) {
        for (int w = 0; w < parent_ptr->width; w++) {
            if (rand() % 7 == 0)
            {
                m_tile.x_pos = w;
                m_tile.y_pos = h;
                layer->tiles[h][w] = m_tile;
            }


        }
    }

}


void create_borders(layer_t * layer, map_t * parent_ptr)
{
    tile_t b_tile;
    b_tile.can_enter = false;
    b_tile.type = border_tile;

    for (int h = 0; h < parent_ptr->height; h++) {
        for (int w = 0; w < parent_ptr->width; w++) {
            if (h == 0 || w == 0 || h == parent_ptr->height-1 || w == parent_ptr->width-1)
            {
                b_tile.x_pos = w;
                b_tile.y_pos = h;
                layer->tiles[h][w] = b_tile;
            }
        }
    }
}
void replace_and_reprint_tile( WINDOW * win, layer_t * move_layer,
                               map_t * parent_ptr, coord_t new_y, coord_t new_x,
                               coord_t old_y, coord_t old_x )
{
    move_tile( move_layer, new_y, new_x, old_y, old_x );
    print_tile( win, parent_ptr, new_y, new_x);
    print_tile( win, parent_ptr, old_y, old_x);
}


void move_tile( layer_t * move_layer, coord_t new_y, coord_t new_x,
                coord_t old_y, coord_t old_x )
{
    tile_t e_tile;
    e_tile.can_enter = true;
    e_tile.type = empty_tile;
    e_tile.y_pos = old_y, e_tile.x_pos = old_x;
    move_layer->tiles[new_y][new_x] = move_layer->tiles[old_y][old_x];
    move_layer->tiles[old_y][old_x] = e_tile;
}


void print_tile(WINDOW *win, map_t * parent_ptr, coord_t y, coord_t x)
{
    for (int i = 0; i < NUM_LAYER_TYPES; i++) {
        char character = get_char(parent_ptr->layers[i]->tiles[y][x].type);
        if (character != '\0') {
            mvwaddch(win, y + parent_ptr->y_buffer, x + parent_ptr->x_buffer, character);
        }

    }
}


void print_layer_buffered(WINDOW *win, layer_t * layer, map_t * parent_ptr)
{

    for (int h = 0; h < parent_ptr->height; h++) {
        for (int w = 0; w < parent_ptr->width; w++) {
            tile_t current_tile = layer->tiles[h][w];
            char character = get_char(current_tile.type);
            if (character != '\0')
                mvwaddch( win, h + parent_ptr->y_buffer, w + parent_ptr->x_buffer, character );
        }
    }
}


char get_char(tile_type_t type)
{
    switch(type) {
    case empty_tile:
        attron(COLOR_PAIR(empty_pair));
        return '\0';
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


int main( void )
{
    srand(time(NULL));
    initscr();
    refresh();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, true);

    init_tile_colors();
    map_t * the_map = create_map(stdscr, MAPHEIGHT, MAPWIDTH);
    print_layer_buffered(stdscr, the_map->layers[terrain_layer], the_map);
    //print_tile(stdscr, the_map, Game_data.player_y, Game_data.player_x);
    while (true) {

        // If theres been a character input, some do things
        switch (getch()) {
        case 'q':
            endwin();
            return 0;

        case KEY_DOWN:
        case 's':
            break;

        case KEY_UP:
        case 'w':
            break;

        case KEY_LEFT:
        case 'a':
            break;

        case KEY_RIGHT:
        case 'd':
            break;

        case 'l':
        case 'k':
            break;
        }


    }

    endwin();

    return 0;
}
