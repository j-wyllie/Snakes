#include "system.h"
#include "tinygl.h"
#include "task.h"
#include "navswitch.h"
#include "led.h"
#include "ir_uart.h"
#include "pacer.h"
#include "../fonts/font3x5_1.h"
#include "tron.h"

// game properties

static tron_lightbike_t player_1, player_2;
static int controlPlayer;

static state_t state = STATE_PLAYING;
static tinygl_pixel_value_t display_ct[TINYGL_WIDTH][TINYGL_HEIGHT] = {{0}};
static tinygl_pixel_value_t display_ls[TINYGL_WIDTH][TINYGL_HEIGHT] = {{0}};

// enums

enum {DISPLAY_UPDATE_RATE = 500};
enum {FLASHER_UPDATE_RATE = 500};
enum {BUTTON_POLL_RATE = 100};
enum {GAME_UPDATE_RATE = 100};

typedef enum {STATE_DUMMY, STATE_INIT, STATE_START,
              STATE_PLAYING, STATE_OVER,
              STATE_READY
             } state_t;

// Setters and Getters

static void display_set_pix(tinygl_pixel_value_t display[][], position_t pos, uint8_t value)
{
    display[pos.x][6 - pos.y] = value;
}

static tron_lightbike_t get_control_player(void)
{
    if (controlPlayer == 1) {
        return player_1;
    }
    
    return player_2;
    
}

static tron_lightbike_t get_listen_player(void)
{
    if (controlPlayer == 1) {
        return player_2;
    }
    
    return player_1;
}

// game functions

static void game_init(void);
static void display_task(void);
static void navswitch_task(void);
static void game_task(void);

static void game_init(void)
{
    // display_init
    tinygl_init (DISPLAY_UPDATE_RATE);
    tinygl_font_set (&font3x5_1);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
    tinygl_text_speed_set (10);

    // navswitch init
    navswitch_init ();

    // Initialise IR driver
    ir_uart_init();

    // game init p1
    direction_t dir_p1 = UP;
    position_t pos_p1;
    pos_p1.x = 1;
    pos_p1.y = 0;
    tron_init(player_1, dir_p1, pos_p1, 4);

    // game init p2
    direction_t dir_p2 = DOWN;
    position_t pos_p2;
    pos_p2.x = 3;
    pos_p2.y = 6;
    tron_init(player_2, dir_p2, pos_p2, 4);

}


static void choose_player(void)
{

    pacer_init(500);
    // display instrution
    char str[] = "Choose Player";
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text(str);


    while(1) {
        pacer_wait();
        tinygl_update ();
        navswitch_update ();
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            break;
        }
    }
    tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);

    // choose player
    char one[2] = {'1','\0'};
    char two[2] = {'2','\0'};
    char selected = '1';

    tinygl_text(one);

    while(1) {
        pacer_wait ();
        tinygl_update ();
        navswitch_update ();

        if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
            tinygl_text(one);
            selected = one[0];
        }
        if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
            tinygl_text(two);
            selected = two[0];
        }
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            if (selected == '1') {
                ir_uart_putc ('2');
            }
            if (selected == '2') {
                ir_uart_putc ('1');
            }
            break;
        }

        if (ir_uart_read_ready_p ()) {
            selected = ir_uart_getc ();
            if (selected == '1' || selected == '2') {
                break;
            }
        }
    }

    controlPlayer = selected - '0';
    tinygl_clear ();
}




static void display_wipe(void)
{
    // controlling lightbike

    int i, j;
    for (i = 0; i < TINYGL_WIDTH; i++) {
        for (j = 0; j < TINYGL_HEIGHT; j++) {
            display_ct[i][j] = 0;
        }
    }

    // listening lightbike

    int k, l;
    for (k = 0; i < TINYGL_WIDTH; k++) {
        for (l = 0; l < TINYGL_HEIGHT; l++) {
            display_ls[k][l] = 0;
        }
    }
}

