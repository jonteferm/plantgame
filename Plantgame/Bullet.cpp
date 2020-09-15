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

	if (this->dir == 8) this->y = this->y+=this->v;
	if (this->dir == 2) this->y = this->y-=this->v;


}

void Bullet::render(TCODImage * img) {
	
	img->putPixel(this->x, this->y, TCODColor::red);
}
