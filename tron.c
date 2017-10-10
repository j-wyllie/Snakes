#include "tron.h"


static tron_lightbike_t lightbike;
static tron_timer timer;
static tinygl_pixel_value_t** display;

void tron_init(uint8_t[TINYGL_WIDTH][TINYGL_HEIGHT] dis, direction_t dir, position_t pos)
{
    display = dis;
    lightbike.direction = dir;
    lightbike.position = pos;
}

void tron_set_pix(position_t p)
{
    display[p.x][p.y] = 1;
}

void tron_set_lightbike_dir(direction_t d)
{
        lightbike.direction = d;
}

uint8_t tron_update()
{
    timer.move_clock++;
    if (timer.move_clock < timer.move_period) {
        return;
    } else {
        timer.move_clock = 0;
    }
    lightbike.position.x += 1;
}
