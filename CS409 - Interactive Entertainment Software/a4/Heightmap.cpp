#include <cmath>
#include <iostream>
#include <math.h>
#include <vector>

#include "ObjLibrary/TextureManager.h"
#include "GetGlut.h"

#include "HeightMap.h"
#include "Shapes.h"

HeightMap::HeightMap(const ObjLibrary::TextureBmp* heightsImage)
{
	m_SizeInCells = heightsImage->getWidth() - 1;
	m_Heights.assign(m_SizeInCells + 1, std::vector<float>(m_SizeInCells + 1, 0.0f));

	for (unsigned int i = 0; i < m_SizeInCells + 1; i++)
	{
		for (unsigned int k = 0; k < m_SizeInCells + 1; k++)
		{
			unsigned char red = heightsImage->getRed(i, k);
			m_Heights[i][k] = red / 255.f;
		}
	}
}


void HeightMap::draw() const
{
	m_DisplayList.draw();

	unsigned int err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::cout << "OpenGL Error after drawing hmap: " << err << '\n';
	}

}

Vector3 HeightMap::getSurfaceNormal(double x, double z, const Vector3& scale) const
{
	assert(isInside({x, 0, z}));

	float shiftedX = (x + 64.0f) / 2;
	float shiftedZ = (z + 64.0f) / 2;

	// determine which cell pos is in
	unsigned int x0 = (unsigned int)shiftedX; //  todo: this only works when m_size in cells is 64.....
	unsigned int z0 = (unsigned int)shiftedZ;

	// calculate coords for far side of that cell
	unsigned int x1 = x0 + 1;
	unsigned int z1 = z0 + 1;

	// calculate fractional parts of each coordinate
	float xFrac = shiftedX - x0;
	float zFrac = shiftedZ - z0;

	if (xFrac > zFrac)
	{
		// pink triangle
        float height00 = getHeight(x0, z0);
        float height11 = getHeight(x1, z1);
        float height10 = getHeight(x1, z0);

        Vector3 v00(x0, height00, z0);
        Vector3 v11(x1, height11, z1);
        Vector3 v10(x1, height10, z0);

        Vector3 v10_to_v00 = v00 - v10;
        Vector3 v10_to_v11 = v11 - v10;
        Vector3 up = v10_to_v00.crossProduct(v10_to_v11);

        up.normalizeSafe();
		up = up.getComponentRatioSafe(scale);
		up.normalize();
		return up;
	}	
	else
	{
		// green triangle
        float height00 = getHeight(x0, z0);
        float height11 = getHeight(x1, z1);
        float height01 = getHeight(x0, z1);

        Vector3 v00(x0, height00, z0);
        Vector3 v11(x1, height11, z1);
        Vector3 v01(x0, height01, z1);

        Vector3 v01_to_v00 = v00 - v01;
        Vector3 v01_to_v11 = v11 - v01;
        Vector3 up = v01_to_v11.crossProduct(v01_to_v00);

        up.normalizeSafe();
		up = up.getComponentRatioSafe(scale);
		up.normalize();
		return up;
	}
}

void HeightMap::drawNormalsGlobal(int gridSize, const Vector3& scale)
{	
	if (m_DrawHeightsDL != nullptr)
		m_DrawHeightsDL->draw();
	else
	{
		m_DrawHeightsDL = new ObjLibrary::DisplayList();
		m_DrawHeightsDL->begin();

		float step = 128.f / gridSize;
		float offset = -64;

		glColor3ub(0, 0, 255);
		for (float i = offset; i < 64; i+= step) // 64
		{
			for (float j = offset; j < 64; j+= step)
			{
				drawNormal(i, j, scale);
			}
		}

		m_DrawHeightsDL->end();
	}
	unsigned int err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::cout << "OpenGL Error after drawing hmap heights: " << err << '\n';
	}
}

void HeightMap::drawNormal(double x, double z, const Vector3& scale) const
{	
	glPushMatrix();
		float height = getHeight({ x, 0, z });
		glTranslatef(x, height, z);
		Shapes::DrawNormal({ 0, 0, 0 }, getSurfaceNormal(x, z, scale), 0.25);
	glPopMatrix();
}

