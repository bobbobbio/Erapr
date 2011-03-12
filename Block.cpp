/*
 *  Block.cpp
 *  erapr
 *
 *  Created by Remi Bernotavicius on 5/4/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Sprite.h"

void Block::transform(Player* p) {
	rectangle r = p->bounds.intersectingRectangle(bounds);
	if(r.width > 0 && r.height > 0)
	{
		bounds.position.x = p->bounds.position.x + ((p->h_flipped ? -1 : 1) * X_OFFSET);
		bounds.position.y = p->bounds.position.y + p->bounds.height - bounds.height - 2;
		lr_velocity = p->getLRVelocity();
		ud_velocity = p->getUDVelocity();
		held = true;
	}
}

void Block::moveAll(Map* mainMap)
{
	if(_alive)
		Sprite::moveAll(mainMap);
	else
	{
		if(bounds.position.y < (int)mainMap->worldHeight() * TILESIZE)
		{
			bounds.position.y += SPEED * 2;
			bounds.position.x += lr_velocity;
		}
		else {
			destroy();
		}
	}
}

void Block::kill()
{
	
	v_flipped = true;
	_alive = false;
}

void Block::tick(Sprite* p, rectangle screen_rect)
{
	if(key[KEY_A])
		transform((Player*)p);
	else
		kick((Player*)p, key[KEY_UP]);
}

void Block::kick(Player* p, bool up) {
	rectangle r = p->bounds.intersectingRectangle(bounds);
	if(r.width > 0 && r.height > 0 && held)
	{
		if(!up)
			applyVelocity(((p->h_flipped ? -1 : 1) * KICK_STRENGTH), LR);
		else
			applyVelocity(KICK_STRENGTH * 10, UD);
		point pg = bounds.position;
		pg.translate(0, -20);
		
		p->applyAccessory(3, 4, pg);
		
		held = false;
	}
}