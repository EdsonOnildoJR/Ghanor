#include "menu.h"

int menu()
{
    int opt;
    int redraw = 1;

    EVENT event_video;

    FONT *h1 = load_ttf_font("fonts/gtek-technology.ttf", 30, 0);
    FONT *font = load_ttf_font("fonts/gtek-technology.ttf", 20, 0);
    VIDEO *video = al_open_video("videos/video.ogv");
    TIMER *timer = al_create_timer(1.0 / FPS);

    QUEUE *queue_video = create_event_queue();
    al_register_event_source(queue_video, al_get_video_event_source(video));
    al_register_event_source(queue_video, al_get_timer_event_source(timer));

    QUEUE *queue_hover = create_event_queue();
    al_register_event_source(queue_hover, al_get_mouse_event_source());
    al_register_event_source(queue_hover, al_get_keyboard_event_source());

    QUEUE *queue_click = create_event_queue();
    al_register_event_source(queue_click, al_get_mouse_event_source());
    al_register_event_source(queue_click, al_get_keyboard_event_source());

    al_start_video(video, al_get_default_mixer());
    al_start_timer(timer);

    int fade = 0;

    while (1)
    {
        if (redraw && al_event_queue_is_empty(queue_video))
        {
            video_display(video);

            if (!fade)
            {
                draw_menu(h1, font);
                //fade = 1;
            }
            menu_mouse_hover(queue_hover, font);

            if ((opt = menu_mouse_click(queue_click)))
                goto done;

            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));

            redraw = false;
        }

        al_wait_for_event(queue_video, &event_video);

        if (event_video.type == ALLEGRO_EVENT_TIMER)
            redraw = true;
    }

done:

    set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);

    al_destroy_font(h1);
    al_destroy_font(font);
    al_close_video(video);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue_hover);
    al_destroy_event_queue(queue_click);
    al_destroy_event_queue(queue_video);

    return opt;
}

void draw_menu(FONT *h1, FONT *font)
{
    COLOR color = al_map_rgb(153, 153, 153);

    al_draw_filled_rectangle(0, 0, DISPLAY_W, DISPLAY_H, al_map_rgba(0, 0, 0, 100));

    al_draw_filled_rectangle(DISPLAY_W / 4, DISPLAY_H / 4, DISPLAY_W - (DISPLAY_W / 4), DISPLAY_H - (DISPLAY_H / 4), al_map_rgba(0, 0, 0, 200));
    al_draw_filled_rectangle((DISPLAY_W / 4) - 1, (DISPLAY_H / 4) - 1, (DISPLAY_W - (DISPLAY_W / 4)) + 1, (DISPLAY_H - (DISPLAY_H / 4)) + 1, al_map_rgba(0, 0, 0, 150));
    al_draw_filled_rectangle((DISPLAY_W / 4) - 3, (DISPLAY_H / 4) - 3, (DISPLAY_W - (DISPLAY_W / 4)) + 3, (DISPLAY_H - (DISPLAY_H / 4)) + 3, al_map_rgba(0, 0, 0, 100));
    al_draw_filled_rectangle((DISPLAY_W / 4) - 5, (DISPLAY_H / 4) - 5, (DISPLAY_W - (DISPLAY_W / 4)) + 5, (DISPLAY_H - (DISPLAY_H / 4)) + 5, al_map_rgba(0, 0, 0, 50));

    al_draw_text(h1, color, DISPLAY_W / 2, (DISPLAY_H / 2) - 110, ALIGN_CENTER, "without name");
    al_draw_text(font, color, DISPLAY_W / 2, (DISPLAY_H / 2) - 30, ALIGN_CENTER, "continuar");
    al_draw_text(font, color, DISPLAY_W / 2, (DISPLAY_H / 2) + 20, ALIGN_CENTER, "novo jogo");
    al_draw_text(font, color, DISPLAY_W / 2, (DISPLAY_H / 2) + 70, ALIGN_CENTER, "creditos");
    al_draw_text(font, color, DISPLAY_W / 2, (DISPLAY_H / 2) + 120, ALIGN_CENTER, "sair");
}

