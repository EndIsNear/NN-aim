#include <iostream>
#include "SDLWrapper.h"
#include "SDL.h"
#include "utils.h"
#include "Player.h"

int main(int argc, char * argv[])
{
	Vector2 bulletPos(450.f, 450.f);
	float bulletRad = 25.f;

	SDLWrapper sdl(640, 480);
	sdl.initSDL();

	Vector2 from(0.f,0.f), step(7.7f, 7.7f);
	step.normalize();

	SDL_Surface * sur = drawCircle(25, 0xff, 0, 0xff);
	SDL_Texture * p1 = sdl.createTex(sur);

	sur = drawCircle(25, 0xff, 0xff, 0x00);
	SDL_Texture * bullet = sdl.createTex(sur);


	sur = drawCircle(7, 0xff, 0xff, 0xff);
	SDL_Texture * eye = sdl.createTex(sur);
	Player player(p1, eye, from, step, 5.f);


	while (!sdl.quit())
	{

		sdl.checkForEvent();
		sdl.clear();

		//TODO: remove, only to check intersect algo
		sdl.drawTex(bullet, bulletPos);

		player.draw(&sdl);
		player.move(0.8f);
		player.isInFov(bulletPos, bulletRad);
		sdl.update();

		from += step;
		from.x = from.getIntX() % 640;
		from.y = from.getIntY() % 480;
		SDL_Delay(5);
	}
	return 0;
}
