#include <string>
#include <sstream>
#include "hud.h"	
#include "gameData.h"
#include "renderContext.h"

void writeText(TTF_Font* v, const std::string& msg, int x, int y, SDL_Color textColor, 
	SDL_Renderer* r)
{
	SDL_Surface* surface = TTF_RenderText_Solid(v, msg.c_str(), textColor);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(r, surface);

    int textWidth = surface->w;
    int textHeight = surface->h;
    SDL_FreeSurface(surface);
    SDL_Rect dst = {x, y, textWidth, textHeight};

    SDL_RenderCopy(r, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
}

hud& hud::getInstance()
{
	static hud instance;
	return instance;
}

hud::~hud()
{
	TTF_CloseFont(font);
	TTF_Quit();
}

hud::hud() : 
	r(Gamedata::getInstance().getXmlInt("hud/r")),
	g(Gamedata::getInstance().getXmlInt("hud/g")),
	b(Gamedata::getInstance().getXmlInt("hud/b")),
	a(Gamedata::getInstance().getXmlInt("hud/a")),
	font(TTF_OpenFont(Gamedata::getInstance().getXmlStr("font/file").c_str(),
					  Gamedata::getInstance().getXmlInt("hud/size"))),
	init(TTF_Init()),
	rend(RenderContext::getInstance().getRenderer()),
	clock(Clock::getInstance())
	
{}

void hud::draw(bool enable) const
{
	if(enable == true)
	{
		SDL_Rect re;
    	re.x = 0;
    	re.y = 0;
    	re.w = 400;
    	re.h = 180;
    	
    	SDL_SetRenderDrawColor( rend, r, g, b, a);
    	SDL_RenderDrawRect( rend, &re );
    	
    	SDL_Color red = {r, g, b, a};
    	std::string fps;
  		std::stringstream strm;
  		strm << "fps: " << clock.getFps();
  		fps = strm.str();
  		string msg1 = "Objective: Clean ocean by eating trash";
  		writeText(font, msg1, 5, 5, red, rend);
  		string msg2 = "ASDW - Move Shark Player";
  		writeText(font, msg2, 5, 35, red, rend);
  		string msg3 = "P - Pause Game";
  		writeText(font, msg3, 5, 65, red, rend);
  		string msg4 = "ESC/q - Quit Game";
  		writeText(font, msg4, 5, 95, red, rend);
  		string msg5 = "F1 - Toggle HUD";
  		writeText(font, msg5, 5, 125, red, rend);
  		writeText(font, fps, 5, 155, red, rend);

	}
}
