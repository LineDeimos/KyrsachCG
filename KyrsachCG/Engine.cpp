#include "Engine.h"

Engine::Engine() {
	updProjMatrix(0.1f, 1000.0f, 90.0f, 400.0f, 600.0f);
	models.push_back(new Model("obj/Prism_in.obj"));
	models.push_back(new Model("obj/Prism_in.obj"));
	move(0, 0, -8);
}

Engine::Engine(int width, int height) : width_(width), height_(height)
{
	updProjMatrix(0.1f, 1000.0f, 90.0f, static_cast<int>(height), static_cast<int>(width));
	models.push_back(new Model("obj/Prism_in.obj"));
	models.push_back(new Model("obj/Prism_in.obj"));
	move(-4, 0, -10);
	scale(0.25f);
	setActive(1);
	move(4, 0, -10);
	scale(0.25f);
	setActive(0);
}

Engine::~Engine()
{
}

void Engine::draw(HDC hdc)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB(0, 255, 0));
	Vec3f light_dir(0, 0, -1);
	for (int k = 0; k < models.size(); k++) {
		for (int i = 0; i < models[k]->nfaces(); i++) {
			std::vector<int> face = models[k]->face(i);
			Vec3f world_coords[3];
			Vec3i tris[3];
			for (int j = 0; j < 3; j++) {
				Vec3f v0 = models[k]->vert(face[j]);
				Vec3f v1 = models[k]->vert(face[(j + 1) % 3]);
				world_coords[j] = v0;
				matProj.applyMatrixToVector(v0);
				matProj.applyMatrixToVector(v1);
				int x0 = (v0.x + 1.) * 600 / 2.;
				int y0 = ((v0.y + 1.) * 400 / 2.);
				int x1 = (v1.x + 1.) * 600 / 2.;
				int y1 = ((v1.y + 1.) * 400 / 2.);
				//drawLine(x0, y0, x1, y1, hdc, RGB(0, 255, 0));
				tris[j] = Vec3i(x0, y0, v0.z);
				//MoveToEx(hdc, x0, y0, NULL);
				//LineTo(hdc, x1, y1);
			}
			Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
			n.normalize();
			float intensity = n * light_dir;
			if (intensity > 0) {
				drawFilledTriangle(hdc, tris[0], tris[1], tris[2], intensity);
			}
		}
	}
}

void Engine::rotX(float z)
{
	Vec3f c = centerM();
	matrix.setTranslationMatrix(-c.x, -c.y, -c.z);
	supMatrix.setTranslationMatrix(c.x, c.y, c.z);
	for (int i = 0; i < models[activeModel_]->nverts(); i++) {
		Vec3f v = models[activeModel_]->vert(i);
		matrix.applyMatrixToVector(v);
		models[activeModel_]->setVert(i, v);
	}
	matrix.setRotationMatrixX(z);
	matrix.multiply(supMatrix);
	for (int i = 0; i < models[activeModel_]->nverts(); i++) {
		Vec3f v = models[activeModel_]->vert(i);
		matrix.applyMatrixToVector(v);
		models[activeModel_]->setVert(i, v);
	}
	return;
}

void Engine::rotY(float z)
{
	Vec3f c = centerM();
	matrix.setTranslationMatrix(-c.x, -c.y, -c.z);
	supMatrix.setTranslationMatrix(c.x, c.y, c.z);
	for (int i = 0; i < models[activeModel_]->nverts(); i++) {
		Vec3f v = models[activeModel_]->vert(i);
		matrix.applyMatrixToVector(v);
		models[activeModel_]->setVert(i, v);
	}
	matrix.setRotationMatrixY(z);
	matrix.multiply(supMatrix);
	for (int i = 0; i < models[activeModel_]->nverts(); i++) {
		Vec3f v = models[activeModel_]->vert(i);
		matrix.applyMatrixToVector(v);
		models[activeModel_]->setVert(i, v);
	}
	return;
}

