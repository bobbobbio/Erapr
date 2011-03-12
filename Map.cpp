/*
 *	erapr
 *     ERAPR Really Ain't Playable, Right?
 *
 *	Created by Remi Bernotavicius 
 *	Copyright (c) 2009 Remi Bernotavicius. All rights reserved.
 */

#include "Map.h"
#include <iostream>

#pragma mark Constructors and Destructors

Map::Map(unsigned int width, unsigned int height, TILESET* bTS, TILESET* sTS) {
	WORLD_WIDTH = width;
	WORLD_HEIGHT = height;
	spriteTileset = sTS;
	landscapeTileset = bTS;
	
	sprites = new vector<sprite>;
	
	//set up the tiles to store nothing
	
	ALLCELLS = new CELL*[WORLD_WIDTH];

	for (int i = 0; i < (int)WORLD_WIDTH; i++) {
		ALLCELLS[i] = new CELL[WORLD_HEIGHT];

		for (int j = 0; j < (int)WORLD_HEIGHT; j++) {
			ALLCELLS[i][j].back_tile = NO_TILE;
			ALLCELLS[i][j].front_tile = NO_TILE;
			ALLCELLS[i][j].solid = false;
			ALLCELLS[i][j]._event = NO_EVENT;
		}
	}
	
}

Map::Map(char path[480], TILESET* bTS, TILESET* sTS) {
	PACKFILE *pfile;
	pfile=pack_fopen(path, "rp");
	
	landscapeTileset = bTS;
	spriteTileset = sTS;
	
	WORLD_HEIGHT = pack_igetl(pfile);
	WORLD_WIDTH = pack_igetl(pfile);	
	
	ALLCELLS = new CELL*[WORLD_WIDTH];
	
	for (int c = 0; c < (int)WORLD_WIDTH; c++)
	{
		ALLCELLS[c] = new CELL[WORLD_HEIGHT];
		
		for (int r = 0; r < (int)WORLD_HEIGHT; r++)
		{
			ALLCELLS[c][r].back_tile = pack_igetl(pfile);
			ALLCELLS[c][r].front_tile = pack_igetl(pfile);
			ALLCELLS[c][r].solid = pack_igetl(pfile);
			ALLCELLS[c][r]._event = pack_igetl(pfile);
			//pack_igetl(pfile);
		}
	}
	
	//get player position
	playerStart.x = pack_igetl(pfile);
	playerStart.y = pack_igetl(pfile);
	
	//get sprite positions
	sprites = new vector<sprite>;
	sprites->resize(pack_igetl(pfile));
	for (int i = 0; i < (int)sprites->size(); i++) {
		sprites->at(i).position.x = pack_igetl(pfile);
		sprites->at(i).position.y = pack_igetl(pfile);
		sprites->at(i).type = pack_igetl(pfile);
	}

	pack_fclose(pfile);
}

Map::~Map() {
	
	//destroy the tiles
	for (int i = 0; i < (int)WORLD_WIDTH; i++) {
		delete [] ALLCELLS[i];
	}
	delete [] ALLCELLS;
	delete sprites;
}

#pragma mark Solid Detection

bool Map::onTheGround(rectangle rect) {
	CELL b = ALLCELLS[tileContainingPoint(rect.position.x + rect.width / 2)][tileContainingPoint(rect.position.y + rect.height + 1)];
	//CELL br = ALLCELLS[tileContainingPoint(rect.position.x + rect.width - 5)][tileContainingPoint(rect.position.y + rect.height + 2)];
	
	return b.solid || rect.position.y + rect.height + 1 > (int)worldHeight() * TILESIZE;
}

