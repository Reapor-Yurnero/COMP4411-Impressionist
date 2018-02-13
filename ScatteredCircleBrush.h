//
// ScatteredCircleBrush.h
//
// The header file for Point Brush. 
//

#ifndef ScatteredCircleBrush_H
#define ScatteredCircleBrush_H

#include "ImpBrush.h"
#include <time.h>       /* time */
#include <math.h>

class ScatteredCircleBrush : public ImpBrush
{
public:
	ScatteredCircleBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	void PlotCircle(const Point source, const Point target, const int radius);
	char* BrushName(void);
};

#endif