//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <cstdio>  
#include <map>  
#include <string>


class Sprite {

public:

  int x;
  int y;
  Sprite() : x(0), y(0) { }

};

class Game {
  // Constants
  static const int          DISPLAY_WIDTH = 480;
  static const int          DISPLAY_HEIGHT = 320;
  static const int          HERO_SPEED = 2;

  static const float        UPDATE_INTERVAL;

  static const std::string  MEDIA_PATH;

public:

  Game();
  ~Game();
  void Start();
  void Stop();

  // Render manager
  void Draw();
  void FillRect(SDL_Rect* rc, int r, int g, int b);

  void Run();
  void Update();

  // Time manager
  void FPSChanged(int fps);

  // Input Manager
  void EventManagement();

  void OnQuit();
  void OnKeyDown(SDL_Event* event);
  void OnKeyUp(SDL_Event* event);

private:

  std::map<int, int>  mKeys; // No SDLK_LAST. SDL2 migration guide suggests std::map  
  int                 mRunning;
  SDL_Window         *mWindow;
  SDL_Renderer       *mRenderer;
  Sprite              mHero;

  
  SDL_Surface        *mScreenSurface  = NULL;   // The surface contained by the window
  SDL_Surface        *mScratchSurface = NULL;   // Surface to use
  SDL_Texture        *mScratchTexture = NULL;   // Texture to use


};

/*************************************************************************************/
/*************************************************************************************/
/***********************************   CPP   *****************************************/
/*************************************************************************************/
/*************************************************************************************/

const float         Game::UPDATE_INTERVAL = 1000.0f / 60.0f;
const std::string   Game::MEDIA_PATH = "../Media/";

Game::Game() :
  mRunning(0), mWindow(NULL), mRenderer(NULL)
{
}

Game::~Game()
{
  Stop();
}

void Game::Start()
{
  int flags = SDL_WINDOW_SHOWN;
  if (SDL_Init(SDL_INIT_EVERYTHING)) {
    return;
  }

  if (SDL_CreateWindowAndRenderer(DISPLAY_WIDTH, DISPLAY_HEIGHT, flags, &mWindow, &mRenderer)) {
    return;
  }

  // Screen surface
  mScreenSurface = SDL_GetWindowSurface(mWindow);
  if (mScreenSurface == NULL) {
    return;
  }

  // Load BMP
  mScratchSurface = SDL_LoadBMP( (Game::MEDIA_PATH + "Scratch.bmp").c_str() );
  if (mScratchSurface == NULL)
  {
    return;
  }

  mScratchTexture = SDL_CreateTextureFromSurface(mRenderer, mScratchSurface);
  if (mScratchTexture == NULL) {
    return;
  }

  mRunning = 1;
  Run();
}

void Game::Draw()
{
  // RENDER USING RENDERER

  // Clear screen  
  SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(mRenderer);

  //// Render hero  
  SDL_Rect heroRect;
  heroRect.x = mHero.x;
  heroRect.y = mHero.y;
  heroRect.w = 20;
  heroRect.h = 20;
  FillRect(&heroRect, 255, 0, 0);

  // Render Scratch
  SDL_Rect scracthRect;
  scracthRect.x = mHero.x + 100;
  scracthRect.y = mHero.y + 100;
  scracthRect.w = 75; // Scale
  scracthRect.h = 75; // Scale
  SDL_RenderCopy(mRenderer, mScratchTexture, NULL, &scracthRect);

  SDL_Rect scracthRect2;
  scracthRect2.x = mHero.x + 200;
  scracthRect2.y = mHero.y + 200;
  scracthRect2.w = 75; // Scale
  scracthRect2.h = 75; // Scale  
  SDL_RenderCopy(mRenderer, mScratchTexture, NULL, &scracthRect2);

  SDL_RenderPresent(mRenderer);



  // RENDER USING SURFACES
  //SDL_Rect scracthRect;
  //scracthRect.x = mHero.x + 100;
  //scracthRect.y = mHero.y + 100;
  //scracthRect.w = 75; // Scale
  //scracthRect.h = 75; // Scale
  //SDL_BlitScaled(mScratchSurface, NULL, mScreenSurface, &scracthRect);

  //SDL_Rect scracthRect2;
  //scracthRect2.x = mHero.x + 200;
  //scracthRect2.y = mHero.y + 200;
  //scracthRect2.w = 75; // Scale
  //scracthRect2.h = 75; // Scale
  //SDL_BlitScaled(mScratchSurface, NULL, mScreenSurface, &scracthRect2);
    
  ////Update the surface
  //SDL_UpdateWindowSurface(mWindow);

}

