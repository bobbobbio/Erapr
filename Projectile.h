/*
 *	erapr
 *     ERAPR Really Ain't Playable, Right?
 *
 *	Created by Remi Bernotavicius 
 *	Copyright (c) 2009 Remi Bernotavicius. All rights reserved.
 */

#define PI 3.14159265
#define RADIUS 3

class Projectile : public Sprite {
public:
	Projectile(){bounds.width = RADIUS*2; bounds.height = RADIUS*2; elastic = false;};
	Projectile(TILESET* tileset, rectangle new_bounds):Sprite(tileset, new_bounds) {};
	void fire(int angle, int speed);
	void draw(BITMAP* b, int dx, int dy);
	void tick(Player* p1, rectangle screen_rect);
};