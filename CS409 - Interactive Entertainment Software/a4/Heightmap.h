#pragma once

#include <string>
#include "ObjLibrary/DisplayList.h"
#include "ObjLibrary/TextureBmp.h"
#include "ObjLibrary/Vector3.h"
#include <vector>

using ObjLibrary::Vector3;

class HeightMap
{
public:
	HeightMap(){}
	HeightMap(const ObjLibrary::TextureBmp* heightsImage);

	void initDisplayList(const std::string& texFilename, float texOffset, float texRepeat);

	void draw() const;
	Vector3 getSurfaceNormal(double x, double z, const Vector3& scale) const;
	void drawNormalsGlobal(int gridSize, const Vector3& scale);
	void drawNormal(double x, double z, const Vector3& scale) const;

	unsigned int getSizeCells() const;
	float getCellSize() const;
	bool isInside(const Vector3 position) const;
	float getHeight(int x, int y) const;
	float getHeight(const Vector3& pos) const;

	float getWorldHeight(int x, int y) const;

private:
	ObjLibrary::DisplayList m_DisplayList;
	ObjLibrary::DisplayList* m_DrawHeightsDL;
	std::vector<std::vector<float>> m_Heights;

	float m_SizeOfCell;
	unsigned int m_SizeInCells;

	float m_HeightRange;
};

