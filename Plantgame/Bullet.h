#include "main.hpp"
#include "Config.h"
#include <iostream>
#include <sstream>
class Bullet
{
private:
	int x;
	int y;
	int dir;
	float v = 2.0f;
	int distTrav = 0;
	int reach = 30;
public:
	Bullet(int dir, int x, int y);
	~Bullet();
	void update(float elapsed);
	void render(TCODImage * img);
	int getDistTravelled();
	int getReach();
};