static void display_task()
{
    if (state == STATE_PLAYING) {
        // clear the screen
        display_wipe();
        tinygl_clear();

        // draw controlling lightbike
        int i = 0;
        while (get_control_player().snake[i].value != 111) {
            display_set_pix(display_ct, get_control_player().snake[i].pos, get_control_player().snake[i].value);
            i++;
        }
        // Update controlling display
        
        int j;
        for (j = 0; j < TINYGL_HEIGHT; j++) {
            for (i = 0; i < TINYGL_WIDTH; i++) {
                tinygl_point_t point = {i, j};
                tinygl_draw_point (point, display_ct[i][j]);
            }
        }


        // draw listening lightbike
        int k = 0;
        while (get_listen_player().snake[k].value != 111) {
            display_set_pix(display_ls, get_listen_player_player().snake[i].pos, get_listen_player_player().snake[k].value);
            k++;
        }
        // Update listening display
        
        int l;

        for (k = 0; k < TINYGL_HEIGHT; k++) {
            for (l = 0; l < TINYGL_WIDTH; l++) {
                tinygl_point_t point = {k, l};
                tinygl_draw_point (point, display_ls[k][l]);
            }
        }
    }

    tinygl_update ();
}

static void navswitch_task()
{
    navswitch_update ();

    if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
        switch (state) {
        case STATE_READY:
            break;

        case STATE_PLAYING:
            tron_set_lightbike_dir(get_control_player(), UP);
            ir_uart_putc('U');
            break;

        default:
            break;
        }
    }

    if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
        switch (state) {
        case STATE_READY:
            break;

        case STATE_PLAYING:
            tron_set_lightbike_dir(get_control_player(), DOWN);
            ir_uart_putc('D');
            break;

        default:
            break;
        }
    }

    if (navswitch_push_event_p (NAVSWITCH_EAST)) {
        switch (state) {
        case STATE_READY:
            break;

        case STATE_PLAYING:
            tron_set_lightbike_dir(get_control_player(), RIGHT);
            ir_uart_putc('R');
            break;

        default:
            break;
        }
    }

    if (navswitch_push_event_p (NAVSWITCH_WEST)) {
        switch (state) {
        case STATE_READY:
            break;

        case STATE_PLAYING:
            tron_set_lightbike_dir(get_control_player(), LEFT);
            ir_uart_putc('L');
            break;

        default:
            break;
        }
    }


    if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
        switch (state) {
        case STATE_READY:
            state = STATE_START;
            break;

        case STATE_PLAYING:
            break;

        default:
            break;
        }
    }
}



static void receive_task()
{
    if (ir_uart_read_ready_p ()) {
        char respone = ir_uart_getc ();

        switch (respone) {
        case 'L':
            tron_set_lightbike_dir(get_listen_player(), LEFT);

        case 'R':
            tron_set_lightbike_dir(get_listen_player(), RIGHT);;

        case 'U':
            tron_set_lightbike_dir(get_listen_player(), UP);

        case 'D':
            tron_set_lightbike_dir(get_listen_player(), DOWN);
        }

    }

}

static void game_task()
{
    switch (state) {
    case STATE_PLAYING:

        tron_update(get_control_player());
        tron_update(get_listen_player());


        /*
            if (! spacey_update ())
            {
                game_over_display (message);
                game_over_ticks = 0;
                led_set (LED1, 1);
                state = STATE_OVER;
            }
            */
        break;

    case STATE_INIT:
        break;

    case STATE_OVER:

    case STATE_READY:

    case STATE_START:
        state = STATE_PLAYING;
        break;
    }
}

int main(void)
{
    game_init();

    choose_player();

    task_t tasks[] = {
        {
            .func = display_task,
            .period = TASK_RATE / DISPLAY_UPDATE_RATE,
            .data = 0
        },
        {
            .func = navswitch_task,
            .period = TASK_RATE / BUTTON_POLL_RATE,
            .data = 0
        },
        {
            .func = receive_task,
            .period = TASK_RATE / GAME_UPDATE_RATE,
            .data = 0
        },
        {
            .func = game_task,
            .period = TASK_RATE / GAME_UPDATE_RATE,
            .data = 0
        },
    };


    system_init ();

    task_schedule (tasks, ARRAY_SIZE (tasks));
    return 0;
}
