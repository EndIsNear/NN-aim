#pragma once
#include <SDL.h>

class SDLWrapper
{
public:
	SDLWrapper(const int width, const int height);
	~SDLWrapper();
	bool initSDL();

	bool quit();
	void checkForEvent();

	void update() { SDL_UpdateWindowSurface(m_window); }
	void setPixelColor(const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue);
	
	void setPixelsColor(const int x, const int y, const int size, const unsigned char red, const unsigned char green, const unsigned char blue);
private:
	SDL_Window * m_window;
	SDL_Surface * m_surface;
	int m_width;
	int m_height;

	//event vars
	bool m_quit;
};