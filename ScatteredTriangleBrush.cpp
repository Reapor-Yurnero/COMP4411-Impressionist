//
// ScatteredTriangleBrush.cpp
//
// The implementation of Circle Brush.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredTriangleBrush.h"

extern float frand();

ScatteredTriangleBrush::ScatteredTriangleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatteredTriangleBrush::BrushBegin(const Point source, const Point target)
{
	BrushMove(source, target);
}

void ScatteredTriangleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredTriangleBrush::BrushMove  document is NULL\n");
		return;
	}

	int size = pDoc->getSize() > 1 ? pDoc->getSize() : 1;
	float r = float(size / (float)sqrt(3));
	//std::cout << size << " " << r << std::endl;
	PlotTriangle(source, target, r, size);
	srand(seedseq_random_using_clock());
	unsigned int scattered_num = rand() % 3 + 1;
	for (unsigned int i = 0; i < scattered_num; i++) {
		int rand_theta = rand() % 360;
		float rand_distance = (rand() % 150) / 100.0 * r;
		Point rand_source(source.x + sin(rand_theta)*rand_distance, source.y + cos(rand_theta)*rand_distance);
		Point rand_target(target.x + sin(rand_theta)*rand_distance, target.y + cos(rand_theta)*rand_distance);
		PlotTriangle(rand_source, rand_target, r, size);
	}
}

void ScatteredTriangleBrush::PlotTriangle(const Point source, const Point target, const float radius, const int size) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_POLYGON);
	SetColor(source);
	glVertex2d(target.x, target.y + radius);
	glVertex2d(target.x + size / 2.0, target.y - radius / 2.0);
	glVertex2d(target.x - size / 2.0, target.y - radius / 2.0);
	glEnd();
}

void ScatteredTriangleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

