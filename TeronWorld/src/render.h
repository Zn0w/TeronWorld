#pragma once

#include <SDL.h>

#include "vector.h"


void draw_triangle(SDL_Renderer* renderer, Triangle triangle)
{
	SDL_RenderDrawLine(renderer, triangle.p[0].x, triangle.p[0].y, triangle.p[1].x, triangle.p[1].y);
	SDL_RenderDrawLine(renderer, triangle.p[1].x, triangle.p[1].y, triangle.p[2].x, triangle.p[2].y);
	SDL_RenderDrawLine(renderer, triangle.p[2].x, triangle.p[2].y, triangle.p[0].x, triangle.p[0].y);
}

void draw_mesh(SDL_Renderer* renderer, Mesh mesh)
{
	for (Triangle triangle : mesh.triangles)
		draw_triangle(renderer, triangle);
}