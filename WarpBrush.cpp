//
// WarpBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "WarpBrush.h"
#include <algorithm>

extern float frand();

WarpBrush::WarpBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void WarpBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	//int size = pDoc->getSize();

	m_nHeight = dlg->m_paintView->getHeight();
	m_nWidth = dlg->m_paintView->getWidth();
	m_pData = new unsigned char[3 * m_nHeight * m_nWidth];
	//memcpy(m_pData, pDoc->m_ucPainting, 3 * m_nHeight * m_nWidth);

	m_startPoint = target;

	//glPointSize((float)size);

	//BrushMove(source, target);
}

void WarpBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("WarpBrush::BrushMove  document is NULL\n");
		return;
	}
	//std::cout << "source" << source.x << " " << source.y << std::endl;
	//std::cout <<"target" << target.x << " " << target.y << std::endl;
	//printf("%lf\n", dlg->getOpacity());
	
	//printf("%d %d %d %d\n", target.x, target.y, target.x - m_startPoint.x, target.y - m_startPoint.y);

	memcpy(m_pData, pDoc->m_ucPainting, 3 * m_nHeight * m_nWidth);

	int size = pDoc->getSize();
	if (size / 2 == 0) return;

	int dx = target.x - m_startPoint.x;
	int dy = target.y - m_startPoint.y;
	int theta = atan2(dy, dx);
	double distance = sqrt(dx * dx + dy * dy);
	int step = distance / size * 2;
	double xstep = cos(theta) * size / 2;
	double ystep = sin(theta) * size / 2;

	auto now = m_startPoint;
	double nextX = now.x, nextY = now.y;

	for (int n = 0; n < step; n++) {
		nextX += xstep;
		nextY += ystep;
		if ((int)nextX != now.x || (int)nextY != now.y) {
			for (int y = -size; y <= size; y++) {
				for (int x = -size; x <= size; x++) {
					int srcX = now.x + x;
					int srcY = now.y + y;
					int dstX = nextX + x;
					int dstY = nextY + y;
					auto srcPtr = pDoc->m_ucPainting + 3 * (srcX + m_nWidth * srcY);
					auto dstPtr = m_pData + 3 * (dstX + m_nWidth * dstY);
					for (int k = 0; k < 3; k++) {
						dstPtr[k] = ((int)dstPtr[k] + (int)srcPtr[k]) / 2;
					}
				}
			}
			now.x = nextX;
			now.y = nextY;
		}
	}

	memcpy(pDoc->m_ucPainting, m_pData, 3 * m_nHeight * m_nWidth);
	m_startPoint = target;

	dlg->m_paintView->refresh();

}

void WarpBrush::BrushEnd(const Point source, const Point target)
{
	delete[] m_pData;

}

