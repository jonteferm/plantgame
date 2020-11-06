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
	vector<Node> path;
	int steps = 0;
	float stepDelay = 0.1f;
public:
	Enemy(int x, int y);
	~Enemy();
	void render();
	void update(vector<Node> path, float &elapsed);
	void move(float &elapsed);
	int getX();
	int getY();
	int getSteps();
	vector<Node> getPath();
};

