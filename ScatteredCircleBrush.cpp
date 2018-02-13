//
// ScatteredCircleBrush.cpp
//
// The implementation of Scattered Circle Brush.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredCircleBrush.h"

extern float frand();


ScatteredCircleBrush::ScatteredCircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatteredCircleBrush::BrushBegin(const Point source, const Point target)
{
	BrushMove(source, target);
}

void ScatteredCircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredCircleBrush::BrushMove  document is NULL\n");
		return;
	}

	int size = pDoc->getSize() > 1 ? pDoc->getSize() : 1;
	float r = float(size / 2.0);
	PlotCircle(source, target, r);
	srand(seedseq_random_using_clock());
	unsigned int scattered_num = rand() % 3 + 1;
	for (int i = 0; i < scattered_num; i++) {
		int rand_theta = rand() % 360;
		float rand_distance = (rand() % 150) / 100.0 * r;
		Point rand_source(source.x + sin(rand_theta)*rand_distance, source.y + cos(rand_theta)*rand_distance);
		Point rand_target(target.x + sin(rand_theta)*rand_distance, target.y + cos(rand_theta)*rand_distance);
		PlotCircle(rand_source, rand_target, r);
	}

}

void ScatteredCircleBrush::PlotCircle(const Point source, const Point target, const int radius) {
	float theta = 2 * float(3.1415) / float(50);
	float tan_component = tanf(theta);
	float radial_component = cosf(theta);

	float x = radius;
	float y = 0;

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

void ScatteredCircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
