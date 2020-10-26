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

void Enemy::update(vector<Node> path) {

	if (this->path.size() == 0) {
		this->path = path;
	}

	move();
}

void Enemy::move() {
	int steps = this->steps;

	if (steps <= this->path.size()) {
		this->x = this->path[this->steps].x;
		this->y = this->path[this->steps].y;

		this->steps++;
	}
}

int Enemy::getX() {
	return this->x;
}

int Enemy::getY() {
	return this->y;
}

int Enemy::getSteps() {
	return this->steps;
}

vector<Node> Enemy::getPath() {
	return this->path;
}