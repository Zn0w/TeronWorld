#pragma once

#include <vector>
#include <fstream>
#include <strstream>

#include "maths.h"


struct Triangle
{
	vec3 p[3];
	vec3 color;
};

struct Mesh
{
	std::vector<Triangle> triangles;
};

bool load_from_obj_file(const char* filename, Mesh* mesh)
{	
	std::ifstream file(filename);
	if (!file.is_open())
		return false;

	// local cache of vetirces
	std::vector<vec3> vertices;

	while (!file.eof())
	{
		char line[256];
		file.getline(line, 256);

		std::strstream s;
		s << line;

		char type;	// will store the first character (it corresponds to type), but will not be used (just for the strstream object)
		if (line[0] == 'v')	// vertex
		{
			vec3 vertex;
			s >> type >> vertex.x >> vertex.y >> vertex.z;
			vertices.push_back(vertex);
		}
		else if (line[0] == 'f')	// face
		{
			int f[3];
			s >> type >> f[0] >> f[1] >> f[2];
			mesh->triangles.push_back({ vertices[f[0] - 1], vertices[f[1] - 1], vertices[f[2] - 1] });
		}
	}
	
	return true;
}