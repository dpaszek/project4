#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "twowaysprite.h"
#include "smartSprite.h"
#include "subjectSprite.h"
#include "player.h"
#include "gameData.h"
#include "engine.h"
#include "frameGenerator.h"

Engine::~Engine() { 
  for(auto& t : dumb)
  {
  	delete t;
  }
  for(auto& t : smart)
  {
  	delete t;
  }
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc.getRenderer() ),
  ocean("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  mount("mountains", Gamedata::getInstance().getXmlInt("mountains/factor")),
  viewport( Viewport::getInstance() ),
  player(new SubjectSprite("shark")),
  dumb(),
  smart(),
  makeVideo( false )
{
  player->setScale(1.5);
  int a = Gamedata::getInstance().getXmlInt("numberOfFish1");
  int b = Gamedata::getInstance().getXmlInt("numberOfStarfish");
  int c = Gamedata::getInstance().getXmlInt("numberOfTrash");
  dumb.reserve(a+b);
  smart.reserve(c);
  
  for(int i = 0; i < a; i++)
  {
  	dumb.push_back(new Sprite("fish1"));
  }
  
  for(int i = 0; i < b; i++)
  {
  	dumb.push_back(new Sprite("starfish"));
  	dumb[i]->setScale(.4);
  }
  
  Vector2f pos = player->getPosition();
  int w = player->getScaledWidth();
  int h = player->getScaledHeight();
  
  for(int i = 0; i < c; i++)
  {
  	smart.push_back(new SmartSprite("trash", pos, w, h));
  	player->attach(smart[i]);
  	smart[i]->setScale(.6);
  }
  
  
  
   
  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  ocean.draw();
  mount.draw();
  
  player->draw();

  for(auto& t : dumb)
  {
  	t->draw();
  }
  
  for(auto& t : smart)
  {
  	t->draw();
  }
  
  std::string fps;
  std::stringstream strm;
  
  strm << "fps: " << clock.getFps();
  fps = strm.str();
  io.writeText(fps, 20, 100);
  
  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  ocean.update();
  mount.update();
  player->update(ticks);
  
  for(auto& t : dumb)
  {
  	t->update(ticks);
  }
  
  for(auto& t : smart)
  {
  	t->update(ticks);
  }
  
  viewport.update(); // always update viewport last
}


void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
        static_cast<Player*>(player)->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        static_cast<Player*>(player)->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        static_cast<Player*>(player)->up();
      }
      if (keystate[SDL_SCANCODE_S]) {
        static_cast<Player*>(player)->down();
      }
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}
