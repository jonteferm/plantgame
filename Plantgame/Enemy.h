#include "main.hpp"
#include "Config.h"
#include <iostream>;
#include <sstream>;
#include <vector>;

using namespace std;

class Enemy
{
private:
	int x;
	int y;
	float v = 1.0f;
	void move();
	vector<Node> path;
	int steps = 0;

public:
	Enemy(int x, int y);
	~Enemy();
	void render();
	void update(vector<Node> path);
	int getX();
	int getY();
	int getSteps();
	vector<Node> getPath();
};

