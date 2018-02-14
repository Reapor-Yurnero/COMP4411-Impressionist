//
// ImpBrush.cpp
//
// The implementation of virtual brush. All the other brushes inherit from it.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ImpBrush.h"

// Static class member initializations
int			ImpBrush::c_nBrushCount	= 0;
ImpBrush**	ImpBrush::c_pBrushes	= NULL;

ImpBrush::ImpBrush(ImpressionistDoc*	pDoc, 
				   char*				name) :
					m_pDoc(pDoc), 
					m_pBrushName(name)
{
}

//---------------------------------------------------
// Return m_pDoc, which connects the UI and brushes
//---------------------------------------------------
ImpressionistDoc* ImpBrush::GetDocument(void)
{
	return m_pDoc;
}

//---------------------------------------------------
// Return the name of the current brush
//---------------------------------------------------
char* ImpBrush::BrushName(void)
{
	return m_pBrushName;
}

//----------------------------------------------------
// Set the color to paint with to the color at source,
// which is the coord at the original window to sample 
// the color from
//----------------------------------------------------

void ImpBrush::SetColor(const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();

	GLubyte color[4];
	memcpy(color, pDoc->GetOriginalPixel(source), 3);
	color[3] = 255* (pDoc->m_pUI->getOpacity());
	printf("%d %d %d %d\n",color[0],color[1],color[2],color[3]);
	glColor4ub(color[0],color[1],color[2],color[3]);

}

uint32_t seedseq_random_using_clock()
{
	uint64_t seed = std::chrono::high_resolution_clock::
		now().time_since_epoch().count();
	std::seed_seq seeder{ uint32_t(seed),uint32_t(seed >> 32) };
	++seed;
	int out;
	seeder.generate(&out, &out + 1);
	return out;
}

bool Point::operator==(const Point & r)
{
	if (x == r.x && y == r.y) return true;
	return false;
}
