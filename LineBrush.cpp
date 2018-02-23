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
		radient = atan(- pDoc->m_ucGradientY[pos] / pDoc->m_ucGradientX[pos]);
		//std::cout << radient << std::endl;
		if (radient < 0) radient = M_PI + radient;
		line_angle = radient * 180.0 / M_PI + 90;
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
		radient = atan(- pDoc->m_ucAGradientY[pos] / pDoc->m_ucAGradientX[pos]);
		//std::cout << radient << std::endl;
		if (radient < 0) radient = M_PI + radient;
		line_angle = radient * 180 / M_PI + 90;
	default:
		break;
	}
	//printf("%d\n", line_angle);
	int startx, starty, endx, endy;
	int dx = abs((size / 2.0)*cosf((float)line_angle*M_PI / 180));
	int dy = abs((size / 2.0)*sinf((float)line_angle*M_PI / 180));
	int width = pDoc->m_nWidth;
	float tempx, tempy;
	if (pDoc->m_pUI->m_nClipped == true) {
		GLubyte* edgemap = pDoc->m_ucEdgeMap;
		if (pDoc->m_pUI->m_nClipAnother == true) edgemap = pDoc->m_ucAnotherEdgeMap;
		tempx = startx = target.x; tempy = starty = target.y;
		while (edgemap[3 * (startx + width*starty)] < 128 && abs(startx-target.x)<=dx && abs(starty - target.y) <= dy) {
			tempx += cosf((float)line_angle*M_PI / 180.0);
			tempy += sinf((float)line_angle*M_PI / 180.0);
			startx = round(tempx);
			starty = round(tempy);
			//std::cout << "s" << startx << " " << starty << std::endl;
		}
		tempx = endx = target.x; tempy = endy = target.y;
		while (edgemap[3 * (endx + width*endy)] < 128 && abs(endx - target.x) <= dx && abs(endy - target.y) <= dy) {
			tempx -= cosf((float)line_angle*M_PI / 180.0);
			tempy -= sinf((float)line_angle*M_PI / 180.0);
			endx = round(tempx);
			endy = round(tempy);
			//std::cout << "t" << endx << " " << endy << std::endl;
		}
	}
	else {
		startx = target.x + (size / 2.0)*cosf((float)line_angle*M_PI / 180);
		starty = target.y + (size / 2.0)*sinf((float)line_angle*M_PI / 180);
		endx = target.x - (size / 2.0)*cosf((float)line_angle*M_PI / 180);
		endy = target.y - (size / 2.0)*sinf((float)line_angle*M_PI / 180);
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_LINES);
		SetColor(source);
		//std::cout <<line_angle<<" "<< cosf((float)line_angle*M_PI/180) << " " << sinf((float)line_angle/M_PI) << std::endl;
		glVertex2d(startx, starty);
		glVertex2d(endx, endy);
	glEnd();
}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}