
#include "Heightmap.h"
#include "ObjLibrary/DisplayList.h"
#include "GetGlut.h"

enum TerrainType
{
	UnderWater,
	AboveWater
};

struct int2
{
	int x;
	int y;
};

using ObjLibrary::DisplayList;

class Terrain 
{
public:
	HeightMap hMapUnderWater;
	HeightMap hMapAboveWater;

	float heights[129][129];
	ObjLibrary::TextureBmp* m_HeightMapImage;

	DisplayList* m_AlgaeDL;
	std::vector<int2> plantPositions;

	Vector3 m_Offset;
	Vector3 m_Scale;

	Terrain(ObjLibrary::TextureBmp* hMapImg, Vector3 offset, Vector3 scale, DisplayList* algaeDl)
		: hMapUnderWater(hMapImg),
		  hMapAboveWater(hMapImg)
	{
		m_AlgaeDL = algaeDl;
		m_HeightMapImage = hMapImg; 
		m_Offset = offset;
		m_Scale.x = scale.x / hMapUnderWater.getSizeCells();
		m_Scale.y = scale.y;
		m_Scale.z = scale.z / hMapUnderWater.getSizeCells();
	}

	void Init(const std::string& underWaterTexFile, const std::string& aboveWaterTexFile, float texOff, float texRepeat)
	{
		hMapUnderWater.initDisplayList(underWaterTexFile, texOff, texRepeat);
		hMapAboveWater.initDisplayList(aboveWaterTexFile, texOff, texRepeat);

		int verts = hMapUnderWater.getSizeCells() + 1;
		for (int i = 0; i < verts; i++)
		{
			for (int j = 0; j < verts; j++)
			{
				heights[i][j] = hMapUnderWater.getHeight(i, j);
			}
		}

		for (int i = 0; i < verts; i++)
		{
			for (int j = 0; j < verts; j++)
			{
				unsigned char green = m_HeightMapImage->getGreen(i, j);
				if (green >= 64)
				{
					plantPositions.push_back({ i, j });
				}
			}
		}

	}

	void render(TerrainType type)
	{
		// draw underwater one no matter what

		glEnable(GL_FOG);
		glPushMatrix();
			glTranslated(m_Offset.x, m_Offset.y, m_Offset.z);
			glScaled(m_Scale.x, m_Scale.y, m_Scale.z);
			hMapUnderWater.draw();
		glPopMatrix();

		if (type == TerrainType::AboveWater) // if above water, also draw the grass
		{
			glDisable(GL_FOG);
			glPushMatrix();
				glTranslated(m_Offset.x, m_Offset.y * 1.1, m_Offset.z);
				glScaled(m_Scale.x, m_Scale.y * 1.1, m_Scale.z);
				hMapAboveWater.draw();
			glPopMatrix();
			glEnable(GL_FOG);
		}

		if (m_AllAlgaeDisplayList == nullptr)
		{
			m_AllAlgaeDisplayList = new DisplayList();
			m_AllAlgaeDisplayList->begin();

			// draw algae at vertices
			const int numVerts = hMapUnderWater.getSizeCells();
			for (int i = 0; i <= numVerts; i++)
			{
				for (int j = 0; j <= numVerts; j++)
				{
					float y = heights[i][j];
					unsigned char green = m_HeightMapImage->getGreen(i, j);

					if (green < 64) continue;
	
					Vector3 pos = m_Offset + Vector3(i, y, j).getComponentProduct(m_Scale);
					glPushMatrix();
						glTranslated(pos.x, pos.y, pos.z);
						glEnable(GL_FOG);
						m_AlgaeDL->draw();
						glDisable(GL_FOG);	
					glPopMatrix();
				}
			}
			m_AllAlgaeDisplayList->end();

		}
		else
		{
			m_AllAlgaeDisplayList->draw();
		}
	}
	
	Terrain(){}

private:
	DisplayList* m_AllAlgaeDisplayList = nullptr;
};
