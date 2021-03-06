#include "utils.h"
#include <cmath>
#include <algorithm>

void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	Uint8 *target_pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;
	*(Uint32 *)target_pixel = pixel;
}

void fill_circle(SDL_Surface *surface, int cx, int cy, int radius, Uint32 pixel)
{
	static const int BPP = 4;

	double r = (double)radius;

	for (double dy = 1; dy <= r; dy += 1.0)
	{
		double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
		int x = static_cast<int>(cx - dx);

		Uint8 *target_pixel_a = (Uint8 *)surface->pixels + (static_cast<int>(cy + r - dy)) * surface->pitch + x * BPP;
		Uint8 *target_pixel_b = (Uint8 *)surface->pixels + (static_cast<int>(cy - r + dy)) * surface->pitch + x * BPP;

		for (; x <= cx + dx; x++)
		{
			*(Uint32 *)target_pixel_a = pixel;
			*(Uint32 *)target_pixel_b = pixel;
			target_pixel_a += BPP;
			target_pixel_b += BPP;
		}
	}
}

SDL_Surface *  drawCircle(const float radius, const char red, const char green, const char blue)
{
	const int size = static_cast<int>((radius + 0.5f) * 2);
	SDL_Surface * result = SDL_CreateRGBSurface(0, size, size, 32, 0, 0, 0, 0);
	fill_circle(result, size / 2, size / 2, size/2, SDL_MapRGB(result->format, red, green, blue));

	return result;
}

bool pointInTriangle(const Vector2& pnt, const Vector2 tri[3])
{
	for (int i = 0; i < 3; ++i) {
		Vector2 normal = (tri[(i + 1) % 3] - tri[i]).perp();
		if ((pnt - tri[i]).dotProduct(normal) < 0.f)
			return false;
	}
	return true;
}

Vector2 closestPointOnSegment(Vector2 pnt, Vector2 a, Vector2 b) {

	Vector2 dir = b - a;
	float t = (pnt - a).dotProduct(dir) / dir.dotProduct(dir);
	t = std::max(0.f, t);
	t = std::min(1.f, t);

	return a + dir * t;

}

Vector2 closestPointOnTriangle(Vector2 p, Vector2 v[3]) {

	if (pointInTriangle(p, v))
		return p;

	float maxDist = 10000.f;
	Vector2 closest;

	for (int i = 0; i < 3; ++i)
	{
		Vector2 temp = closestPointOnSegment(p, v[i], v[(i + 1) % 3]);
		float dist = (temp - p).lengthSq();

		if (dist < maxDist)
		{
			maxDist = dist;
			closest = temp;
		}

	}
	return closest;
}

bool intersectCircleTriangle(Vector2 center, float radius, Vector2 v[3])
{
	Vector2 closest = closestPointOnTriangle(center, v);
	float dist = (center - closest).lengthSq();

	return dist < radius * radius;
}

