/*
 *  World.cpp
 *  erapr
 *
 *  Created by Remi Bernotavicius on 12/21/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Sprite.h"

World::World(Map* pmap, Player* plr, rectangle pscreen_rect)
{
	map = pmap;
	p1 = plr;
	screen_rect = pscreen_rect;
	
	spritesn = new vector<Sprite*>;
	
	//load and place enemies
	for (int i = 0; i < (int)map->sprites->size() ; i++) {
		if(map->sprites->at(i).type == 1)
		{
			Enemy* e = new Enemy(map->spriteTileset, rectangle(map->sprites->at(i).position.x, map->sprites->at(i).position.y - 1, SPRITE_W, SPRITE_H));
			spritesn->push_back((Sprite*)e);
		}
		else if(map->sprites->at(i).type == 170)
		{
			Item* it = new Item(map->spriteTileset, rectangle(map->sprites->at(i).position.x, map->sprites->at(i).position.y - 1, SPRITE_W, SPRITE_H));
			it->scoreValue = 100;
			spritesn->push_back(it);
		}
		else if(map->sprites->at(i).type == 137)
		{
			Block* b = new Block(map->spriteTileset, rectangle(map->sprites->at(i).position.x, map->sprites->at(i).position.y - 1, SPRITE_W, SPRITE_H));
			b->elastic = true;
			spritesn->push_back(b);
		}
		else
		{
			Sprite* s = new Sprite(map->spriteTileset, rectangle(map->sprites->at(i).position.x, map->sprites->at(i).position.y - 1, SPRITE_W, SPRITE_H));
			spritesn->push_back(s);
		}
		spritesn->at(spritesn->size() - 1)->type = map->sprites->at(i).type;
	}
	
	//set up projectile array
	proj = new vector<Projectile>;
}

void World::tick()
{
	move();
	collisionDetect();
}

void World::move()
{
	#ifdef DEBUG
		if(key[KEY_W])
		{
			Sprite::GRAVITY *= -1;
		}
	#endif
	
	//move the player
	p1->moveAll(map);
	
	for (int i = 0; i < (int)spritesn->size(); i++)
	{
		//only update if on screen
		bool on_screen = screen_rect.containsPoint(spritesn->at(i)->bounds.position.x - p1->mapx, spritesn->at(i)->bounds.position.y - p1->mapy);
		
		if(spritesn->at(i)->type == 137 //types to animate even when off screen
		   || on_screen)
		{
			spritesn->at(i)->moveAll(map);
			spritesn->at(i)->tick(p1, screen_rect);
			if(!spritesn->at(i)->active())
			{
				spritesn->erase(spritesn->begin() + i);
			}
		}
	}
	
	//move projectiles
	for (int i = 0; i < (int)proj->size(); i++) {
		proj->at(i).moveAll(map);
		
		if(!proj->at(i).active())
		{
			proj->erase(proj->begin() + i);
		}
	}
	
	//update map scrolling
	p1->mapx = std::min(std::max(0, p1->bounds.position.x - screen_rect.width / 2), (int)map->worldWidth() * TILESIZE - screen_rect.width);
	p1->mapy = std::min(std::max(0, p1->bounds.position.y - screen_rect.height / 2), (int)map->worldHeight() * TILESIZE - screen_rect.height);
	
}

void World::collisionDetect()
{
	//projectile-enemy collision detection
	
	for (int i = 0; i < (int)proj->size(); i++) {
		point p = proj->at(i).bounds.position;
		//cout << "(" << p.x << ", " << p.y << ")";
		
		//remove projectiles if they stop
		if(proj->at(i).getLRVelocity() == 0) {
			proj->erase(proj->begin() + i);
		}
		else
		{
			
			for(int j = 0; j < (int)spritesn->size(); j++) {
				if (spritesn->at(j)->type == 1 && ((Enemy*)spritesn->at(j))->bounds.containsPoint(p) && ((Enemy*)spritesn->at(j))->alive()) {
					p1->applyAccessory(3, 4, p);
					((Enemy*)spritesn->at(j))->kill();
					p1->score += ((Enemy*)spritesn->at(j))->scoreValue;
					proj->erase(proj->begin() + i);
				}
			}
		}
	}
	
	
	//sprite-sprite collision
	for(int i = 0; i < (int)spritesn->size(); i++) {
		
		for(int k = 0; k < (int)spritesn->size(); k++)
		{
			Block* b = (Block*)spritesn->at(k);
			rectangle r = spritesn->at(i)->bounds.intersectingRectangle(b->bounds);
			if(b->type == 137) //block
			{
				if (spritesn->at(i)->type == 1 && r.width > 0 && r.height > 0 && ((Enemy*)spritesn->at(i))->alive() 
					&& (abs(b->getUDVelocity()) > 0 || abs(b->getLRVelocity()) > 0)) {
					
					point p = b->bounds.position;
					p.translate(20, -20);
					p1->applyAccessory(3, 4, p);
					((Enemy*)spritesn->at(i))->kill();
					b->kill();
					p1->score += ((Enemy*)spritesn->at(i))->scoreValue;
				}
			}
		}
		
		//player - sprite collision
		if(spritesn->at(i)->containsAdjRect(p1->bounds))
		{
			if(spritesn->at(i)->type == 1 && ((Enemy*)spritesn->at(i))->alive())
			{
				if(p1->getUDVelocity() >= 0) {
					p1->event = DIE_EVENT;
					p1->lives--;
				} else {
					((Enemy*)spritesn->at(i))->kill();
					p1->score += ((Enemy*)spritesn->at(i))->scoreValue * 2;
					p1->bounce();
					p1->applyAccessory(3, 4, p1->bounds.position);
					if(key[KEY_SPACE])
						p1->jump();
				}
			}
			else if(spritesn->at(i)->type == 170 && !spritesn->at(i)->hidden)
			{
				spritesn->at(i)->hidden = true;
				p1->score += ((Item*)spritesn->at(i))->scoreValue;
				ScoreSprite* ss = new ScoreSprite(map->spriteTileset, rectangle(spritesn->at(i)->bounds.position.x, spritesn->at(i)->bounds.position.y, 10, 10));
				ss->type = 200;
				spritesn->push_back(ss);
			}
		}
		
		if(spritesn->at(i)->type == 200)
		{
			if(!((ScoreSprite*)spritesn->at(i))->alive())
				spritesn->erase(spritesn->begin() + i);
		}
	}
	
}

void World::addProjectile()
{
	if(proj->size() < 4) {
		//old angle: rand() % 180 + (p1->flipped ?  -90 : 90);
		Projectile p;
		p.fire((proj->size() * 20 * (p1->h_flipped ? -1 : 1) + (p1->h_flipped ?  -180 : 0)), 30);
		p.bounds.position.x = p1->bounds.position.x + (p1->h_flipped ? p1->bounds.width : 0);
		p.bounds.position.y = p1->bounds.position.y + (p1->bounds.height / 3);
		proj->push_back(p);
	}
}

void World::draw(BITMAP* dest)
{	
	//draw bullets
	for (int i = 0; i < (int)proj->size(); i++) {
		proj->at(i).draw(dest, -1 * p1->mapx, -1 * p1->mapy);
	}
	
	//draw sprites
	for (int i = 0; i < (int)spritesn->size(); i++) {
		if(!spritesn->at(i)->hidden)
		{
			spritesn->at(i)->draw(dest, -1 * p1->mapx, -1 * p1->mapy);
			#ifdef SPRITE_DEBUG
				textprintf_ex(dest, font, spritesn->at(i)->bounds.position.x - p1->mapx, spritesn->at(i)->bounds.position.y - p1->mapy, makecol(255, 0, 0), 0, "%d", spritesn->at(i)->type);
			#endif
		}
	}
	
	//draw the player
	p1->draw(dest, -1 * p1->mapx, -1 * p1->mapy);
	
	#ifdef DEBUG
		textprintf_ex(dest, font, 5, 40, makecol(255, 255, 255), -1, "bullets: %d", (int)proj->size());
		textprintf_ex(dest, font, 5, 50, makecol(255, 255, 255), -1, "position: %d, %d", p1->bounds.position.x, p1->bounds.position.y);
	#endif
}

World::~World()
{
	delete spritesn;
	delete proj;
}