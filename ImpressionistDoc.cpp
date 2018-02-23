// 
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//

#include <FL/fl_ask.H>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "ImpBrush.h"

// Include individual brush headers here.
#include "PointBrush.h"
#include "CircleBrush.h"
#include "ScatteredCircleBrush.h"
#include "ScatteredPointBrush.h"
#include "LineBrush.h"
#include "ScatteredLineBrush.h"
#include "EraserBrush.h"
#include "BlurBrush.h"
#include "TriangleBrush.h"
#include "ScatteredTriangleBrush.h"
#include "RingBrush.h"
#include "ScatteredRingBrush.h"
#include "WarpBrush.h"


#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc() 
{
	// Set NULL image name as init. 
	m_imageName[0]	='\0';	

	m_nWidth		= -1;
	m_nHeight		= -1;
	m_ucBitmap		= NULL;
	m_ucPainting	= NULL;
	m_ucGradientNorm = NULL;
	m_ucGradientX = NULL;
	m_ucGradientY = NULL;
	m_ucBlurredIntensityMap = NULL;
	m_ucIntensityMap = NULL;
	m_ucEdgeMap = NULL;
	m_ucGradientBitmap = NULL;
	m_ucAGradientX = NULL;
	m_ucAGradientY = NULL;
	m_ucABlurredIntensityMap = NULL;
	m_ucAIntensityMap = NULL;
	m_ucAnotherEdgeMap = NULL;

	// create one instance of each brush
	ImpBrush::c_nBrushCount	= NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes	= new ImpBrush* [ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS]	= new PointBrush( this, "Points" );

	// Note: You should implement these 5 brushes.  They are set the same (PointBrush) for now
	ImpBrush::c_pBrushes[BRUSH_LINES]				
		= new LineBrush( this, "Lines" );
	ImpBrush::c_pBrushes[BRUSH_CIRCLES]				
		= new CircleBrush( this, "Circles" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS]	
		= new ScatteredPointBrush( this, "Scattered Points" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]		
		= new ScatteredLineBrush( this, "Scattered Lines" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES]	
		= new ScatteredCircleBrush( this, "Scattered Circles" );
	ImpBrush::c_pBrushes[BRUSH_BLUR]
		= new BlurBrush(this, "Blur");  
	ImpBrush::c_pBrushes[BRUSH_TRIANGLE]
		= new TriangleBrush(this, "Triangles"); 
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_TRIANGLE]
		= new ScatteredTriangleBrush(this, "Scattered Triangles"); 
	ImpBrush::c_pBrushes[BRUSH_RING]
		= new RingBrush(this, "Rings");  
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_RING]
		= new ScatteredRingBrush(this, "Scattered Rings");  
	ImpBrush::c_pBrushes[BRUSH_WARP]
		= new WarpBrush(this, "Warps");
	ImpBrush::c_pBrushes[BRUSH_ERASER]
		= new EraserBrush(this, "Erasers"); 

	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];
	// default 0 mode for stroke direction
	m_nStrokeMode = 0;

}

ImpressionistDoc::~ImpressionistDoc()
{
	clearHistory();
}


//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui) 
{
	m_pUI	= ui;
}

