//
// PointBrush.h
//
// The header file for Point Brush. 
//

#ifndef LineBrush_H
#define LineBrush_H

#include "ImpBrush.h"
#include <math.h>

class LineBrush : public ImpBrush
{
public:
	LineBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	void PlotCircle(const Point source, const Point target, const float radius);
	char* BrushName(void);
};

#endif