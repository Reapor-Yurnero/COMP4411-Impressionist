//
// ScatteredRingBrush.cpp
//
// The implementation of Scattered Circle Brush.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredRingBrush.h"

extern float frand();


ScatteredRingBrush::ScatteredRingBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatteredRingBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	int line_width = dlg->getLineWidth();
	glLineWidth((float)line_width);
	BrushMove(source, target);
}

void ScatteredRingBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredRingBrush::BrushMove  document is NULL\n");
		return;
	}

	int size = pDoc->getSize() > 1 ? pDoc->getSize() : 1;
	float r = float(size / 2.0);
	PlotRing(source, target, r);
	srand(seedseq_random_using_clock());
	unsigned int scattered_num = rand() % 3 + 1;
	for (unsigned int i = 0; i < scattered_num; i++) {
		int rand_theta = rand() % 360;
		float rand_distance = (rand() % 150) / 100.0 * r;
		Point rand_source(source.x + sin(rand_theta)*rand_distance, source.y + cos(rand_theta)*rand_distance);
		Point rand_target(target.x + sin(rand_theta)*rand_distance, target.y + cos(rand_theta)*rand_distance);
		PlotRing(rand_source, rand_target, r);
	}

}

void ScatteredRingBrush::PlotRing(const Point source, const Point target, const float radius) {
	float theta = 2 * float(3.1415) / float(50);
	float tan_component = tanf(theta);
	float radial_component = cosf(theta);

	float x = radius;
	float y = 0;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_LINE_LOOP);
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

void ScatteredRingBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
