#ifndef _APPLICATION_INITIALIZER
#define _APPLICATION_INITIALIZER

#include <atomic>
#include "Application.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#ifdef _DEBUG
#include <iostream>
#include <Windows.h>
#include "GameHolder.h"
#endif

namespace Application
{
#ifndef LOCK_VERSION_ONE
	std::atomic_flag MainApplication::Lock = ATOMIC_FLAG_INIT;
#else
	std::atomic<bool> MainApplication::Lock(false);
#endif

	MainApplication::MainApplication()
	{

	}

	int MainApplication::Run()
	{
		return 0;
	}

	void MainApplication::Test()
	{

		al_init();

		struct Sprite
		{
			int maxFrame;
			int curFrame;
			int curRow;
			int frameCount;
			int frameDelay;
			int frameWidth;
			int frameHeight;
			int animationColumns;

			ALLEGRO_BITMAP *image;
		};

		Sprite championImage;

		championImage.maxFrame = 6;
		championImage.curFrame = 1;
		championImage.curRow = 1;
		championImage.frameCount = 1;
		championImage.frameDelay = 2;
		championImage.frameWidth = 32;
		championImage.frameHeight = 32;
		championImage.animationColumns = 6;

		championImage.image = al_load_bitmap("champion.png");

		Sprite backgroundImage;

		backgroundImage.image = al_load_bitmap("background.png");

		Sprite mobImage;

		mobImage.maxFrame = 6;
		mobImage.curFrame = 1;
		mobImage.curRow = 1;
		mobImage.frameCount = 1;
		mobImage.frameDelay = 2;
		mobImage.frameWidth = 32;
		mobImage.frameHeight = 32;
		mobImage.animationColumns = 6;

		mobImage.image = al_load_bitmap("mob.png");

		//MENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENU
		int menuOption = 0;
		//MENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENU

		using namespace Game;
		GameHolder* game = new GameHolder();

		ALLEGRO_DISPLAY *display = al_create_display(800, 600);
		ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();;
		ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60); // 60 - FPS


		ALLEGRO_FONT *arial18 = al_load_ttf_font("Sources/data/arial.ttf", 30, 0);;


		al_install_keyboard();
		al_init_image_addon();
		al_init_primitives_addon();
		al_init_font_addon();
		al_init_ttf_addon();

		al_register_event_source(event_queue, al_get_display_event_source(display));
		al_register_event_source(event_queue, al_get_keyboard_event_source());
		al_register_event_source(event_queue, al_get_timer_event_source(timer));

		al_start_timer(timer);

		srand(time(NULL));

		while (game->Running()) // GAMELOOP
		{
			ALLEGRO_EVENT ev;
			al_wait_for_event(event_queue, &ev);

			if (ev.type == ALLEGRO_EVENT_KEY_DOWN) //if button is CLICKED
			{
				switch (ev.keyboard.keycode)
				{
				case ALLEGRO_KEY_ESCAPE: // if escape is catched
					game->Catched(Keys::Escape);
					game->Exit(); //Exits the game: is there point of catching it? Should go to pause menu? 
					break;
				case ALLEGRO_KEY_UP: //if up is catched
					game->Catched(Keys::Up); // potential change of line, may be omitted if only 1 lane is to be implemented
					break;
				case ALLEGRO_KEY_DOWN: //if down is catched
					game->Catched(Keys::Down); // potential change of line, may be omitted if only 1 lane is to be implemented
					break;
				case ALLEGRO_KEY_A: //if A is catched (move left starts)
					game->Catched(Keys::A); // if 
					break;
				case ALLEGRO_KEY_D: //if D is catched (move right starts)
					game->Catched(Keys::D);
					break;
				case ALLEGRO_KEY_SPACE: //if space is catched, attack key ?
					game->Catched(Keys::Space);
					break;
				case ALLEGRO_KEY_ENTER: //if enter is catched 
					game->Catched(Keys::Enter);
					break;
				}
			}
			else if (ev.type == ALLEGRO_EVENT_KEY_UP) //if button is UNCLICKED
			{
				switch (ev.keyboard.keycode)
				{
				case ALLEGRO_KEY_A: //if A (move left) is cancelled
					game->Catched(Keys::A);
					break;
				case ALLEGRO_KEY_D: //if D (move right) is cancelled
					game->Catched(Keys::D);
					break;
				}
			}


				if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) //close the application after clicking X on top-right corner of ALLEGRO window
				{
					//logic operation to close game
					game->Exit();
				}