void ImpressionistDoc::setLineAngle(int angle)
{
	m_pUI->setLineAngle(angle);
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName() 
{
	return m_imageName;
}

void ImpressionistDoc::addHistory()
{
	if (!m_history.empty()) {
		for (int i = m_history.size() - 1; i > m_nHistoryIndex; i--) {
			delete[] m_history[i];
			m_history.pop_back();
			printf("remove history on index %d\n", i);
		}
	}
	unsigned char *data;
	if (m_history.size() > 50) {
		data = m_history[0];
		m_history.pop_front();
		printf("remove first history\n");
	}
	else {
		data = new unsigned char[3 * m_nHeight * m_nWidth];
	}
	m_history.push_back(data);
	memcpy(data, m_ucPainting, 3 * m_nHeight * m_nWidth);
	m_nHistoryIndex = m_history.size() - 1;
	printf("add history on index %d\n", m_nHistoryIndex);
}


void ImpressionistDoc::swapView()

{

	unsigned char * temp = m_ucBitmap;

	m_ucBitmap = m_ucPainting;

	m_ucPainting = temp;

	m_pUI->m_origView->refresh();

	m_pUI->m_paintView->refresh();

}

void ImpressionistDoc::mirrorView()
{
	memcpy(m_ucPainting, m_ucBitmap, 3 * m_nHeight * m_nWidth);
	addHistory();
	m_pUI->m_paintView->refresh();
}

void ImpressionistDoc::clearHistory()
{
	for (auto item : m_history) {
		delete[] item;
	}
	m_history.clear();
	m_nHistoryIndex = 0;
	printf("clear history\n");
}

void ImpressionistDoc::redoHistory()
{
	if (m_history.size() > 1 && m_nHistoryIndex < m_history.size() - 1) {
		++m_nHistoryIndex;
		memcpy(m_ucPainting, m_history[m_nHistoryIndex], 3 * m_nHeight * m_nWidth);
		m_pUI->m_paintView->refresh();
		printf("redo history onto index %d\n", m_nHistoryIndex);
	}
}


void ImpressionistDoc::undoHistory()
{
	if (m_history.size() > 1 && m_nHistoryIndex > 0 ) {
		--m_nHistoryIndex;
		memcpy(m_ucPainting, m_history[m_nHistoryIndex], 3 * m_nHeight * m_nWidth);
		m_pUI->m_paintView->refresh();
		printf("undo history onto index %d\n", m_nHistoryIndex);
	}
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	BrushType = type;
	m_pCurrentBrush	= ImpBrush::c_pBrushes[type];
}

int ImpressionistDoc::getBrushType()
{
	return BrushType;
}

void ImpressionistDoc::setStrokeDirection(int mode)
{
	m_nStrokeMode = mode;
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}

void ImpressionistDoc::setSize(int size)
{
	m_pUI->setSize(size);
}

//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(char *iname) 
{
	// try to open the image to read
	unsigned char*	data;
	int				width, 
					height;

	if ( (data=readBMP(iname, width, height))==NULL ) 
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth		= width;
	m_nPaintWidth	= width;
	m_nHeight		= height;
	m_nPaintHeight	= height;

	// release old storage
	if ( m_ucBitmap ) delete [] m_ucBitmap;
	if ( m_ucPainting ) delete [] m_ucPainting;
	
	if (m_ucGradientNorm) delete [] m_ucGradientNorm;
	if (m_ucGradientX) delete [] m_ucGradientX;
	if (m_ucGradientY) delete [] m_ucGradientY;
	if (m_ucIntensityMap) delete [] m_ucIntensityMap;
	if (m_ucBlurredIntensityMap) delete [] m_ucBlurredIntensityMap;
	if (m_ucEdgeMap) delete[] m_ucEdgeMap;
	if (m_ucGradientBitmap) delete[] m_ucGradientBitmap;
	if (m_ucAGradientX) delete[] m_ucAGradientX;
	if (m_ucAGradientY) delete[] m_ucAGradientY;
	if (m_ucAIntensityMap) delete[] m_ucAIntensityMap;
	if (m_ucABlurredIntensityMap) delete[] m_ucABlurredIntensityMap;
	if (m_ucAnotherEdgeMap) delete[] m_ucAnotherEdgeMap;

	m_ucBitmap		= data;

	// generate intensity map
	m_ucIntensityMap = new unsigned char[width * height];
	for (int i = 0; i < width*height; i++) {
		m_ucIntensityMap[i] = GetIntensity(m_ucBitmap[3 * i], m_ucBitmap[3 * i + 1], m_ucBitmap[3 * i + 2]);
	}
	/*
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			std::cout << (int)m_ucIntensityMap[i + j*width] << " ";
		}
		printf("\n");
	}*/
	printf("intensity\n");


	// blur the intensity map by Gaussian filter
	m_ucBlurredIntensityMap = new GLubyte[width * height];
	for (int i = 0; i < width; i++) {
		m_ucBlurredIntensityMap[i] = m_ucIntensityMap[i];
		m_ucBlurredIntensityMap[(height-1)*width+i] = m_ucIntensityMap[(height - 1)*width + i];
	}
	for (int i = 0; i < height; i++) {
		m_ucBlurredIntensityMap[i*width] = m_ucIntensityMap[i*width];
		m_ucBlurredIntensityMap[i*width+width-1] = m_ucIntensityMap[i*width+width-1];
	}
	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
			m_ucBlurredIntensityMap[i + j*width] =  (
				m_ucIntensityMap[(i - 1) + (j - 1)*width] + 2 * m_ucIntensityMap[(i)+(j - 1)*width] +
				m_ucIntensityMap[(i + 1) + (j - 1)*width] + 2 * m_ucIntensityMap[(i - 1) + (j)*width] +
				4 * m_ucIntensityMap[(i)+(j)*width] + 2 * m_ucIntensityMap[(i + 1) + (j)*width] +
				m_ucIntensityMap[(i - 1) + (j + 1)*width] + 2 * m_ucIntensityMap[(i)+(j + 1)*width] +
				m_ucIntensityMap[(i + 1) + (j + 1)*width]) / 16;
		}
	}
	/*
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			std::cout << (int )m_ucBlurredIntensityMap[i + j*width] << " ";
		}
		printf("\n");
	}*/
	printf("blur\n");

	// obtain the gradient value
	m_ucGradientNorm = new int[width*height];
	m_ucGradientX = new int[width*height];
	m_ucGradientY = new int[width*height];
	for (int i = 1; i < width - 1; ++i) {
		int j = 0;
		m_ucGradientX[i + j*width] = -2 * m_ucBlurredIntensityMap[(i - 1) + (j)*width] + 2 * m_ucBlurredIntensityMap[(i + 1) + (j)*width] -
			m_ucBlurredIntensityMap[(i - 1) + (j + 1)*width] + m_ucBlurredIntensityMap[(i + 1) + (j + 1)*width];
		m_ucGradientY[i + j*width] = -1 * m_ucBlurredIntensityMap[(i - 1) + (j + 1)*width] -
			2 * m_ucBlurredIntensityMap[(i)+(j + 1)*width] - m_ucBlurredIntensityMap[(i + 1) + (j + 1)*width];
	}
	for (int i = 1; i < width - 1; ++i) {
		int j = height - 1;
		m_ucGradientX[i + j*width] = -1 * m_ucBlurredIntensityMap[(i - 1) + (j - 1)*width] + m_ucBlurredIntensityMap[(i + 1) + (j - 1)*width]
			- 2 * m_ucBlurredIntensityMap[(i - 1) + (j)*width] + 2 * m_ucBlurredIntensityMap[(i + 1) + (j)*width];
		m_ucGradientY[i + j*width] = 1 * m_ucBlurredIntensityMap[(i - 1) + (j - 1)*width] + 2 * m_ucBlurredIntensityMap[(i)+(j - 1)*width]
			+ m_ucBlurredIntensityMap[(i + 1) + (j - 1)*width];
	}
	for (int j = 1; j < height - 1; ++j) {
		int i = 0;
		m_ucGradientX[i + j*width] = m_ucBlurredIntensityMap[(i + 1) + (j - 1)*width] + 2 * m_ucBlurredIntensityMap[(i + 1) + (j)*width] + m_ucBlurredIntensityMap[(i + 1) + (j + 1)*width];
		m_ucGradientY[i+j*width] = 2 * m_ucBlurredIntensityMap[(i)+(j - 1)*width] - 2 * m_ucBlurredIntensityMap[(i)+(j + 1)*width] + m_ucBlurredIntensityMap[(i + 1) + (j - 1)*width] - m_ucBlurredIntensityMap[(i + 1) + (j + 1)*width];
	}
	for (int j = 1; j < height - 1; ++j) {
		int i = width-1;
		m_ucGradientX[i + j*width] = -1 * m_ucBlurredIntensityMap[(i - 1) + (j - 1)*width] - 2 * m_ucBlurredIntensityMap[(i - 1) + (j)*width] - m_ucBlurredIntensityMap[(i - 1) + (j + 1)*width];
		m_ucGradientY[i + j*width] = 2 * m_ucBlurredIntensityMap[(i)+(j - 1)*width] - 2 * m_ucBlurredIntensityMap[(i)+(j + 1)*width] + 1 * m_ucBlurredIntensityMap[(i - 1) + (j - 1)*width] - 1 * m_ucBlurredIntensityMap[(i - 1) + (j + 1)*width];
	}
	m_ucGradientX[0] = 2 * m_ucBlurredIntensityMap[1] + m_ucBlurredIntensityMap[1 + width];
	m_ucGradientY[0] = -2 * m_ucBlurredIntensityMap[width] - m_ucBlurredIntensityMap[1+width];
	m_ucGradientX[width - 1] = -2 * m_ucBlurredIntensityMap[width-2] - 1 * m_ucBlurredIntensityMap[2*width-2];
	m_ucGradientY[width - 1] = -1 * m_ucBlurredIntensityMap[2*width-2]
		- 2 * m_ucBlurredIntensityMap[2*width-1];
	m_ucGradientX[(height - 1)*width] = m_ucBlurredIntensityMap[(height-2)*width +1] + 2 * m_ucBlurredIntensityMap[(height - 1)*width+1];
	m_ucGradientY[(height - 1)*width] = 2 * m_ucBlurredIntensityMap[(height - 2)*width]
		+ m_ucBlurredIntensityMap[(height - 2)*width+1];
	m_ucGradientX[height*width - 1] = -1 * m_ucBlurredIntensityMap[(height - 1)*width - 2] - 2 * m_ucBlurredIntensityMap[height*width-2];
	m_ucGradientY[height*width - 1] = 1 * m_ucBlurredIntensityMap[(height - 1)*width - 2] + 2 * m_ucBlurredIntensityMap[(height-1)*width-1];
	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
			m_ucGradientX[i + j*width] = 
				-1 * m_ucBlurredIntensityMap[(i - 1) + (j - 1)*width] + m_ucBlurredIntensityMap[(i + 1) + (j - 1)*width] 
				-2 * m_ucBlurredIntensityMap[(i - 1) + (j)*width] + 2 * m_ucBlurredIntensityMap[(i + 1) + (j)*width]
				-1 * m_ucBlurredIntensityMap[(i - 1) + (j + 1)*width] + m_ucBlurredIntensityMap[(i + 1) + (j + 1)*width];
			m_ucGradientY[i + j*width] =
				1 * m_ucBlurredIntensityMap[(i - 1) + (j - 1)*width] + 2 * m_ucBlurredIntensityMap[(i)+(j - 1)*width]
				+ m_ucBlurredIntensityMap[(i + 1) + (j - 1)*width] - 1 * m_ucBlurredIntensityMap[(i - 1) + (j + 1)*width]
				-2 * m_ucBlurredIntensityMap[(i)+(j + 1)*width] - m_ucBlurredIntensityMap[(i + 1) + (j + 1)*width];
		}
	}

	// generate edge image
	m_ucEdgeMap = new unsigned char[width*height*3];
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			m_ucGradientNorm[i+j*width] = sqrt(pow(m_ucGradientX[i + j*width], 2) + pow(m_ucGradientY[i + j*width], 2));
			m_ucEdgeMap[3*(i + j*width)] = m_ucEdgeMap[3 * (i + j*width)+1] = m_ucEdgeMap[3*(i + j*width) + 2] = m_ucGradientNorm[i + j*width] > 128 ? 255 : 0;
		}
	}

	
	/*for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			std::cout << m_ucGradientX[i + j*width] << " "<< m_ucGradientY[i + j*width] <<"||";
		}
		printf("\n");
	}/*
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			std::cout << m_ucGradientY[i + j*width] << " ";
		}
		printf("\n");
	}*/
	printf("gradient\n");


	// allocate space for draw view
	m_ucPainting	= new unsigned char [width*height*3];
	memset(m_ucPainting, 0, width*height*3);

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(), 
								m_pUI->m_mainWindow->y(), 
								width*2, 
								height+25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);	
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);	
	m_pUI->m_paintView->refresh();

	// refresh draw history
	clearHistory();
	addHistory();

	return 1;
}

