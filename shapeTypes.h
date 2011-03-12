/*
 *	erapr
 *     ERAPR Really Ain't Playable, Right?
 *
 *	Created by Remi Bernotavicius 
 *	Copyright (c) 2009 Remi Bernotavicius. All rights reserved.
 */

#include <algorithm>

typedef struct point {
	int x;
	int y;
	point() {
		x = 0;
		y = 0;
	}
	point(int nx, int ny) {
		x = nx;
		y = ny;
	}
	void set_location(int nx, int ny) {
		x = nx;
		y = ny;
	}
	void translate(int dx, int dy) {
		x += dx;
		y += dy;
	}
	void translate(point np) {
		x += np.x;
		y += np.y;
	}
	bool equals(point p) {
		return p.x == x && p.y == y;
	}
};

typedef struct rectangle {
	point position;
	int width;
	int height;
	
	rectangle() {
		width = 0;
		height = 0;
	}
	
	bool containsPoint(point p) {
		return p.x >= position.x && p.x <= position.x + width && p.y >= position.y && p.y <= position.y + height;
	}
	
	bool containsPoint(int x, int y) {
		return x >= position.x && x <= position.x + width && y >= position.y && y <= position.y + height;
	}
	
	bool equals(rectangle r) {
		return r.width == width && r.height == height && r.position.equals(position);
	}
	
	rectangle intersectingRectangle(rectangle rect) {
		int x = std::max(position.x, rect.position.x);
		int y = std::max(position.y, rect.position.y);
		int r_width = std::min(position.x + width, rect.position.x + rect.width) - x;
		int r_height = std::min(position.y + height, rect.position.y + rect.height) - y;
		return rectangle(x, y, r_width, r_height);
	}
	
	rectangle(int x, int y, int nwidth, int nheight) {
		position.x = x;
		position.y = y;
		width = nwidth;
		height = nheight;
	}
};