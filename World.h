/*
 *  World.h
 *  erapr
 *
 *  Created by Remi Bernotavicius on 12/21/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

class World
{
public:
	void tick();
	World(Map* pmap, Player* plr, rectangle pscreen_rect);
	void draw(BITMAP* dest);
	void addProjectile();
	virtual ~World();
private:
	Map* map;
	Player* p1;
	rectangle screen_rect;
	vector<Sprite*>* spritesn;
	vector<Projectile>* proj;
	void move();
	void collisionDetect();
};