void HeightMap::initDisplayList(const std::string& texFilename, float texOffset, float texRepeats)
{
	ObjLibrary::TextureManager::activate(texFilename);
	m_DisplayList.begin();

		float offset = (m_Heights.size() - 1) / -2.0f;

		glEnable(GL_TEXTURE_2D);
		ObjLibrary::TextureManager::activate(texFilename);
		glColor3d(1.0, 1.0, 1.0);

		for (unsigned int i0 = 0; i0 < m_SizeInCells; i0++)
		{
			unsigned int i1 = i0 + 1;

			float tex_i0 = texOffset + (float)i0 / m_SizeInCells * texRepeats;
			float tex_i1 = texOffset + (float)i1 / m_SizeInCells * texRepeats;

			glBegin(GL_TRIANGLE_STRIP);
			for (unsigned int k = 0; k <= m_SizeInCells; k++)  // z
			{
				float tex_k = texOffset + (float)k / m_SizeInCells * texRepeats;

				glTexCoord2d(tex_i1, tex_k);
				glVertex3d(i1, m_Heights[i1][k], k);
				glTexCoord2d(tex_i0, tex_k);
				glVertex3d(i0, m_Heights[i0][k], k);
			}
			glEnd();

		}
		glDisable(GL_TEXTURE_2D);
	m_DisplayList.end();
}

unsigned int HeightMap::getSizeCells() const
{
	return m_SizeInCells;
}

float HeightMap::getCellSize() const
{
	return m_SizeOfCell;
}

// Linearly maps a number between A and B to a number between C and D. // todo: move to a more reasonable spot
float MapFloat(float fromLower, float fromUpper, float toLower, float toUpper, float x)
{
	assert(x >= fromLower && x <= fromUpper);

	float result = (x - fromLower) / (fromUpper - fromLower) * (toUpper - toLower) + toLower;

	assert(result >= toLower && result <= toUpper);
	return result; 
}


bool HeightMap::isInside(const Vector3 position) const 
// todo: this is garbage that won't work for different 
//       resolution heightmaps
{
	if (position.x > m_SizeInCells || position.x < -((float)m_SizeInCells))
		return false;
	if (position.z > m_SizeInCells || position.z < -((float)m_SizeInCells))
		return false;
	return true; 
}

float HeightMap::getHeight(int x, int y) const
{
	return m_Heights[x][y];
}

float HeightMap::getWorldHeight(int x, int y) const
{
	return MapFloat(0, 1, -30, 15, m_Heights[x][y]);
}


float HeightMap::getHeight(const Vector3& pos) const
{
	assert(isInside(pos));

	float shiftedX = (pos.x + 64.0f) / 2;
	float shiftedZ = (pos.z + 64.0f) / 2;

	// determine which cell pos is in
	unsigned int x0 = (unsigned int)shiftedX; //  todo: this only works when m_size in cells is 64.....
	unsigned int z0 = (unsigned int)shiftedZ;

	// calculate coords for far side of that cell
	unsigned int x1 = x0 + 1;
	unsigned int z1 = z0 + 1;

	// calculate fractional parts of each coordinate
	float xFrac = shiftedX - x0;
	float zFrac = shiftedZ - z0;

	if (xFrac > zFrac)
	{
		// pink
		// calculate barycentric coordinates
		float weight00 = 1.0f - xFrac; 
		float weight11 = zFrac;
		float weight10 = 1.0f - weight00 - weight11;

		float height00 = getHeight(x0, z0);
		float height11 = getHeight(x1, z1);
		float height10 = getHeight(x1, z0);

		float height =  weight00 * height00 +
			   weight11 * height11 +
			   weight10 * height10;


		// 0-1 -> -30-15
		float scaledHeight = MapFloat(0, 1, -30, 15, height);

		assert(scaledHeight >= -30 && scaledHeight <= 15);
		return scaledHeight;

	}
	else
	{
		// green
		float weight00 = 1.0f - zFrac;
		float weight11 = xFrac;
		float weight01 = 1.0f - weight00 - weight11;

		float height00 = getHeight(x0, z0);
		float height11 = getHeight(x1, z1);
		float height01 = getHeight(x0, z1);

		float height = weight00 * height00 +
			   weight11 * height11 +
			   weight01 * height01;

		// 0-1 -> -30-15
		float scaledHeight = MapFloat(0, 1, -30, 15, height);

		assert(scaledHeight >= -30 && scaledHeight <= 15);
		return scaledHeight;
	}
}
