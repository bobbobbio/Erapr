/*
 *	erapr
 *     ERAPR Really Ain't Playable, Right?
 *
 *	Created by Remi Bernotavicius 
 *	Copyright (c) 2009 Remi Bernotavicius. All rights reserved.
 */

#include <allegro.h>
#include "mapdata.h"
#include "data1.h"
#include "../Map.h"

#define TILE 0
#define SOLID 1
#define SPRITE 3
#define EVENT 4
#define FRONTL 5
#define PLAYER_POSITION 6

extern Map* CM;
extern DATAFILE *data, *gamedata;
extern BITMAP* Buffer;
extern BITMAP* back_mapBuffer;
extern BITMAP* front_mapBuffer;
extern BITMAP* solid_mapBuffer;
extern BITMAP* event_mapBuffer;
extern BITMAP* item_mapBuffer;
extern BITMAP* shaded;
extern BITMAP* CT;
extern BITMAP* p;
extern BITMAP* b;
extern int scrollX, scrollY, M, CN, rr, delay;
extern bool cont;
extern TILESET *bgTS, *itTS;

//functions
void draw_all();
void redraw_map();
void AddTile();
void PickUpTile();
void ScrollUpdate();
void ReadInput();
void NewMap();
void loadmap();
void savemap();
void init();
void deinit();
void ChangeRes();
int tileformula(int in);
point mapsize();
void dither(BITMAP* bmp);


