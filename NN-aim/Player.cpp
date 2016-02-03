#include "Player.h"

#include "utils.h"

void Player::draw(SDLWrapper * sdl)
{
	sdl->drawLine(m_pos, m_leftAim, 0, 0xff, 0xff);
	sdl->drawLine(m_pos, m_rightAim, 0, 0xff, 0xff);

	sdl->drawTex(m_body, m_pos);
	sdl->drawTex(m_eye, m_pos + m_dir * static_cast<float>(m_size));
}

void Player::setNetworkWeights(const std::vector<float>& weights)
{
	m_network.setWeights(weights);
}

Move Player::getMove(bool playerInFov, bool bulletInFov, bool canShoot, float fov) const
{
	return m_network.calculateMove(playerInFov, bulletInFov, canShoot, fov);
}

void Player::move(float speed, const SDL_Rect& area)
{
	m_pos += (m_dir * speed);
	int crnLimit = area.x + m_size;
	if (m_pos.getIntX() < crnLimit) m_pos.x = static_cast<float>(crnLimit);
	crnLimit = area.y + m_size;
	if (m_pos.getIntY() < crnLimit) m_pos.y = static_cast<float>(crnLimit);
	crnLimit = area.x + area.w - m_size;
	if (m_pos.getIntX() > crnLimit) m_pos.x = static_cast<float>(crnLimit);
	crnLimit = area.y + area.h - m_size;
	if (m_pos.getIntY() > crnLimit) m_pos.y = static_cast<float>(crnLimit);

	Vector2 aim = m_pos + m_dir * m_fovScale;
	Vector2 offset = (m_pos - aim).perp();
	offset *= m_crnMargin;
	m_leftAim = aim + offset;
	m_rightAim = aim - offset;
}

bool Player::isInFov(const Vector2& pos, float radius)
{
	Vector2 tri[3] = {m_pos, m_leftAim, m_rightAim};
	bool res = intersectCircleTriangle(pos,radius, tri);
	return res;
}

