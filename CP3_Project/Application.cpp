#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "Application.h"
#include "Converter.h"
#include "GameHolder.h"
#include "Sprite.h"

#include "NeuralNetwork.h"

namespace Application
{
	MainApplication::MainApplication()
	{
		
	}

	int MainApplication::Run()
	{
		return 0;
	}

	void MainApplication::Test()
	{
		using namespace Display;
		
		using namespace Game;
		GameHolder* game = new GameHolder();

		ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
		const double tickTime = 1.0 / SingleDataKeeper::Instance()->GetInt("ticksPerSecond");
		ALLEGRO_TIMER *timer = al_create_timer(tickTime); // 60 - FPS

		al_register_event_source(event_queue, al_get_display_event_source(game->Display));
		al_register_event_source(event_queue, al_get_keyboard_event_source());
		al_register_event_source(event_queue, al_get_timer_event_source(timer));

		al_start_timer(timer);

		Keys keyDown = Keys::NotAKey;
		ALLEGRO_EVENT ev;

		while (game->Running()) // GAMELOOP
		{
			al_wait_for_event(event_queue, &ev);

			if (ev.type == ALLEGRO_EVENT_KEY_DOWN) //if button is CLICKED
				keyDown = Converter::AllegroKeyToAscii(ev.keyboard.keycode);
			else if (ev.type == ALLEGRO_EVENT_KEY_UP) //if button is UNCLICKED
				keyDown = Keys::NotAKey;

			if (keyDown != Keys::NotAKey)
			{
				game->Catched(keyDown);
				if (keyDown == Keys::Escape)
					keyDown = Keys::NotAKey;
			}

			if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) //close the application after clicking X on top-right corner of ALLEGRO window
				game->Exit();

			else if (ev.type == ALLEGRO_EVENT_TIMER)
				game->FrameElapsed(&ParametrizedObject<MainApplication>(*this), &ParametrizedEventArgs<int>(tickTime));

			//Render
			if (al_is_event_queue_empty(event_queue))
			{
				al_flip_display();
				al_clear_to_color(al_map_rgb(0, 0, 0));
			}

			al_wait_for_event(event_queue, &ev);
			al_flush_event_queue(event_queue);
		}
		
		al_destroy_event_queue(event_queue);
		al_destroy_timer(timer);
	
		delete game;
	}
}