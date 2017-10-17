#include "tron.h"



#define LIGHTBIKE_INC 1
#define LIGHTBIKE_MOVE_PERIOD 200


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
            if (player->position.y < TINYGL_HEIGHT - 1) {
                player->position.y += LIGHTBIKE_INC;
            } else {
                player->position.y = 0;
            }
            break;
        case DOWN:
            if (player->position.y > 0) {
                player->position.y -= LIGHTBIKE_INC;
            } else {
                player->position.y = TINYGL_HEIGHT - 1;
            }
            break;
        case LEFT:
            if (player->position.x > 0) {
                player->position.x -= LIGHTBIKE_INC;
            } else {
                player->position.x = TINYGL_WIDTH - 1;
            }
            break;
        case RIGHT:
            if (player->position.x < TINYGL_WIDTH - 1) {
                player->position.x += LIGHTBIKE_INC;
            } else {
                player->position.x = 0;
            }
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

which_bike_t tron_collision(tron_lightbike_t* control, tron_lightbike_t* listner)
{
    if (control->position.x == listner->position.x && control->position.y == listner->position.y){
        return BOTH;
    }
    int i;
    for (i = 1; i < 4 ;i++) {
        if ( control->position.x == listner->snake[i].pos.x && control->position.y == listner->snake[i].pos.y){
            return CONTROLER;
        }
    }
    for (i = 1; i < 4; i++){
        if (listner->position.x == control->snake[i].pos.x && listner->position.y == control->snake[i].pos.y){
            return LISTNER;
        }
    }

    // check if the position of snake head
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
