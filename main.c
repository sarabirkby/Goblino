#include "main.h"


game_info_t Game_data;


void create_enemy_data( map_t * parent_ptr )
{

    enemy_t * enemy = (enemy_t *) calloc(Game_data.num_enemies, sizeof(enemy_t));

    for (int i = 0; i < Game_data.num_enemies; i++) {

        coord_t y, x;

        do {
            y = rand() % parent_ptr->height;
            x = rand() % parent_ptr->width;
        } while (!check_spawn_loc(parent_ptr, enemy_layer, y, x));

        enemy[i].y_pos = y;
        enemy[i].x_pos = x;
        enemy[i].attacking_player = false;
        enemy[i].sight_tiles = NULL;


    }

    Game_data.enemies = enemy;

}


tile_t create_empty_tile( void )
{
    tile_t e_tile;
    e_tile.can_enter = true;
    e_tile.type = empty_tile;

    return e_tile;
}


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
    // layer addresses start at terrain
    parent_ptr->layers[terrain_layer] = create_terrain_layer(parent_ptr);

    parent_ptr->layers[enemy_layer] = create_enemy_layer(parent_ptr);

    parent_ptr->layers[player_layer] = create_player_layer(parent_ptr);

    parent_ptr->layers[cursor_layer] = create_cursor_layer(parent_ptr);

}

