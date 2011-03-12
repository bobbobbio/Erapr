/*
 *  Item.cpp
 *  erapr
 *
 *  Created by Remi Bernotavicius on 10/7/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Sprite.h"

rectangle Item::adj_bounds() {
	rectangle tempOne = bounds;
	tempOne.position.x += ITEM_BUFFER;
	tempOne.position.y += ITEM_BUFFER;
	tempOne.width -= ITEM_BUFFER * 2;
	tempOne.height -= ITEM_BUFFER * 2;
	return tempOne;
}