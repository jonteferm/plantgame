/*
* Copyright (c) 2010 Jice
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The name of Jice may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY JICE ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL JICE BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <stdio.h>
#include <math.h>
#include "Config.h"
#include "main.hpp"
#include <iostream>
#include <sstream>
#include "Smoke.h";
#include "Bullet.h";
#include <vector>;

// gas simulation
// based on Jos Stam, "Real-Time Fluid Dynamics for Games". Proceedings of the Game Developer Conference, March 2003.
// http://www.dgp.toronto.edu/people/stam/reality/Research/pub.html




float force=40.0f;
float source=5000.0f;
float stepDelay=0.05f;
float shotDelay = 0.2f;

int playerx=N/4,playery=N/4;

char objPos[SIZE][SIZE];

Smoke *smoke;
TCODImage *img;
std::vector<Bullet> shotsFired;

float u[SIZE], v[SIZE], u_prev[SIZE], v_prev[SIZE];
float dens[SIZE], dens_prev[SIZE];

void get_from_UI ( float * d, float * u, float * v, float elapsed, TCOD_key_t k,TCOD_mouse_t mouse ) {
	int i,j;
	float vx=0.0f,vy=0.0f;
	shotDelay -= elapsed;
	stepDelay -= elapsed;
	if ( stepDelay < 0.0f ) {
		if (toupper(k.c) == 'W' && playery > 0 ) {
			playery--;
			vy -= force;
		} 
		if (toupper(k.c) == 'S' && playery < N/2-1 ) {
			playery++;
			vx += force;
		} 
		if (toupper(k.c) == 'A' && playerx > 0 ) {
			playerx--;
			vx -= force;
		}
		if (toupper(k.c) == 'D' && playerx < N/2-1 ) {
			playerx++;
			vx += force;
		}
		stepDelay=0.05f; 
	}

	for ( i=0 ; i<SIZE; i++ ) {
		u[i] = v[i] = d[i] = 0.0f;
	}

	if (TCODConsole::isKeyPressed(TCODK_1)) {
		int posX = playerx;
		int posY = playery;

		if(objPos[posX][posY] == '\0') objPos[posX][posY] = 'p';
	}

	if ( !mouse.lbutton && !mouse.rbutton ) return;

	i = mouse.cx*2;
	j = mouse.cy*2;
	if ( i<1 || i>N || j<1 || j>N ) return;
	
	if ( mouse.rbutton ) {
		float dx,dy,l;
		dx=mouse.cx-playerx;
		dy=mouse.cy-playery;
		l=sqrt(dx*dx+dy*dy);

		if ( l > 0 ) {
			l = 1.0f/l;
			dx*=l;
			dy*=l;
			
			u[IX(playerx*2,playery*2)] = force * dx;
			v[IX(playerx*2,playery*2)] = force * dy;
			d[IX(playerx*2,playery*2)] = source;
		}
	}
	if (shotDelay < 0.0f) {
		if (mouse.lbutton) {
			shotsFired.push_back(Bullet());
		}
	}

}

void update(float elapsed, TCOD_key_t k, TCOD_mouse_t mouse) {
	get_from_UI(dens_prev,u_prev,v_prev, elapsed,k,mouse);
	smoke->update(u, v, u_prev, v_prev, elapsed, dens, dens_prev);

	for (std::vector<int>::size_type i = 0; i != shotsFired.size(); i++) {
		shotsFired[i].update();
	}
}

void render() {
	static TCODColor fire = TCODColor::lightAmber;
	
	smoke->render(img);
	img->blit2x(TCODConsole::root, 0, 0);

	TCODConsole::root->print(2,HEIGHT-2,"%4d fps", TCODSystem::getFps());
	TCODConsole::root->setDefaultForeground(TCODColor::white);
	TCODConsole::root->putChar(playerx,playery,'@');

	for (int x = 0; x <= N; x++) {
		for (int y = 0; y <= N; y++) {
			char obj = objPos[x][y];
			if (obj != '\0')
			{
				TCODConsole::root->putChar(x, y, obj);
			}
		}
	}

	TCODConsole::root->putChar(playerx, playery, '@');
}


int main (int argc, char *argv[]) {
	// initialize the game window
	TCODConsole::initRoot(WIDTH,HEIGHT,"Plant Game", false,TCOD_RENDERER_SDL);
	TCODSystem::setFps(25);
	TCODMouse::showCursor(true);

	img = new TCODImage(WIDTHx2, HEIGHTx2);
	smoke = new Smoke();
	smoke->init();

	bool endCredits=false;


	while (!TCODConsole::isWindowClosed()) {
		TCOD_key_t k;
		TCOD_mouse_t mouse;

		TCODSystem::checkForEvent(TCOD_EVENT_KEY|TCOD_EVENT_MOUSE, &k, &mouse);
/*
		v_prev[IX(N/2,0)] = 1.0f;
		u_prev[IX(N/3,N/3)]=1.0f;
		dens_prev[IX(N/2,0)] = 128.0f;
*/
		if ( k.vk == TCODK_PRINTSCREEN ) {
			// screenshot
			if (! k.pressed ) TCODSystem::saveScreenshot(NULL);
			k.vk=TCODK_NONE;
		} else if ( k.lalt && (k.vk == TCODK_ENTER || k.vk == TCODK_KPENTER) ) {
			// switch fullscreen
			if (! k.pressed ) TCODConsole::setFullscreen(!TCODConsole::isFullscreen());
			k.vk=TCODK_NONE;
		}
		// update the game
		update(TCODSystem::getLastFrameLength(),k,mouse);

		// render the game screen
		render();
		TCODConsole::root->print(5,99,"Arrows to move, left mouse button to shoot, 1 = plant seed");
		// render libtcod credits
		if (! endCredits ) endCredits = TCODConsole::renderCredits(4,4,true);

		// flush updates to screen
		TCODConsole::root->flush();
	}


	return 0;	
}
