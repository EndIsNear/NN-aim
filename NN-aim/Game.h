#pragma once
#include "Player.h"
#include "SDLWrapper.h"
#include "utils.h"
#include "NeuralNetwork.h"

//some game constants
const int width = 960;
const int height = 720;
const int bufferZoneSize = 100;
const int playerFieldWidth = (width - bufferZoneSize) / 2;


//size in pixels
const float playerRad = 25.f;
const float playerEyeRad = 5.f;
const float bulletRad = 5.f;
const float fovLen = 16.f;
const float fovMargin = 0.05f;
const float minFovMargin = 0.03f;
const float maxFovMargin = 0.7f;

//max speed
//offset = speed * dir
const float playerSpeed = 2.f;
const float bulletSpeed = 4.f;
const float rotStep = 0.02f;

//start pos
const Vector2 leftStPos(100.f, height/2);
const Vector2 rightStPos(width - 100, height/2);
const Vector2 leftStDir(1.f, 0.f);
const Vector2 rightStDir(-1.f, 0.f);

//game limit in frames
const int maxGameFrames = 2000;

struct Bullet
{
	Bullet() : active(false) {}
	Vector2 pos;
	Vector2 dir;
	bool active;
};


class Game
{
public:
	Game(int maxFrames = maxGameFrames);
	~Game();
	bool init(SDLWrapper * sdl);

	void draw();
	void move();
	void reset();
	void getNNRating(int& left, int& right);
	bool end();

	//players are 0 and 1
	void makeMove(const int player);
	void move(int player);
	void turnLeft(int player);
	void turnRight(int player);
	void changeFov(int player, float mult);
	void shoot(int player);

	void setNetworkWeights(int player, const std::vector<float>& weights);
	int getPlayerIndex(int player) const { return m_players[player]->getIndex(); }
	void setPlayerIndex(int player, int index) { m_players[player]->setIndex(index); }

	bool playerInFov(int player);
	bool bulletInFov(int player);
	float currentFov(int player);
	bool canShoot(int player);

	Player * m_players[2];
private:
	SDLWrapper * m_sdl;
	SDL_Texture * m_texBullet;
	SDL_Rect m_plAreas[2];
	//at any moment every player can shoot only 1 bullet
	Bullet m_bullets[2];
	int m_score[2];//game score
	int m_frameCnt;
	int m_bulletsCnt[2];
	const int m_maxGameFrames;

	void checkForHit();
	void remInactiveBullets();
	bool checkForColision(const Vector2& lPos, const Vector2& rPos, const float lRad, const float rRad);
	void drawScore();
};
