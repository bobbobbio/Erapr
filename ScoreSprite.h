/*
 *  ScoreSprite.h
 *  erapr
 *
 *  Created by Remi Bernotavicius on 10/13/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

class ScoreSprite : public Sprite
{
public:
	void moveAll(Map* mainMap);
	void draw(BITMAP* drw, int dx, int dy);
	ScoreSprite(TILESET* tileset, rectangle new_bounds):Sprite(tileset, new_bounds) { value = 100; life = 10;}
	int value;
	bool alive();
private:
	int life;
};