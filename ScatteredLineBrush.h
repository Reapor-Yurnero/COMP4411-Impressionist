//
// ScatteredPointBrush.h
//
// The header file for Point Brush. 
//

#ifndef ScatteredLineBrush_H
#define ScatteredLineBrush_H

#include "ImpBrush.h"
#include <math.h>

class ScatteredLineBrush : public ImpBrush
{
public:
	ScatteredLineBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	void PlotLine(const Point source, const Point target, const int size, const int line_angle);
	char* BrushName(void);
};

#endif