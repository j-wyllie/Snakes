/**********************************************************************
    ENCE260 Assignment

    Joshua, WYLLIE - jwy31
    Man On, LAI    - mla134
**********************************************************************/

#include "player.h"

static int controlPlayer;

tron_lightbike_t player_1, player_2;


// returns the control player
tron_lightbike_t* get_control_player(void)
{
    if (controlPlayer == 1) {
        return &player_1;
    } else {
        return &player_2;
    }
}

// returns the listen player
tron_lightbike_t* get_listen_player(void)
{
    if (controlPlayer == 1) {
        return &player_2;
    } else {
        return &player_1;
    }
}

// sets the control player
void set_control_player(int num)
{
    controlPlayer = num;
}


// initalises the players
void players_init()
{
  // game init p1
    direction_t dir_p1 = UP;
    position_t pos_p1;
    pos_p1.x = 1;
    pos_p1.y = 0;
    tron_init(&player_1, dir_p1, pos_p1, 4);

    // game init p2
    direction_t dir_p2 = DOWN;
    position_t pos_p2;
    pos_p2.x = 3;
    pos_p2.y = 6;
    tron_init(&player_2, dir_p2, pos_p2, 4);
}
