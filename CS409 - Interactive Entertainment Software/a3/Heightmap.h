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

	unsigned int getSizeCells() const;
	float getCellSize() const;
	bool isInside(const Vector3 position) const;
	float getHeight(int x, int y) const;

private:
	ObjLibrary::DisplayList m_DisplayList;
	std::vector<std::vector<float>> m_Heights;

	float m_SizeOfCell;
	unsigned int m_SizeInCells;

	float m_HeightRange;
};

