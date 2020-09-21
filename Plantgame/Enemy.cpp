#include "Enemy.h"



Enemy::Enemy(int x, int y)
{
	this->x = x;
	this->y = y;
}


Enemy::~Enemy()
{
}

void Enemy::render() {
	TCODConsole::root->putChar(this->x, this->y, 'C');
}

void Enemy::update() {

}

void Enemy::move() {

}