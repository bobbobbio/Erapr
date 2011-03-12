/*
 *  Item.h
 *  erapr
 *
 *  Created by Remi Bernotavicius on 10/7/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#define ITEM_BUFFER 15

class Item : public Sprite
{
public:
	void moveAll(Map* mainMap) {}
	Item(TILESET* tileset, rectangle new_bounds):Sprite(tileset, new_bounds) {}
	int scoreValue;
private:
	rectangle adj_bounds();
};