int ImpressionistDoc::loadAnotherImage(char * iname)
{
	// try to open the image to read
	unsigned char*	data;
	int				width,height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	if (width != m_nWidth || height != m_nHeight)
	{
		fl_alert("Different size BMP!");
		return 0;
	}

	printf("release\n");
	// release old storage
	if (m_ucBitmap) delete[] m_ucBitmap;
	//if (m_ucPainting) delete[] m_ucPainting;

	if (m_ucGradientNorm) delete[] m_ucGradientNorm;
	if (m_ucGradientX) delete[] m_ucGradientX;
	if (m_ucGradientY) delete[] m_ucGradientY;
	if (m_ucIntensityMap) delete[] m_ucIntensityMap;
	if (m_ucBlurredIntensityMap) delete[] m_ucBlurredIntensityMap;
	if (m_ucEdgeMap) delete[] m_ucEdgeMap;
	if (m_ucAnotherEdgeMap) delete[] m_ucAnotherEdgeMap;
	m_ucBitmap = data;


	printf("load\n");
	// generate intensity map
	m_ucIntensityMap = new unsigned char[width * height];
	for (int i = 0; i < width*height; i++) {
		m_ucIntensityMap[i] = GetIntensity(m_ucBitmap[3 * i], m_ucBitmap[3 * i + 1], m_ucBitmap[3 * i + 2]);
	}
	/*
	for (int j = 0; j < height; ++j) {
	for (int i = 0; i < width; ++i) {
	std::cout << (int)m_ucIntensityMap[i + j*width] << " ";
	}
	printf("\n");
	}*/
	printf("intensity\n");


	// blur the intensity map by Gaussian filter
	m_ucBlurredIntensityMap = new GLubyte[width * height];
	for (int i = 0; i < width; i++) {
		m_ucBlurredIntensityMap[i] = m_ucIntensityMap[i];
		m_ucBlurredIntensityMap[(height - 1)*width + i] = m_ucIntensityMap[(height - 1)*width + i];
	}
	for (int i = 0; i < height; i++) {
		m_ucBlurredIntensityMap[i*width] = m_ucIntensityMap[i*width];
		m_ucBlurredIntensityMap[i*width + width - 1] = m_ucIntensityMap[i*width + width - 1];
	}
	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
			m_ucBlurredIntensityMap[i + j*width] = (
				m_ucIntensityMap[(i - 1) + (j - 1)*width] + 2 * m_ucIntensityMap[(i)+(j - 1)*width] +
				m_ucIntensityMap[(i + 1) + (j - 1)*width] + 2 * m_ucIntensityMap[(i - 1) + (j)*width] +
				4 * m_ucIntensityMap[(i)+(j)*width] + 2 * m_ucIntensityMap[(i + 1) + (j)*width] +
				m_ucIntensityMap[(i - 1) + (j + 1)*width] + 2 * m_ucIntensityMap[(i)+(j + 1)*width] +
				m_ucIntensityMap[(i + 1) + (j + 1)*width]) / 16;
		}
	}
	/*
	for (int j = 0; j < height; ++j) {
	for (int i = 0; i < width; ++i) {
	std::cout << (int )m_ucBlurredIntensityMap[i + j*width] << " ";
	}
	printf("\n");
	}*/
	printf("blur\n");


	// obtain the gradient value
	m_ucGradientNorm = new int[width*height];
	m_ucGradientX = new int[width*height];
	m_ucGradientY = new int[width*height];
	for (int i = 1; i < width - 1; ++i) {
		int j = 0;
		m_ucGradientX[i + j*width] = -2 * m_ucBlurredIntensityMap[(i - 1) + (j)*width] + 2 * m_ucBlurredIntensityMap[(i + 1) + (j)*width] -
			m_ucBlurredIntensityMap[(i - 1) + (j + 1)*width] + m_ucBlurredIntensityMap[(i + 1) + (j + 1)*width];
		m_ucGradientY[i + j*width] = -1 * m_ucBlurredIntensityMap[(i - 1) + (j + 1)*width] -
			2 * m_ucBlurredIntensityMap[(i)+(j + 1)*width] - m_ucBlurredIntensityMap[(i + 1) + (j + 1)*width];
	}
	for (int i = 1; i < width - 1; ++i) {
		int j = height - 1;
		m_ucGradientX[i + j*width] = -1 * m_ucBlurredIntensityMap[(i - 1) + (j - 1)*width] + m_ucBlurredIntensityMap[(i + 1) + (j - 1)*width]
			- 2 * m_ucBlurredIntensityMap[(i - 1) + (j)*width] + 2 * m_ucBlurredIntensityMap[(i + 1) + (j)*width];
		m_ucGradientY[i + j*width] = 1 * m_ucBlurredIntensityMap[(i - 1) + (j - 1)*width] + 2 * m_ucBlurredIntensityMap[(i)+(j - 1)*width]
			+ m_ucBlurredIntensityMap[(i + 1) + (j - 1)*width];
	}
	for (int j = 1; j < height - 1; ++j) {
		int i = 0;
		m_ucGradientX[i + j*width] = m_ucBlurredIntensityMap[(i + 1) + (j - 1)*width] + 2 * m_ucBlurredIntensityMap[(i + 1) + (j)*width] + m_ucBlurredIntensityMap[(i + 1) + (j + 1)*width];
		m_ucGradientY[i + j*width] = 2 * m_ucBlurredIntensityMap[(i)+(j - 1)*width] - 2 * m_ucBlurredIntensityMap[(i)+(j + 1)*width] + m_ucBlurredIntensityMap[(i + 1) + (j - 1)*width] - m_ucBlurredIntensityMap[(i + 1) + (j + 1)*width];
	}
	for (int j = 1; j < height - 1; ++j) {
		int i = width - 1;
		m_ucGradientX[i + j*width] = -1 * m_ucBlurredIntensityMap[(i - 1) + (j - 1)*width] - 2 * m_ucBlurredIntensityMap[(i - 1) + (j)*width] - m_ucBlurredIntensityMap[(i - 1) + (j + 1)*width];
		m_ucGradientY[i + j*width] = 2 * m_ucBlurredIntensityMap[(i)+(j - 1)*width] - 2 * m_ucBlurredIntensityMap[(i)+(j + 1)*width] + 1 * m_ucBlurredIntensityMap[(i - 1) + (j - 1)*width] - 1 * m_ucBlurredIntensityMap[(i - 1) + (j + 1)*width];
	}
	m_ucGradientX[0] = 2 * m_ucBlurredIntensityMap[1] + m_ucBlurredIntensityMap[1 + width];
	m_ucGradientY[0] = -2 * m_ucBlurredIntensityMap[width] - m_ucBlurredIntensityMap[1 + width];
	m_ucGradientX[width - 1] = -2 * m_ucBlurredIntensityMap[width - 2] - 1 * m_ucBlurredIntensityMap[2 * width - 2];
	m_ucGradientY[width - 1] = -1 * m_ucBlurredIntensityMap[2 * width - 2]
		- 2 * m_ucBlurredIntensityMap[2 * width - 1];
	m_ucGradientX[(height - 1)*width] = m_ucBlurredIntensityMap[(height - 2)*width + 1] + 2 * m_ucBlurredIntensityMap[(height - 1)*width + 1];
	m_ucGradientY[(height - 1)*width] = 2 * m_ucBlurredIntensityMap[(height - 2)*width]
		+ m_ucBlurredIntensityMap[(height - 2)*width + 1];
	m_ucGradientX[height*width - 1] = -1 * m_ucBlurredIntensityMap[(height - 1)*width - 2] - 2 * m_ucBlurredIntensityMap[height*width - 2];
	m_ucGradientY[height*width - 1] = 1 * m_ucBlurredIntensityMap[(height - 1)*width - 2] + 2 * m_ucBlurredIntensityMap[(height - 1)*width - 1];
	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
			m_ucGradientX[i + j*width] =
				-1 * m_ucBlurredIntensityMap[(i - 1) + (j - 1)*width] + m_ucBlurredIntensityMap[(i + 1) + (j - 1)*width]
				- 2 * m_ucBlurredIntensityMap[(i - 1) + (j)*width] + 2 * m_ucBlurredIntensityMap[(i + 1) + (j)*width]
				- 1 * m_ucBlurredIntensityMap[(i - 1) + (j + 1)*width] + m_ucBlurredIntensityMap[(i + 1) + (j + 1)*width];
			m_ucGradientY[i + j*width] =
				1 * m_ucBlurredIntensityMap[(i - 1) + (j - 1)*width] + 2 * m_ucBlurredIntensityMap[(i)+(j - 1)*width]
				+ m_ucBlurredIntensityMap[(i + 1) + (j - 1)*width] - 1 * m_ucBlurredIntensityMap[(i - 1) + (j + 1)*width]
				- 2 * m_ucBlurredIntensityMap[(i)+(j + 1)*width] - m_ucBlurredIntensityMap[(i + 1) + (j + 1)*width];
		}
	}

	// generate edge image
	m_ucEdgeMap = new unsigned char[width*height * 3];
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			m_ucGradientNorm[i + j*width] = sqrt(pow(m_ucGradientX[i + j*width], 2) + pow(m_ucGradientY[i + j*width], 2));
			m_ucEdgeMap[3 * (i + j*width)] = m_ucEdgeMap[3 * (i + j*width) + 1] = m_ucEdgeMap[3 * (i + j*width) + 2] = m_ucGradientNorm[i + j*width] > 128 ? 255 : 0;
		}
	}

	/*
	for (int j = 0; j < height; ++j) {
	for (int i = 0; i < width; ++i) {
	std::cout << m_ucGradientX[i + j*width] << " ";
	}
	printf("\n");
	}
	for (int j = 0; j < height; ++j) {
	for (int i = 0; i < width; ++i) {
	std::cout << m_ucGradientY[i + j*width] << " ";
	}
	printf("\n");
	}*/
	printf("gradient\n");


	// display it on origView
	m_pUI->m_origView->refresh();

	printf("newimage\n");
	m_pUI->m_paintView->refresh();

	return 1;
}

