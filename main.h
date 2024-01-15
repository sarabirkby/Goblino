#include <curses.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

//#include "map_util.c"

#define PDC_RGB
#define N_COLORS 8

#define MAPWIDTH 30
#define MAPHEIGHT 20


#define NUM_LAYER_TYPES 2

#define COLOR_BROWN COLOR_WHITE+1
#define COLOR_TAN COLOR_BROWN+1
#define COLOR_LTGREEN COLOR_TAN+1
#define COLOR_DKRED COLOR_LTGREEN+1


typedef enum {
    empty_tile,
    player_tile,
    border_tile,
    looking_cursor_tile,
    grass1_tile,
    grass2_tile,
    mud_tile
} tile_type_t;

typedef enum {
    empty_pair,
    player_pair,
    looking_cursor_pair,
    border_pair,
    grass1_pair,
    grass2_pair,
    mud_pair
} tile_color_t;

typedef enum {
    down_dir,
    up_dir,
    left_dir,
    right_dir
} move_dir_t;

typedef enum {
    terrain_layer,
    player_layer,
    cursor_layer
} layer_id_t;

typedef uint8_t coord_t;


typedef struct {
    coord_t player_y;
    coord_t player_x;

    coord_t cursor_y;
    coord_t cursor_x;


} game_info_t;

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






map_t * create_map(WINDOW * win, uint8_t height, uint8_t width);

void get_layers(map_t * parent_ptr);

layer_t * create_layer(layer_id_t id, map_t * parent_ptr);

layer_t * create_terrain_layer(map_t * parent_ptr);

layer_t * create_player_layer(map_t * parent_ptr);

void create_player(layer_t * layer, map_t * parent_ptr);

void create_grasslands(layer_t * layer, map_t * parent_ptr);

void create_grass(layer_t * layer, map_t * parent_ptr);

void create_mud(layer_t * layer, map_t * parent_ptr);

void create_borders(layer_t * layer, map_t * parent_ptr);


void replace_and_reprint_tile( WINDOW * win, layer_t * move_layer, map_t * parent_ptr,
                               coord_t new_y, coord_t new_x, coord_t old_y, coord_t old_x );

void move_tile( layer_t * move_layer, coord_t new_y, coord_t new_x,
                coord_t old_y, coord_t old_x );

void print_tile(WINDOW * win, map_t * parent_ptr, coord_t y, coord_t x);

void print_layer_buffered(WINDOW * win, layer_t * layer, map_t * parent);

char get_char(tile_type_t type);

void init_tile_colors( void );
