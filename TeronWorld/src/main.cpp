#include <stdio.h>
#include <chrono>
#include <algorithm>

#include <SDL.h>

#include "maths.h"
#include "graphics.h"
#include "render.h"


bool running = false;

Mesh cube;
mat4x4 projection_matrix;
float fTheta = 0.0f;
vec3 camera = { 0 };


void init(SDL_Window* window)
{
	/*cube.triangles = {
		// SOUTH
		{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		// EAST                                                      
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

		// NORTH                                                     
		{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

		// WEST                                                      
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

		// TOP                                                       
		{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

		// BOTTOM                                                    
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f }
	};*/
	if (!load_from_obj_file("resources/SpaceShipFighter.obj", &cube))
	{
		printf("Failed to load obj resource\n");
		return;
	}

	// Projection Matrix
	float fNear = 0.1f;
	float fFar = 1000.0f;
	float fFov = 90.0f;
	int width, height;

	SDL_GetWindowSize(window, &width, &height);
	float fAspectRatio = (float)height / (float)width;

	float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

	projection_matrix.m[0][0] = fAspectRatio * fFovRad;
	projection_matrix.m[1][1] = fFovRad;
	projection_matrix.m[2][2] = fFar / (fFar - fNear);
	projection_matrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
	projection_matrix.m[2][3] = 1.0f;
	projection_matrix.m[3][3] = 0.0f;
	
	running = true;
}

void destroy()
{
	
}

void update()
{
	/*SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	// project 3d point to 2d plane
	for (Triangle triangle : cube.triangles)
	{
		Triangle projected_triangle;
		multiply_mat_vec(&projection_matrix, &triangle.p[0], &projected_triangle.p[0]);
		multiply_mat_vec(&projection_matrix, &triangle.p[1], &projected_triangle.p[1]);
		multiply_mat_vec(&projection_matrix, &triangle.p[2], &projected_triangle.p[2]);

		// scale into view
		projected_triangle.p[0].x += 1.0f;
		projected_triangle.p[0].y += 1.0f;
		projected_triangle.p[1].x += 1.0f;
		projected_triangle.p[1].y += 1.0f;
		projected_triangle.p[2].x += 1.0f;
		projected_triangle.p[2].y += 1.0f;

		projected_triangle.p[0].x *= 0.5f * (float)window_width;
		projected_triangle.p[0].y *= 0.5f * (float)window_height;
		projected_triangle.p[1].x *= 0.5f * (float)window_width;
		projected_triangle.p[1].y *= 0.5f * (float)window_height;
		projected_triangle.p[2].x *= 0.5f * (float)window_width;
		projected_triangle.p[2].y *= 0.5f * (float)window_height;

		draw_triangle(renderer, projected_triangle);
	}*/
}


