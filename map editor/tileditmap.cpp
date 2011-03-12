/*
 *	erapr
 *     ERAPR Really Ain't Playable, Right?
 *
 *	Created by Remi Bernotavicius 
 *	Copyright (c) 2009 Remi Bernotavicius. All rights reserved.
 */

#include "tileedit.h"
#include <iostream>
#include <cmath>


void dither(BITMAP* bmp) {
	
	for (int c = 0; c < bmp->w; c++) {
		if(c % 2 == 0)
			putpixel(bmp, c, 0, makecol(0, 0, 0));
	}
	blit(bmp, bmp, 0, 0, 1, 1, bmp->w - 1, 1);
	for (int r = 2; r < bmp->h; r*=2) {
		blit(bmp, bmp, 0, 0, 0, r, bmp->w, r);
	}
	
}

void redraw_map()
{
	set_palette((RGB*)data[MAIN_PAL].dat);
	clear_to_color(back_mapBuffer, 0);
	clear_to_color(front_mapBuffer, 0);
	clear_to_color(solid_mapBuffer, 0);
	clear_to_color(event_mapBuffer, 0);
	CM->draw_all(back_mapBuffer, BACK);
	CM->draw_all(front_mapBuffer, FRONT);
	CM->draw_event_all(event_mapBuffer);
	CM->draw_solid_all(solid_mapBuffer);
}

void draw_all()
{
	set_palette((RGB*)data[MAIN_PAL].dat);
	clear_to_color(Buffer, makecol(255,0,255));
	clear_to_color(CT, 0);
	
	//we always draw the back layer
	draw_sprite(Buffer, back_mapBuffer, scrollX, scrollY);
	
	switch(M)
	{
		case TILE:
		{
			CM->drawtile(CT, CN, 0, 0);
			
			//cursor
			rect(CT, 0, 0, TILESIZE-1, TILESIZE-1, makecol(255,0,0));
		}break;
		case SOLID:
		{
			CM->drawsolid(CT, CN, 0, 0);
			
			//only show solids in solid mode
			draw_sprite(Buffer, solid_mapBuffer, scrollX, scrollY);
		}break;
		case EVENT:
		{
			CM->drawevent(CT, CN, 0, 0);
			
			//only show events in event mode
			draw_sprite(Buffer, event_mapBuffer, scrollX, scrollY);
		}break;
		case SPRITE:
		{
			CM->drawSprite(CT, CN, 0, -(itTS->tileHeight - TILESIZE));
			textprintf_ex(CT, font, 0, 0, makecol(255, 255, 255), 0, "%d", CN);
			//draw_sprite(CT, b, -1 * (CN - 1) * 34, TILESIZE - SPRITE_H);
		}break;
		case FRONTL:
		{	
			//shade the screen
			draw_sprite(Buffer, shaded, 0, 0);
			CM->drawtile(CT, CN, 0, 0);
			rect(CT, 0, 0, TILESIZE-1, TILESIZE-1, makecol(0,0,255));
		}break;
		case PLAYER_POSITION:
		{
			draw_sprite(CT, p, 0, TILESIZE - p->h);
		}break;
	}
	
	BITMAP* temp = create_bitmap(itTS->tileWidth, itTS->tileHeight);
	clear_to_color(temp, 0); //fill temp with blank pixels
	blit(p, temp, 0, 0, 0, 0, itTS->tileWidth, itTS->tileHeight);
	//draw player
	draw_sprite(Buffer, temp, scrollX + CM->playerStart.x, scrollY + CM->playerStart.y);
	
	//draw sprites
	for(int i = 0; i < (int)CM->sprites->size(); i++) {
		CM->drawSprite(temp, CM->sprites->at(i).type, 0, 0);
		draw_sprite(Buffer, temp, scrollX + CM->sprites->at(i).position.x, scrollY + CM->sprites->at(i).position.y);
	}
	delete temp;
	
	//we always draw the front tiles
	draw_sprite(Buffer, front_mapBuffer, scrollX, scrollY);
	
	draw_sprite(Buffer, CT, CM->tileContainingPoint(mouse_x)*TILESIZE, CM->tileContainingPoint(mouse_y)*TILESIZE);
	rect(Buffer, mouse_x, mouse_y, mouse_x+4, mouse_y+4, makecol(0,255,0));
}

