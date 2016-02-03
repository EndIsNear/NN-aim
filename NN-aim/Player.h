#pragma once
#include "SDL.h"
#include "Vec2.h"
#include "SDLWrapper.h"
#include "NeuralNetwork.h"

class Player
{
public:
	Player(SDL_Texture * body, SDL_Texture * eye, Vector2 pos, Vector2 dir, float fovScale, float fovMar) :
		m_body(body), m_eye(eye), m_pos(pos), m_dir(dir), m_fovScale(fovScale), m_crnMargin(fovMar), m_index(-1)
	{
		int w;
		SDL_QueryTexture(body, NULL, NULL, &w, NULL);
		m_size = w / 2;
		m_fovScale *= w;
	}

	void draw(SDLWrapper * sdl);
	void move(float speed, const SDL_Rect& area);
	bool isInFov(const Vector2& pos, float radius);

	void setNetworkWeights(const std::vector<float>& weights);
	Move getMove(bool playerInFov, bool bulletInFov, bool canShoot, float fov) const;
	int getIndex() const { return m_index; }
	void setIndex(int index) { m_index = index; }

	Vector2 m_pos;
	Vector2 m_dir;
	float m_crnMargin;
private:
	int m_index;
	NeuralNetwork m_network;

	SDL_Texture * m_body;
	SDL_Texture * m_eye;

	int m_size;
	float m_fovScale;

	Vector2 m_leftAim;
	Vector2 m_rightAim;
};
