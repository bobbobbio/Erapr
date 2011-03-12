/*
 *	erapr
 *     ERAPR Really Ain't Playable, Right?
 *
 *	Created by Remi Bernotavicius 
 *	Copyright (c) 2009 Remi Bernotavicius. All rights reserved
 .
 */
#include "tileedit.h"
#include <iostream>

#define INPUT_DELAY 3

void ReadInput()
{
	if(key[KEY_ESC] || key[KEY_Q])
	{
		if(alert("Are you sure you", "want to quit?", 
				 "", "Yes", "No", 0, 0)==1)
			cont=false;
	}
	
	
	//mode changing
	if(key[KEY_1])
		M = TILE;
	if(key[KEY_2])
		M = SOLID;
	if(key[KEY_3])
		M = FRONTL;
	if(key[KEY_4])
		M = EVENT;
	if(key[KEY_5])
		M = SPRITE;
	if (key[KEY_6])
		M = PLAYER_POSITION;
	
	
	//find max scrolling number
	int max;
	switch (M) {
		case SOLID:
			max = CM->solidCount() - 1;
			break;
			
		case EVENT:
			max = CM->eventCount() - 1;
			break;
		
		case SPRITE:
			max = (int)CM->spriteCount() - 1;
			break;
			
		default:
			max = (int)CM->landscapeTileset->size() - 1;
			break;
	}
	
	//fix CN if we changed modes
	CN = std::min(CN, max);
	
	delay++;
	if(delay > INPUT_DELAY) {
		if(key[KEY_X])
		{
			CN++;
			
			if (CN > max) {
				CN = 0;
			}
			
			delay = 0;
		}
		
		if(key[KEY_Z])
		{
			CN--;
			if(CN < 0)
				CN = max;
			
			delay = 0;
		}
	}

	if(key[KEY_N])
		NewMap();
	if(key[KEY_S])
		savemap();
	if(key[KEY_L])
		loadmap();
	

	if(mouse_b & 1)
	{
		AddTile();
	}

	if(mouse_b & 2)
	{
		PickUpTile();
	}
	if(key[KEY_R])
		ChangeRes();

}
void init() 
{
	//setup allegro
	int res;
	allegro_init();
	set_color_depth(8);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
	if (res != 0) {
		//allegro_message(allegro_error); //causes error ??
		exit(-1);
	}
	
	install_timer();
	install_keyboard();
	install_mouse();
	
	//load and setup tilesets
	data = load_datafile("../Resources/maindata.dat");
	gamedata = load_datafile("../Resources/mapdata.dat");
	
	bgTS = new TILESET(18, 11, (BITMAP*)gamedata[T2].dat);
	
	itTS = new TILESET(18, 11, (BITMAP*)gamedata[enemies].dat);
	itTS->tileWidth = SPRITE_W;
	itTS->tileHeight = SPRITE_H;
	
	gui_bg_color = 0; //bg color for dialogs and so forth
	rr=1;
	Buffer = create_bitmap(SCREEN_W,SCREEN_H);
	CT = create_bitmap(TILESIZE,TILESIZE);
	BITMAP* a = (BITMAP*)gamedata[player].dat;
	b = (BITMAP*)gamedata[enemies].dat;
	p = create_bitmap(32 * 2, 42);
	blit(a, p, 0, 0, 0, 0, a->w / 2, a->h);
	blit(b, p, 0, SPRITE_H, a->w / 2, 0, 32, 42);
	M = TILE;
	cont=true;
	
	NewMap();
}
void ChangeRes()
{
	int in1 = 0;
	int in2 = 0;
	rr++;
	if(rr==4)rr=1;
	switch(rr)
	{
		case 1:{in1=640;in2=480;}break;
		case 2:{in1=800;in2=600;}break;
		case 3:{in1=1280;in2=1024;}break;
	}
	set_color_depth(8);
	set_gfx_mode(GFX_AUTODETECT, in1, in2, 0, 0);
	
	gui_fg_color = makecol(0, 0, 0);
	gui_mg_color = makecol(128, 128, 128);
	gui_bg_color = makecol(200, 240, 200);
	
	Buffer = create_bitmap(in1,in2);
	draw_all();
}
void deinit() {
	unload_datafile(data);
	unload_datafile(gamedata);
	destroy_bitmap(Buffer);
	destroy_bitmap(CT);
	clear_keybuf();
}
