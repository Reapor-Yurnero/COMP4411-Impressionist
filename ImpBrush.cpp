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
	//printf("%d %d %d %d\n",color[0],color[1],color[2],color[3]);
	color[0] = color[0] * (pDoc->m_pUI->m_RScale);
	color[1] = color[1] * (pDoc->m_pUI->m_GScale);
	color[2] = color[2] * (pDoc->m_pUI->m_BScale);
	glColor4ub(color[0],color[1],color[2],color[3]);

}

void ImpBrush::SetBlurredColor(const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();
	GLubyte color[4];
	if (source.x == 0 || source.y == 0 || source.x == pDoc->m_nWidth || source.y == pDoc->m_nHeight) {
		SetColor(source);
		return;
	}
	GLubyte* LU = pDoc->GetOriginalPixel(source.x - 1, source.y - 1);
	GLubyte* UU = pDoc->GetOriginalPixel(source.x - 1, source.y);
	GLubyte* RU = pDoc->GetOriginalPixel(source.x + 1, source.y - 1);
	GLubyte* LL = pDoc->GetOriginalPixel(source.x - 1, source.y);
	GLubyte* CC = pDoc->GetOriginalPixel(source.x, source.y);
	GLubyte* RR = pDoc->GetOriginalPixel(source.x + 1, source.y);
	GLubyte* LD = pDoc->GetOriginalPixel(source.x - 1, source.y - 1);
	GLubyte* DD = pDoc->GetOriginalPixel(source.x, source.y - 1);
	GLubyte* RD = pDoc->GetOriginalPixel(source.x + 1, source.y - 1);
	color[0] = (1 * LU[0] + 2 * UU[0] + 1 * RU[0] +
		2 * LL[0] + 4 * CC[0] + 2 * RR[0] +
		1 * LD[0] + 2 * DD[0] + 1 * RD[0]) / 16;
	color[1] = (1 * LU[1] + 2 * UU[1] + 1 * RU[1] +
		2 * LL[1] + 4 * CC[1] + 2 * RR[1] +
		1 * LD[1] + 2 * DD[1] + 1 * RD[1]) / 16;
	color[2] = (1 * LU[2] + 2 * UU[2] + 1 * RU[2] +
		2 * LL[2] + 4 * CC[2] + 2 * RR[2] +
		1 * LD[2] + 2 * DD[2] + 1 * RD[2]) / 16;
	color[3] = 255 * (pDoc->m_pUI->getOpacity());
	color[0] = color[0] * (pDoc->m_pUI->m_RScale);
	color[1] = color[1] * (pDoc->m_pUI->m_GScale);
	color[2] = color[2] * (pDoc->m_pUI->m_BScale);
	glColor4ub(color[0], color[1], color[2], color[3]);
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
