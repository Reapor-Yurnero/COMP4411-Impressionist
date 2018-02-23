//
// TriangleBrush.cpp
//
// The implementation of Circle Brush.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "TriangleBrush.h"

extern float frand();

TriangleBrush::TriangleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void TriangleBrush::BrushBegin(const Point source, const Point target)
{
	BrushMove(source, target);
}

void TriangleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("TriangleBrush::BrushMove  document is NULL\n");
		return;
	}

	int size = pDoc->getSize() > 1 ? pDoc->getSize() : 1;
	float r = float(size / (float)sqrt(3));
	//std::cout << size << " " << r << std::endl;
	PlotTriangle(source, target, r, size);
}

void TriangleBrush::PlotTriangle(const Point source, const Point target, const float radius, const int size) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_POLYGON);
	SetColor(source);
	glVertex2d(target.x, target.y + radius);
	glVertex2d(target.x + size/2.0, target.y - radius/2.0);
	glVertex2d(target.x - size / 2.0, target.y - radius / 2.0);
	glEnd();
}

void TriangleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

