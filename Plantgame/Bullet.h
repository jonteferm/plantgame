#include "main.hpp"
#include "Config.h"
#include <iostream>
#include <sstream>
class Bullet
{
private:
	// 2D velocity maps (current and previous)
	int x;
	int y;
	int dir;
	float v = 1.0f;
public:
	Bullet(int dir, int x, int y);
	~Bullet();
	void update(float elapsed);
	void render(TCODImage * img);

};
