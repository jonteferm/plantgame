#include "main.hpp"
#include "Config.h"
#include <iostream>
#include <sstream>
class Bullet
{
private:
	// 2D velocity maps (current and previous)
	float * u;
	float * v;
	float * u_prev;
	float * v_prev;
	
public:
	Bullet();
	~Bullet();
	void update();
	void render(TCODImage * img);
};