void AddTile()
{
	int nx,ny;
	nx = (CM->tileContainingPoint(mouse_x)*TILESIZE-scrollX)/TILESIZE;
	ny = (CM->tileContainingPoint(mouse_y)*TILESIZE-scrollY)/TILESIZE;
	
	//std::cout << "\n" << nx << ", " << ny;
	
	switch(M)
	{
		case SOLID:
		{
			CM->ALLCELLS[nx][ny].solid = CN;
			CM->drawsolid(solid_mapBuffer, CN, nx * TILESIZE, ny * TILESIZE);
		}break;
		case EVENT:
		{
			CM->ALLCELLS[nx][ny]._event = CN;
			CM->drawevent(event_mapBuffer, CN, nx * TILESIZE, ny * TILESIZE);
			
		}break;
		case FRONTL:
		{
			CM->ALLCELLS[nx][ny].front_tile = CN;
			CM->drawtile(front_mapBuffer, CN, nx * TILESIZE, ny * TILESIZE);
		}break;
		case SPRITE:
		{
			if(CN == 0){
				for (int i = 0; i < (int)CM->sprites->size(); i++) {
					rectangle bounds = rectangle(CM->sprites->at(i).position.x, CM->sprites->at(i).position.y, 32, 42);
					if (bounds.containsPoint(nx * TILESIZE + (TILESIZE / 2), ny * TILESIZE + (TILESIZE / 2))) {
						CM->sprites->erase(CM->sprites->begin() + i);
					}
				}
			}else {
				//add new enemy only if one doesn't exist at this point
				point p = point(nx * TILESIZE, ny * TILESIZE - (itTS->tileHeight - TILESIZE));
				bool found = false;
				for (int i = 0; i < (int)CM->sprites->size(); i++) {
					if (CM->sprites->at(i).position.equals(p)) {
						found = true;
						break;
					}
				}
				if (!found) {
					sprite t = sprite(p, CN);
					CM->sprites->push_back(t);
					//std::cout << "adding emeny";
				}
			}
		}break;
		case TILE:
		{
			CM->ALLCELLS[nx][ny].back_tile = CN;
			CM->drawtile(back_mapBuffer, CN, nx * TILESIZE, ny * TILESIZE);
			
			//make default tile solid
			CM->ALLCELLS[nx][ny].solid = (CN != 0);
			CM->drawsolid(solid_mapBuffer, CM->ALLCELLS[nx][ny].solid, nx * TILESIZE, ny * TILESIZE);
		}break;
			
		case PLAYER_POSITION:
		{
			CM->playerStart.x = nx * TILESIZE;
			CM->playerStart.y = (ny + 1) * TILESIZE - p->h;
		}break;
	}
}
void PickUpTile()
{
	int nx,ny;
	nx = (CM->tileContainingPoint(mouse_x)*TILESIZE-scrollX)/TILESIZE;
	ny = (CM->tileContainingPoint(mouse_y)*TILESIZE-scrollY)/TILESIZE;
	switch(M)
	{
		case SOLID:
		{
			CN = CM->ALLCELLS[nx][ny].solid;
		}break;
		case EVENT:
		{
			CN = CM->ALLCELLS[nx][ny]._event;
		}break;
		case TILE:
		{
			CN = CM->ALLCELLS[nx][ny].back_tile;
		}break;
		case FRONTL:
		{
			CN = CM->ALLCELLS[nx][ny].front_tile;
		}break;
	}
}
void ScrollUpdate()
{
	if(mouse_y < 20)
		scrollY+=TILESIZE;
	if(mouse_y < 10)
		scrollY+=TILESIZE;
	if(mouse_y > SCREEN_H-20)
		scrollY-=TILESIZE;
	if(mouse_y > SCREEN_H-10)
		scrollY-=TILESIZE;
	if(mouse_x < 20)
		scrollX+=TILESIZE;
	if(mouse_x < 10)
		scrollX+=TILESIZE;
	if(mouse_x > SCREEN_W-20)
		scrollX-=TILESIZE;
	if(mouse_x > SCREEN_W-10)
		scrollX-=TILESIZE;
	if(scrollX > 0)
		scrollX=0;
	if(scrollY > 0)
		scrollY=0;
	int tempx = ((CM->worldWidth()*TILESIZE)-SCREEN_W)*(-1);
	int tempy = ((CM->worldHeight()*TILESIZE)-SCREEN_H)*(-1);
	if(scrollX < tempx)
		scrollX = tempx;
	if(scrollY < tempy)
		scrollY = tempy;
}