int ImpressionistDoc::loadGradientImage(char * iname)
{
	// try to open the image to read
	unsigned char*	data;
	int				width, height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	if (width != m_nWidth || height != m_nHeight)
	{
		fl_alert("Different size BMP!");
		return 0;
	}

	printf("release\n");
	// release old storage
	if (m_ucGradientBitmap) delete[] m_ucGradientBitmap;
	// if (m_ucPainting) delete[] m_ucPainting;
	if (m_ucAGradientX) delete[] m_ucAGradientX;
	if (m_ucAGradientY) delete[] m_ucAGradientY;
	if (m_ucAIntensityMap) delete[] m_ucAIntensityMap;
	if (m_ucABlurredIntensityMap) delete[] m_ucABlurredIntensityMap;
	//if (m_ucEdgeMap) delete[] m_ucEdgeMap;
	m_ucGradientBitmap = data;
	printf("load\n");


	// generate intensity map
	m_ucAIntensityMap = new unsigned char[width * height];
	for (int i = 0; i < width*height; i++) {
		m_ucAIntensityMap[i] = GetIntensity(m_ucGradientBitmap[3 * i], m_ucGradientBitmap[3 * i + 1], m_ucGradientBitmap[3 * i + 2]);
	}
	/*
	for (int j = 0; j < height; ++j) {
	for (int i = 0; i < width; ++i) {
	std::cout << (int)m_ucAIntensityMap[i + j*width] << " ";
	}
	printf("\n");
	}*/
	printf("intensity\n");


	// blur the intensity map by Gaussian filter
	m_ucABlurredIntensityMap = new GLubyte[width * height];
	for (int i = 0; i < width; i++) {
		m_ucABlurredIntensityMap[i] = m_ucAIntensityMap[i];
		m_ucABlurredIntensityMap[(height - 1)*width + i] = m_ucAIntensityMap[(height - 1)*width + i];
	}
	for (int i = 0; i < height; i++) {
		m_ucABlurredIntensityMap[i*width] = m_ucAIntensityMap[i*width];
		m_ucABlurredIntensityMap[i*width + width - 1] = m_ucAIntensityMap[i*width + width - 1];
	}
	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
			m_ucABlurredIntensityMap[i + j*width] = (
				m_ucAIntensityMap[(i - 1) + (j - 1)*width] + 2 * m_ucAIntensityMap[(i)+(j - 1)*width] +
				m_ucAIntensityMap[(i + 1) + (j - 1)*width] + 2 * m_ucAIntensityMap[(i - 1) + (j)*width] +
				4 * m_ucAIntensityMap[(i)+(j)*width] + 2 * m_ucAIntensityMap[(i + 1) + (j)*width] +
				m_ucAIntensityMap[(i - 1) + (j + 1)*width] + 2 * m_ucAIntensityMap[(i)+(j + 1)*width] +
				m_ucAIntensityMap[(i + 1) + (j + 1)*width]) / 16;
		}
	}
	/*
	for (int j = 0; j < height; ++j) {
	for (int i = 0; i < width; ++i) {
	std::cout << (int )m_ucABlurredIntensityMap[i + j*width] << " ";
	}
	printf("\n");
	}*/
	printf("blur\n");


	// obtain the gradient value
	m_ucGradientNorm = new int[width*height];
	m_ucAGradientX = new int[width*height];
	m_ucAGradientY = new int[width*height];
	for (int i = 1; i < width - 1; ++i) {
		int j = 0;
		m_ucAGradientX[i + j*width] = -2 * m_ucABlurredIntensityMap[(i - 1) + (j)*width] + 2 * m_ucABlurredIntensityMap[(i + 1) + (j)*width] -
			m_ucABlurredIntensityMap[(i - 1) + (j + 1)*width] + m_ucABlurredIntensityMap[(i + 1) + (j + 1)*width];
		m_ucAGradientY[i + j*width] = -1 * m_ucABlurredIntensityMap[(i - 1) + (j + 1)*width] -
			2 * m_ucABlurredIntensityMap[(i)+(j + 1)*width] - m_ucABlurredIntensityMap[(i + 1) + (j + 1)*width];
	}
	for (int i = 1; i < width - 1; ++i) {
		int j = height - 1;
		m_ucAGradientX[i + j*width] = -1 * m_ucABlurredIntensityMap[(i - 1) + (j - 1)*width] + m_ucABlurredIntensityMap[(i + 1) + (j - 1)*width]
			- 2 * m_ucABlurredIntensityMap[(i - 1) + (j)*width] + 2 * m_ucABlurredIntensityMap[(i + 1) + (j)*width];
		m_ucAGradientY[i + j*width] = 1 * m_ucABlurredIntensityMap[(i - 1) + (j - 1)*width] + 2 * m_ucABlurredIntensityMap[(i)+(j - 1)*width]
			+ m_ucABlurredIntensityMap[(i + 1) + (j - 1)*width];
	}
	for (int j = 1; j < height - 1; ++j) {
		int i = 0;
		m_ucAGradientX[i + j*width] = m_ucABlurredIntensityMap[(i + 1) + (j - 1)*width] + 2 * m_ucABlurredIntensityMap[(i + 1) + (j)*width] + m_ucABlurredIntensityMap[(i + 1) + (j + 1)*width];
		m_ucAGradientY[i + j*width] = 2 * m_ucABlurredIntensityMap[(i)+(j - 1)*width] - 2 * m_ucABlurredIntensityMap[(i)+(j + 1)*width] + m_ucABlurredIntensityMap[(i + 1) + (j - 1)*width] - m_ucABlurredIntensityMap[(i + 1) + (j + 1)*width];
	}
	for (int j = 1; j < height - 1; ++j) {
		int i = width - 1;
		m_ucAGradientX[i + j*width] = -1 * m_ucABlurredIntensityMap[(i - 1) + (j - 1)*width] - 2 * m_ucABlurredIntensityMap[(i - 1) + (j)*width] - m_ucABlurredIntensityMap[(i - 1) + (j + 1)*width];
		m_ucAGradientY[i + j*width] = 2 * m_ucABlurredIntensityMap[(i)+(j - 1)*width] - 2 * m_ucABlurredIntensityMap[(i)+(j + 1)*width] + 1 * m_ucABlurredIntensityMap[(i - 1) + (j - 1)*width] - 1 * m_ucABlurredIntensityMap[(i - 1) + (j + 1)*width];
	}
	m_ucAGradientX[0] = 2 * m_ucABlurredIntensityMap[1] + m_ucABlurredIntensityMap[1 + width];
	m_ucAGradientY[0] = -2 * m_ucABlurredIntensityMap[width] - m_ucABlurredIntensityMap[1 + width];
	m_ucAGradientX[width - 1] = -2 * m_ucABlurredIntensityMap[width - 2] - 1 * m_ucABlurredIntensityMap[2 * width - 2];
	m_ucAGradientY[width - 1] = -1 * m_ucABlurredIntensityMap[2 * width - 2]
		- 2 * m_ucABlurredIntensityMap[2 * width - 1];
	m_ucAGradientX[(height - 1)*width] = m_ucABlurredIntensityMap[(height - 2)*width + 1] + 2 * m_ucABlurredIntensityMap[(height - 1)*width + 1];
	m_ucAGradientY[(height - 1)*width] = 2 * m_ucABlurredIntensityMap[(height - 2)*width]
		+ m_ucABlurredIntensityMap[(height - 2)*width + 1];
	m_ucAGradientX[height*width - 1] = -1 * m_ucABlurredIntensityMap[(height - 1)*width - 2] - 2 * m_ucABlurredIntensityMap[height*width - 2];
	m_ucAGradientY[height*width - 1] = 1 * m_ucABlurredIntensityMap[(height - 1)*width - 2] + 2 * m_ucABlurredIntensityMap[(height - 1)*width - 1];
	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
			m_ucAGradientX[i + j*width] =
				-1 * m_ucABlurredIntensityMap[(i - 1) + (j - 1)*width] + m_ucABlurredIntensityMap[(i + 1) + (j - 1)*width]
				- 2 * m_ucABlurredIntensityMap[(i - 1) + (j)*width] + 2 * m_ucABlurredIntensityMap[(i + 1) + (j)*width]
				- 1 * m_ucABlurredIntensityMap[(i - 1) + (j + 1)*width] + m_ucABlurredIntensityMap[(i + 1) + (j + 1)*width];
			m_ucAGradientY[i + j*width] =
				1 * m_ucABlurredIntensityMap[(i - 1) + (j - 1)*width] + 2 * m_ucABlurredIntensityMap[(i)+(j - 1)*width]
				+ m_ucABlurredIntensityMap[(i + 1) + (j - 1)*width] - 1 * m_ucABlurredIntensityMap[(i - 1) + (j + 1)*width]
				- 2 * m_ucABlurredIntensityMap[(i)+(j + 1)*width] - m_ucABlurredIntensityMap[(i + 1) + (j + 1)*width];
		}
	}


	/*
	for (int j = 0; j < height; ++j) {
	for (int i = 0; i < width; ++i) {
	std::cout << m_ucAGradientX[i + j*width] << " ";
	}
	printf("\n");
	}
	
	for (int j = 0; j < height; ++j) {
	for (int i = 0; i < width; ++i) {
	std::cout << m_ucAGradientY[i + j*width] << " ";
	}
	printf("\n");
	}*/
	printf("gradient\n");


	return 1;
}

