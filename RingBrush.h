//
// RingBrush.h
//
// The header file for Point Brush. 
//

#ifndef RingBrush_H
#define RingBrush_H

#include "ImpBrush.h"
#include <math.h>

class RingBrush : public ImpBrush
{
public:
	RingBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	void PlotRing(const Point source, const Point target, const float radius);
	char* BrushName(void);
};

#endif#pragma once
