#ifndef STRUCTS_H
#define STRUCTS_H

#include "Common.h"

typedef struct
{
    int    running;
    time_t time;        // global timer
    time_t seconds;     // second timer (time/1000)
    time_t ticks;       // total game ticks
    time_t frames;      // total frames drawn
    time_t tick_time;   // target time interval of logic cycles
    time_t frame_time;  // target time interval between draws
    int    tick_rate;   // target rate of logic cycles
    int    frame_rate;  // target frame rate
    int    fps;         // actual measured fps
    float  fps_avg;     // average fps (frames/seconds)
} System_t;

typedef struct
{
    uint8_t keycode;
    uint8_t type;
    time_t time;
} KeyEvent_t;

typedef struct
{
    KeyEvent_t queue[256];
    uint8_t keystates[256];
    uint8_t queue_head;
    uint8_t queue_tail;
} Keyboard_t;

typedef struct
{
    double x, y;
} Vec2;

typedef struct
{
    int x, y;
} Vec2_int;

typedef struct
{
    char* filename;
    uint8_t* pixels;
    uint16_t width, height;
    uint16_t transparent;
    int offset_x, offset_y;
} Texture_t;

/*
typedef struct
{
    Texture_t texture;
    uint16_t height;
    uint16_t num_frames;
    uint16_t frame;
} Sprite_t;
*/

typedef struct
{
    id_t     id;
    flags_t  flags;
    Vec2     position;
    Vec2_int grid_loc;  // location on the grid
    Vec2     direction; // direction
    double   angle;     // current direction in radians
    Vec2     velocity;  // velocity
    float    magnitude; // magnitude of velocity
    int      radius;    // collision box/circle radius
    
    flags_t control;
    uint8_t ai_mode;
    int     ai_timer;
    id_t    target_id; // index number in the object array of the AI's target
    Vec2    move_target;

    uint8_t color;
    id_t texture_id;
} Object_t;

typedef struct
{
    uint8_t r,g,b;
} Color_t;

typedef struct
{
    Color_t colors[256];
} Palette_t;

typedef struct {
    uint8_t texture_id : 6;
    uint8_t material_type : 3; //generic, grass, sand, wood, water, snow, metal, plastic ...?
    uint8_t obstacle : 1;
    uint8_t block_bullets : 1;
    uint8_t entity_type : 2; // none, door, switch, breakable
    uint8_t entity_value : 3; // door id (0-7), switch id (0-7), breakable_health (0-7)
} Tile_t;

typedef struct
{
    int level_num; // current level number
    int width; // width in tiles
    int height; // height in tiles
    Tile_t* tilemap;
} Map_t;

typedef struct
{
    Map_t Map;
    Object_t* Objects;
    Object_t** ObjectsById;
    id_t object_count;
    id_t object_capacity;
    id_t max_id;
    id_t id_capacity;
    id_t player_id;
} GameData_t;

typedef void (*fnp)(); // function pointer

typedef struct {
    fnp init;
    fnp input;
    fnp update;
    fnp draw;
    fnp exit;
    flags_t flags;
} State_t;

typedef struct {
    Vec2 pos;
    Vec2 vel;
    int8_t life;
    uint8_t color;
} Particle_t;

#endif/* STRUCTS_H */
