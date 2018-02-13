//
// ScatteredPointBrush.h
//
// The header file for Scattered Point Brush. 
//

#ifndef ScatteredPointBrush_H
#define ScatteredPointBrush_H

#include "ImpBrush.h"
#include <vector>
#include <algorithm>

class ScatteredPointBrush : public ImpBrush
{
public:
	ScatteredPointBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif