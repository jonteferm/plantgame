#include "Bullet.h"

Bullet::Bullet(int dir, int x, int y)
{
	this->dir = dir;
	this->x = x;
	this->y = y;
}


Bullet::~Bullet()
{
}

void Bullet::update(float elapsed) {

	if (dir == 6) {
		this->x = this->x += this->v;
	}
	else if (dir == 4) {
		this->x = this->x -= this->v;
	}
	else if (dir == 8) {
		this->y = this->y -= this->v;
	}
	else if (dir == 2) 
	{
		this->y = this->y += this->v;
	}
	else if (dir == 9) {
		this->y = this->y -= this->v;
		this->x = this->x += this->v;
	}
	else if (dir == 7) {
		this->y = this->y -= this->v;
		this->x = this->x -= this->v;
	}
	else if (dir == 3) {
		this->y = this->y += this->v;
		this->x = this->x += this->v;
	}
	else if (dir == 1) {
		this->y = this->y += this->v;
		this->x = this->x -= this->v;
	}

	this->distTrav++;
}

void Bullet::render(TCODImage * img) {
	
	img->putPixel(this->x, this->y, TCODColor::red);
}

int Bullet::getDistTravelled() {
	return this->distTrav;
}

int Bullet::getReach() {
	return this->reach;
}