void Engine::rotZ(float z)
{
	Vec3f c = centerM();
	matrix.setTranslationMatrix(-c.x, -c.y, -c.z);
	supMatrix.setTranslationMatrix(c.x, c.y, c.z);
	for (int i = 0; i < models[activeModel_]->nverts(); i++) {
		Vec3f v = models[activeModel_]->vert(i);
		matrix.applyMatrixToVector(v);
		models[activeModel_]->setVert(i, v);
	}
	matrix.setRotationMatrixZ(z);
	matrix.multiply(supMatrix);
	for (int i = 0; i < models[activeModel_]->nverts(); i++) {
		Vec3f v = models[activeModel_]->vert(i);
		matrix.applyMatrixToVector(v);
		models[activeModel_]->setVert(i, v);
	}
	return;
}

void Engine::move(float x, float y, float z)
{
	matrix.setTranslationMatrix(x, y, z);
	for (int i = 0; i < models[activeModel_]->nverts(); i++) {
		Vec3f v = models[activeModel_]->vert(i);
		matrix.applyMatrixToVector(v);
		models[activeModel_]->setVert(i, v);
	}
	return;
}

void Engine::scale(float s)
{
	Vec3f c = centerM();
	matrix.setTranslationMatrix(-c.x, -c.y, -c.z);
	supMatrix.setTranslationMatrix(c.x, c.y, c.z);
	for (int i = 0; i < models[activeModel_]->nverts(); i++) {
		Vec3f v = models[activeModel_]->vert(i);
		matrix.applyMatrixToVector(v);
		models[activeModel_]->setVert(i, v);
	}
	matrix.setScalingMatrix(s);
	matrix.multiply(supMatrix);
	for (int i = 0; i < models[activeModel_]->nverts(); i++) {
		Vec3f v = models[activeModel_]->vert(i);
		matrix.applyMatrixToVector(v);
		models[activeModel_]->setVert(i, v);
	}
	return;
}

void Engine::updProjMatrix(float fNear, float fFar, float fFov, float scrH, float scrW)
{
	matProj.setProjectionMatrix(fNear, fFar, fFov, scrH, scrW);
}

void Engine::drawLine(float _x1, float _y1, float _x2, float _y2, HDC hdc, COLORREF color)
{
	int x1 = round(_x1);
	int x2 = round(_x2);
	int y1 = round(_y1);
	int y2 = round(_y2);
	int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
	int dy = abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2;
	int e2 = err;
	for (;;)
	{
		SetPixel(hdc, x1, y1, color);
		if (x1 == x2 && y1 == y2) break;
		e2 = err;
		if (e2 > -dx)
			err -= dy, x1 += sx;
		if (e2 < dy)
			err += dx, y1 += sy;
	}
}

Vec3f Engine::centerM()
{
	Vec3f tmp;
	for (int i = 0; i < models[activeModel_]->nverts(); i++)
		tmp = tmp + models[activeModel_]->vert(i);
	tmp.x = tmp.x / models[activeModel_]->nverts();
	tmp.y = tmp.y / models[activeModel_]->nverts();
	tmp.z = tmp.z / models[activeModel_]->nverts();
	return tmp;
}

void Engine::drawFilledTriangle(HDC hdc, Vec3i t0, Vec3i t1, Vec3i t2, float intensity)
{
	if (t0.y == t1.y && t0.y == t2.y) return;
	if (t0.y > t1.y) std::swap(t0, t1);
	if (t0.y > t2.y) std::swap(t0, t2);
	if (t1.y > t2.y) std::swap(t1, t2);
	int total_height = t2.y - t0.y;
	for (int i = 0; i < total_height; i++) {
		bool second_half = i > t1.y - t0.y || t1.y == t0.y;
		int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
		float alpha = (float)i / total_height;
		float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height;
		Vec3i A = t0 + Vec3f(t2 - t0) * alpha;
		Vec3i B = second_half ? t1 + Vec3f(t2 - t1) * beta : t0 + Vec3f(t1 - t0) * beta;
		if (A.x > B.x) std::swap(A, B);
		for (int j = A.x; j <= B.x; j++) {
			float phi = B.x == A.x ? 1. : (float)(j - A.x) / (float)(B.x - A.x);
			Vec3i P = Vec3f(A) + Vec3f(B - A) * phi;
			int idx = P.x + P.y * width_;
			if (P.x < 0 || P.x >= width_ || P.y < 0 || P.y >= height_) {
				continue;
			}
			SetPixel(hdc, P.x, P.y, RGB(intensity * 255, intensity * 255, intensity * 255));
		}
	}
}

void Engine::setActive(int numM)
{
	activeModel_ = numM;
}
