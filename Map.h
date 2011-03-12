/*
 *	erapr
 *     ERAPR Really Ain't Playable, Right?
 *
 *	Created by Remi Bernotavicius 
 *	Copyright (c) 2009 Remi Bernotavicius. All rights reserved.
 */

#import <Allegro/Allegro.h>
#include "shapeTypes.h"
#include <vector>

#define NO_TILE 0 //This tile number represesnts no tile.
#define NO_EVENT 0
#define EXIT_EVENT 1 //event that makes the player exit.
#define DIE_EVENT 2	//event kills the player.

#define TILESIZE 32 //size of tiles
#define ITEMSIZE 32 //size of item

#define SPRITE_W 32
#define SPRITE_H 40
#define FRONT false
#define BACK true

using namespace std;

typedef struct TILESET
{
	int width;
	int height;
	int tileWidth;
	int tileHeight;
	int tileBuffer;
	BITMAP* image;
	
	int size() {
		return width * height + 1;
	}
	
	TILESET(int nwidth, int nheight, BITMAP* nimg) {
		tileWidth = TILESIZE;
		tileHeight = TILESIZE;
		width = nwidth;
		height = nheight;
		image = nimg;
		tileBuffer = 2;
	}
	
	TILESET() {
		tileWidth = TILESIZE;
		tileHeight = TILESIZE;
		width = 0;
		height = 0;
		tileBuffer = 2;
		image = NULL;
	}
};

typedef struct sprite
{
	point position;
	int type;
	sprite()
	{
		position = point(0, 0);
		type = 0;
	}
	sprite(point g, int t)
	{
		position = g;
		type = t;
		
	}	
	bool equals(sprite other) {
		return other.position.equals(position) && other.type == type;
	}
};


class Map
{
	typedef struct CELL
	{
		int front_tile;
		int back_tile;
		int solid;
		int _event;
	};
	
public:
	//Constructors and Destructors
	Map(unsigned int width, unsigned int height, TILESET* bTS, TILESET* iTS);
	Map(char path[480], TILESET* bTS, TILESET* iTS);
	~Map();
	
	
	//Fields
	CELL** ALLCELLS;
	TILESET* spriteTileset;
	TILESET* landscapeTileset;
	
	point playerStart;
	vector<sprite>* sprites;
	
	//Drawing
	void draw_all(BITMAP* dest, bool backFront);
	void draw_solid_all(BITMAP* dest);
	void draw_event_all(BITMAP* dest);
	void draw_item_all(BITMAP* dest);
	
	void drawSprite(BITMAP* dest, int n, int dx, int dy);
	void drawtile(BITMAP* dest, int n, int dx, int dy);
	void drawevent(BITMAP* dest, int n, int dx, int dy);
	void drawsolid(BITMAP* dest, int n, int dx, int dy);

	//Solid Detection
	point solid(rectangle rect, bool jumping);
	bool onTheGround(rectangle rect);
	

	//Accessor and Utility Methods
	unsigned int worldWidth();
	unsigned int worldHeight();
	
	int updateItems(BITMAP* uPg, rectangle rect);
	int eventForRectangle(rectangle rect);
	int* listOfItemCounts();
	int tileContainingPoint(int ino);
	
	int eventCount();
	int solidCount();
	int spriteCount();
	
	//Saving to File
	void saveToFile(char path[480]);
	
private:
	//internal drawing method
	void _draw(BITMAP* dest, int n, int dx, int dy, TILESET* t);
	
	//More Fields
	unsigned int WORLD_HEIGHT;
	unsigned int WORLD_WIDTH;
};