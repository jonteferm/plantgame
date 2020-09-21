#include "main.hpp"
#include "Config.h"

class Enemy
{
private:
	int x;
	int y;
	float v = 1.0f;
	void move();
public:
	Enemy(int x, int y);
	~Enemy();
	void render();
	void update();
};

