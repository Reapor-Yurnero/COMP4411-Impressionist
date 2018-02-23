//
// EraserBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "EraserBrush.h"

extern float frand();

EraserBrush::EraserBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void EraserBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize((float)size);

	BrushMove(source, target);
}

void EraserBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("EraserBrush::BrushMove  document is NULL\n");
		return;
	}
	//std::cout << "source" << source.x << " " << source.y << std::endl;
	//std::cout <<"target" << target.x << " " << target.y << std::endl;
	//printf("%lf\n", dlg->getOpacity());
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_POINTS);
	glColor3f(0.0, 0.0, 0.0);

	glVertex2d(target.x, target.y);

	glEnd();
}

void EraserBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

