#include <curses.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "gob_util.h"

#define PDC_BGR

#if 0
#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_YELLOW 6
#endif
#define COLOR_LTGRAY 7
#define COLOR_GRAY 8
#define COLOR_LTBLUE 9
#define COLOR_LTGREEN 10
#define COLOR_LTCYAN 11
#define COLOR_LTRED 12
#define COLOR_LTMAGENTA 13
#define COLOR_LTYELLOW 14
#define COLOR_WHITE 15

#define COLOR_BROWN 17
#define COLOR_DKBROWN 18
#define COLOR_LTBROWN 19

#define MAPWIDTH 30
#define MAPHEIGHT 20


#define NUM_LAYER_TYPES 4


typedef enum {
    empty_tile,
    player_tile,
    cursor_tile,
    lackey_tile,
    desc_tile,
    border_tile,
    wall_tile,
    floor_tile,
    grass1_tile,
    grass2_tile,
    mud_tile
} tile_type_t;

typedef enum {
    empty_pair,
    player_pair,
    cursor_pair,
    lackey_pair,
    border_pair,
    wall_pair,
    floor_pair,
    grass1_pair,
    grass2_pair,
    mud_pair,
} tile_color_t;

typedef enum {
    down_dir,
    up_dir,
    left_dir,
    right_dir
} move_dir_t;

typedef enum {
    terrain_layer,
    enemy_layer,
    player_layer,
    cursor_layer
} layer_id_t;

typedef uint8_t coord_t;


typedef struct {
    uint8_t height;
    uint8_t width;

    double condition;

    coord_t min_y;
    coord_t min_x;

    coord_t max_y;
    coord_t max_x;
} building_t;





typedef struct {
    tile_type_t type;

    coord_t y_pos;
    coord_t x_pos;

    bool can_enter;

} tile_t;

typedef struct {
    layer_id_t id;

    tile_t ** tiles;

} layer_t;

typedef struct {
    layer_t ** layers;
    uint8_t height;
    uint8_t width;

    uint8_t y_buffer;
    uint8_t x_buffer;
} map_t;


typedef struct {
    coord_t y_pos;
    coord_t x_pos;

    tile_t * sight_tiles;

    bool attacking_player;
} enemy_t;

typedef struct {
    coord_t player_y;
    coord_t player_x;

    coord_t cursor_y;
    coord_t cursor_x;

    bool look_mode;

    uint8_t num_enemies;

    enemy_t * enemies;
} game_info_t;



void create_enemy_data( map_t * parent_ptr );

tile_t create_empty_tile( void );

map_t * create_map(WINDOW * win, uint8_t height, uint8_t width);

void get_layers( map_t * parent_ptr );


layer_t * create_layer(layer_id_t id, map_t * parent_ptr);

layer_t * create_terrain_layer(map_t * parent_ptr);

layer_t * create_player_layer(map_t * parent_ptr);

layer_t * create_enemy_layer(map_t * parent_ptr);

layer_t * create_cursor_layer(map_t * parent_ptr);


void destroy_tile(layer_t * layer, coord_t y, coord_t x);

void destroy_and_reprint_tile( WINDOW * win, map_t * parent_ptr, layer_t * layer,
                               coord_t y, coord_t x );


void create_grasslands(layer_t * layer, map_t * parent_ptr);

void create_grass(layer_t * layer, map_t * parent_ptr);

void create_mud(layer_t * layer, map_t * parent_ptr);

void create_buildings(layer_t * layer, map_t * parent_ptr);

void create_borders(layer_t * layer, map_t * parent_ptr);


void create_player(layer_t * layer);


void create_enemies( layer_t * layer );

void create_lackey(layer_t * layer, coord_t y, coord_t x);


void create_cursor(layer_t * layer);


bool check_can_move(map_t * map_ptr, coord_t y, coord_t x);

bool check_can_look(map_t * map_ptr, coord_t y, coord_t x);

bool check_spawn_loc(map_t * map_ptr, layer_id_t id, coord_t y, coord_t x);


void replace_and_reprint_tile( WINDOW * win, layer_t * move_layer, map_t * parent_ptr,
                               coord_t new_y, coord_t new_x, coord_t old_y, coord_t old_x );

void move_tile( layer_t * move_layer, coord_t new_y, coord_t new_x,
                coord_t old_y, coord_t old_x );

void move_and_reprint_down( WINDOW * win, map_t * parent_map,
                            layer_t * move_layer, coord_t old_y, coord_t old_x);

void move_and_reprint_up( WINDOW * win, map_t * parent_map,
                            layer_t * move_layer, coord_t old_y, coord_t old_x);

void move_and_reprint_left( WINDOW * win, map_t * parent_map,
                            layer_t * move_layer, coord_t old_y, coord_t old_x);

void move_and_reprint_right( WINDOW * win, map_t * parent_map,
                            layer_t * move_layer, coord_t old_y, coord_t old_x);


void print_tile(WINDOW * win, map_t * parent_ptr, coord_t y, coord_t x);

void print_layer_buffered(WINDOW * win, layer_t * layer, map_t * parent);

void print_all_layers(WINDOW * win, map_t * map_ptr);


void print_looking_desc(WINDOW * win, map_t * map_ptr);

void clear_looking_desc(WINDOW * win, map_t * map_ptr);


char get_char(tile_type_t type);

char * get_tile_name(tile_type_t type);

char * get_tile_desc(tile_type_t type);


void move_cursor_buffered( map_t * map_ptr, coord_t y, coord_t x );

void init_tile_colors( void );
