/*
 *	erapr
 *     ERAPR Really Ain't Playable, Right?
 *
 *	Created by Remi Bernotavicius 
 *	Copyright (c) 2009 Remi Bernotavicius. All rights reserved.
 */

#include "Sprite.h"

int Sprite::GRAVITY = 2;

/*
	draw
	Draws the sprite at its position plus (dx, dy), (the scrolling change).
 */
void Sprite::draw(BITMAP* drw, int dx, int dy) {
	
	BITMAP* currentFrame = create_bitmap(bounds.width, bounds.height);
	BITMAP* temp = create_bitmap(bounds.width, bounds.height);
	
	clear_to_color(temp, 0);
	
	int n = frame + max(type - 1, 0);
	int fx,fy;
	fy = 0;
	fy= (n/t->width) * (t->tileHeight + t->tileBuffer) + t->tileBuffer;
	if(n >= (int)t->width)
		n-=t->width*(n/t->width);
	fx= n*(t->tileWidth + t->tileBuffer) + t->tileBuffer;
	
	blit(t->image, currentFrame, fx, fy, 0, 0, t->tileWidth, t->tileHeight);
	
	if (!h_flipped) {
		if(!v_flipped)
			draw_sprite(temp, currentFrame, 0, 0);
		else
			draw_sprite_v_flip(temp, currentFrame, 0, 0);
	} else {
		if(!v_flipped)
			draw_sprite_h_flip(temp, currentFrame, 0, 0);
		else
			draw_sprite_vh_flip(temp, currentFrame, 0, 0);
	}
	draw_sprite(drw, temp, bounds.position.x + dx, bounds.position.y + dy);
	if(access_count > 0) {
		blit(t->image, currentFrame, accessory*bounds.width, 0, 0, 0, bounds.width, bounds.height);
		draw_sprite(drw, currentFrame, access_p.x + dx, access_p.y + dy + 20);
		access_count--;
	}
		
	destroy_bitmap(temp);
	destroy_bitmap(currentFrame);
}

/*
	draw
	Draw at position.
 */
void Sprite::draw(BITMAP* drw) {
	draw(drw, 0, 0);
}

int Sprite::getUDVelocity() {
	return ud_velocity;
}

int Sprite::getLRVelocity() {
	return lr_velocity;
}

void Sprite::tick(Sprite* p, rectangle screen_rect)
{
	//Meant to be overridden by subclasses
}

/*
	Applys a velocity to the sprite, nv, at direction nd (only supports left and right) (UD, LR)
 */
void Sprite::applyVelocity(int nv, int nd) {
	if(nd == UD) {
		ud_velocity += nv;
		ud_velocity = std::max(std::min(ud_velocity, TILESIZE), -TILESIZE);
	} else if (nd == LR){
		lr_velocity += nv;
		lr_velocity = std::max(std::min(lr_velocity, TILESIZE), -TILESIZE);
	}
}

void Sprite::destroy()
{
	_active = false;
}

bool Sprite::active()
{
	return _active;
}

void Sprite::moveAll(Map* mainMap)
{
	move(mainMap, LR);
	move(mainMap, UD);
}

void Sprite::applyAccessory(int acc, int time, point p) {
	accessory = acc;
	access_count = time;
	access_p = p;
}

void Sprite::move(Map* mainMap, int dir) {
	
	bool onTheGround = mainMap->onTheGround(bounds);
	
	//apply gravity
	
	if(dir == UD) {
		bounds.position.y -= ud_velocity;
		if(!onTheGround && ud_velocity > -1 * TILESIZE)
			ud_velocity -= GRAVITY;
	}
	else if(dir == LR) {
		bounds.position.x += lr_velocity;
		
		if(onTheGround && !elastic)
		{
			facum += FRICTION;
			if(facum >= 1) {
				if(abs(lr_velocity) < facum)
					lr_velocity = 0;
				if(lr_velocity > 0)
					lr_velocity -= facum;
				else if(lr_velocity < 0)
					lr_velocity += facum;
				facum -= 1;
			}
		}else if(!onTheGround && !elastic)
		{
			facum += FRICTION_AIR;
			if(facum >= 1) {
				if(abs(lr_velocity) < facum)
					lr_velocity = 0;
				if(lr_velocity > 0)
					lr_velocity -= facum;
				else if(lr_velocity < 0)
					lr_velocity += facum;
				facum -= 1;
			}
		}
	}
	
	
	if (!onTheGround && mainMap->onTheGround(bounds)) {
		applyNaturalForce();
	}
	
	//move for solid
	point rp = mainMap->solid(bounds, !onTheGround);
	bounds.position.translate(rp);
	if (rp.y != 0) {
		applyNaturalForce();
	}
	if (rp.x > 8 || rp.x < -8) {
		if(!elastic)
			lr_velocity = 0;
		else {
			lr_velocity *= -1;
		}

	}
	
	
	//constrain
	bounds.position.x = std::max(1, std::min(bounds.position.x, (int)mainMap->worldWidth() * TILESIZE - bounds.width - TILESIZE));
	bounds.position.y = std::max(1, std::min(bounds.position.y, (int)mainMap->worldHeight() * TILESIZE - bounds.height));

}

rectangle Sprite::adj_bounds() {
	return bounds;
}

bool Sprite::containsAdjRect(rectangle p) {
	
	//cout << adj_bounds().width;
	
	rectangle temp = p.intersectingRectangle(adj_bounds());
	return (temp.width > 0 && temp.height > 0);
}

void Sprite::applyNaturalForce() {
	ud_velocity = 0;
}

Sprite::~Sprite() {

}

Sprite::Sprite() {
	h_flipped = false;
	v_flipped = false;
	ud_velocity = 0;
	lr_velocity = 0;
	type = 0;
	frame = 0;
	hidden = false;
	_active = true;
}

Sprite::Sprite(TILESET* tileset, rectangle new_bounds) {
	t = tileset;
	bounds.width = new_bounds.width;
	bounds.height = new_bounds.height;
	bounds.position.x = new_bounds.position.x;
	bounds.position.y = new_bounds.position.y;
	h_flipped = false;
	v_flipped = false;
	type = 0;
	frame = 0;
	ud_velocity = 0;
	lr_velocity = 0;
	accessory = 0;
	access_count = 0;
	facum = 0;
	elastic = false;
	hidden = false;
	_active = true;
}