/*
 * Maps.h
 *
 *  Created on: Dec 20, 2016
 *      Author: Carl
 */

#ifndef LOCAL_MAPS_H_
#define LOCAL_MAPS_H_


//Map size
class Map {
public:
	int x = 0;
	int y = 0;
	int w = 5022;
	int h = 5022;

	// Render map
	void render(SDL_Renderer* gRenderer, int camx, int camy);
};

#endif /* LOCAL_MAPS_H_ */
