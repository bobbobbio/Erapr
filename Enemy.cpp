/*
 *	erapr
 *     ERAPR Really Ain't Playable, Right?
 *
 *	Created by Remi Bernotavicius 
 *	Copyright (c) 2009 Remi Bernotavicius. All rights reserved.
 */

#include "Sprite.h"

void Enemy::moveAll(Map* mainMap) {
	
	frame = ((bounds.position.x / 10) % 2);
	
	if(_alive) {
		step_counter++;
		if (step_counter > RANGE * 2) {
			step_counter = 1;
		}
		
		if (step_counter <= RANGE) {
			bounds.position.x += SPEED;
			h_flipped = false;
		} else {
			bounds.position.x -= SPEED;
			h_flipped = true;
		}
		
		//std::cout << bounds.position.x << ", " << bounds.position.y << "\n";
		int tempx = bounds.position.x;
		Sprite::move(mainMap, UD);
		
		if (tempx != bounds.position.x || bounds.position.x + SPEED >= (int)mainMap->worldWidth() * TILESIZE || bounds.position.x - SPEED <= 0) {
			step_counter = step_counter <= RANGE ? RANGE + 1 : 0;
		}
		
		frame = ((bounds.position.x / 10) % frameCount);
	} else {
		if(bounds.position.y < (int)mainMap->worldHeight() * TILESIZE)
			bounds.position.y += SPEED * 2;
		else {
			_draw = false;
			destroy();
		}

	}
}

void Enemy::draw(BITMAP* drw, int dx, int dy) {
	if(_draw)
		Sprite::draw(drw, dx, dy);
}

void Enemy::kill() {
	_alive = false;
	v_flipped = true;
}

bool Enemy::alive() {
	return _alive;
}

rectangle Enemy::adj_bounds() {
	rectangle tempOne = bounds;
	tempOne.position.x += ENEMY_BUFFER;
	tempOne.position.y += ENEMY_BUFFER;
	tempOne.width -= ENEMY_BUFFER * 2;
	tempOne.height -= ENEMY_BUFFER * 2;
	return tempOne;
}