//This method returns a point of dx, dy, how much the 
//rectangle needs to move to make it not on a solid anymore
point Map::solid(rectangle rect, bool jumping) {
	point returnPos = point(0, 0);
	
	point tile;
	
	CELL top = ALLCELLS[tileContainingPoint(rect.position.x + rect.width / 2)][tileContainingPoint(rect.position.y)];
	CELL l_bottom = ALLCELLS[tileContainingPoint(rect.position.x + rect.width / 3)][tileContainingPoint(rect.position.y + rect.height)];
	CELL bottom = ALLCELLS[tileContainingPoint(rect.position.x + rect.width / 2)][tileContainingPoint(rect.position.y + rect.height)];
	CELL r_bottom = ALLCELLS[tileContainingPoint(rect.position.x + rect.width * 2/3)][tileContainingPoint(rect.position.y + rect.height)];
	//CELL left = ALLCELLS[tileContainingPoint(rect.position.x)][tileContainingPoint(rect.position.y + rect.height / 2)];
	//CELL right = ALLCELLS[tileContainingPoint(rect.position.x + rect.width)][tileContainingPoint(rect.position.y + rect.height / 2)];
	
	//std::cout << jumping << "\n";
	
	bool bsolid = bottom.solid || r_bottom.solid || l_bottom.solid;
	
	bool bottomFirst = (top.solid || bsolid) && jumping;
	
	for (int j = 0; j < 2; j++) {
		if(bottomFirst) {
			//bottom
			for(int i = 1; i < rect.width; i += rect.width / 2) {
				tile = point(tileContainingPoint(rect.position.x + i), tileContainingPoint(rect.position.y + rect.height));
				if (ALLCELLS[tile.x][tile.y].solid) {
					rectangle r = rect.intersectingRectangle(rectangle(tile.x * TILESIZE, tile.y * TILESIZE, TILESIZE, TILESIZE));
					
					//std::cout << "bottom" << "\n";
					
					returnPos.translate(0, -1 * r.height - 1);
					rect.position.translate(0, -1 * r.height - 1);
				}
			}
			if(!bsolid) {
				//top
				for(int i = 1; i < rect.width; i += rect.width / 2) {
					tile = point(tileContainingPoint(rect.position.x + i), tileContainingPoint(rect.position.y));
					if (ALLCELLS[tile.x][tile.y].solid) {
						rectangle r = rect.intersectingRectangle(rectangle(tile.x * TILESIZE, tile.y * TILESIZE, TILESIZE, TILESIZE));
						
						//std::cout << "top" << "\n";
						
						returnPos.translate(0, r.height + 1);
						rect.position.translate(0, r.height + 1);
					}
				}
			}
		} else {
			//right
			for(int i = 0; i <= rect.height; i += rect.height / 2) {
				tile = point(tileContainingPoint(rect.position.x + rect.width), tileContainingPoint(rect.position.y + i));
				if (ALLCELLS[tile.x][tile.y].solid) {
					rectangle r = rect.intersectingRectangle(rectangle(tile.x * TILESIZE, tile.y * TILESIZE, TILESIZE, TILESIZE));
					
					//std::cout << "right" << "\n";
					
					returnPos.translate(-1 * r.width - 1 , 0);
					rect.position.translate(-1 * r.width - 1, 0);
				}
			}
			//left
			for(int i = 0; i <= rect.height; i += rect.height / 2) {
				tile = point(tileContainingPoint(rect.position.x), tileContainingPoint(rect.position.y + i));
				if (ALLCELLS[tile.x][tile.y].solid) {
					rectangle r = rect.intersectingRectangle(rectangle(tile.x * TILESIZE, tile.y * TILESIZE, TILESIZE, TILESIZE));
					
					//std::cout << "left" << "\n";
					
					returnPos.translate(r.width + 1, 0);
					rect.position.translate(r.width + 1, 0);
				}
			}
		}
		bottomFirst = false;
	}
	
	return returnPos;
}

#pragma mark Drawing Methods


void Map::_draw(BITMAP* dest, int n, int dx, int dy, TILESET* t) {
	
	n--;
	int x,y;
	y= (n/t->width) * (t->tileHeight + t->tileBuffer) + t->tileBuffer;
	if(n >= (int)t->width)
		n-=t->width*(n/t->width);
	x= n*(t->tileWidth + t->tileBuffer) + t->tileBuffer;
	
	if(n >= 0) {
		blit(t->image, dest, x, y, dx, dy, t->tileWidth, t->tileHeight);
	}
	else
		rectfill(dest, dx, dy, dx + t->tileWidth -1, dy + t->tileHeight -1, 0);
}

