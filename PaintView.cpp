//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "paintview.h"
#include "ImpBrush.h"


#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN	4
#define RIGHT_MOUSE_DRAG	5
#define RIGHT_MOUSE_UP		6
#define AUTO_PAINT			7


#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )
#endif

static int		eventToDo;
static int		isAnEvent=0;
static Point	coord;

PaintView::PaintView(int			x, 
					 int			y, 
					 int			w, 
					 int			h, 
					 const char*	l)
						: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;
	m_pOldPainting = NULL;
}


void PaintView::draw()
{
	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
	#endif // !MESA

	if(!valid())
	{

		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		ortho();

		glClear( GL_COLOR_BUFFER_BIT );
	}

	Point scrollpos;// = GetScrollPosition();
	scrollpos.x = 0;
	scrollpos.y	= 0;

	m_nWindowWidth	= w();
	m_nWindowHeight	= h();

	int drawWidth, drawHeight;
	drawWidth = min( m_nWindowWidth, m_pDoc->m_nPaintWidth );
	drawHeight = min( m_nWindowHeight, m_pDoc->m_nPaintHeight );

	int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
	if ( startrow < 0 ) startrow = 0;

	m_pPaintBitstart = m_pDoc->m_ucPainting + 
		3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

	m_nDrawWidth	= drawWidth;
	m_nDrawHeight	= drawHeight;

	m_nStartRow		= startrow;
	m_nEndRow		= startrow + drawHeight;
	m_nStartCol		= scrollpos.x;
	m_nEndCol		= m_nStartCol + drawWidth;

	if ( m_pDoc->m_ucPainting && !isAnEvent) 
	{
		RestoreContent();
		//printf("NOevent\n");
	}

	if ( m_pDoc->m_ucPainting && isAnEvent) 
	{
		// Clear it after processing.
		isAnEvent	= 0;	

		Point source( coord.x + m_nStartCol, m_nEndRow - coord.y );
		Point target( coord.x, m_nWindowHeight - coord.y );
		//std::cout << source.x << " " << source.y << " " << target.x << " " << target.y << std::endl;
		// This is the event handler
		//std::cout << m_nWindowWidth << " " << m_nWindowHeight << std::endl;
		SaveOldContent();
		switch (eventToDo) 
		{
		case LEFT_MOUSE_DOWN:
			if (target.x > drawWidth || source.y < 0) break;
			m_pDoc->m_pCurrentBrush->BrushBegin( source, target );
			break;
		case LEFT_MOUSE_DRAG:
			if (target.x > drawWidth || source.y < 0) break;
			m_pDoc->m_pCurrentBrush->BrushMove( source, target );
			m_pDoc->m_pUI->m_origView->trackCursor(source.x,source.y+(m_nWindowHeight-drawHeight));
			break;
		case LEFT_MOUSE_UP:
			m_pDoc->m_pCurrentBrush->BrushEnd( source, target );
			SaveCurrentContent();
			m_pDoc->addHistory();
			//RestoreContent();
			break;
		case RIGHT_MOUSE_DOWN:
			//SaveCurrentContent();
			RightStart = target;
			break;
		case RIGHT_MOUSE_DRAG:
			RightEnd = target;
			glLineWidth(5);
			glBegin(GL_LINES);
			glColor3f(0, 0, 1);
			glVertex2d(RightStart.x, RightStart.y);
			glVertex2d(RightEnd.x, RightEnd.y);
			glEnd();
			//RestoreContent();
			break;

		case RIGHT_MOUSE_UP:
			//RestoreContent();
			RightEnd = target;
			if ((m_pDoc->getBrushType() == 1 || m_pDoc->getBrushType() == 4) && m_pDoc->m_nStrokeMode == 0) {
				//std::cout << RightStart.x << " " << RightStart.y << std::endl;
				//std::cout << RightEnd.x << " " << RightEnd.y << std::endl;
				int angle = getRightCursorAngle(RightStart, RightEnd);
				//printf("%d\n", angle);
				m_pDoc->setLineAngle(angle);
			}
			break;

		case AUTO_PAINT:
			AutoPaint();
			break;
		default:
			printf("Unknown event!!\n");		
			break;
		}
		SaveCurrentContent();
		//RestoreContent();
		CompareOldCurrent();
	}

	glFlush();
	RestoreContent();
	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
	#endif // !MESA

}


