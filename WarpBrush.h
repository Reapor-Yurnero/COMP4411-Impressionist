//
// WarpBrush.h
//
// The header file for Point Brush. 
//

#ifndef WarpBrush_H
#define WarpBrush_H

#include "ImpBrush.h"

class WarpBrush : public ImpBrush
{
public:
	WarpBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
private:
	unsigned char *m_pData;
	int m_nHeight, m_nWidth;
	Point m_startPoint;
};

#endif