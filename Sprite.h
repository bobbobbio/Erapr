/*
 *	erapr
 *     ERAPR Really Ain't Playable, Right?
 *
 *	Created by Remi Bernotavicius 
 *	Copyright (c) 2009 Remi Bernotavicius. All rights reserved.
 */

#include <Allegro/Allegro.h>
#include "Map.h"
#include <math.h>
#include <iostream>

#define DEBUG
//#define SPRITE_DEBUG

#define FRICTION 0.3
#define FRICTION_AIR 0.2

#define UD 1 //(Up/Down)
#define LR 2 //(Left/Right)

class Sprite
{
public:
	rectangle bounds;
	void draw(BITMAP* drw); //draws the sprite to the bitmap
	virtual void draw(BITMAP* drw, int dx, int dy);
	int frame; //current frame of tyleset
	Sprite(TILESET* tileset, rectangle new_bounds);
	Sprite();
	void move(Map* mainMap, int dir); //updates the sprite's position and applys gravity and solids, with the passed map
	
	bool h_flipped; //when set to true it flips the image when drawn
	bool v_flipped;
	
	void applyVelocity(int nv, int nd); //applys a velocity to the sprite
										//direction of UD (Up/Down) or LR (Left/Right)
	int getUDVelocity();
	int getLRVelocity();
	void applyNaturalForce();//stops the sprite, ie clears all velocity
	virtual void tick(Sprite* p, rectangle screen_rect);
	virtual void moveAll(Map* mainMap);
	void destroy();
	bool active();
	
	bool containsAdjRect(rectangle p);
	void applyAccessory(int acc, int time, point p);
	bool elastic;
	int type;
	bool hidden;
	virtual ~Sprite();
	
	static int GRAVITY;
	
protected:
	virtual rectangle adj_bounds();
	TILESET* t;
	int lr_velocity;
	int ud_velocity;
	int accessory;
	point access_p;
	int access_count;
	double facum;
	bool _active;

};

#include "Player.h"
#include "Block.h"
#include "ScoreSprite.h"
#include "Item.h"
#include "Projectile.h"
#include "Enemy.h"
#include "World.h"


