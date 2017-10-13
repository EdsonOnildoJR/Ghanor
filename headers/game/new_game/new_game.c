int new_game()
{
    int redraw = 1;

    EVENT event_video;

    COLOR color = al_map_rgb(200, 200, 200);

    FONT *h1 = load_ttf_font("fonts/gtek-technology.ttf", 30, 0);
    FONT *font = load_ttf_font("fonts/gtek-technology.ttf", 20, 0);
    FONT *roboto = load_ttf_font("fonts/Roboto-Regular.ttf", 12, 0);

    VIDEO *video = al_open_video("videos/video.ogv");
    TIMER *timer = al_create_timer(1.0 / FPS);

    QUEUE *queue_video = create_event_queue();
    al_register_event_source(queue_video, al_get_video_event_source(video));
    al_register_event_source(queue_video, al_get_timer_event_source(timer));

    QUEUE *queue_mouse = create_event_queue();
    al_register_event_source(queue_mouse, al_get_mouse_event_source());

    QUEUE *queue_keyboard = create_event_queue();
    al_register_event_source(queue_keyboard, al_get_keyboard_event_source());

    al_start_video(video, al_get_default_mixer());
    al_start_timer(timer);

    while (1)
    {
        if (redraw && al_event_queue_is_empty(queue_video))
        {
            video_display(video);

            al_draw_filled_rectangle(0, 0, DISPLAY_W, DISPLAY_H, al_map_rgba(0, 0, 0, 100));

            // HUMANOS

            al_draw_filled_rectangle(DISPLAY_W / 6, DISPLAY_H / 6, DISPLAY_W / 2.1, DISPLAY_H - (DISPLAY_H / 6), al_map_rgba(0, 0, 0, 200));
            al_draw_filled_rectangle((DISPLAY_W / 6) - 5, (DISPLAY_H / 6) - 5, (DISPLAY_W / 2.1) + 5, (DISPLAY_H - (DISPLAY_H / 6)) + 5, al_map_rgba(0, 0, 0, 50));
            al_draw_filled_rectangle((DISPLAY_W / 6) - 3, (DISPLAY_H / 6) - 3, (DISPLAY_W / 2.1) + 3, (DISPLAY_H - (DISPLAY_H / 6)) + 3, al_map_rgba(0, 0, 0, 100));
            al_draw_filled_rectangle((DISPLAY_W / 6) - 1, (DISPLAY_H / 6) - 1, (DISPLAY_W / 2.1) + 1, (DISPLAY_H - (DISPLAY_H / 6)) + 1, al_map_rgba(50, 0, 0, 150));
            
            al_draw_text(font, color, DISPLAY_W / 3.1, (DISPLAY_H / 6) + 50, ALIGN_CENTER, "humanos");
            al_draw_text(roboto, color, DISPLAY_W / 3.1, (DISPLAY_H / 6) + 100, ALIGN_CENTER, "JUNTE-SE AOS HUMANOS E IMPESSA O REINADO DOS SINTOZOIDES");

            // SINTOZOIDES

            al_draw_filled_rectangle(DISPLAY_W / 1.9, DISPLAY_H / 6, DISPLAY_W - (DISPLAY_W / 6), DISPLAY_H - (DISPLAY_H / 6), al_map_rgba(0, 0, 0, 200));
            al_draw_filled_rectangle((DISPLAY_W / 1.9) - 5, (DISPLAY_H / 6) - 5, (DISPLAY_W - (DISPLAY_W / 6)) + 5, (DISPLAY_H - (DISPLAY_H / 6)) + 5, al_map_rgba(0, 0, 0, 50));
            al_draw_filled_rectangle((DISPLAY_W / 1.9) - 3, (DISPLAY_H / 6) - 3, (DISPLAY_W - (DISPLAY_W / 6)) + 3, (DISPLAY_H - (DISPLAY_H / 6)) + 3, al_map_rgba(0, 0, 0, 100));
            al_draw_filled_rectangle((DISPLAY_W / 1.9) - 1, (DISPLAY_H / 6) - 1, (DISPLAY_W - (DISPLAY_W / 6)) + 1, (DISPLAY_H - (DISPLAY_H / 6)) + 1, al_map_rgba(0, 0, 50, 150));

            al_draw_text(font, color, DISPLAY_W - (DISPLAY_W / 3.1), (DISPLAY_H / 6) + 50, ALIGN_CENTER, "sintozoides");

            // EVENTS

            while (!al_event_queue_is_empty(queue_keyboard))
            {
                EVENT ev;

                al_wait_for_event(queue_keyboard, &ev);

                if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
                    goto done;
            }

            while (!al_event_queue_is_empty(queue_mouse))
            {
                EVENT ev;

                al_wait_for_event(queue_mouse, &ev);
            }

            // UPDATE DISPLAY

            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));

            redraw = false;
        }

        al_wait_for_event(queue_video, &event_video);

        if (event_video.type == ALLEGRO_EVENT_TIMER)
            redraw = true;
    }

done:

    al_destroy_font(h1);
    al_destroy_font(font);
    al_destroy_font(roboto);
    al_close_video(video);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue_video);
    al_destroy_event_queue(queue_keyboard);

    return 0;
}