int ImpressionistDoc::loadEdgeImage(char * iname)
{
	// try to open the image to read
	unsigned char*	data;
	int				width, height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	if (width != m_nWidth || height != m_nHeight)
	{
		fl_alert("Different size BMP!");
		return 0;
	}
	if (m_ucAnotherEdgeMap) delete[] m_ucAnotherEdgeMap;
	m_ucAnotherEdgeMap = data;
	//for (int i = 0; i < width*height; ++i) {printf("%d %d %d\n", m_ucAnotherEdgeMap[3 * i], m_ucAnotherEdgeMap[3 * i+1], m_ucAnotherEdgeMap[3 * i+2]);}
	return 1;
}


//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(char *iname) 
{

	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);

	return 1;
}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() 
{

	// Release old storage
	if ( m_ucPainting ) 
	{
		delete [] m_ucPainting;

		// allocate space for draw view
		m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);

		// refresh paint view as well	
		m_pUI->m_paintView->refresh();
	}
	
	return 0;
}

//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( int x, int y )
{
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nWidth ) 
		x = m_nWidth-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nHeight ) 
		y = m_nHeight-1;

	return (GLubyte*)(m_ucBitmap + 3 * (y*m_nWidth + x));
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const Point p )
{
	return GetOriginalPixel( p.x, p.y );
}

