/*
 *	erapr
 *     ERAPR Really Ain't Playable, Right?
 *
 *	Created by Remi Bernotavicius 
 *	Copyright (c) 2009 Remi Bernotavicius. All rights reserved.
 */

#include "main.h"

#define SOUND

#ifdef SOUND
	#include <aldumb.h>
#endif

#include <cstdlib>
#include <iostream>

#define screen_width 1066
#define screen_height 600
#define translation_factor 10
#define waitTime 40

#define DISPLAY_TYPE GFX_AUTODETECT_WINDOWED

long wait_inc = 0;

void timerUpdate() {
	wait_inc++;
}
END_OF_FUNCTION(timerUpdate);

void runGameWithMapAndPlayer(Map* mainMap, Player* p1) {
	
	//setup timer
	wait_inc = 0;
	install_timer();
	LOCK_VARIABLE(wait);
	LOCK_FUNCTION(timerUpdate);
	install_int_ex(timerUpdate, BPS_TO_TIMER(waitTime));
	
	rectangle screen_rect = rectangle(0, 0, screen_width, screen_height);
	
	//create buffer
	BITMAP* buffer = create_bitmap(screen_width, screen_height);
	
	//create layers
	BITMAP* map_front = create_bitmap(mainMap->worldWidth() * TILESIZE, mainMap->worldHeight() * TILESIZE);
	BITMAP* map_back = create_bitmap(mainMap->worldWidth() * TILESIZE, mainMap->worldHeight() * TILESIZE);
	
	//draw the map parts onto the layers
	mainMap->draw_all(map_front, FRONT);
	mainMap->draw_all(map_back, BACK);
	
	//setup player
	p1->bounds.position.set_location(mainMap->playerStart.x, mainMap->playerStart.y - 13);
	p1->frame = 0;
	
	//initialize variables
	bool space_key_gap = true;
	bool shift_key_gap = true;
	
	time_t startTime = time(NULL);
	
	#ifdef SOUND
		dumb_register_dat_it_quick(DUMB_DAT_IT);
		bool s_key_gap = true;
	#endif
	
	//load data file
	DATAFILE* data = load_datafile(MAP_DATA);
	
	//load the music
	#ifdef SOUND
		DUH* mainMusic = (DUH*)data[main_music].dat;
		AL_DUH_PLAYER *dp = al_start_duh(mainMusic, 2, 0, 1.0f, 4096, 48000);
		bool music_playing = true;
	#endif
	
	//load background images
	BITMAP* b_bottom = (BITMAP*)data[bg1_b].dat;
	BITMAP* b_top = (BITMAP*)data[bg1_t].dat;
	
	//setup world
	World* world = new World(mainMap, p1, screen_rect);
	
	//the main game loop
	while(p1->event == NO_EVENT){
		
		//without this, it makes the cpu soar to 100% usage
		rest(1);
		
		//do only if there are frames to draw
		if(wait_inc > 0) {
			
			//read keyboard
			p1->fast = key[KEY_LSHIFT];
			
			if(key[KEY_LEFT]){
				p1->move_left();
			}
			if(key[KEY_RIGHT]){
				p1->move_right();
			}
			if(key[KEY_DOWN]) {
				
			}
			
			#ifdef SOUND
				if(key[KEY_S])
				{
					if(s_key_gap)
					{
						if(music_playing)
							al_pause_duh(dp);
						else
							al_resume_duh(dp);
						
						music_playing = !music_playing;
						s_key_gap = false;
					}
				}
				else
				{
					s_key_gap = true;
				}
			#endif
			
			if(key[KEY_Z]) {
				if(shift_key_gap) {
					world->addProjectile();
					shift_key_gap = false;
				}
			} else {
				shift_key_gap = true;
			}

			if(key[KEY_SPACE]) {
				if (mainMap->onTheGround(p1->bounds) && space_key_gap) {
					p1->jump();
					space_key_gap = false;
				}
			} else {
				space_key_gap = true;
			}
			if(key[KEY_ESC]) {
				p1->event = DIE_EVENT;
			}
			
			
			//TODO: Fix Joystick Support
			//read joystick
			
			if(num_joysticks > 0) {
				
				poll_joystick();
	
				
				cout << "\n";

				if (joy[0].button[0].b) {
					p1->jump();
				}
			}
			
			
			//update world
			world->tick();
			
			//update music
			#ifdef SOUND
				al_poll_duh(dp);
			#endif
			
			//clears the screen
			clear_to_color(buffer, makecol(0.482 * 255, 0.776 * 255, 0.647 * 255));
			
			//draw background
			
			//bottom
			for (int i = -1 * ((p1->mapx / 2) % b_bottom->w); i < screen_width; i += b_bottom->w) {
				draw_sprite(buffer, b_bottom, i, -1 * p1->mapy + (mainMap->worldHeight() * TILESIZE) - b_bottom->h);
			}
			//top
			for (int i = -1 * ((p1->mapx / 2) % b_top->w); i < screen_width; i += b_top->w) {
				draw_sprite(buffer, b_top, i, -1 * p1->mapy);
			}
			
			//draw the map's back layer
			draw_sprite(buffer, map_back, -1 * p1->mapx, -1 * p1->mapy);
			
			//process the event
			int readEvent = mainMap->eventForRectangle(p1->bounds);
			if(readEvent != NO_EVENT)
				p1->event = readEvent;
			
			world->draw(buffer);
			
			//draw the map's front layer
			draw_sprite(buffer, map_front, -1 * p1->mapx, -1 * p1->mapy);
			
			//draw the stats
			p1->drawStats(buffer, 5, 5);
			time_t min = ((time(NULL) - startTime) / 60);
			time_t sec = ((time(NULL) - startTime) % 60);
			if(min >= 10)
				textprintf_ex(buffer, font, 5, 65, makecol(225, 225, 225), -1, "%ld:%ld", min, sec);
			else
				textprintf_ex(buffer, font, 5, 65, makecol(225, 225, 225), -1, "%ld:0%ld", min, sec);
			
			if(music_playing)
				textprintf_ex(buffer, font, 5, 75, makecol(225, 225, 225), -1, "sound on");
			else
				textprintf_ex(buffer, font, 5, 75, makecol(225, 225, 225), -1, "sound off");
			
			
			//draw the buffer to the screen
			blit(buffer, screen, 0, 0, 0, 0, screen_width, screen_height);
			
			
			//decrease frame debt
			wait_inc--;
		}
	}
	
	//stop the music
	#ifdef SOUND
		al_stop_duh(dp);
	#endif
	
	//game results screen
	if(p1->event == EXIT_EVENT) {
		clear_to_color(screen, makecol(0, 0, 0));
		clear_to_color(buffer, makecol(0, 0, 0));
		p1->bounds.position.set_location(50, 10);
		p1->draw(buffer, 0, 0);
		textprintf_ex(buffer, font, screen_width / 2 - 20, 10, makecol(0, 255, 0), 0, "LEVEL COMPLETE");
		textprintf_ex(buffer, font, screen_width / 2 - 20, screen_height * 2 / 3, makecol(255, 255, 255), 0, "-Press Enter-");
		
		draw_sprite(screen, buffer, 0, 0);

		
		while (!key[KEY_ENTER]) {
			//wait
		}
	}
	
	p1->event = NO_EVENT;
	
	//unload data
	unload_datafile(data);
	destroy_bitmap(buffer);
	destroy_bitmap(map_front);
	destroy_bitmap(map_back);
	
	//clear out the esc key
	while (key[KEY_ENTER]) {
		//do nothing
	}
}
END_OF_FUNCTION(runGameWithMapAndPlayer);


