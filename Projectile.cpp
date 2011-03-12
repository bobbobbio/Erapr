/*
 *	erapr
 *     ERAPR Really Ain't Playable, Right?
 *
 *	Created by Remi Bernotavicius 
 *	Copyright (c) 2009 Remi Bernotavicius. All rights reserved.
 */

#include "Sprite.h"

void Projectile::draw(BITMAP* b, int dx, int dy) {
	circlefill(b, bounds.position.x + dx + RADIUS, bounds.position.y + dy + RADIUS, RADIUS, makecol(bounds.position.x % 255, 0, 0));
}

void Projectile::fire(int angle, int speed) {
	applyVelocity(speed * cos(angle * PI / 180.0), LR);
	applyVelocity(speed * sin(angle * PI / 180.0), UD);
}

void Projectile::tick(Player* p1, rectangle screen_rect)
{
	if(bounds.position.x - p1->mapx > screen_rect.width || bounds.position.x - p1->mapx < 0 || bounds.position.y - p1->mapy < 0 || bounds.position.y - p1->mapy > screen_rect.height)
	{
		destroy();
	}
}