int main(int argc, char* argv[])
{
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	const int window_width = 1280, window_height = 720;
	
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(window_width, window_height, 0, &window, &renderer);

	if (!window || !renderer)
	{
		printf("Failed to use SDL2: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	init(window);

	auto last_time = std::chrono::steady_clock::now();
	while (running)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		//update();

		auto this_time = std::chrono::steady_clock::now();
		//std::chrono::duration<float> diff = last_time - this_time;
		float fElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(this_time - last_time).count();
		last_time = this_time;

		printf("dt = %f ms\n", fElapsedTime);

		// Set up rotation matrices
		mat4x4 matRotZ, matRotX;
		fTheta += 0.01f * fElapsedTime * M_PI / 6;

		// Rotation Z
		matRotZ.m[0][0] = cosf(fTheta);
		matRotZ.m[0][1] = sinf(fTheta);
		matRotZ.m[1][0] = -sinf(fTheta);
		matRotZ.m[1][1] = cosf(fTheta);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;

		// Rotation X
		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cosf(fTheta * 0.5f);
		matRotX.m[1][2] = sinf(fTheta * 0.5f);
		matRotX.m[2][1] = -sinf(fTheta * 0.5f);
		matRotX.m[2][2] = cosf(fTheta * 0.5f);
		matRotX.m[3][3] = 1;

		Mesh mesh_to_raster;
		for (Triangle triangle : cube.triangles)
		{
			Triangle projected_triangle, translated_triangle, rotatedZ_triangle, rotatedZX_triangle;

			// Rotate in Z-Axis
			multiply_mat_vec(&matRotZ, &triangle.p[0], &rotatedZ_triangle.p[0]);
			multiply_mat_vec(&matRotZ, &triangle.p[1], &rotatedZ_triangle.p[1]);
			multiply_mat_vec(&matRotZ, &triangle.p[2], &rotatedZ_triangle.p[2]);

			// Rotate in X-Axis
			multiply_mat_vec(&matRotX, &rotatedZ_triangle.p[0], &rotatedZX_triangle.p[0]);
			multiply_mat_vec(&matRotX, &rotatedZ_triangle.p[1], &rotatedZX_triangle.p[1]);
			multiply_mat_vec(&matRotX, &rotatedZ_triangle.p[2], &rotatedZX_triangle.p[2]);

			// Offset into the screen
			translated_triangle = rotatedZX_triangle;
			translated_triangle.p[0].z = rotatedZX_triangle.p[0].z + 8.0f;
			translated_triangle.p[1].z = rotatedZX_triangle.p[1].z + 8.0f;
			translated_triangle.p[2].z = rotatedZX_triangle.p[2].z + 8.0f;

			// find normals
			vec3 normal, line1, line2;
			
			line1.x = translated_triangle.p[1].x - translated_triangle.p[0].x;
			line1.y = translated_triangle.p[1].y - translated_triangle.p[0].y;
			line1.z = translated_triangle.p[1].z - translated_triangle.p[0].z;

			line2.x = translated_triangle.p[2].x - translated_triangle.p[0].x;
			line2.y = translated_triangle.p[2].y - translated_triangle.p[0].y;
			line2.z = translated_triangle.p[2].z - translated_triangle.p[0].z;

			normal.x = line1.y * line2.z - line1.z * line2.y;
			normal.y = line1.z * line2.x - line1.x * line2.z;
			normal.z = line1.x * line2.y - line1.y * line2.x;

			// normalize the normal
			float normal_length = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
			normal.x /= normal_length;
			normal.y /= normal_length;
			normal.z /= normal_length;

			if (
				normal.x * (translated_triangle.p[0].x - camera.x) +
				normal.y * (translated_triangle.p[0].y - camera.y) +
				normal.z * (translated_triangle.p[0].z - camera.z) < 0.0f
				)
			{
				// illumination
				vec3 light_direction = { 0.0f, 0.0f, -1.0f };
				// normalize the light direction vector
				float light_direction_length = sqrtf(
					light_direction.x * light_direction.x + light_direction.y * light_direction.y + light_direction.z * light_direction.z);
				light_direction.x /= light_direction_length;
				light_direction.y /= light_direction_length;
				light_direction.z /= light_direction_length;

				// How similar is normal to light direction
				float dot_product = normal.x * light_direction.x + normal.y * light_direction.y + normal.z * light_direction.z;
				translated_triangle.color = { 255.0f * dot_product, 255.0f * dot_product, 255.0f * dot_product };
				
				// project 3d point to 2d plane
				multiply_mat_vec(&projection_matrix, &translated_triangle.p[0], &projected_triangle.p[0]);
				multiply_mat_vec(&projection_matrix, &translated_triangle.p[1], &projected_triangle.p[1]);
				multiply_mat_vec(&projection_matrix, &translated_triangle.p[2], &projected_triangle.p[2]);

				projected_triangle.color = translated_triangle.color;

				// scale into view
				projected_triangle.p[0].x += 1.0f;
				projected_triangle.p[0].y += 1.0f;
				projected_triangle.p[1].x += 1.0f;
				projected_triangle.p[1].y += 1.0f;
				projected_triangle.p[2].x += 1.0f;
				projected_triangle.p[2].y += 1.0f;

				projected_triangle.p[0].x *= 0.5f * (float)window_width;
				projected_triangle.p[0].y *= 0.5f * (float)window_height;
				projected_triangle.p[1].x *= 0.5f * (float)window_width;
				projected_triangle.p[1].y *= 0.5f * (float)window_height;
				projected_triangle.p[2].x *= 0.5f * (float)window_width;
				projected_triangle.p[2].y *= 0.5f * (float)window_height;

				mesh_to_raster.triangles.push_back(projected_triangle);
			}
		}

		// sort the collection of triangles in the mesh from back to front (so the furthest visible triangles will be drawn first)
		std::sort(mesh_to_raster.triangles.begin(), mesh_to_raster.triangles.end(), [](Triangle &t1, Triangle &t2) {
			// find triangles' mid point
			float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
			float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;

			// so, if t1 is further from the camera, then swap
			return z1 > z2;
		});

		for (Triangle triangle : mesh_to_raster.triangles)
			fill_triangle(renderer, triangle);

		SDL_RenderPresent(renderer);

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
		}
	}

	destroy();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}