void Game::Stop()
{
  if (NULL != mRenderer) {
    SDL_DestroyRenderer(mRenderer);
    mRenderer = NULL;
  }
  if (NULL != mWindow) {
    SDL_DestroyWindow(mWindow);
    mWindow = NULL;
  }
  SDL_Quit();
}

void Game::FillRect(SDL_Rect* rc, int r, int g, int b)
{
  SDL_SetRenderDrawColor(mRenderer, r, g, b, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(mRenderer, rc);
}

void Game::FPSChanged(int fps)
{
  //sprintf(szFps, "%s: %d FPS", "SDL2 Base C++ - Use Arrow Keys to Move", fps);
  std::string title = std::string("Test - FPS = ") + std::to_string(fps);

  SDL_SetWindowTitle(mWindow, title.c_str());
}

// Input manager
void Game::EventManagement()
{
  SDL_Event event;

  if (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      OnQuit();
      break;
    case SDL_KEYDOWN:
      OnKeyDown(&event);
      break;
    case SDL_KEYUP:
      OnKeyUp(&event);
      break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEMOTION:
      break;
    }
  }
}

void Game::Run()
{
  // Time manager
  int past = SDL_GetTicks();
  int now = past;
  int pastFps = past;
  int fps = 0;

  while (mRunning) {
    int timeElapsed = 0;

    // Input Manager
    EventManagement();

    // update/draw  
    now = SDL_GetTicks();
    timeElapsed = now - past;
    if (timeElapsed >= UPDATE_INTERVAL) {
      past = now;

      Update();
      Draw();

      ++fps;
    }

    // fps  
    if (now - pastFps >= 1000) {
      pastFps = now;
      FPSChanged(fps);
      fps = 0;
    }
  }
}

void Game::Update()
{
  if (mKeys[SDLK_LEFT]) {
    mHero.x -= HERO_SPEED;
  }
  if (mKeys[SDLK_RIGHT]) {
    mHero.x += HERO_SPEED;
  }
  if (mKeys[SDLK_UP]) {
    mHero.y -= HERO_SPEED;
  }
  if (mKeys[SDLK_DOWN]) {
    mHero.y += HERO_SPEED;
  }
}


// Event or input
void Game::OnQuit()
{
  mRunning = 0;
}

// Input Manager
void Game::OnKeyDown(SDL_Event* evt)
{
  mKeys[evt->key.keysym.sym] = 1;
}
void Game::OnKeyUp(SDL_Event* evt)
{
  mKeys[evt->key.keysym.sym] = 0;
}




// MAIN
int main(int argc, char** argv)
{
  Game game;
  game.Start();
  return 0;
}


























