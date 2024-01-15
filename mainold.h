#include <curses.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "map_util.c"

#define PDC_RGB
#define N_COLORS 8

#define MAPWIDTH 30
#define MAPHEIGHT 20

#define COLOR_BROWN COLOR_WHITE+1
#define COLOR_TAN COLOR_BROWN+1
#define COLOR_LTGREEN COLOR_TAN+1
#define COLOR_DKRED COLOR_LTGREEN+1



// Raises an error if a map pointer doesn't exist
#define check_map_ptr(mp) if (mp == NULL) fprintf(stderr, "map pointer empty")

// Moves to a new graphical cell and crashes the program if it does not work.
#define move_and_check(y, x) if (move(y, x) == ERR) return 1





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

/*
typedef struct {

} cell_t;
*/
typedef uint8_t coord_t;

typedef struct {
    coord_t x;
    coord_t y;
    bool can_enter;
    tile_type_t type;
} tile_t;


typedef struct {
    uint8_t width;
    uint8_t height;

    tile_t **tiles;

    uint8_t x_buffer;
    uint8_t y_buffer;

    coord_t cursor_y;
    coord_t cursor_x;

} map_t;

typedef struct {
    coord_t y;
    coord_t x;
} player_t;

typedef struct {
    bool cursor_on;
    coord_t y;
    coord_t x;
} cursor_t;

map_t *create_map(uint8_t width, uint8_t height);

void destroy_map(map_t * source);

void copy_map(map_t *dest, const map_t *source);

map_t *create_map_copy(map_t *source);



void clear_tile(map_t * layer, coord_t del_y, coord_t del_x);



void generate_borders(map_t *layer);

void generate_grass(map_t *layer);

void generate_grasslands(map_t *layer);


void generate_player_tile(map_t * layer, coord_t gen_y, coord_t gen_x);


void generate_looking_cursor(map_t * layer, coord_t start_y, coord_t start_x);




void move_player(map_t * disp_ptr, map_t * terrain_ptr, move_dir_t dir);

void move_looking_cursor();



bool check_can_enter()



char get_char(const tile_type_t type);


// Prints a character matrix with the top left corner at the origin of the console.
void print_chars_raw(map_t * source);

/*
 * Prints a character matrix with the top left corner placed at a distance from the
 * console origin determined by the x_buffer and y_buffer map members.
 *      I.e., if x_buffer = 10 and y_buffer = 5, the characters of a map structure will
 *      be printed in a rectangle starting 10 cells right of the origin and 5 cells down.
 */
void print_chars_buffered(WINDOW *win, map_t * source);


void reprint_moved_cells(WINDOW * win, map_t * display_map,
                        tile_t * dest_tile, tile_t * source_tile);


void clear_cell(map_t * layer, coord_t del_y, coord_t del_x);
