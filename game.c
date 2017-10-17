/**********************************************************************
    ENCE260 Assignment

    Joshua, WYLLIE - jwy31
    Man On, LAI    - mla134
**********************************************************************/


#include "system.h"
#include "tinygl.h"
#include "task.h"
#include "navswitch.h"
#include "led.h"
#include "ir_uart.h"
#include "pacer.h"
#include "../fonts/font3x5_1.h"
#include "button.h"
#include "tron.h"
#include "player.h"

#define TEXT_SPEED 10
#define FLASH_RATE 5

/**********************************************************************
TODO:
*
**********************************************************************/

// game properties


enum {DISPLAY_UPDATE_RATE = 700};
enum {BUTTON_POLL_RATE = 200};
enum {GAME_UPDATE_RATE = 100};
enum {RECEIVE_RATE = 250};

typedef enum {STATE_INIT, STATE_START,
              STATE_PLAYING, STATE_OVER,
             } state_t;

static state_t state = STATE_INIT;
static tinygl_pixel_value_t display_buffer[TINYGL_WIDTH][TINYGL_HEIGHT] = {{0}};


// Setters and Getters

// set a pixel of the display buffer on
static void display_set_pix(position_t pos, uint8_t value)
{
    display_buffer[pos.x][6 - pos.y] = value;
}

// game functions

static void game_init(void);
static void display_task(void);
static void navswitch_task(void);
static void game_task(void);

// initalises game environment
static void game_init(void)
{
    // display_init
    tinygl_init (DISPLAY_UPDATE_RATE);
    tinygl_font_set (&font3x5_1);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
    tinygl_text_speed_set (TEXT_SPEED);

    // navswitch init
    navswitch_init();

    // initatise button
    button_init();

    // initialise IR driver
    ir_uart_init();

    // initalise players
    players_init();
}


// displays whether you won or lost
void display_over_message(who_lost)
{
    pacer_init(500);
    // display instrution
    char textWin[] = "WIN";
    char textLose[] = "LOST";

    switch(who_lost) {
    case BOTH:
        tinygl_text(textLose);
        break;
    case LISTNER:
        tinygl_text(textWin);
        break;
    case CONTROLER:
        tinygl_text(textLose);
        break;
    }
    while(1) {
        pacer_wait();
        tinygl_update();
        button_update();
        if (button_push_event_p(0)) {
            return;
        }
    }
}

// prints welcom message and decides listener and control player
static void choose_player(void)
{
    pacer_init(500);
    // display instrution
    char text[] = "WELCOME TO SNAKES";
    tinygl_text(text);

    while(1) {
        pacer_wait();
        tinygl_update ();
        navswitch_update ();

        static char c = '\0';
        if (ir_uart_read_ready_p()) {       // other player clicked first
            c = ir_uart_getc();
            if (c == '1') {
                set_control_player(2);
                break;
            }
        }

        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            ir_uart_putc ('1');             // you clicked first
            set_control_player(1);
            break;
        }
    }

    tinygl_clear();
}

// clears the display buffer
static void display_wipe(void)
{
    int i, j;
    for (i = 0; i < TINYGL_WIDTH; i++) {
        for (j = 0; j < TINYGL_HEIGHT; j++) {
            display_buffer[i][j] = 0;
        }
    }
}

// draws to and updates the display
static void display_task()
{
    if (state == STATE_PLAYING) {
        // clear the screen
        display_wipe();
        tinygl_clear();

        // draw controlling lightbike
        int i = 0;
        while (get_control_player()->snake[i].value != 111) {
            display_set_pix(get_control_player()->snake[i].pos, get_control_player()->snake[i].value);
            i++;
        }

        // draw listening lightbike
        static uint8_t dimmer;
        int j = 0;
        dimmer++;
        if (dimmer > FLASH_RATE) {          // dimm listning bike
            dimmer = 0;
            while (get_listen_player()->snake[j].value != 111) {
                display_set_pix(get_listen_player()->snake[j].pos, get_listen_player()->snake[j].value);
                j++;
            }
        }

        // Update display
        for (j = 0; j < TINYGL_HEIGHT; j++)
            for (i = 0; i < TINYGL_WIDTH; i++) {
                tinygl_point_t point = {i, j};
                tinygl_draw_point (point, display_buffer[i][j]);
            }
    }

    tinygl_update ();
}

// update and send player directions
static void navswitch_task()
{
    navswitch_update ();

    if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
        switch (state) {
        case STATE_PLAYING:
            if(get_control_player()->last_direction == DOWN) {
                break;
            }
            tron_set_lightbike_dir(get_control_player(), UP);
            ir_uart_putc('U');
            break;

        default:
            break;
        }
    }

    if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
        switch (state) {
        case STATE_PLAYING:
            if(get_control_player()->last_direction == UP) {
                break;
            }
            tron_set_lightbike_dir(get_control_player(), DOWN);
            ir_uart_putc('D');
            break;

        default:
            break;
        }
    }

    if (navswitch_push_event_p (NAVSWITCH_EAST)) {
        switch (state) {
        case STATE_PLAYING:
            if(get_control_player()->last_direction == LEFT) {
                break;
            }
            tron_set_lightbike_dir(get_control_player(), RIGHT);
            ir_uart_putc('R');
            break;

        default:
            break;
        }
    }

    if (navswitch_push_event_p (NAVSWITCH_WEST)) {
        switch (state) {
        case STATE_PLAYING:
            if(get_control_player()->last_direction == RIGHT) {
                break;
            }
            tron_set_lightbike_dir(get_control_player(), LEFT);
            ir_uart_putc('L');
            break;

        default:
            break;
        }
    }


    if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
        switch (state) {
        case STATE_PLAYING:
            break;

        default:
            break;
        }
    }
}

// receves and sets other players direction
static void receive_task()
{

    if (ir_uart_read_ready_p ()) {
        char respone = ir_uart_getc ();

        switch (respone) {
        case 'L':
            tron_set_lightbike_dir(get_listen_player(), LEFT);
            break;

        case 'R':
            tron_set_lightbike_dir(get_listen_player(), RIGHT);
            break;

        case 'U':
            tron_set_lightbike_dir(get_listen_player(), UP);
            break;

        case 'D':
            tron_set_lightbike_dir(get_listen_player(), DOWN);
            break;
        }

    }

}

// updates game logic depending on game state
static void game_task()
{
    static which_bike_t who_lost;

    switch (state) {
    case STATE_PLAYING:
        if ((who_lost = tron_collision(get_control_player(), get_listen_player())) == NEITHER) {
            tron_update(get_control_player());
            tron_update(get_listen_player());
        } else {
            state = STATE_OVER;
        }
        break;

    case STATE_INIT:
        game_init();
        state = STATE_START;
        break;

    case STATE_OVER:
        display_over_message(who_lost);
        state = STATE_INIT;
        break;

    case STATE_START:
        choose_player();
        state = STATE_PLAYING;
        break;
    }
}

// main
int main(void)
{
    system_init ();

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
            .period = TASK_RATE / RECEIVE_RATE,
            .data = 0
        },
        {
            .func = game_task,
            .period = TASK_RATE / GAME_UPDATE_RATE,
            .data = 0
        },
    };

    task_schedule (tasks, ARRAY_SIZE (tasks));
    return 0;
}