////Using SDL and standard IO
//#include <SDL.h>
//#include <stdio.h>
//#include <cstdio>  
//#include <map>  
//
//#include <string>
//
//
//
//
//class Sprite {
//
//  public:
//
//    int x;
//    int y;
//    Sprite() : x(0), y(0) { }
//
//};
//
//class Game {
//    // Constants
//    static const int          DISPLAY_WIDTH     = 480;
//    static const int          DISPLAY_HEIGHT    = 320;
//    static const int          HERO_SPEED        = 2;
//
//    static const float        UPDATE_INTERVAL;
//
//    static const std::string  MEDIA_PATH;
//
//  public:
//
//    Game();
//    ~Game();
//    void Start();
//    void Stop();
//
//    // Render manager
//    void Draw();
//    void FillRect(SDL_Rect* rc, int r, int g, int b);
//    
//    void Run();
//    void Update();
//
//    // Time manager
//    void FPSChanged(int fps);
//
//    // Input Manager
//    void EventManagement();
//
//    void OnQuit();
//    void OnKeyDown(SDL_Event* event);
//    void OnKeyUp(SDL_Event* event);
//
//  private:
//
//    std::map<int, int>  mKeys; // No SDLK_LAST. SDL2 migration guide suggests std::map  
//    int                 mRunning;
//    SDL_Window*         mWindow;
//    SDL_Renderer*       mRenderer;
//    Sprite              mHero;
//
//};
//
///*************************************************************************************/
///*************************************************************************************/
///***********************************   CPP   *****************************************/
///*************************************************************************************/
///*************************************************************************************/
//
//const float         Game::UPDATE_INTERVAL = 1000.0f / 60.0f;
//const std::string   Game::MEDIA_PATH      = "../Media/";
//
//Game::Game() :
//  mRunning(0), mWindow(NULL), mRenderer(NULL)
//{
//}
//
//Game::~Game() 
//{
//  Stop();
//}
//
//void Game::Start()
//{
//  int flags = SDL_WINDOW_SHOWN;
//  if (SDL_Init(SDL_INIT_EVERYTHING)) {
//    return;
//  }
//
//  if (SDL_CreateWindowAndRenderer(DISPLAY_WIDTH, DISPLAY_HEIGHT, flags, &mWindow, &mRenderer)) {
//    return;
//  }
//
//  mRunning = 1;
//  Run();
//}
//
//void Game::Draw()
//{
//  SDL_Rect heroRect;
//
//  // Clear screen  
//  SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
//  SDL_RenderClear(mRenderer);
//
//  // Render hero  
//  heroRect.x = mHero.x;
//  heroRect.y = mHero.y;
//  heroRect.w = 20;
//  heroRect.h = 20;
//  FillRect(&heroRect, 255, 0, 0);
//
//  SDL_RenderPresent(mRenderer);
//}
//
//void Game::Stop()
//{
//  if (NULL != mRenderer) {
//    SDL_DestroyRenderer(mRenderer);
//    mRenderer = NULL;
//  }
//  if (NULL != mWindow) {
//    SDL_DestroyWindow(mWindow);
//    mWindow = NULL;
//  }
//  SDL_Quit();
//}
//
//void Game::FillRect(SDL_Rect* rc, int r, int g, int b)
//{
//  SDL_SetRenderDrawColor(mRenderer, r, g, b, SDL_ALPHA_OPAQUE);
//  SDL_RenderFillRect(mRenderer, rc);
//}
//
//void Game::FPSChanged(int fps)
//{
//  //sprintf(szFps, "%s: %d FPS", "SDL2 Base C++ - Use Arrow Keys to Move", fps);
//  std::string title = std::string("Test - FPS = ") + std::to_string( fps );
//
//  SDL_SetWindowTitle(mWindow, title.c_str() );
//}
//
//// Input manager
//void Game::EventManagement()
//{
//  SDL_Event event;
//
//  if (SDL_PollEvent(&event)) {
//    switch (event.type) {
//    case SDL_QUIT:
//      OnQuit();
//      break;
//    case SDL_KEYDOWN:
//      OnKeyDown(&event);
//      break;
//    case SDL_KEYUP:
//      OnKeyUp(&event);
//      break;
//    case SDL_MOUSEBUTTONDOWN:
//    case SDL_MOUSEBUTTONUP:
//    case SDL_MOUSEMOTION:
//      break;
//    }
//  }
//}
//
//void Game::Run()
//{
//  // Time manager
//  int past = SDL_GetTicks();
//  int now = past;
//  int pastFps = past;
//  int fps = 0;
//
//  while (mRunning) {
//    int timeElapsed = 0;
//    
//    // Input Manager
//    EventManagement();
//
//    // update/draw  
//    now = SDL_GetTicks();
//    timeElapsed = now - past;
//    if (timeElapsed >= UPDATE_INTERVAL) {
//      past = now;
//
//      Update();
//      Draw();
//
//      ++fps;
//    }
//
//    // fps  
//    if (now - pastFps >= 1000) {
//      pastFps = now;
//      FPSChanged(fps);
//      fps = 0;
//    }
//  }
//}
//
//void Game::Update()
//{
//  if (mKeys[SDLK_LEFT]) {
//    mHero.x -= HERO_SPEED;
//  }
//  if (mKeys[SDLK_RIGHT]) {
//    mHero.x += HERO_SPEED;
//  }
//  if (mKeys[SDLK_UP]) {
//    mHero.y -= HERO_SPEED;
//  }
//  if (mKeys[SDLK_DOWN]) {
//    mHero.y += HERO_SPEED;
//  }
//}
//
//
//// Event or input
//void Game::OnQuit()
//{
//  mRunning = 0;
//}
//
//// Input Manager
//void Game::OnKeyDown(SDL_Event* evt)
//{
//  mKeys[evt->key.keysym.sym] = 1;
//}
//void Game::OnKeyUp(SDL_Event* evt)
//{
//  mKeys[evt->key.keysym.sym] = 0;
//}
//
//
//
//
//// MAIN
//int main(int argc, char** argv)
//{
//  Game game;
//  game.Start();
//  return 0;
//}