#include "main.hpp"
#include "Config.h"
#include <iostream>
#include <sstream>
class Smoke
{
private:
	// 2D velocity maps (current and previous)
	float * u;
	float * v;
	float * u_prev;
	float * v_prev;
	// density maps (current and previous)
	float * dens;
	float * dens_prev;
	float  visc = 1E-6f;
	float diff = 1E-5f;
public:
	Smoke();
	~Smoke();
	void init();
	void update(float * u, float * v, float * u_prev, float * v_prev, float elapsed, float * dens, float * dens_prev);
	void render(TCODImage * img);
	void advect(int b, float * d, float * d0, float * u, float * v, float dt);
	void project(float * u, float * v, float * p, float * div);
	void add_source(float * x, float * s, float dt);
	void diffuse(int b, float * x, float * x0, float diff, float dt);
	void update_density(float * x, float * x0, float * u, float * v, float diff, float dt);
	void update_velocity(float * u, float * v, float * u0, float * v0, float visc, float dt);
	void set_bnd(int b, float * x);
};

