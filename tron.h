#ifndef TRON_H
#define TRON_H

#include "system.h"
#include "tinygl.h"

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
    direction_t direction;
    position_t position;
} tron_lightbike_t;

typedef struct
{
    uint16_t move_period;
    uint16_t move_clock;
} tron_timer;

void tron_init(uint8_t[TINYGL_WIDTH][TINYGL_HEIGHT], direction_t, position_t);

void tron_set_pix(position_t);

void tron_set_lightbike_dir(direction_t);

uint8_t tron_update(void);

#endif
