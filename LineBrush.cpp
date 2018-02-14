//
// LineBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "LineBrush.h"

extern float frand();

static Point curr(-1,-1);
static Point prev(-1,-1);

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void LineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int line_width = dlg->getLineWidth();

	glLineWidth((float)line_width);

	BrushMove(source, target);
}

void LineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;


	if (pDoc == NULL) {
		printf("LineBrush::BrushMove  document is NULL\n");
		return;
	}
	
	int size = pDoc->getSize();
	int mode = pDoc->m_nStrokeMode;
	int line_angle = dlg->getLineAngle();
	switch (mode) {
	case 0: // sliderbar or right click line
		line_angle = dlg->getLineAngle();
		break;
	case 1: // gradient

		break;
	case 2: // cursor direction
		curr = target;
		if (curr.x != -1 && prev.x != -1 && !(curr == prev)) {
			float dx = curr.x - prev.x;
			float dy = curr.y - prev.y;
			float radient = atan(dy / dx);
			if (radient < 0) radient = M_PI + radient;
			line_angle = radient * 180 / M_PI;
		}
		prev = curr;
		break;
	default:
		break;
	}
	//printf("%d\n", line_angle);
	glBegin(GL_LINES);
	SetColor(source);
	//std::cout <<line_angle<<" "<< cosf((float)line_angle*M_PI/180) << " " << sinf((float)line_angle/M_PI) << std::endl;
	glVertex2d(target.x + (size/2.0)*cosf((float)line_angle*M_PI / 180), target.y+ (size / 2.0)*sinf((float)line_angle*M_PI / 180));
	glVertex2d(target.x - (size / 2.0)*cosf((float)line_angle*M_PI / 180), target.y - (size / 2.0)*sinf((float)line_angle*M_PI / 180));

	glEnd();
}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