GLubyte ImpressionistDoc::GetIntensity(GLubyte r, GLubyte g, GLubyte b)
{
	return (GLubyte) 0.299 * r + 0.587 * g + 0.144 * b;
}

void ImpressionistDoc::applyDim()
{
	double dim = m_pUI->m_nDimvalue;
	int w = m_nWidth;
	int h = m_nHeight;
	unsigned int mark = 0;
	std::vector<int> & v = m_pUI->m_paintView->UpdatePos;
	unsigned int size = v.size();
	//std::cout << size << std::endl;
	//for (int i = 0; i < v.size(); ++i) printf("%d %d\n", i, v[i]);
	for (int i = 0; i < w*h; ++i) {
		if (size != 0 && mark != size && v[mark] == i) {
			//printf("%d %d\n", mark, v[mark]);
			++mark;
			continue;
		}
		m_ucPainting[3 * i] = m_ucBitmap[3 * i] * dim;
		m_ucPainting[3 * i+1] = m_ucBitmap[3 * i+1] * dim;
		m_ucPainting[3 * i+2] = m_ucBitmap[3 * i+2] * dim;
	}
	//printf("dimend\n");
	m_pUI->m_paintView->refresh();
}

void ImpressionistDoc::disolve()
{
	int w = m_nWidth;
	int h = m_nHeight;
	if (m_ucPainting) delete[] m_ucPainting;
	m_ucPainting = new GLubyte[w*h * 3];
	for (int i = 0; i < w*h; ++i) {
		m_ucPainting[3 * i] = m_ucBitmap[3 * i] * 0.5 + m_ucGradientBitmap[3 * i] * 0.5;
		m_ucPainting[3 * i+1] = m_ucBitmap[3 * i+1] * 0.5 + m_ucGradientBitmap[3 * i+1] * 0.5;
		m_ucPainting[3 * i+2] = m_ucBitmap[3 * i+2] * 0.5 + m_ucGradientBitmap[3 * i+2] * 0.5;
	}
	m_pUI->m_paintView->refresh();
}

void ImpressionistDoc::updateEdgeMap()
{
	int width = m_nWidth; int height = m_nHeight;
	int threshold = m_pUI->m_nThreshold;
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			m_ucEdgeMap[3 * (i + j*width)] = m_ucEdgeMap[3 * (i + j*width) + 1] = m_ucEdgeMap[3 * (i + j*width) + 2] = m_ucGradientNorm[i + j*width] > threshold ? 255 : 0;
		}
	}
}

