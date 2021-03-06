#include "Smoke.h"

Smoke::Smoke()
{

}


Smoke::~Smoke()
{
}

void Smoke::init(){
	this->u = new float[SIZE];
	this->v = new float[SIZE];
	this->u_prev = new float[SIZE];
	this->v_prev = new float[SIZE];
	this->dens = new float[SIZE];
	this->dens_prev = new float[SIZE];

	memset(u, 0, sizeof(float)*SIZE);
	memset(v, 0, sizeof(float)*SIZE);
	memset(u_prev, 0, sizeof(float)*SIZE);
	memset(v_prev, 0, sizeof(float)*SIZE);
	for (int i = 0; i < SIZE; i++) dens[i] = 0.0f;
	memcpy(dens_prev, dens, sizeof(float)*SIZE);
}


void Smoke::update(float *u, float *v, float *u_prev, float *v_prev, float elapsed, float *dens, float *dens_prev) {
	this->u = u;
	this->v = v;
	this->u_prev = u_prev;
	this->v_prev = v_prev;
	this->dens = dens;
	this->dens_prev = dens_prev;

	update_velocity(this->u, this->v, this->u_prev, this->v_prev, this->visc, elapsed);
	update_density(this->dens, this->dens_prev, this->u, this->v, this->diff, elapsed);
}


void Smoke::render(TCODImage * img) {
	static TCODColor smoke = TCODColor::lightGrey;
	static TCODColor grass = TCODColor::darkestGreen;
	for (int x = 0; x <= N; x++) {
		for (int y = 0; y <= N; y++) {
			float coef = (float)(dens[IX(x, y)] / 128.0f);
			coef = CLAMP(0.0f, 1.0f, coef);
			img->putPixel(x, y, TCODColor::lerp(grass, smoke, coef));
		}
	}
}

// update density map according to velocity map
// b : boundary width
// d : current density map
// d0 : previous density map
// u,v : current velocity map
// dt : elapsed time
void Smoke::advect(int b, float * d, float * d0, float * u, float * v, float dt) {
	int i0, j0, i1, j1;
	float x, y, s0, t0, s1, t1, dt0;

	dt0 = dt * N;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			x = i - dt0 * u[IX(i, j)];
			y = j - dt0 * v[IX(i, j)];
			if (x < 0.5) x = 0.5;
			if (x > N + 0.5) x = N + 0.5;
			i0 = (int)x;
			i1 = i0 + 1;
			if (y < 0.5) y = 0.5;
			if (y > N + 0.5) y = N + 0.5;
			j0 = (int)y;
			j1 = j0 + 1;
			s1 = x - i0;
			s0 = 1 - s1;
			t1 = y - j0;
			t0 = 1 - t1;
			d[IX(i, j)] = s0 * (t0*d0[IX(i0, j0)] + t1 * d0[IX(i0, j1)]) +
				s1 * (t0*d0[IX(i1, j0)] + t1 * d0[IX(i1, j1)]);
		}
	}
	set_bnd(b, d);
}

void Smoke::project(float * u, float * v, float * p, float * div) {
	float h = 1.0 / N;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			div[IX(i, j)] = -0.5*h*(u[IX(i + 1, j)] - u[IX(i - 1, j)] +
				v[IX(i, j + 1)] - v[IX(i, j - 1)]);
			p[IX(i, j)] = 0;
		}
	}
	set_bnd(0, div); set_bnd(0, p);

	for (int k = 0; k < 20; k++) {
		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= N; j++) {
				p[IX(i, j)] = (div[IX(i, j)] + p[IX(i - 1, j)] + p[IX(i + 1, j)] +
					p[IX(i, j - 1)] + p[IX(i, j + 1)]) / 4;
			}
		}
		set_bnd(0, p);
	}

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			u[IX(i, j)] -= 0.5*(p[IX(i + 1, j)] - p[IX(i - 1, j)]) / h;
			v[IX(i, j)] -= 0.5*(p[IX(i, j + 1)] - p[IX(i, j - 1)]) / h;
		}
	}
	set_bnd(1, u); set_bnd(2, v);
}

// update density map according to density sources
// x : density map
// s : density source map
// dt : elapsed time
void Smoke::add_source(float *x, float *s, float dt) {
	for (int i = 0; i < SIZE; i++) {
		x[i] += dt * s[i];
	}
}

// update density or velocity map for diffusion
// b : boundary width
// x : current density map
// x0 : previous density map
// diff : diffusion coef
// dt : elapsed time
void Smoke::diffuse(int b, float *x, float *x0, float diff, float dt) {
	float a = diff * dt*N*N;
	for (int k = 0; k < 20; k++) {
		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= N; j++) {
				x[IX(i, j)] = (x0[IX(i, j)] + a * (x[IX(i - 1, j)] + x[IX(i + 1, j)]
					+ x[IX(i, j - 1)] + x[IX(i, j + 1)])) / (1 + 4 * a);
			}
		}
		set_bnd(b, x);
	}
}

void Smoke::update_density(float * x, float * x0, float * u, float * v, float diff, float dt) {
	add_source(x, x0, dt);
	SWAP(x0, x); diffuse(0, x, x0, diff, dt);
	SWAP(x0, x); advect(0, x, x0, u, v, dt);
}

void Smoke::update_velocity(float * u, float * v, float * u0, float * v0, float visc, float dt) {
	add_source(u, u0, dt);
	add_source(v, v0, dt);
	SWAP(u0, u); diffuse(1, u, u0, visc, dt);
	SWAP(v0, v); diffuse(2, v, v0, visc, dt);
	project(u, v, u0, v0);
	SWAP(u0, u); SWAP(v0, v);
	advect(1, u, u0, u0, v0, dt); advect(2, v, v0, u0, v0, dt);
	project(u, v, u0, v0);
}

// set boundary conditions
void Smoke::set_bnd(int b, float * x) {
	for (int i = 1; i <= N; i++) {
		// west and east walls 
		x[IX(0, i)] = b == 1 ? -x[IX(1, i)] : x[IX(1, i)];
		x[IX(N + 1, i)] = b == 1 ? -x[IX(N, i)] : x[IX(N, i)];
		// boundary doesn't work on north and south walls...
		// dunno why...
		x[IX(i, 0)] = b == 1 ? -x[IX(i, 1)] : x[IX(i, 1)];
		x[IX(i, N + 1)] = b == 1 ? -x[IX(i, N)] : x[IX(i, N)];
	}
	// boundary conditions at corners
	x[IX(0, 0)] = 0.5*(x[IX(1, 0)] + x[IX(0, 1)]);
	x[IX(0, N + 1)] = 0.5*(x[IX(1, N + 1)] + x[IX(0, N)]);
	x[IX(N + 1, 0)] = 0.5*(x[IX(N, 0)] + x[IX(N + 1, 1)]);
	x[IX(N + 1, N + 1)] = 0.5*(x[IX(N, N + 1)] + x[IX(N + 1, N)]);
}





