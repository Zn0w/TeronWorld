#pragma once

#include <cmath>


struct vec3
{
	float x, y, z, w;
};

vec3 add(vec3* v1, vec3* v2)
{
	return { v1->x + v2->x, v1->y + v2->y, v1->z + v2->z };
}

vec3 subtract(vec3* v1, vec3* v2)
{
	return { v1->x - v2->x, v1->y - v2->y, v1->z - v2->z };
}

vec3 multiply(vec3* v, float s)
{
	return { v->x * s, v->y * s, v->z * s };
}

float dot_product(vec3* v1, vec3* v2)
{
	return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

float length(vec3* v)
{
	return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

vec3 normalize(vec3* v)
{
	float v_length = length(v);
	v->x /= v_length;
	v->y /= v_length;
	v->z /= v_length;
}

vec3 cross_product(vec3* v1, vec3* v2)
{
	vec3 v;
	v.x = v1->y * v2->z - v1->z * v2->y;
	v.y = v1->z * v2->x - v1->x * v2->z;
	v.z = v1->x * v2->y - v1->y * v2->x;
	return v;
}

// row-column
struct mat4x4
{
	float m[4][4] = { 0 };
};

vec3 multiply_mat_vec(mat4x4* m, vec3* i)
{
	vec3 v;
	v.x = i->x * m->m[0][0] + i->y * m->m[1][0] + i->z * m->m[2][0] + i->w * m->m[3][0];
	v.y = i->x * m->m[0][1] + i->y * m->m[1][1] + i->z * m->m[2][1] + i->w * m->m[3][1];
	v.z = i->x * m->m[0][2] + i->y * m->m[1][2] + i->z * m->m[2][2] + i->w * m->m[3][2];
	v.w = i->x * m->m[0][3] + i->y * m->m[1][3] + i->z * m->m[2][3] + i->w * m->m[3][3];
	return v;
}

mat4x4 make_identity_matrix()
{
	mat4x4 matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	return matrix;
}

mat4x4 make_rotation_X_matrix(float angle_rad)
{
	mat4x4 matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = cosf(angle_rad);
	matrix.m[1][2] = sinf(angle_rad);
	matrix.m[2][1] = -sinf(angle_rad);
	matrix.m[2][2] = cosf(angle_rad);
	matrix.m[3][3] = 1.0f;
	return matrix;
}

mat4x4 make_rotation_Y_matrix(float angle_rad)
{
	mat4x4 matrix;
	matrix.m[0][0] = cosf(angle_rad);
	matrix.m[0][2] = sinf(angle_rad);
	matrix.m[2][0] = -sinf(angle_rad);
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = cosf(angle_rad);
	matrix.m[3][3] = 1.0f;
	return matrix;
}

mat4x4 make_rotation_Z_matrix(float angle_rad)
{
	mat4x4 matrix;
	matrix.m[0][0] = cosf(angle_rad);
	matrix.m[0][1] = sinf(angle_rad);
	matrix.m[1][0] = -sinf(angle_rad);
	matrix.m[1][1] = cosf(angle_rad);
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	return matrix;
}

mat4x4 make_translation_matrix(float x, float y, float z)
{
	mat4x4 matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	matrix.m[3][0] = x;
	matrix.m[3][1] = y;
	matrix.m[3][2] = z;
	return matrix;
}

mat4x4 make_projection_matrix(float fFovDegrees, float fAspectRatio, float fNear, float fFar)
{
	float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
	mat4x4 matrix;
	matrix.m[0][0] = fAspectRatio * fFovRad;
	matrix.m[1][1] = fFovRad;
	matrix.m[2][2] = fFar / (fFar - fNear);
	matrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
	matrix.m[2][3] = 1.0f;
	matrix.m[3][3] = 0.0f;
	return matrix;
}

mat4x4 multiply_mat_mat(mat4x4* m1, mat4x4* m2)
{
	mat4x4 matrix;
	for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
			matrix.m[r][c] = m1->m[r][0] * m2->m[0][c] + m1->m[r][1] * m2->m[1][c] + m1->m[r][2] * m2->m[2][c] + m1->m[r][3] * m2->m[3][c];
	return matrix;
}