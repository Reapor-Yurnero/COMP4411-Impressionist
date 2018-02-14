//
// CircleBrush.cpp
//
// The implementation of Circle Brush.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "circlebrush.h"

extern float frand();

CircleBrush::CircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void CircleBrush::BrushBegin(const Point source, const Point target)
{
	BrushMove(source, target);
}

void CircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("CircleBrush::BrushMove  document is NULL\n");
		return;
	}

	int size = pDoc->getSize() > 1 ? pDoc->getSize() : 1;
	float r = float(size / 2.0);
	PlotCircle(source, target, r);
}

void CircleBrush::PlotCircle(const Point source, const Point target, const float radius) {
	float theta = 2 * float(3.1415) / float(50);
	float tan_component = tanf(theta);
	float radial_component = cosf(theta);

	float x = radius;
	float y = 0;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_POLYGON);
		SetColor(source);

		for (int i = 0; i < 50; i++) {
			glVertex2d(target.x + x, target.y + y);

			float tx = -y;
			float ty = x;

			x += tx * tan_component;
			y += ty * tan_component;

			x *= radial_component;
			y *= radial_component;
		}
	glEnd();
}

void CircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

