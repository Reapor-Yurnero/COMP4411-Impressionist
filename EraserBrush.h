//
// EraserBrush.h
//
// The header file for Point Brush. 
//

#ifndef EraserBrush_H
#define EraserBrush_H

#include "ImpBrush.h"

class EraserBrush : public ImpBrush
{
public:
	EraserBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif