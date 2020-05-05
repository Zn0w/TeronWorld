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

	projection_matrix = make_projection_matrix(fFov, fAspectRatio, fNear, fFar);
	
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

		// make transformation matrices
		mat4x4 Z_rotation_matrix, X_rotation_matrix;
		fTheta += 0.01f * fElapsedTime * M_PI / 6;
		Z_rotation_matrix = make_rotation_Z_matrix(fTheta);
		X_rotation_matrix = make_rotation_X_matrix(fTheta);

		mat4x4 translation_matrix = make_translation_matrix(0.0f, 0.0f, 8.0f);

		// combine transformation matrices into one world matrix
		mat4x4 world_matrix = make_identity_matrix();
		world_matrix = multiply_mat_mat(&Z_rotation_matrix, &X_rotation_matrix);
		world_matrix = multiply_mat_mat(&world_matrix, &translation_matrix);

		Mesh mesh_to_raster;
		for (Triangle triangle : cube.triangles)
		{
			Triangle projected_triangle, transformed_triangle;

			transformed_triangle.p[0] = multiply_mat_vec(&world_matrix, &triangle.p[0]);
			transformed_triangle.p[1] = multiply_mat_vec(&world_matrix, &triangle.p[1]);
			transformed_triangle.p[2] = multiply_mat_vec(&world_matrix, &triangle.p[2]);

			// find normals
			vec3 normal, line1, line2;
			
			line1 = subtract(&transformed_triangle.p[1], &transformed_triangle.p[0]);
			line2 = subtract(&transformed_triangle.p[2], &transformed_triangle.p[0]);

			normal = cross_product(&line1, &line2);

			normal = normalize(&normal);

			// Get Ray from triangle to camera
			vec3 camera_ray = subtract(&transformed_triangle.p[0], &camera);
			
			// If ray is aligned with normal, then triangle is visible
			if (dot_product(&normal, &camera_ray) < 0.0f)
			{
				// illumination
				vec3 light_direction = { 0.0f, 0.0f, -1.0f };
				light_direction = normalize(&light_direction);

				// How similar is normal to light direction
				float dp = max(0.1f, dot_product(&light_direction, &normal));
				transformed_triangle.fill_color = { 255.0f * dp, 255.0f * dp, 255.0f * dp };
				
				// project 3d point to 2d plane
				projected_triangle.p[0] = multiply_mat_vec(&projection_matrix, &transformed_triangle.p[0]);
				projected_triangle.p[1] = multiply_mat_vec(&projection_matrix, &transformed_triangle.p[1]);
				projected_triangle.p[2] = multiply_mat_vec(&projection_matrix, &transformed_triangle.p[2]);

				projected_triangle.fill_color = transformed_triangle.fill_color;

				// Scale into view, we moved the normalising into cartesian space
				// out of the matrix.vector function from the previous videos, so
				// do this manually
				projected_triangle.p[0] = divide(&projected_triangle.p[0], projected_triangle.p[0].w);
				projected_triangle.p[1] = divide(&projected_triangle.p[1], projected_triangle.p[1].w);
				projected_triangle.p[2] = divide(&projected_triangle.p[2], projected_triangle.p[2].w);

				// Offset vertices into visible normalized space
				vec3 offset_view = { 1,1,0 };
				projected_triangle.p[0] = add(&projected_triangle.p[0], &offset_view);
				projected_triangle.p[1] = add(&projected_triangle.p[1], &offset_view);
				projected_triangle.p[2] = add(&projected_triangle.p[2], &offset_view);

				projected_triangle.p[0].x *= 0.5f * (float)window_width;
				projected_triangle.p[0].y *= 0.5f * (float)window_height;
				projected_triangle.p[1].x *= 0.5f * (float)window_width;
				projected_triangle.p[1].y *= 0.5f * (float)window_height;
				projected_triangle.p[2].x *= 0.5f * (float)window_width;
				projected_triangle.p[2].y *= 0.5f * (float)window_height;

				// test of the outline rendering
				projected_triangle.outline_color = { 255.0f, 0.0f, 0.0f };
				//projected_triangle.render_outline = false;
				
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
			render(renderer, triangle);

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