			else if (ev.type == ALLEGRO_EVENT_TIMER)
			{
				//champion animation update
				if (++championImage.frameCount >= championImage.frameDelay)
				{
					if (++championImage.curFrame >= championImage.maxFrame)
						championImage.curFrame = 0;

					championImage.frameCount = 0;
				}

				//mob animation update
				if (++mobImage.frameCount >= mobImage.frameDelay)
				{
					if (++mobImage.curFrame >= mobImage.maxFrame)
						mobImage.curFrame = 0;

					mobImage.frameCount = 0;
				}
			}

			//Render
			if (al_is_event_queue_empty(event_queue))
			{
				//rendering background
				al_draw_bitmap(backgroundImage.image, 0, 0, 0);

				//rendering champion
				al_draw_bitmap_region(championImage.image, championImage.curFrame * championImage.frameWidth,
					championImage.curRow * championImage.frameHeight, championImage.frameWidth, championImage.frameHeight, champion->x, champion->y, 0);

				//HOW DOES IT WORK
				// al_draw_bitmap_region(image, currently displayed column, currently displayed row, frame width, frame height, X-pos of Champion, Y-pos of champion, 0


				//champion healthbar
				al_draw_textf(arial18, al_map_rgb(255, 255, 255), 20, 20, 0, "CHAMPIONS HP  %d", champion->hp);
				// font, color, x, y, 0 (flags), text


				//MENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENU
				//NOTE: You should have two different operations encoded while player uses W or S: in game, it should change lane, in menu it should change option (increment or decrement menuOption) 
				//NOTE: I have got no idea how to switch between game and pause menus. Maybe two allegro clocks initialized, if else on them - no clue...
				//NOTE: Continue should be unavailable until the game starts

				//Explaination of al_draw_text
				//al_draw_text(font, al_map_rgb(RGB color), x-pos, y-pos, flag, contain

				while (!menuIsDisplayed) //I have got no clue how to get it - it should return 0 when one chooses any option
				{
					if (menuOption != 0)
					{
						al_draw_text(arial18, al_map_rgb(255, 255, 255), 400, 400, ALLEGRO_ALIGN_CENTER, "CONTINUE");
					}
					else
					{
						al_draw_text(arial18, al_map_rgb(250, 130, 0), 400, 400, ALLEGRO_ALIGN_CENTER, "CONTINUE");
					}
					if (menuOption != 1)
					{
						al_draw_text(arial18, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTER, "NEW GAME");
					}
					else
					{
						al_draw_text(arial18, al_map_rgb(250, 130, 0), 400, 300, ALLEGRO_ALIGN_CENTER, "NEW GAME");
					}
					if (menuOption != 2)
					{
						al_draw_text(arial18, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTER, "QUIT");
					}
					else
					{
						al_draw_text(arial18, al_map_rgb(250, 130, 0), 400, 200, ALLEGRO_ALIGN_CENTER, "QUIT");
					}
				}

				//MENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENUMENU

				//mobs rendering
				for (int i = 0; mobs.size(); i++)
				{
					al_draw_bitmap_region(mobImage.image, mobImage.curFrame * mobImage.frameWidth,
						mobImage.curRow * mobImage.frameHeight, mobImage.frameWidth, mobImage.frameHeight, mob[i]->x, mob[i]->y, 0);
					
				//HOW DOES IT WORK
				// al_draw_bitmap_region(image, currently displayed column, currently displayed row, frame width, frame height, X-pos of Mob, Y-pos of Mob, 0

				//mobs healthbar
					al_draw_textf(arial18, al_map_rgb(255, 255, 255), mob[i]->x, mob[i]->y - 30, 0, "MOB HP %d", champion->hp);
				}



				al_flip_display();
				al_clear_to_color(al_map_rgb(0, 0, 0));
			}



			al_wait_for_event(event_queue, &ev);
			al_flush_event_queue(event_queue);
		}
		delete game;

		al_destroy_font(arial18)
		al_destroy_display(display);
		al_destroy_event_queue(event_queue);
		al_destroy_timer(timer);
	}
}

#endif