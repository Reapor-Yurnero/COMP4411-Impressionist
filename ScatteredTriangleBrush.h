//
// ScatteredTriangleBrush.h
//
// The header file for Point Brush. 
//

#ifndef ScatteredTriangleBrush_H
#define ScatteredTriangleBrush_H

#include "ImpBrush.h"
#include <math.h>

class ScatteredTriangleBrush : public ImpBrush
{
public:
	ScatteredTriangleBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	void PlotTriangle(const Point source, const Point target, const float radius, const int size);
	char* BrushName(void);
};

#endif