/**********************************************************************
    ENCE260 Assignment

    Joshua, WYLLIE - jwy31
    Man On, LAI    - mla134
**********************************************************************/

#ifndef PLAYER_H
#define PLAYER_H

#include "tron.h"


tron_lightbike_t* get_control_player(void);

tron_lightbike_t* get_listen_player(void);

void set_control_player(int);

void players_init(void);

#endif
