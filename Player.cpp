/*
 *  Player.cpp
 *  erapr
 *
 *  Created by Remi Bernotavicius on 2/13/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Sprite.h"

void Player::drawStats(BITMAP* bmp, int x, int y) {
	
	textprintf_ex(bmp, font, x, y, makecol(255, 255, 255), -1, "score: %d", score);
	BITMAP* currentFrame = create_bitmap(bounds.width * 20 / bounds.height, 20);
	stretch_blit(t->image, currentFrame, 0, 0, bounds.width, bounds.height, 0, 0, currentFrame->w, currentFrame->h);
	draw_sprite(bmp, currentFrame, x, y + 10);
	textprintf_ex(bmp, font, x + 10, y + 20, makecol(255, 255, 255), -1, " x %d", lives);
	destroy_bitmap(currentFrame);
	
}

void Player::move(Map* mainMap, int dir) {
	if(lr_velocity == 0)
		frame = 0;
	if(ud_velocity != 0)
		frame = 2;
	Sprite::move(mainMap, dir);
}

void Player::move_left() {
	if(lr_velocity > -(fast ? fast_speed : slow_speed))
		applyVelocity(-STEP, LR);
	h_flipped = true;
	frame = ((bounds.position.x / 30) % 3);
}

void Player::bounce() {
	ud_velocity *= -1;
}

void Player::jump() {
	applyVelocity(jump_velocity, UD);
}

void Player::move_right() {
	if(lr_velocity < (fast ? fast_speed : slow_speed))
		applyVelocity(STEP, LR);
	h_flipped = false;
	frame = ((bounds.position.x / 30) % 3);
}