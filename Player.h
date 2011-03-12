/*
 *	erapr
 *     ERAPR Really Ain't Playable, Right?
 *
 *	Created by Remi Bernotavicius 
 *	Copyright (c) 2009 Remi Bernotavicius. All rights reserved.
 */

#define STEP 3
#define jump_velocity 24
#define slow_speed 8
#define fast_speed 12

class Player : public Sprite
{
public:
	Player(TILESET* tileset, rectangle new_bounds):Sprite(tileset, new_bounds) {lives = 5; score = 0; fast = false; mapx = 0; mapy = 0; event = NO_EVENT;};
	int score;
	int lives;
	void drawStats(BITMAP* bmp, int x, int y);
	bool fast;
	void move(Map* mainMap, int dir);
	
	void bounce();
	void jump();
	
	void move_left();
	void move_right();
	
	int mapx;
	int mapy;
	
	int event;
};