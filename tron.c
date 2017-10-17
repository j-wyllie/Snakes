#include "tron.h"



#define LIGHTBIKE_INC 1
#define LIGHTBIKE_MOVE_PERIOD 100


void tron_init(tron_lightbike_t* player, direction_t dir, position_t pos, uint8_t snake_length)
{
    player->direction = dir;
    player->position = pos;

    player->snake[0].pos = pos;
    player->snake[0].value = 1;
    int i;
    for (i = 1; i < 8; i++) {
        if (i < snake_length) {
            player->snake[i].pos.x = 111;
            player->snake[i].pos.y = 111;
            player->snake[i].value = 1;
        } else {
            player->snake[i].pos.x = 111;
            player->snake[i].pos.y = 111;
            player->snake[i].value = 111;
        }
    }

    player->timer.move_clock = 0;
    player->timer.move_period = LIGHTBIKE_MOVE_PERIOD;
}



void tron_set_lightbike_dir(tron_lightbike_t* player, direction_t d)
{
    player->direction = d;
}



void tron_move_lightbike(tron_lightbike_t* player)
{
    // update head position
    switch (player->direction)
    {
        case UP:
            player->position.y += LIGHTBIKE_INC;
            break;
        case DOWN:
            player->position.y -= LIGHTBIKE_INC;
            break;
        case LEFT:
            player->position.x -= LIGHTBIKE_INC;
            break;
        case RIGHT:
            player->position.x += LIGHTBIKE_INC;
            break;
    }

    // update snake positions
    position_t temp = player->snake[0].pos;
    player->snake[0].pos = player->position;

    position_t temp1 = player->snake[1].pos;
    player->snake[1].pos = temp;

    temp = player->snake[2].pos;
    player->snake[2].pos = temp1;

    temp1 = player->snake[3].pos;
    player->snake[3].pos = temp;
}

which_bike_t tron_collision()
{
    // TODO: check for colisiion;
    return NEITHER;
}

uint8_t tron_update(tron_lightbike_t* player)
{

    player->timer.move_clock++;
    if (player->timer.move_clock >= player->timer.move_period) {
        player->timer.move_clock = 0;
        tron_move_lightbike(player);
    }

    player->last_direction = player->direction;

    return 0;
}
