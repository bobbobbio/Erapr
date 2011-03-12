/*
 *	erapr
 *     ERAPR Really Ain't Playable, Right?
 *
 *	Created by Remi Bernotavicius 
 *	Copyright (c) 2009 Remi Bernotavicius. All rights reserved.
 */

#include <allegro.h>
#include "tileedit.h"
#import <iostream>

void deinit();
void init();
DATAFILE *data, *gamedata;
BITMAP* Buffer;
BITMAP* back_mapBuffer;
BITMAP* front_mapBuffer;
BITMAP* solid_mapBuffer;
BITMAP* event_mapBuffer;
BITMAP* item_mapBuffer;
BITMAP* shaded;
BITMAP* CT;
BITMAP* p;
BITMAP* b;
bool cont;
Map* CM;
int scrollX, scrollY, M, CN, rr, delay;
TILESET *bgTS, *itTS;

int main() {
	init();
	
	while (cont)
    {
		ScrollUpdate();
		ReadInput();

		draw_all();

		draw_sprite(screen,Buffer,0,0);
		//rest(40);
	}

	deinit();
	return 0;
}
END_OF_MAIN()
