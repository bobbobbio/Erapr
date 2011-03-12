/*
 *	erapr
 *     ERAPR Really Ain't Playable, Right?
 *
 *	Created by Remi Bernotavicius 
 *	Copyright (c) 2009 Remi Bernotavicius. All rights reserved.
 */

#define SPEED 3
#define RANGE 100
#define ENEMY_BUFFER 10

class Enemy : public Sprite
{
public:
	void moveAll(Map* mainMap); //updates the enemy's position and applys gravity and solids, with the passed map
	Enemy(TILESET* tileset, rectangle new_bounds):Sprite(tileset, new_bounds) {
		step_counter = 0; _alive = true; _draw = true; scoreValue = 100; frameCount = 4;}
	
	
	bool alive();
	void kill();
	void draw(BITMAP* drw, int dx, int dy);
	int scoreValue;
	int frameCount;
private:
	rectangle adj_bounds();
	bool _alive;
	bool _draw;
	int step_counter;
};