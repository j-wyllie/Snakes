/**********************************************************************
    ENCE260 Assignment

    Joshua, WYLLIE - jwy31
    Man On, LAI    - mla134
**********************************************************************/

#include "player.h"

#define P1_INIT_DIR UP
#define P1_INIT_X 1
#define P1_INIT_Y 0

#define P2_INIT_DIR DOWN
#define P2_INIT_X 3
#define P2_INIT_Y 6


static int control_player;

tron_lightbike_t player_1, player_2;

// returns the control player
tron_lightbike_t* get_control_player(void)
{
    if (control_player == 1) {
        return &player_1;
    } else {
        return &player_2;
    }
}

// returns the listen player
tron_lightbike_t* get_listen_player(void)
{
    if (control_player == 1) {
        return &player_2;
    } else {
        return &player_1;
    }
}

// sets the control player
void set_control_player(int num)
{
    control_player = num;
}


// initalises the players
void players_init()
{
    // game init p1
    direction_t dir_p1 = P1_INIT_DIR;
    position_t pos_p1;
    pos_p1.x = P1_INIT_X;
    pos_p1.y = P1_INIT_Y;
    tron_init(&player_1, dir_p1, pos_p1);

    // game init p2
    direction_t dir_p2 = P2_INIT_DIR;
    position_t pos_p2;
    pos_p2.x = P2_INIT_X;
    pos_p2.y = P2_INIT_Y;
    tron_init(&player_2, dir_p2, pos_p2);
}