int main(int argc, const char *argv[])
{
	allegro_init();
	install_keyboard();
	
	//setup sound
	#ifdef SOUND
		atexit(&dumb_exit);
		install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL);
	#endif

	//install the joystick
	install_joystick(JOY_TYPE_AUTODETECT);
	
	//calibrate all the joysticks
	for (int i = 0; i < num_joysticks; i++) {
		calibrate_joystick_name(i);
		calibrate_joystick(i);
	}
	
	//Changed to windowed for now because a crash can cause problems in fullscreen
	//32 color depth for the menus
	set_color_depth(32);
	set_gfx_mode(DISPLAY_TYPE, screen_width, screen_height, 0, 0);
	
	//load datafile
	DATAFILE* data = load_datafile(MAIN_DATA);
	
	//SPLASH SCREEN
	BITMAP* splash = (BITMAP*)data[SPLASH].dat;
	draw_sprite(screen, splash, screen_width / 2 - (splash->w / 2), -35);
	BITMAP* loadedPlayer = (BITMAP*)data[PLAYER].dat;
	BITMAP* menu_player = create_bitmap(loadedPlayer->w / 4, loadedPlayer->h);
	blit(loadedPlayer, menu_player, 0, 0, 0, 0, loadedPlayer->w / 2, loadedPlayer->h);
	draw_sprite(screen, menu_player, screen_width / 2 - menu_player->w / 2, screen_height * 2 / 3);
	
	//wait for enter
	while(!key[KEY_ENTER]){}
	while(key[KEY_ENTER]){}
	
	//setup the menu
	typedef struct menuitem {
		const char *path;
		const char *_name;
	};
	
	/*Hard-coded menu*/
	menuitem menu[3];
	
	menu[0]._name = "Test Map";
	menu[0].path = MAP1;
	
	menu[1]._name = "Awesome Second Map";
	menu[1].path = MAP2;
	
	menu[2]._name = "Epic Third Map";
	menu[2].path = MAP3;
	/*Hard-coded menu end*/
	
	clear_to_color(screen, makecol(0, 0, 0));
	
	int selectedMenuItem = 0;
	const int rest = 100;
	int count = rest;
	
	BITMAP* buffer = create_bitmap(screen_width, screen_height);
	
	BITMAP* world1 = (BITMAP*)data[WORLD_1].dat;
	
	//run the menu
	while (!key[KEY_ESC]) {
		
		//clear_to_color(buffer, makecol(0, 0, 0));
		draw_sprite(buffer, world1, screen_width / 2 - (world1->w / 2), 0);
		
		if(count > 0)
			count--;
		if(key[KEY_ENTER]) { //run the selected map
			char temp[480] = "";
			for(int i = 0; i < 480; i++) {
				temp[i] = menu[selectedMenuItem].path[i];
			}
			//starting game...
			
			//Changed color depth to 8 for the game play
			set_color_depth(8);
			set_gfx_mode(DISPLAY_TYPE, screen_width, screen_height, 0, 0);
			
			//load datafile
			DATAFILE* map_data = load_datafile(MAP_DATA);
			
			//set pallete
			set_palette((RGB*)data[MAIN_PAL].dat);
			
			//pull out the tilesets for background and items
			TILESET* bgTS = new TILESET(18, 11, (BITMAP*)map_data[T2].dat);
			TILESET* spTS = new TILESET(18, 9, (BITMAP*)map_data[enemies].dat);
			spTS->tileWidth = SPRITE_W;
			spTS->tileHeight = SPRITE_H;
			
			TILESET* pTS = new TILESET(4, 0, (BITMAP*)map_data[player].dat);
			pTS->tileBuffer = 0;
			pTS->tileWidth = 32;
			pTS->tileHeight = 54;
			
			//create the player
			Player* plr = new Player(pTS, rectangle(0, 0, 32, 54));
			
			Map* map = new Map(temp, bgTS, spTS);
			runGameWithMapAndPlayer(map, plr);
			
			//32 color depth for the menus
			set_color_depth(32);
			set_gfx_mode(DISPLAY_TYPE, screen_width, screen_height, 0, 0);
			
			
		} else if (key[KEY_UP] && count <= 0) { //go up in the menu selection
			selectedMenuItem--;
			if (selectedMenuItem < 0) {
				selectedMenuItem = ARRAY_SIZE(menu) -1;
			}
			count = rest;
		} else if(key[KEY_DOWN] && count <= 0) { //go down in the menu selection
			selectedMenuItem++;
			if (selectedMenuItem >= ARRAY_SIZE(menu)) {
				selectedMenuItem = 0;
			}
			count = rest;
		}
		
		//draw the menu
		for (int i = 0; i < ARRAY_SIZE(menu); i++) {
			textout_centre_ex(buffer, font, menu[i]._name, screen_width / 2, screen_height / 2 + i * 10 - 50, makecol(255, 255, 255), -1);
			if(i == selectedMenuItem)
				draw_sprite(buffer, menu_player, screen_width / 2 - 105, screen_height / 2 + i * 10 - 70);
		}
		//textout_centre_ex(buffer, font, "Erapr", screen_width / 2, screen_height / 6, makecol(255, 255, 255), -1);
		//textout_centre_ex(buffer, font, "select a map", screen_width / 2, screen_height / 6 + 20, makecol(255, 255, 255), -1);
		
		draw_sprite(screen, buffer, 0, 0);
		//blit(buffer, screen, 0, 0, 0, 0, screen_width, screen_height);
	}
	
	//causes error
	//unload_datafile(data);
	//unload_datafile(map_data);
	destroy_bitmap(menu_player);
	destroy_bitmap(buffer);
	
	return 0;
}
END_OF_MAIN();