layer_t * create_layer(layer_id_t id, map_t * parent_ptr)
{
    layer_t * layer = (layer_t *) calloc(1, sizeof(layer_t));

    layer->id = id;

    // Create the rows
    layer->tiles = (tile_t **) calloc(parent_ptr->height, sizeof(tile_t *));

    // Creates an array of tiles for every row
    for (int h = 0; h < parent_ptr->height; h++) {
        layer->tiles[h] = (tile_t *) calloc(parent_ptr->width, sizeof(tile_t));
    }

    tile_t e_tile = create_empty_tile();

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

layer_t * create_enemy_layer(map_t * parent_ptr)
{
    layer_t * en_layer = create_layer(enemy_layer, parent_ptr);

    create_enemy_data( parent_ptr );
    create_enemies( en_layer );

    return en_layer;
}

layer_t * create_player_layer(map_t * parent_ptr)
{
    layer_t * p_layer = create_layer(player_layer, parent_ptr);

    create_player(p_layer);

    return p_layer;
}


layer_t * create_cursor_layer(map_t * parent_ptr)
{
    layer_t * c_layer = create_layer(cursor_layer, parent_ptr);

    return c_layer;
}


void destroy_tile(layer_t * layer, coord_t y, coord_t x)
{
    tile_t e_tile = create_empty_tile();
    e_tile.y_pos = y, e_tile.x_pos = x;

    layer->tiles[y][x] = e_tile;
}


void destroy_and_reprint_tile( WINDOW * win, map_t * parent_ptr, layer_t * layer,
                               coord_t y, coord_t x )
{
    destroy_tile(layer, y, x);
    print_tile(win, parent_ptr, y, x);
}


void create_grasslands(layer_t * layer, map_t * parent_ptr)
{
    create_grass(layer, parent_ptr);
    create_mud(layer, parent_ptr);
    create_buildings(layer, parent_ptr);
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

/** @fn void create_buildings(layer_t * layer, map_t * parent_ptr)
  * @brief Creates 1-3 buildings across the terrain map.
  * @param layer  The  pointer to the layer which the building will be added to.
  * @param parent_ptr  The pointer to the map the layer is a member of.
  */
void create_buildings(layer_t * layer, map_t * parent_ptr)
{
    int num_buildings = rand() % 3 + 1;

    tile_t w_tile, f_tile;

    w_tile.can_enter = false;
    w_tile.type = wall_tile;
    f_tile.can_enter = true;
    f_tile.type = floor_tile;

    building_t * buildings = calloc(num_buildings, sizeof(building_t));
    for (int i = 0; i < num_buildings; i++) {
        coord_t starting_y = rand() % parent_ptr->height;
        coord_t starting_x = rand() % parent_ptr->width;

        coord_t mid_y = parent_ptr->height / 2;
        coord_t mid_x = parent_ptr->width / 2;

        buildings[i].height = rand() % parent_ptr->height / 2 + 3;

        buildings[i].width = rand() % parent_ptr->width / 2 + 3;

        uint8_t loop_y_start, loop_x_start, loop_y_end, loop_x_end;
        if (starting_y < mid_y) {
            loop_y_start = starting_y;
            loop_y_end = starting_y + buildings[i].height;
        }
        else {  // bottom
            loop_y_start = starting_y - buildings[i].height;
            loop_y_end = starting_y;
        }
        if (starting_x < mid_x) {
                loop_x_start = starting_x;
                loop_x_end = starting_x + buildings[i].width;
        }
        else {
                loop_x_start = starting_x - buildings[i].width;
                loop_x_end = starting_x;
        }


        buildings[i].min_y = loop_y_start;
        buildings[i].min_x = loop_x_start;
        buildings[i].max_y = loop_y_end;
        buildings[i].max_x = loop_x_end;

        buildings[i].condition = get_gaussian();

        if (buildings[i].condition < 0 || buildings[i].condition > 1) {
            fprintf(stderr, "out of bounds: %f", buildings[i].condition);
        }
    }



    for (int i = 0; i < num_buildings; i++) {
        for (int y = buildings[i].min_y; y <= buildings[i].max_y; y++) {
            for (int x = buildings[i].min_x; x <= buildings[i].max_x; x++) {

                bool do_build = true;
                for (int j = 0; j < num_buildings; j++) {
                    if ( buildings[j].height * buildings[j].width
                         > buildings[i].height * buildings[i].width
                         && y >= buildings[j].min_y && y <= buildings[j].max_y
                         && x >= buildings[j].min_x && x <= buildings[j].max_x )
                        do_build = false;
                }

                if (do_build) {
                    if ( y == buildings[i].min_y || x == buildings[i].min_x
                         || y == buildings[i].max_y || x == buildings[i].max_x ) {
                        if( rand() % 100 <= (int)(buildings[i].condition * 100) ) {
                            w_tile.y_pos = y, w_tile.x_pos = x;

                            layer->tiles[y][x] = w_tile;
                        }
                    }
                    else if ( rand() % 100 <= (int)(buildings[i].condition * 100) ) {
                        f_tile.y_pos = y, f_tile.x_pos = x;
                        layer->tiles[y][x] = f_tile;
                    }
                }
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

void create_player(layer_t * layer)
{
    tile_t p_tile;
    p_tile.can_enter = false;
    p_tile.type = player_tile;
    p_tile.y_pos = Game_data.player_y;
    p_tile.x_pos = Game_data.player_x;


    layer->tiles[Game_data.player_y][Game_data.player_x] = p_tile;
}


void create_enemies( layer_t * layer )
{

    for (int i = 0; i < Game_data.num_enemies; i++) {
        create_lackey(layer, Game_data.enemies[i].y_pos, Game_data.enemies[i].x_pos);
    }
}

void create_lackey( layer_t * layer, coord_t y, coord_t x )
{
    tile_t * dest_tile = &layer->tiles[y][x];
    dest_tile->can_enter = false;
    dest_tile->type = lackey_tile;
}



void create_vision_tiles( map_t * map_ptr, enemy_t * enemy)
{
    int max_vision_tiles = (VISION_LEN / 3) * 3 + (VISION_LEN / 3) * 5 + (VISION_LEN / 3 + VISION_LEN % 3) * 7;
    tile_t ** vis_tiles = (tile_t **) calloc(max_vision_tiles, sizeof(tile_t *));
    uint8_t current_tile_index = 0;
    for (dir_t dir = down_dir; dir <= right_dir; dir++)
    {
        for (int i = 0; i < VISION_LEN; i++) {
            uint8_t current_width;
            if (i >= 2 * VISION_LEN / 3)
                current_width = 7;
            else if (i >= VISION_LEN / 3)
                current_width = 5;
            else
                current_width = 3;

            for (int j = -current_width/2; j < current_width/2; j++) {
                coord_t tile_y, tile_x;
                switch(dir) {
                case down_dir:
                    tile_y = enemy->y_pos + i;
                    tile_x = enemy->x_pos + j;

                    break;

                case up_dir:
                    tile_y = enemy->y_pos - i;
                    tile_x = enemy->x_pos + j;

                    break;

                case left_dir:
                    tile_y = enemy->y_pos + j;
                    tile_x = enemy->x_pos - i;

                    break;

                case right_dir:
                    tile_y = enemy->y_pos + j;
                    tile_x = enemy->x_pos + i;

                    break;

                default:
                    fprintf(stderr, "Direction out of bounds");
                }

                if (check_can_look(map_ptr, tile_y, tile_x)) {
                    vis_tiles[current_tile_index++] = &map_ptr->layers[enemy_layer]->tiles[tile_y][tile_x];
                    mvaddch(tile_y+map_ptr->y_buffer, tile_x+map_ptr->x_buffer, 'V');
                }

            }
        }
    }
}


void create_cursor(layer_t * layer)
{
    tile_t c_tile;
    c_tile.can_enter = false;
    c_tile.type = cursor_tile;
    c_tile.y_pos = Game_data.cursor_y;
    c_tile.x_pos = Game_data.cursor_x;

    layer->tiles[c_tile.y_pos][c_tile.x_pos] = c_tile;
}


bool check_can_move(map_t * map_ptr, coord_t y, coord_t x)
{
    for (int i = 0; i < NUM_LAYER_TYPES; i++) {
        // If any tiles cannot be entered, you cannot enter
        if ( !map_ptr->layers[i]->tiles[y][x].can_enter )
            return false;
    }
    // Only if all have can_enter
    return true;
}


bool check_can_look(map_t * map_ptr, coord_t y, coord_t x)
{
    if ( y >= 0 && y < map_ptr->height && x >= 0 && x < map_ptr->width )
        return true;
    else
        return false;
}

bool check_spawn_loc(map_t * map_ptr, layer_id_t id, coord_t y, coord_t x)
{
    for (int i = 0; i < id; i++) {
        if ( !map_ptr->layers[i]->tiles[y][x].can_enter )
            return false;
    }

    return true;
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
    if (new_y == old_y && new_x == old_x)
    {
        fprintf(stderr, "Cannot move to the same tile: (%d, %d)\n", new_x, new_y);
        return;
    }
    // copy old tile to new
    tile_t tile_to_copy = move_layer->tiles[old_y][old_x];
    tile_to_copy.y_pos = new_y, tile_to_copy.x_pos = new_x;
    move_layer->tiles[new_y][new_x] = tile_to_copy;

    // and makes the old tile empty
    destroy_tile(move_layer, old_y, old_x);
}

void move_and_reprint_down( WINDOW * win, map_t * parent_map,
                           layer_t * move_layer, coord_t old_y, coord_t old_x)
{
    move_tile(move_layer, old_y+1, old_x, old_y, old_x);
    print_tile(win, parent_map, old_y, old_x);
    print_tile(win, parent_map, old_y+1, old_x);
}

void move_and_reprint_up( WINDOW * win, map_t * parent_map,
                           layer_t * move_layer, coord_t old_y, coord_t old_x)
{
    move_tile(move_layer, old_y-1, old_x, old_y, old_x);
    print_tile(win, parent_map, old_y, old_x);
    print_tile(win, parent_map, old_y-1, old_x);
}

void move_and_reprint_left( WINDOW * win, map_t * parent_map,
                           layer_t * move_layer, coord_t old_y, coord_t old_x)
{
    move_tile(move_layer, old_y, old_x-1, old_y, old_x);
    print_tile(win, parent_map, old_y, old_x);
    print_tile(win, parent_map, old_y, old_x-1);
}

void move_and_reprint_right( WINDOW * win, map_t * parent_map,
                           layer_t * move_layer, coord_t old_y, coord_t old_x)
{
    move_tile(move_layer, old_y, old_x+1, old_y, old_x);
    print_tile(win, parent_map, old_y, old_x);
    print_tile(win, parent_map, old_y, old_x+1);

}

// Reprints a tile for every layer, meaning the top-most
// layer with a non-empty tile will be printed.
void print_tile(WINDOW *win, map_t * parent_ptr, coord_t y, coord_t x)
{
    for (int i = 0; i < NUM_LAYER_TYPES; i++) {
        char character = get_char(parent_ptr->layers[i]->tiles[y][x].type);
        if (character != '\0') {
            mvwaddch(win, y + parent_ptr->y_buffer, x + parent_ptr->x_buffer, character);
        }
    }
    Game_data.reprinted_tile = true;
}


void print_layer_buffered(WINDOW * win, layer_t * layer, map_t * parent_ptr)
{

    for (int h = 0; h < parent_ptr->height; h++) {
        for (int w = 0; w < parent_ptr->width; w++) {
            tile_t current_tile = layer->tiles[h][w];
            char character = get_char(current_tile.type);
            // Don't display empty tiles and the cursor if not in look mode
            if (current_tile.type != empty_tile
                && ( Game_data.look_mode || current_tile.type != cursor_tile ) )
            {
                mvwaddch( win, h + parent_ptr->y_buffer, w + parent_ptr->x_buffer, character );
            }
        }
    }

    Game_data.reprinted_tile = true;
}


void print_all_layers(WINDOW * win, map_t * map_ptr)
{
    for (int i = 0; i < NUM_LAYER_TYPES; i++) {
        //if (i != enemy_layer )    // debug
        print_layer_buffered(win, map_ptr->layers[i], map_ptr);
    }
}


void print_looking_desc(WINDOW * win, map_t * map_ptr)
{
    clear_looking_desc(win, map_ptr);

    tile_type_t looking_at = empty_tile;
    for (int i = 0; i < NUM_LAYER_TYPES; i++) {
        tile_t current_tile = map_ptr->layers[i]->tiles[Game_data.cursor_y][Game_data.cursor_x];
        if (current_tile.type != cursor_tile && current_tile.type != empty_tile)
            looking_at = current_tile.type;
    }

    if (looking_at != empty_tile)
        mvwaddch(win, map_ptr->height+map_ptr->y_buffer+1, map_ptr->x_buffer, get_char(looking_at));

    // Clears color
    attron(COLOR_PAIR(wall_pair));
    attroff(COLOR_PAIR(wall_pair));

    waddch(win, ' ');
    char * title = get_tile_name(looking_at);
    waddstr(win, title);
    waddstr(win, ":\n");

    char * desc = get_tile_desc(looking_at);
    wmove(win, map_ptr->height+map_ptr->y_buffer+2, map_ptr->x_buffer);
    waddstr(win, desc);

    Game_data.reprinted_tile = true;

}

void clear_looking_desc(WINDOW * win, map_t * map_ptr)
{
    for (int i=0; i < 4; i++)
        mvwdeleteln(win, map_ptr->y_buffer+map_ptr->height+1, map_ptr->x_buffer);

    Game_data.reprinted_tile = true;
}


void print_enemy_menu(WINDOW * win, map_t * map_ptr)
{
    attron(COLOR_PAIR(lackey_pair));
    attroff(COLOR_PAIR(lackey_pair));

    coord_t current_y = map_ptr->y_buffer;
    coord_t current_x = map_ptr->x_buffer+map_ptr->width+2;
    mvwaddstr(win, current_y, current_x, "Enemies:");

    current_y += 2;

    move(current_y, current_x);
    for (int i = 0; i < Game_data.num_enemies; i++) {
        coord_t enemy_y = Game_data.enemies[i].y_pos;
        coord_t enemy_x = Game_data.enemies[i].x_pos;

        waddch(win, get_char(map_ptr->layers[enemy_layer]->tiles[enemy_y][enemy_x].type));
        attroff(COLOR_PAIR(lackey_pair));
        waddch(win, ' ');
    }

    Game_data.reprinted_tile = true;

}

void print_enemy_desc(WINDOW * win, map_t * map_ptr)
{

}

void clear_enemy_menu(WINDOW * win, map_t * map_ptr)
{
    for (int i = 0; i < 1+NUM_ENEMY_ROWS; i++) {
        move_cursor_buffered(win, map_ptr, i, map_ptr->width);
        for (int j = map_ptr->x_buffer+map_ptr->width; j < win->_maxx; j++)
        {
            wdelch(win);
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
    case cursor_tile:
        attron(COLOR_PAIR(cursor_pair));
        return 'X';
    case lackey_tile:
        attron(COLOR_PAIR(lackey_pair));
        return 'g';
    case border_tile:
        attron(COLOR_PAIR(border_pair));
        return '#';
    case wall_tile:
        attron(COLOR_PAIR(wall_pair));
        return 'O';
    case floor_tile:
        attron(COLOR_PAIR(floor_pair));
        return '=';
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
        return 'N';
    }
}


char * get_tile_name(tile_type_t type)
{

    switch(type) {
    case empty_tile:
        return "Empty Tile";

    case player_tile:
        return "Player";

    case lackey_tile:
        return "Goblin Lackey";

    case border_tile:
        return "Border";

    case wall_tile:
        return "Wall";

    case floor_tile:
        return "Floor";

    case grass1_tile:
        return "Soft Grass";

    case grass2_tile:
        return "Spiky Grass";

    case mud_tile:
        return "Mud";


    default:
        fprintf(stderr, "type not accounted for!");
        return "You shouldn't see this.";
    }
}


char * get_tile_desc(tile_type_t type)
{
    switch(type) {
    case empty_tile:
        return "There's nothing here.";

    case player_tile:
        return "A very handsome and courageous goblin king.";

    case lackey_tile:
        return "An unexceptional goblin henchman.";

    case border_tile:
        return "The edge of this area.";

    case wall_tile:
        return "A strong, sturdy wall. No getting through this.";

    case floor_tile:
        return "A seemingly stable wooden floor. Great for walking on.";

    case grass1_tile:
        return "Short, soft grass.";

    case grass2_tile:
        return "Tall, spiky grass.";

    case mud_tile:
        return "Sticky brown mud. Yuck.";

    default:
        fprintf(stderr, "You shouldn't be able to look at this.");
        return "You shouldn't see this.";
    }
}


void init_color_256( short color, short r, short g, short b )
{
    short modifier = 1000 / 255;
    r *= modifier, g *= modifier, b *= modifier;
    init_color(color, r, g, b);
}
void init_tile_colors( void )
{

    start_color();
    if (can_change_color()) {
        init_color_256(COLOR_BROWN, 210, 105, 30);
        init_color_256(COLOR_DKBROWN, 100, 65, 23);
        init_color_256(COLOR_LTBROWN, 205, 133, 63);
    }
    else {
        printf("ERROR!");
    }

    init_pair(empty_pair, COLOR_WHITE, COLOR_BLACK);
    init_pair(player_pair, COLOR_WHITE, COLOR_RED);
    init_pair(lackey_pair, COLOR_LTGREEN, COLOR_LTCYAN);
    init_pair(cursor_pair, COLOR_WHITE, COLOR_YELLOW);
    init_pair(wall_pair, COLOR_LTGRAY, COLOR_GRAY);
    init_pair(floor_pair, COLOR_DKBROWN, COLOR_LTBROWN);
    init_pair(border_pair, COLOR_LTRED, COLOR_RED);
    init_pair(grass1_pair, COLOR_GREEN, COLOR_LTGREEN);
    init_pair(grass2_pair, COLOR_LTGREEN, COLOR_GREEN);
    init_pair(mud_pair, COLOR_LTBROWN, COLOR_BROWN);
}

void move_cursor_buffered( WINDOW * win, map_t * map_ptr,
                           coord_t y, coord_t x )
{
    wmove(win, y+map_ptr->y_buffer, x+map_ptr->x_buffer);
}

void move_to_enemy( WINDOW * win, map_t * map_ptr )
{
    enemy_t current_enemy = Game_data.enemies[Game_data.selected_enemy];
    move_cursor_buffered(win, map_ptr, current_enemy.y_pos,
                         current_enemy.x_pos);
}

void move_to_looking( WINDOW * win, map_t * map_ptr )
{
    move_cursor_buffered(win, map_ptr, Game_data.cursor_y,
                         Game_data.cursor_x);
}

int main( void )
{
    Game_data.player_y = 1, Game_data.player_x = 1;
    Game_data.cursor_y = Game_data.player_y;
    Game_data.cursor_x = Game_data.player_x;

    Game_data.look_mode = false;
    Game_data.reprinted_tile = false;

    Game_data.selected_enemy = 0;
    Game_data.num_enemies = 3;

    srand(time(NULL));
    initscr();
    refresh();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, true);

    init_tile_colors();

    map_t * the_map = create_map(stdscr, MAPHEIGHT, MAPWIDTH);


    print_all_layers(stdscr, the_map);
    while (true) {
        if (Game_data.reprinted_tile) {
            if (Game_data.enemy_mode)
                move_to_enemy(stdscr, the_map);
            else if (Game_data.look_mode)
                move_to_looking(stdscr, the_map);
        }
        refresh();
        // If theres been a character input, some do things
        switch (getch()) {
        case 'x':
            endwin();
            fprintf(stderr, "%d", COLORS);
            return 0;

        case KEY_DOWN:
        case 's':
            if (Game_data.look_mode) {
                if (check_can_look(the_map, Game_data.cursor_y + 1, Game_data.cursor_x))
                {
                    move_and_reprint_down( stdscr, the_map, the_map->layers[cursor_layer],
                                           Game_data.cursor_y, Game_data.cursor_x);
                    Game_data.cursor_y++;
                    print_looking_desc(stdscr, the_map);
                }
            }
            else if (check_can_move(the_map, Game_data.player_y + 1, Game_data.player_x))
            {
                move_and_reprint_down( stdscr, the_map, the_map->layers[player_layer],
                                       Game_data.player_y, Game_data.player_x );
                Game_data.player_y++;
            }
            break;

        case KEY_UP:
        case 'w':
            if (Game_data.look_mode) {
                if (check_can_look(the_map, Game_data.cursor_y - 1, Game_data.cursor_x))
                {
                    move_and_reprint_up( stdscr, the_map, the_map->layers[cursor_layer],
                                           Game_data.cursor_y, Game_data.cursor_x);
                    Game_data.cursor_y--;
                    print_looking_desc(stdscr, the_map);
                }
            }
            else if (check_can_move(the_map, Game_data.player_y - 1,
                                    Game_data.player_x))
            {
                move_and_reprint_up(stdscr, the_map,
                                    the_map->layers[player_layer],
                                    Game_data.player_y, Game_data.player_x);
                Game_data.player_y--;
            }
            break;

        case KEY_LEFT:
        case 'a':
            if (Game_data.look_mode) {
                if (check_can_look(the_map, Game_data.cursor_y,
                                   Game_data.cursor_x - 1))
                {
                    move_and_reprint_left( stdscr, the_map,
                                           the_map->layers[cursor_layer],
                                           Game_data.cursor_y,
                                           Game_data.cursor_x);
                    Game_data.cursor_x--;
                    print_looking_desc(stdscr, the_map);
                }
            }
            else if (check_can_move(the_map, Game_data.player_y,
                                    Game_data.player_x - 1))
            {
                move_and_reprint_left(stdscr, the_map,
                                      the_map->layers[player_layer],
                                      Game_data.player_y,
                                      Game_data.player_x);
                Game_data.player_x--;
            }
            break;

        case KEY_RIGHT:
        case 'd':
            if (Game_data.look_mode) {
                if (check_can_look(the_map, Game_data.cursor_y, Game_data.cursor_x + 1))
                {
                    move_and_reprint_right( stdscr, the_map, the_map->layers[cursor_layer],
                                           Game_data.cursor_y, Game_data.cursor_x);
                    Game_data.cursor_x++;
                    print_looking_desc(stdscr, the_map);
                }
            }
            else if (check_can_move(the_map, Game_data.player_y, Game_data.player_x + 1))
            {
                move_and_reprint_right(stdscr, the_map, the_map->layers[player_layer], Game_data.player_y, Game_data.player_x);
                Game_data.player_x++;
            }
            break;

        case 'l':
        case 'k':
            if (Game_data.enemy_mode)
                break;

            if (Game_data.look_mode) {
                Game_data.look_mode = false;
                curs_set(FALSE);
                destroy_and_reprint_tile( stdscr, the_map,
                                          the_map->layers[cursor_layer],
                                          Game_data.cursor_y,
                                          Game_data.cursor_x );

                clear_looking_desc(stdscr, the_map);

            }
            else {
                Game_data.look_mode = true;
                Game_data.cursor_y = Game_data.player_y;
                Game_data.cursor_x = Game_data.player_x;
                curs_set(2);
                create_cursor(the_map->layers[cursor_layer]);
                move_cursor_buffered(stdscr, the_map, Game_data.cursor_y,
                                     Game_data.cursor_x);
                print_tile(stdscr, the_map,
                           Game_data.cursor_y, Game_data.cursor_x);

                print_looking_desc(stdscr, the_map);
            }
            break;

        case 'r':
            if (Game_data.look_mode)
                break;

            if (Game_data.enemy_mode) {
                Game_data.enemy_mode = false;
                curs_set(FALSE);
                clear_enemy_menu(stdscr, the_map);
            }
            else {
                Game_data.enemy_mode = true;
                curs_set(2);
                print_enemy_menu(stdscr, the_map);
                move_to_enemy(stdscr, the_map);
            }

            break;

        case 'e':
            if (!Game_data.enemy_mode)
                break;

            if (Game_data.selected_enemy == Game_data.num_enemies-1)
                Game_data.selected_enemy = 0;
            else
                Game_data.selected_enemy++;

            move_to_enemy(stdscr, the_map);
            break;

        case 'q':
            if (!Game_data.enemy_mode)
                break;

            if (Game_data.selected_enemy == 0)
                Game_data.selected_enemy = Game_data.num_enemies-1;
            else
                Game_data.selected_enemy--;

            move_to_enemy(stdscr, the_map);
            break;
        }


    }

    endwin();

    return 0;
}


// TODO: ADD FUNCTION THAT PRINTS VISION TILES IN ENEMY MODE
