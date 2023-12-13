#pragma once
#include <windows.h>
#include <vector>
#include <cmath>
#include <limits>
#include "Model.h"
#include "Matrix4.h"

class Engine
{
	//Model* model = NULL;
	std::vector<Model*> models;
	Matrix4 matrix;
	Matrix4 supMatrix;
	Matrix4 matProj;

	int width_ = 600;
	int height_ = 400;
	const int depth = 255;
	unsigned int activeModel_ = 0;

public:

	Engine();
	Engine(int width, int height);
	~Engine();

	void draw(HDC hdc);
	void rotX(float z);
	void rotY(float z);
	void rotZ(float z);
	void move(float x, float y, float z);
	void scale(float s);
	void updProjMatrix(float fNear, float fFar, float fFov, float scrH, float scrW);
	void drawLine(float _x1, float _y1, float _x2, float _y2, HDC hdc, COLORREF color);
	Vec3f centerM();
	void drawFilledTriangle(HDC hdc, Vec3i t0, Vec3i t1, Vec3i t2, float intansity);
	void setActive(int numM);
};