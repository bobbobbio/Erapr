/*
 *  ScoreSprite.cpp
 *  erapr
 *
 *  Created by Remi Bernotavicius on 10/13/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Sprite.h"

void ScoreSprite::moveAll(Map* mainMap) {
	if(alive())
	{
		bounds.position.y -= 5;
		life--;
	}
}

bool ScoreSprite::alive()
{
	return life > 0;
}

void ScoreSprite::draw(BITMAP* drw, int dx, int dy) {
	textprintf_ex(drw, font, bounds.position.x + dx, bounds.position.y + dy, makecol(255, 255, 255), -1, "%d", value);
}