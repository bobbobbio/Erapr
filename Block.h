/*
 *	erapr
 *     ERAPR Really Ain't Playable, Right?
 *
 *	Created by Remi Bernotavicius 
 *	Copyright (c) 2009 Remi Bernotavicius. All rights reserved.
 */

#define X_OFFSET 10
#define KICK_STRENGTH 4

class Block : public Sprite {
public:
	Block(TILESET* tileset, rectangle new_bounds):Sprite(tileset, new_bounds) {held = false;_alive = true;};
	void transform(Player* p);
	void kick(Player* p, bool up);
	void tick(Sprite* p, rectangle screen_rect);
	void kill();
	void moveAll(Map* mainMap);
private:
	bool held;
	bool _alive;
};