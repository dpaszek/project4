#include "hud.h"	
#include "gameData.h"

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
	init(TTF_Init())
{

}

void hud::draw(bool enable) const
{
	if(enable == true)
	{
		
	}
}
