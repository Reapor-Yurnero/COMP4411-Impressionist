//
// ScatteredRingBrush.h
//
// The header file for Point Brush. 
//

#ifndef ScatteredRingBrush_H
#define ScatteredRingBrush_H

#include "ImpBrush.h"
#include <time.h>       /* time */
#include <math.h>

class ScatteredRingBrush : public ImpBrush
{
public:
	ScatteredRingBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	void PlotRing(const Point source, const Point target, const float radius);
	char* BrushName(void);
};

#endif