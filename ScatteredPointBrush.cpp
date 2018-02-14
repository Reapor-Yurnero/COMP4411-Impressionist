//
// ScatteredPointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredPointBrush.h"

extern float frand();

ScatteredPointBrush::ScatteredPointBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatteredPointBrush::BrushBegin(const Point source, const Point target)
{

	glPointSize((float)1.0);

	BrushMove(source, target);
}

void ScatteredPointBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredPointBrush::BrushMove  document is NULL\n");
		return;
	}

	int size = pDoc->getSize();
	int radius = size / 2;
	std::vector<int> p_vector((size / 2 * 2 + 1)*(size / 2 * 2 + 1), 0);
	for (unsigned int i = 0; i < p_vector.size() / 2; i++) p_vector[i] = 1;
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(p_vector.begin(), p_vector.end(), g);
	for (unsigned int i = 0; i < p_vector.size(); i++) {
		if (p_vector[i] == 1) {
			int dx = i % (radius * 2 + 1) - radius;
			int dy = i / (radius * 2 + 1) - radius;
			Point temp_source(source.x + dx, source.y + dy);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBegin(GL_POINTS);
				SetColor(temp_source);
				glVertex2d(target.x + dx, target.y + dy);
			glEnd();
		}
	}
}

void ScatteredPointBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