void NewMap()
{
    scrollX=0;
    scrollY=0;
    CN=1;
	delete CM;
	point sz = mapsize();
	
	std::cout << "map created";
	
	CM = new Map(sz.x, sz.y, bgTS, itTS);
	destroy_bitmap(back_mapBuffer);
	destroy_bitmap(front_mapBuffer);
	destroy_bitmap(solid_mapBuffer);
	destroy_bitmap(event_mapBuffer);
	back_mapBuffer = create_bitmap(CM->worldWidth()*TILESIZE, CM->worldHeight()*TILESIZE);
	front_mapBuffer = create_bitmap(CM->worldWidth()*TILESIZE, CM->worldHeight()*TILESIZE);
	event_mapBuffer = create_bitmap(CM->worldWidth()*TILESIZE, CM->worldHeight()*TILESIZE);
	solid_mapBuffer = create_bitmap(CM->worldWidth()*TILESIZE, CM->worldHeight()*TILESIZE);
	shaded = create_bitmap(CM->worldWidth()*TILESIZE, CM->worldHeight()*TILESIZE);
	dither(shaded);
	
	redraw_map();
}
void loadmap()
{
	char path[480] = "";
    if(file_select_ex("Choose a map file to open", path, "map", 480,400, 200) != 0)
    {
		delete CM;
		CM = new Map(path, bgTS, itTS);
		CN=1;
		scrollX=0;
		scrollY=0;
		destroy_bitmap(back_mapBuffer);
		destroy_bitmap(front_mapBuffer);
		destroy_bitmap(solid_mapBuffer);
		destroy_bitmap(event_mapBuffer);
		back_mapBuffer = create_bitmap(CM->worldWidth()*TILESIZE, CM->worldHeight()*TILESIZE);
		front_mapBuffer = create_bitmap(CM->worldWidth()*TILESIZE, CM->worldHeight()*TILESIZE);
		event_mapBuffer = create_bitmap(CM->worldWidth()*TILESIZE, CM->worldHeight()*TILESIZE);
		solid_mapBuffer = create_bitmap(CM->worldWidth()*TILESIZE, CM->worldHeight()*TILESIZE);
		shaded = create_bitmap(CM->worldWidth()*TILESIZE, CM->worldHeight()*TILESIZE);
		dither(shaded);
		redraw_map();
	}
}

void savemap()
{
	char path[480] = "newmap.map";
    if(file_select_ex("Choose where to save", path, "map", 480,400, 200) != 0)
    {
		CM->saveToFile(path);
    }
}

int quit() {
	cont = false;
	return D_CLOSE;
}


//code taken from the allegro example file exgui.c
int my_button_proc(int msg, DIALOG *d, int c)
{
	int ret = d_button_proc(msg, d, c);
	if (ret == D_CLOSE && d->dp3)
		return ((int (*)(void))d->dp3)();
	return ret;
}


point mapsize()
{
	set_palette((RGB*)data[MAIN_PAL].dat);
	char width[(32 + 1) * 4] = "40";
	char height[(32 + 1) * 4] = "30";
	DIALOG the_dialog[] = {
		{ d_clear_proc, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL},
		{ d_shadow_box_proc, 0, 0, 220, 100, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL },
		{ d_ctext_proc, 10, 10, 160, 8, 0, 0, 0, 0, 0, 0, (void*)"Set Map Size", NULL, NULL},
		{ d_text_proc, 10, 30, 0, 0, 0, 0, 0, 0, 0, 0, (void*)"width:", NULL, NULL},
		{ d_edit_proc, 70, 30, 100, 8, 0, 0, 0, 0, 32, 0, width, NULL, NULL} ,
		{ d_text_proc, 10, 40, 0, 0, 0, 0, 0, 0, 0, 0, (void*)"height:", NULL, NULL},
		{ d_edit_proc, 70, 40, 100, 8, 0, 0, 0, 0, 32, 0, height, NULL, NULL} ,
		{ d_button_proc, 10, 60, 80, 20, 0, 0, 'c', D_EXIT, 0, 0, (void*)"&Create", NULL, NULL},
		{ my_button_proc, 130, 60, 80, 20, 0, 0, 'e', D_EXIT, 0, 0, (void*)"&Exit", NULL, (void*)quit},
		{ d_keyboard_proc, 0, 0, 0, 0, 0, 0, 0, 0, KEY_ESC, 0, (void*)quit, NULL, NULL},
		{ NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL }
	};
	
	//set_dialog_color(the_dialog, gui_fg_color, gui_bg_color);
	
	for (int i = 0; the_dialog[i].proc; i++) {
		the_dialog[i].fg = makecol(0, 0, 0);
		the_dialog[i].bg = makecol(255, 255, 255);
   }
	
	centre_dialog(the_dialog);
	popup_dialog(the_dialog, -1);
	
	int minw = (int) ceil((double)SCREEN_W / TILESIZE);
	int minh = (int) ceil((double)SCREEN_H / TILESIZE);
	
	return point(std::max(atoi(width), minw), std::max(atoi(height), minh));
	
}