int PaintView::handle(int event)
{
	switch(event)
	{
	case FL_ENTER:
	    redraw();
		break;
	case FL_PUSH:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DOWN;
		else
			eventToDo=LEFT_MOUSE_DOWN;
		isAnEvent=1;
		redraw();
		break;
	case FL_DRAG:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DRAG;
		else
			eventToDo=LEFT_MOUSE_DRAG;
		isAnEvent=1;
		redraw();
		break;
	case FL_RELEASE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_UP;
		else
			eventToDo=LEFT_MOUSE_UP;
		isAnEvent=1;
		redraw();
		break;
	case FL_MOVE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		m_pDoc->m_pUI->m_origView->trackCursor(coord.x + m_nStartCol, m_nEndRow - coord.y + m_nWindowHeight - m_nDrawHeight);
		//redraw();
		break;
	default:
		return 0;
		break;

	}



	return 1;
}

void PaintView::refresh()
{
	redraw();
}

void PaintView::resizeWindow(int width, int height)
{
	resize(x(), y(), width, height);
}

void PaintView::SaveCurrentContent()
{
	// Tell openGL to read from the front buffer when capturing
	// out paint strokes
	glReadBuffer(GL_FRONT);

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	
	glReadPixels( 0, 
				  m_nWindowHeight - m_nDrawHeight, 
				  m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart );
}

void PaintView::SaveOldContent()
{
	int width = m_pDoc->m_nWidth;
	int height = m_pDoc->m_nHeight;
	if (m_pOldPainting) delete[] m_pOldPainting;
	m_pOldPainting = new GLubyte[width*height * 3];
	memcpy(m_pOldPainting, m_pDoc->m_ucPainting, width*height * 3);
}


void PaintView::RestoreContent()
{
	glDrawBuffer(GL_BACK);

	glClear( GL_COLOR_BUFFER_BIT );

	glRasterPos2i( 0, m_nWindowHeight - m_nDrawHeight );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	glDrawPixels( m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart);

//	glDrawBuffer(GL_FRONT);
}

void PaintView::AutoPaintTrigger()
{
	isAnEvent = 1;
	eventToDo = AUTO_PAINT;
	redraw();
}

void PaintView::CompareOldCurrent()
{
	int width = m_pDoc->m_nWidth;
	int height = m_pDoc->m_nHeight;
	for (int i = 0; i < width*height; ++i) {
		if (m_pOldPainting[3 * i] != m_pDoc->m_ucPainting[3 * i] ||
			m_pOldPainting[3 * i + 1] != m_pDoc->m_ucPainting[3 * i + 1] ||
			m_pOldPainting[3 * i + 2] != m_pDoc->m_ucPainting[3 * i + 2]) 
		{
			UpdatePos.push_back(i);
		}
	}
	std::sort(UpdatePos.begin(), UpdatePos.end());
	UpdatePos.erase(std::unique(UpdatePos.begin(), UpdatePos.end()), UpdatePos.end());
}

void PaintView::AutoPaint()
{
	m_pDoc->clearCanvas(); refresh();
	//RestoreContent();
	// printf("printfonce\n");
	std::random_device rd;
	std::mt19937 g(rd());
	srand(time(NULL));
	int spacing = m_pDoc->m_pUI->m_nSpacing;
	int width = m_pDoc->m_nWidth;
	int height = m_pDoc->m_nHeight;
	int size = m_pDoc->getSize();
	std::vector<Point> PaintOrder;
	for (int j = 0; j < height; j += spacing) {
		for (int i = 0; i < width; i += spacing) {
			float dx = rand() % 50 / 100.0 * size;
			float dy = rand() % 50 / 100.0 * size;
			Point temp(i + dx, j + dy);
			PaintOrder.push_back(temp);
		}
	}
	std::shuffle(PaintOrder.begin(), PaintOrder.end(), g);
	for (int i = 0; i < PaintOrder.size(); ++i) {
		m_pDoc->setSize((rand() % 20 / 100.0 + 0.9)*size);
		m_pDoc->m_pCurrentBrush->BrushBegin(PaintOrder[i], PaintOrder[i]);
		m_pDoc->m_pCurrentBrush->BrushEnd(PaintOrder[i], PaintOrder[i]);
		m_pDoc->setSize(size);
	}
}

int PaintView::getRightCursorAngle(Point s, Point t)
{
	float dx = t.x - s.x;
	float dy = t.y - s.y;
	float slope = dy / dx;
	float radian = atan(slope);
	//std::cout << radian << std::endl;
	if (radian < 0) radian = M_PI + radian;
	int degree = radian / M_PI * 180;
	return degree;
}