void Map::draw_all(BITMAP* dest, bool backFront) {
	
	for(int c = 0;c < (int)WORLD_WIDTH; c++)
	{
		for(int r = 0;r < (int)WORLD_HEIGHT; r++)
		{
			int x=(c*TILESIZE);
			int y=(r*TILESIZE);
			int tilenum = backFront ? ALLCELLS[c][r].back_tile : ALLCELLS[c][r].front_tile;
			drawtile(dest, tilenum, x, y);
		}
	}
}

void Map::draw_solid_all(BITMAP* dest) {
	
	for(int c = 0;c < (int)WORLD_WIDTH; c++)
	{
		for(int r = 0;r < (int)WORLD_HEIGHT; r++)
		{
			drawsolid(dest, ALLCELLS[c][r].solid, c*TILESIZE, r*TILESIZE);
		}
	}
}

void Map::draw_event_all(BITMAP* dest) {
	
	for(int c = 0;c < (int)WORLD_WIDTH; c++)
	{
		for(int r = 0;r < (int)WORLD_HEIGHT; r++)
		{
			drawevent(dest, ALLCELLS[c][r]._event, c*TILESIZE,r*TILESIZE);
		}
	}
}

void Map::drawSprite(BITMAP* dest, int n, int dx, int dy) {
	_draw(dest, n, dx, dy, spriteTileset);
}

void Map::drawtile(BITMAP* dest, int n, int dx, int dy) {
	_draw(dest, n, dx, dy, landscapeTileset);
}

void Map::drawevent(BITMAP* dest, int n, int dx, int dy) {
	
	if(n != 0) {
		rect(dest, dx, dy, dx + TILESIZE-1, dy + TILESIZE-1, 18);
		textprintf_centre_ex(dest, font, dx + TILESIZE / 2, dy + TILESIZE / 2, makecol(255, 0, 0), -1, "%d", n);
	} else
		rectfill(dest, dx, dy, dx + TILESIZE-1, dy + TILESIZE-1, 0);
}

void Map::drawsolid(BITMAP* dest, int n, int dx, int dy) {
	
	//more solid types to come eventually
	switch (n) {
		case 0:
			rectfill(dest, dx, dy, dx + TILESIZE-1, dy + TILESIZE-1, 0);
			break;
			
		case 1:
			rect(dest, dx, dy, dx + TILESIZE-1, dy + TILESIZE-1, 18);
			break;
	}

}


#pragma mark Accessor and Utility Methods

unsigned int Map::worldWidth() {
	return WORLD_WIDTH;
}

unsigned int Map::worldHeight() {
	return WORLD_HEIGHT;
}

int Map::eventForRectangle(rectangle rect) {
	int c = tileContainingPoint(rect.position.x + rect.width / 2);
	int r = tileContainingPoint(rect.position.y + rect.height / 2);
	return ALLCELLS[c][r]._event;
}


/** Returns the tile number for the position coordinate, for both dimensions */
int Map::tileContainingPoint(int ino)
{
    int out;
    out = ( ino / TILESIZE );
	
    return out;
}


int Map::eventCount() {
	//20 events for now
	return 20;
}

int Map::solidCount() {
	return 2;
}

int Map::spriteCount() {
	return 217;
}

#pragma mark Saving To File

void Map::saveToFile(char path[480]) {
	PACKFILE *pfile;
	
	pfile=pack_fopen(path, "wp");
	
	pack_iputl(WORLD_HEIGHT, pfile);
	pack_iputl(WORLD_WIDTH, pfile);
	
	for (int c=0; c < (int)WORLD_WIDTH; c++)
	{
		for (int r=0; r < (int)WORLD_HEIGHT; r++)
		{
			pack_iputl(ALLCELLS[c][r].back_tile, pfile);
			pack_iputl(ALLCELLS[c][r].front_tile, pfile);
			pack_iputl(ALLCELLS[c][r].solid, pfile);
			pack_iputl(ALLCELLS[c][r]._event, pfile);
		}
	}
	
	//put player position
	pack_iputl(playerStart.x, pfile);
	pack_iputl(playerStart.y, pfile);
	
	//put sprites
	pack_iputl(sprites->size(), pfile);
	for (int i = 0; i < (int)sprites->size(); i++) {
		pack_iputl(sprites->at(i).position.x, pfile);
		pack_iputl(sprites->at(i).position.y, pfile);
		pack_iputl(sprites->at(i).type, pfile);
	}
	
	pack_fclose(pfile);
}