static int get_menu_mouse_opt(EVENT ev)
{
    if ((ev.mouse.x > (DISPLAY_W / 2) - 70 && ev.mouse.x < (DISPLAY_W / 2) + 70) && ev.mouse.y > (DISPLAY_H / 2) - 30 && ev.mouse.y < (DISPLAY_H / 2) - 10)
    {
        //*count = 1;
        return 1;
    }

    else if ((ev.mouse.x > (DISPLAY_W / 2) - 75 && ev.mouse.x < (DISPLAY_W / 2) + 75) && ev.mouse.y > (DISPLAY_H / 2) + 20 && ev.mouse.y < (DISPLAY_H / 2) + 40)
    {
        //*count = 2;
        return 2;
    }

    else if ((ev.mouse.x > (DISPLAY_W / 2) - 65 && ev.mouse.x < (DISPLAY_W / 2) + 65) && ev.mouse.y > (DISPLAY_H / 2) + 70 && ev.mouse.y < (DISPLAY_H / 2) + 90)
    {
        //*count = 3;
        return 3;
    }

    else if ((ev.mouse.x > (DISPLAY_W / 2) - 30 && ev.mouse.x < (DISPLAY_W / 2) + 30) && ev.mouse.y > (DISPLAY_H / 2) + 120 && ev.mouse.y < (DISPLAY_H / 2) + 140)
    {
        //*count = 4;
        return 4;
    }

    return 0;
}

static int get_menu_keyboard_opt(EVENT ev)
{
    static int opt = 1;

    if (opt == 5)
    {
        opt = 1;
        return 1;
    }

    if (opt == 0)
    {
        opt = 4;
        return 4;
    }

    switch (ev.keyboard.keycode)
    {
    case ALLEGRO_KEY_S:
    case ALLEGRO_KEY_D:
    case ALLEGRO_KEY_DOWN:
    case ALLEGRO_KEY_RIGHT:
        opt++;
        break;

    case ALLEGRO_KEY_W:
    case ALLEGRO_KEY_A:
    case ALLEGRO_KEY_UP:
    case ALLEGRO_KEY_LEFT:
        opt--;
        break;
    }

    return opt;
}

void menu_mouse_hover(QUEUE *queue, FONT *font)
{
    static int opt = 1;

    while (!al_is_event_queue_empty(queue))
    {
        EVENT ev;

        al_wait_for_event(queue, &ev);

        if (ev.type == EVENT_MOUSE_AXES)
            opt = get_menu_mouse_opt(ev);

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
            opt = get_menu_keyboard_opt(ev);
    }

    //printf("\n\n%d\n\n", opt);

    COLOR color = al_map_rgb(255, 255, 255);

    switch (opt)
    {
    case 1:
        set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
        al_draw_text(font, color, (DISPLAY_W / 2) + 1, (DISPLAY_H / 2) - 29, ALIGN_CENTER, "continuar");
        break;

    case 2:
        set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
        al_draw_text(font, color, (DISPLAY_W / 2) + 1, (DISPLAY_H / 2) + 21, ALIGN_CENTER, "novo jogo");
        break;

    case 3:
        set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
        al_draw_text(font, color, (DISPLAY_W / 2) + 1, (DISPLAY_H / 2) + 71, ALIGN_CENTER, "creditos");
        break;

    case 4:
        set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT);
        al_draw_text(font, color, (DISPLAY_W / 2) + 1, (DISPLAY_H / 2) + 121, ALIGN_CENTER, "sair");
        break;

    default:
        set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
    }
}

int menu_mouse_click(QUEUE *queue)
{
    while (!al_is_event_queue_empty(queue))
    {
        EVENT ev;

        al_wait_for_event(queue, &ev);

        if (ev.type == EVENT_MOUSE_BUTTON_UP)
            return get_menu_mouse_opt(ev);

        else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER || ev.keyboard.keycode == ALLEGRO_KEY_SPACE)
            return get_menu_keyboard_opt(ev);
    }

    return 0;
}

static void video_display(ALLEGRO_VIDEO *video)
{
    ALLEGRO_BITMAP *frame = al_get_video_frame(video);
    int w, h, x, y;

    if (!frame)
        return;

    w = DISPLAY_W + 300;
    h = DISPLAY_H + 300;

    x = (DISPLAY_W - w) / 2;
    y = (DISPLAY_H - h) / 2;

    al_draw_scaled_bitmap(frame, 0, 0,
                          al_get_bitmap_width(frame),
                          al_get_bitmap_height(frame), x, y, w, h, 0);
}