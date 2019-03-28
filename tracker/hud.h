#ifndef HUD_H
#define HUD_H
#include <SDL_image.h>
#include <SDL.h>
#include "ioMod.h"
#include "gameData.h"

class hud
{

public:
	hud();
	~hud();
	void draw(bool) const;
	static hud& getInstance();
	
	hud(const hud&) = delete;
	hud& operator=(const hud&) = delete;
private:
	int r;
	int g;
	int b; 
	int a;
	TTF_Font* font;
	int init;
};

#endif
