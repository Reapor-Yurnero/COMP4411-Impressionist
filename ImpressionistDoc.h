// 
// impressionistDoc.h
//
// header file for Doc 
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include "impressionist.h"
#include "bitmap.h"

class ImpressionistUI;

class ImpressionistDoc 
{
// Functions
public:
	ImpressionistDoc();

	void	setUI(ImpressionistUI* ui);		// Assign the UI to use

	int		loadImage(char *iname);			// called by the UI to load image
	int		saveImage(char *iname);			// called by the UI to save image


	int     clearCanvas();                  // called by the UI to clear the drawing canvas
	void	setBrushType(int type);			// called by the UI to set the brushType
	int		getBrushType();					// called to get brush type ID [0, 1,...
	void	setStrokeDirection(int mode);	// called by the UI to set the stroke direction mode
	int		getSize();						// get the UI size for brush
	void	setSize(int size);				// set the UI size for brush
	void	setLineAngle(int angle);
	char*	getImageName();					// get the current image name
	void	swapview();						// swap the original and painting view
	void	updatePaintingView();
	

// Attributes
public:
	// Dimensions of original window.
	int				m_nWidth, 
					m_nHeight;
	// Dimensions of the paint window.
	int				m_nPaintWidth, 
					m_nPaintHeight;	
	// Bitmaps for original image and painting.
	unsigned char*	m_ucBitmap;
	unsigned char*	m_ucPainting;
	
	// Bitmaps for the intensity image
	unsigned char*	m_ucIntensityMap;
	unsigned char*	m_ucBlurredIntensityMap;

	// Matrices for gradient
	int*			m_ucGradientX;
	int*			m_ucGradientY;
	int*			m_ucGradientNorm;


	// The current active brush.
	ImpBrush*			m_pCurrentBrush;
	// The current stroke direction mode.
	int				m_nStrokeMode;
	// Size of the brush.
	int m_nSize;							

	ImpressionistUI*	m_pUI;

// Operations
public:
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel( int x, int y );   
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel( const Point p );  
	// Get the gradient Map of the original picture
	// void GetGradient(unsigned char* bitmap, int* x, int* y, int* norm);
	// Get intensity value from a RGB pixel
	GLubyte GetIntensity(GLubyte r, GLubyte g, GLubyte b);



private:
	char			m_imageName[256];
	int				BrushType;

};

extern void MessageBox(char *message);

#endif
