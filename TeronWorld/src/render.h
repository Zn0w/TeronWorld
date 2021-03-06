#pragma once

#include <SDL.h>

#include "maths.h"
#include "graphics.h"


void draw_triangle(SDL_Renderer* renderer, Triangle& triangle)
{
	SDL_SetRenderDrawColor(renderer, triangle.outline_color.x, triangle.outline_color.y, triangle.outline_color.z, SDL_ALPHA_OPAQUE);

	SDL_RenderDrawLine(renderer, triangle.p[0].x, triangle.p[0].y, triangle.p[1].x, triangle.p[1].y);
	SDL_RenderDrawLine(renderer, triangle.p[1].x, triangle.p[1].y, triangle.p[2].x, triangle.p[2].y);
	SDL_RenderDrawLine(renderer, triangle.p[2].x, triangle.p[2].y, triangle.p[0].x, triangle.p[0].y);
}

static float max(float a, float b, float c)
{
	if (a > b && a > c)
		return a;
	else if (b > a && b > c)
		return b;
	else
		return c;
}

static float min(float a, float b, float c)
{
	if (a < b && a < c)
		return a;
	else if (b < a && b < c)
		return b;
	else
		return c;
}

void fill_triangle(SDL_Renderer* renderer, Triangle& triangle)
{
	SDL_SetRenderDrawColor(renderer, triangle.fill_color.x, triangle.fill_color.y, triangle.fill_color.z, SDL_ALPHA_OPAQUE);
	
	float max_x = max(triangle.p[0].x, triangle.p[1].x, triangle.p[2].x);
	float max_y = max(triangle.p[0].y, triangle.p[1].y, triangle.p[2].y);
	float min_x = min(triangle.p[0].x, triangle.p[1].x, triangle.p[2].x);
	float min_y = min(triangle.p[0].y, triangle.p[1].y, triangle.p[2].y);

	for (int y = min_y; y < max_y; y++) {
		for (int x = min_x; x < max_x; x++) {
			if ((triangle.p[0].x - triangle.p[1].x) * (y - triangle.p[0].y) - (triangle.p[0].y - triangle.p[1].y) * (x - triangle.p[0].x) > 0.0f &&
				(triangle.p[1].x - triangle.p[2].x) * (y - triangle.p[1].y) - (triangle.p[1].y - triangle.p[2].y) * (x - triangle.p[1].x) > 0 &&
				(triangle.p[2].x - triangle.p[0].x) * (y - triangle.p[2].y) - (triangle.p[2].y - triangle.p[0].y) * (x - triangle.p[2].x) > 0)
			{
				SDL_RenderDrawPoint(renderer, x, y);
			}
		}
	}
}

void render(SDL_Renderer* renderer, Triangle& triangle)
{
	if (triangle.render_fill)
		fill_triangle(renderer, triangle);
	if (triangle.render_outline)
		draw_triangle(renderer, triangle);
}

void draw_mesh(SDL_Renderer* renderer, Mesh mesh)
{
	for (Triangle triangle : mesh.triangles)
		draw_triangle(renderer, triangle);
}