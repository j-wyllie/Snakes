#include "tron.h"

/* HIHI */

#define LIGHTBIKE_INC 1
#define LIGHTBIKE_MOVE_PERIOD 150

static tron_lightbike_t lightbike;
static tron_timer timer;

void tron_init(direction_t dir, position_t pos, uint8_t snake_length)
{
    lightbike.direction = dir;
    lightbike.position = pos;

    lightbike.snake[0].pos = pos;
    lightbike.snake[0].value = 1;
    int i;
    for (i = 1; i < 8; i++) {
        if (i < snake_length) {
            lightbike.snake[i].pos.x = 111;
            lightbike.snake[i].pos.y = 111;
            lightbike.snake[i].value = 1;
        } else {
            lightbike.snake[i].pos.x = 111;
            lightbike.snake[i].pos.y = 111;
            lightbike.snake[i].value = 111;
        }
    }

    timer.move_clock = 0;
    timer.move_period = LIGHTBIKE_MOVE_PERIOD;
}

void tron_set_lightbike_dir(direction_t d)
{
        lightbike.direction = d;
}

tron_lightbike_t tron_get_lightbike()
{
    return lightbike;
}

void tron_move_lightbike()
{
    // update head position
    switch (lightbike.direction)
    {
        case UP:
            lightbike.position.y += LIGHTBIKE_INC;
            break;
        case DOWN:
            lightbike.position.y -= LIGHTBIKE_INC;
            break;
        case LEFT:
            lightbike.position.x -= LIGHTBIKE_INC;
            break;
        case RIGHT:
            lightbike.position.x += LIGHTBIKE_INC;
            break;
    }

    // update snake positions
    position_t temp = lightbike.snake[0].pos;
    lightbike.snake[0].pos = lightbike.position;

    position_t temp1 = lightbike.snake[1].pos;
    lightbike.snake[1].pos = temp;

    temp = lightbike.snake[2].pos;
    lightbike.snake[2].pos = temp1;

    temp1 = lightbike.snake[3].pos;
    lightbike.snake[3].pos = temp;

    /*
    position_t temp1 = lightbike.position;
    position_t temp2;
    int i = 0;
    while (lightbike.snake[i].value != 111) {
        temp2 = lightbike.snake[i].pos;
        lightbike.snake[i].pos = temp1;
        temp1 = temp2;
    } */
    /*
    pixel_t previous;
    pixel_t current;
    current.pos = lightbike.position;
    int i = 0;
    while (lightbike.snake[i].value != 111) {
        previous = current;
        current = lightbike.snake[i];
        lightbike.snake[i] = previous;
        lightbike.snake[i].value = 1;
    }
    */
}

uint8_t tron_update()
{
    timer.move_clock++;
    if (timer.move_clock < timer.move_period) {
        return 0;
    } else {
        timer.move_clock = 0;
        tron_move_lightbike();
    }
    return 0;
}
