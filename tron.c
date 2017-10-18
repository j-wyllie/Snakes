/**********************************************************************
    ENCE260 Assignment

    Joshua, WYLLIE - jwy31
    Man On, LAI    - mla134
**********************************************************************/

#include "tron.h"


// initalises a bikes values
void tron_init(tron_lightbike_t* player, direction_t dir, position_t pos)
{
    player->direction = dir;
    player->position = pos;

    player->snake[0].pos = pos;
    player->snake[0].value = 1;
    int i;
    for (i = 1; i < LIGHTBIKE_SNAKE_LEN; i++) {
        player->snake[i].pos.x = LIGHTBIKE_DONT_DISPLAY;
        player->snake[i].pos.y = LIGHTBIKE_DONT_DISPLAY;
        player->snake[i].value = 1;
    }

    player->timer.move_clock = 0;
    player->timer.move_period = LIGHTBIKE_MOVE_PERIOD;
}

// sets lightbike direction
void tron_set_lightbike_dir(tron_lightbike_t* player, direction_t d)
{
    player->direction = d;
}
// moves the possition of the lightbike
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

// checks for a collison between lightbikes
which_bike_t tron_collision(tron_lightbike_t* control, tron_lightbike_t* listner)
{
    if (control->position.x == listner->position.x && control->position.y == listner->position.y){
        return BOTH;
    }
    int i;
    for (i = 1; i < LIGHTBIKE_SNAKE_LEN ;i++) {
        if ( control->position.x == listner->snake[i].pos.x && control->position.y == listner->snake[i].pos.y){
            return CONTROLER;
        }
    }
    for (i = 1; i < LIGHTBIKE_SNAKE_LEN; i++){
        if (listner->position.x == control->snake[i].pos.x && listner->position.y == control->snake[i].pos.y){
            return LISTNER;
        }
    }

    // check if the position of snake head
    return NEITHER;
}

// updates the lightbikes, checks if movements nessiary
void tron_update(tron_lightbike_t* player)
{
    player->timer.move_clock++;
    if (player->timer.move_clock >= player->timer.move_period) {
        player->timer.move_clock = 0;
        tron_move_lightbike(player);
    }

    player->last_direction = player->direction;
}
