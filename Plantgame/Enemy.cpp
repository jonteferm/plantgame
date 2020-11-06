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

void Enemy::update(vector<Node> path, float &elapsed) {
	this->path = path;
	this->steps = 0;
}

void Enemy::move(float &elapsed) {
	int steps = this->steps;

	this->stepDelay -= elapsed;

	if (this->stepDelay < 0.0f) {
		if (steps <= this->path.size()-1) {
			this->x = this->path[this->steps].x;
			this->y = this->path[this->steps].y;

			this->steps++;
		}
		this->stepDelay = 0.1f;
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