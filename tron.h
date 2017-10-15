#ifndef TRON_H
#define TRON_H

#include "system.h"
#include "tinygl.h"

#define LIGHTBIKE_MAX_LEN 8

typedef enum
{
    UP, DOWN, LEFT, RIGHT
} direction_t;

typedef struct
{
    uint8_t x;
    uint8_t y;
} position_t;

typedef struct
{
    position_t pos;
    uint8_t value;
} pixel_t;

typedef struct
{
    direction_t direction;
    position_t position;            // position of the snake's head
    pixel_t snake[8];
} tron_lightbike_t;

typedef struct
{
    uint16_t move_period;
    uint16_t move_clock;
} tron_timer;

void tron_init(tron_lightbike_t*, direction_t, position_t, uint8_t);

void tron_set_lightbike_dir(tron_lightbike_t*, direction_t);

void tron_move_lightbike(tron_lightbike_t*);

uint8_t tron_update(tron_lightbike_t*);

#endif
