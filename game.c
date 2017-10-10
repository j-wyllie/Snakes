#include "system.h"
#include "tinygl.h"
#include "task.h"
#include "navswitch.h"
#include "led.h"
#include "../fonts/font3x5_1.h"
#include "tron.h"

//tinygl_pixel_value_t

enum {DISPLAY_UPDATE_RATE = 500};
enum {FLASHER_UPDATE_RATE = 500};
enum {BUTTON_POLL_RATE = 100};
enum {GAME_UPDATE_RATE = 100};

typedef enum {STATE_DUMMY, STATE_INIT, STATE_START,
              STATE_PLAYING, STATE_OVER,
              STATE_READY} state_t;

static state_t state = STATE_PLAYING;
static uint8_t display[TINYGL_WIDTH][TINYGL_HEIGHT] = {{0}};


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

    // game init
    direction_t d = UP;
    position_t p;
    p.x = 2;
    p.y = 0;
    tron_init(display, d, p);
}

static void display_task(void *data)
{
    int i, j;
    if (state == STATE_PLAYING) {
        /* Update display.  */
        for (j = 0; j < TINYGL_HEIGHT; j++)
            for (i = 0; i < TINYGL_WIDTH; i++)
            {
                tinygl_point_t point = {i, j};
                tinygl_draw_point (point, display[i][j]);
            }
    }

    tinygl_update ();
}

static void navswitch_task(void *data)
{
    navswitch_update ();

    if (navswitch_push_event_p (NAVSWITCH_NORTH))
    {
        switch (state)
        {
        case STATE_READY:
            break;

        case STATE_PLAYING:
            tron_set_lightbike_dir(UP);
            break;

        default:
            break;
        }
    }

    if (navswitch_push_event_p (NAVSWITCH_SOUTH))
    {
        switch (state)
        {
        case STATE_READY:
            break;

        case STATE_PLAYING:
            tron_set_lightbike_dir(DOWN);
            break;

        default:
            break;
        }
    }

    if (navswitch_push_event_p (NAVSWITCH_EAST))
    {
        switch (state)
        {
        case STATE_READY:
            break;

        case STATE_PLAYING:
            tron_set_lightbike_dir(RIGHT);
            break;

        default:
            break;
        }
    }

    if (navswitch_push_event_p (NAVSWITCH_WEST))
    {
        switch (state)
        {
        case STATE_READY:
            break;

        case STATE_PLAYING:
            tron_set_lightbike_dir(LEFT);
            break;

        default:
            break;
        }
    }


    if (navswitch_push_event_p (NAVSWITCH_PUSH))
    {
        switch (state)
        {
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

static void game_task(void *data)
{
    switch (state)
    {
    case STATE_PLAYING:
        tron_update();
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

int main (void)
{
    game_init();

    task_t tasks[] =
    {
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
            .func = game_task,
            .period = TASK_RATE / GAME_UPDATE_RATE,
            .data = 0
        },
    };


    system_init ();

    task_schedule (tasks, ARRAY_SIZE (tasks));
    return 0;
}
