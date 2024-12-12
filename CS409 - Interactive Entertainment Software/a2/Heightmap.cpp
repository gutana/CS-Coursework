#include "HeightMap.h"
#include "ObjLibrary/TextureManager.h"
#include "GetGlut.h"
#include <iostream>
#include <vector>
#include <math.h>

HeightMap::HeightMap(const ObjLibrary::TextureBmp* heightsImage, Vector3 center, float minHeight, float maxHeight)
{
	assert(heightsImage->isSquare());
	m_Position = center;
	m_MinHeight = minHeight;
	m_MaxHeight = maxHeight;
	m_HeightRange = m_MaxHeight - m_MinHeight;
	m_SizeOfCell = 1;

	m_SizeInCells = heightsImage->getWidth() - 1;

	m_Heights.assign(m_SizeInCells + 1, std::vector<float>(m_SizeInCells + 1, 0.0f));

	for (unsigned int i = 0; i < m_SizeInCells + 1; i++)
	{
		for (unsigned int k = 0; k < m_SizeInCells + 1; k++)
		{
			unsigned char red = heightsImage->getRed(i, k);
			m_Heights[i][k] = (red / 255.f) * m_HeightRange + m_MinHeight;
			assert(m_Heights[i][k] <= m_MaxHeight);
		}
	}
}


void HeightMap::draw() const
{
	unsigned int err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::cout << "OpenGL Error before drawing hmap: " << err << '\n';
	}


	glEnable(GL_TEXTURE_2D);
	m_DisplayList.draw();
	glDisable(GL_TEXTURE_2D);

	unsigned int err2 = glGetError();
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

	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, m_Position.z);
	glTranslatef(offset, 0, offset);

	if (texFilename == "Resources/grass1.bmp")
	{
		glScalef(1, 1.1, 1);
	}

	ObjLibrary::TextureManager::activate(texFilename);
	for (unsigned int i0 = 0; i0 < m_SizeInCells; i0++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		unsigned int i1 = i0 + 1;

		float tex_i0 = texOffset + (float)i0 / m_SizeInCells * texRepeats;
		float tex_i1 = texOffset + (float)i1 / m_SizeInCells * texRepeats;

		glColor3f(1, 1, 1);

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

	glPopMatrix();

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
	if (std::abs(position.x - m_Position.x) >= m_SizeInCells / 2)
		return false;
	if (std::abs(position.z - m_Position.z) >= m_SizeInCells / 2)
		return false;
	return true;
}

float HeightMap::getHeight(int x, int y) const
{
	return m_Heights[x][y];
}
