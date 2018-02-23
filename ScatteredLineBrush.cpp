//
// ScatteredLineBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredLineBrush.h"

extern float frand();

static Point curr(-1, -1);
static Point prev(-1, -1);

ScatteredLineBrush::ScatteredLineBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatteredLineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int line_width = dlg->getLineWidth();

	glLineWidth((float)line_width);

	BrushMove(source, target);
}

void ScatteredLineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;


	if (pDoc == NULL) {
		printf("ScatteredLineBrush::BrushMove  document is NULL\n");
		return;
	}

	int size = pDoc->getSize();
	int mode = pDoc->m_nStrokeMode;
	int line_angle = dlg->getLineAngle();
	float radient = 0; int pos = 0;
	switch (mode) {
	case 0: // sliderbar or right click line
		line_angle = dlg->getLineAngle();
		break;
	case 1: // gradient 
			//printf("begin\n");
		pos = source.x + pDoc->m_nWidth*source.y;
		//std::cout <<source.x <<" "<<source.y<<" "<<pos << std::endl;
		//std::cout << pDoc->m_ucGradientY[pos] << " " << pDoc->m_ucGradientX[pos]<<std::endl;
		if (pDoc->m_ucGradientX[pos] == 0) {
			line_angle = 0;
			break;
		}
		radient = atan(pDoc->m_ucGradientY[pos] / pDoc->m_ucGradientX[pos]);
		//std::cout << radient << std::endl;
		if (radient < 0) radient = M_PI + radient;
		line_angle = radient * 180 / M_PI + 90;
		//std::cout << line_angle << std::endl;
		//printf("end\n");
		break;
	case 2: // cursor direction
		curr = target;
		if (curr.x != -1 && prev.x != -1 && !(curr == prev)) {
			float dx = curr.x - prev.x;
			float dy = curr.y - prev.y;
			if (dx == 0) {
				line_angle = 90;
				break;
			}
			radient = atan(dy / dx);
			if (radient < 0) radient = M_PI + radient;
			line_angle = radient * 180 / M_PI;
		}
		prev = curr;
		break;
	case 3: // another gradient
		pos = source.x + pDoc->m_nWidth*source.y;
		//std::cout <<source.x <<" "<<source.y<<" "<<pos << std::endl;
		//std::cout << pDoc->m_ucGradientY[pos] << " " << pDoc->m_ucGradientX[pos]<<std::endl;
		if (pDoc->m_ucAGradientX[pos] == 0) {
			line_angle = 0;
			break;
		}
		radient = atan(pDoc->m_ucAGradientY[pos] / pDoc->m_ucAGradientX[pos]);
		//std::cout << radient << std::endl;
		if (radient < 0) radient = M_PI + radient;
		line_angle = radient * 180 / M_PI + 90;
	default:
		break;
	}
	//printf("%d\n", line_angle);
	PlotLine(source, target, size, line_angle);
	srand(seedseq_random_using_clock());
	float r = float(size / 2.0);
	unsigned int scattered_num = rand() % 5 + 1;
	for (unsigned int i = 0; i < scattered_num; i++) {
		int rand_theta = rand() % 360;
		float rand_distance = (rand() % 80) / 100.0 * r;
		Point rand_source(source.x + sin(rand_theta)*rand_distance, source.y + cos(rand_theta)*rand_distance);
		Point rand_target(target.x + sin(rand_theta)*rand_distance, target.y + cos(rand_theta)*rand_distance);
		PlotLine(rand_source, rand_target, size, line_angle);
	}
}

void ScatteredLineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

void ScatteredLineBrush::PlotLine(const Point source, const Point target, const int size, const int line_angle)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_LINES);
	SetColor(source);
	//std::cout <<line_angle<<" "<< cosf((float)line_angle*M_PI/180) << " " << sinf((float)line_angle/M_PI) << std::endl;
	glVertex2d(target.x + (size / 2.0)*cosf((float)line_angle*M_PI / 180), target.y + (size / 2.0)*sinf((float)line_angle*M_PI / 180));
	glVertex2d(target.x - (size / 2.0)*cosf((float)line_angle*M_PI / 180), target.y - (size / 2.0)*sinf((float)line_angle*M_PI / 180));
	glEnd();
}

