#include "HeightMap.h"
#include "ObjLibrary/TextureManager.h"
#include "GetGlut.h"
#include <iostream>
#include <vector>
#include <math.h>

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

bool HeightMap::isInside(const Vector3 position) const
{
	if (position.x > m_SizeInCells)
		return false;
	if (position.z > m_SizeInCells)
		return false;
	return true; 
}

float HeightMap::getHeight(int x, int y) const
{
	return m_Heights[x][y];
}
