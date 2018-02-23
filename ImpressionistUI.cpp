//
// impressionistUI.h
//
// The user interface part for the program.
//


#include <FL/fl_ask.h>

#include <math.h>

#include "impressionistUI.h"
#include "impressionistDoc.h"

/*
//------------------------------ Widget Examples -------------------------------------------------
Here is some example code for all of the widgets that you may need to add to the 
project.  You can copy and paste these into your code and then change them to 
make them look how you want.  Descriptions for all of the widgets here can be found 
in links on the fltk help session page.

//---------Window/Dialog and Menubar-----------------------------------
	
	//----To install a window--------------------------
	Fl_Window* myWindow = new Fl_Window(600, 300, "MyWindow");
		myWindow->user_data((void*)(this));	// record self to be used by static callback functions
		
		// install menu bar
		myMenubar = new Fl_Menu_Bar(0, 0, 600, 25);
		Fl_Menu_Item ImpressionistUI::myMenuItems[] = {
			{ "&File",		0, 0, 0, FL_SUBMENU },
				{ "&Load...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load },
				{ "&Save...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save }.
				{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
				{ 0 },
			{ "&Edit",		0, 0, 0, FL_SUBMENU },
				{ "&Copy",FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_copy, (void *)COPY },
				{ "&Cut",	FL_ALT + 'x', (Fl_Callback *)ImpressionistUI::cb_cut, (void *)CUT },
				{ "&Paste",	FL_ALT + 'v', (Fl_Callback *)ImpressionistUI::cb_paste, (void *)PASTE },
				{ 0 },
			{ "&Help",		0, 0, 0, FL_SUBMENU },
				{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
				{ 0 },
			{ 0 }
		};
		myMenubar->menu(myMenuItems);
    m_mainWindow->end();

	//----The window callback--------------------------
	// One of the callbacks
	void ImpressionistUI::cb_load(Fl_Menu_* o, void* v) 
	{	
		ImpressionistDoc *pDoc=whoami(o)->getDocument();

		char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
		if (newfile != NULL) {
			pDoc->loadImage(newfile);
		}
	}


//------------Slider---------------------------------------

	//----To install a slider--------------------------
	Fl_Value_Slider * mySlider = new Fl_Value_Slider(10, 80, 300, 20, "My Value");
	mySlider->user_data((void*)(this));	// record self to be used by static callback functions
	mySlider->type(FL_HOR_NICE_SLIDER);
    mySlider->labelfont(FL_COURIER);
    mySlider->labelsize(12);
	mySlider->minimum(1);
	mySlider->maximum(40);
	mySlider->step(1);
	mySlider->value(m_nMyValue);
	mySlider->align(FL_ALIGN_RIGHT);
	mySlider->callback(cb_MyValueSlides);

	//----The slider callback--------------------------
	void ImpressionistUI::cb_MyValueSlides(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyValue=int( ((Fl_Slider *)o)->value() ) ;
	}
	

//------------Choice---------------------------------------
	
	//----To install a choice--------------------------
	Fl_Choice * myChoice = new Fl_Choice(50,10,150,25,"&myChoiceLabel");
	myChoice->user_data((void*)(this));	 // record self to be used by static callback functions
	Fl_Menu_Item ImpressionistUI::myChoiceMenu[3+1] = {
	  {"one",FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)ONE},
	  {"two",FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)TWO},
	  {"three",FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)THREE},
	  {0}
	};
	myChoice->menu(myChoiceMenu);
	myChoice->callback(cb_myChoice);
	
	//-----The choice callback-------------------------
	void ImpressionistUI::cb_myChoice(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
		ImpressionistDoc* pDoc=pUI->getDocument();

		int type=(int)v;

		pDoc->setMyType(type);
	}


//------------Button---------------------------------------

	//---To install a button---------------------------
	Fl_Button* myButton = new Fl_Button(330,220,50,20,"&myButtonLabel");
	myButton->user_data((void*)(this));   // record self to be used by static callback functions
	myButton->callback(cb_myButton);

	//---The button callback---------------------------
	void ImpressionistUI::cb_myButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI*)(o->user_data()));
		ImpressionistDoc* pDoc = pUI->getDocument();
		pDoc->startPainting();
	}


//---------Light Button------------------------------------
	
	//---To install a light button---------------------
	Fl_Light_Button* myLightButton = new Fl_Light_Button(240,10,150,25,"&myLightButtonLabel");
	myLightButton->user_data((void*)(this));   // record self to be used by static callback functions
	myLightButton->callback(cb_myLightButton);

	//---The light button callback---------------------
	void ImpressionistUI::cb_myLightButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI *pUI=((ImpressionistUI*)(o->user_data()));

		if (pUI->myBool==TRUE) pUI->myBool=FALSE;
		else pUI->myBool=TRUE;
	}

//----------Int Input--------------------------------------

    //---To install an int input-----------------------
	Fl_Int_Input* myInput = new Fl_Int_Input(200, 50, 5, 5, "&My Input");
	myInput->user_data((void*)(this));   // record self to be used by static callback functions
	myInput->callback(cb_myInput);

	//---The int input callback------------------------
	void ImpressionistUI::cb_myInput(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyInputValue=int( ((Fl_Int_Input *)o)->value() );
	}

//------------------------------------------------------------------------------------------------
*/

//------------------------------------- Help Functions --------------------------------------------

//------------------------------------------------------------
// This returns the UI, given the menu item.  It provides a
// link from the menu items to the UI
//------------------------------------------------------------
ImpressionistUI* ImpressionistUI::whoami(Fl_Menu_* o)	
{
	return ( (ImpressionistUI*)(o->parent()->user_data()) );
}


//--------------------------------- Callback Functions --------------------------------------------

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen image
// This is called by the UI when the load image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_load_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
	if (newfile != NULL) {
		pDoc->loadImage(newfile);
	}
}

void ImpressionistUI::cb_load_another_image(Fl_Menu_ * o, void * v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL) {
		pDoc->loadAnotherImage(newfile);
	}
}

void ImpressionistUI::cb_load_gradient_image(Fl_Menu_ * o, void * v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL) {
		pDoc->loadGradientImage(newfile);
	}
}


//------------------------------------------------------------------
// Brings up a file chooser and then saves the painted image
// This is called by the UI when the save image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_save_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Save File?", "*.bmp", "save.bmp" );
	if (newfile != NULL) {
		pDoc->saveImage(newfile);
	}
}

//-------------------------------------------------------------
// Brings up the paint dialog
// This is called by the UI when the brushes menu item
// is chosen
//-------------------------------------------------------------
void ImpressionistUI::cb_brushes(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_brushDialog->show();
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc=whoami(o)->getDocument();

	pDoc->clearCanvas();
}

//------------------------------------------------------------
// Causes the Impressionist program to exit
// Called by the UI when the quit menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_exit(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_mainWindow->hide();
	whoami(o)->m_brushDialog->hide();

}

//------------------------------------------------------------
// Undo the previous step
// Called by the UI when the undo menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_undo(Fl_Menu_* o, void* v)
{
	whoami(o)->m_pDoc->undoHistory();
}

//------------------------------------------------------------
// Redo the next step
// Called by the UI when the undo menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_redo(Fl_Menu_* o, void* v)
{
	whoami(o)->m_pDoc->redoHistory();
}

void ImpressionistUI::cb_swap(Fl_Menu_ * o, void * v)
{
	whoami(o)->getDocument()->swapview();
}

void ImpressionistUI::cb_EdgeView(Fl_Menu_ * o, void * v)
{
	whoami(o)->m_origView->imageChoice = 1;
	whoami(o)->m_origView->refresh();
}

void ImpressionistUI::cb_OriginalView(Fl_Menu_ * o, void * v)
{
	whoami(o)->m_origView->imageChoice = 0;
	whoami(o)->m_origView->refresh();
}

void ImpressionistUI::cb_GradientView(Fl_Menu_ * o, void * v)
{
	whoami(o)->m_origView->imageChoice = 2;
	whoami(o)->m_origView->refresh();
}

void ImpressionistUI::cb_RGBScale(Fl_Menu_ * o, void * v)
{
	whoami(o)->m_RGBConfigDialog->show();
}

void ImpressionistUI::cb_Disolve(Fl_Menu_ * o, void * v)
{
	whoami(o)->getDocument()->disolve();
}

//-----------------------------------------------------------
// Brings up an about dialog box
// Called by the UI when the about menu item is chosen
//-----------------------------------------------------------
void ImpressionistUI::cb_about(Fl_Menu_* o, void* v) 
{
	fl_message("Impressionist FLTK version for CS341, Spring 2002");
}

//------- UI should keep track of the current for all the controls for answering the query from Doc ---------
//-------------------------------------------------------------
// Sets the type of brush to use to the one chosen in the brush 
// choice.  
// Called by the UI when a brush is chosen in the brush choice
//-------------------------------------------------------------
void ImpressionistUI::cb_brushChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc=pUI->getDocument();

	int type=(int)v;


	pDoc->setBrushType(type);
}

//-------------------------------------------------------------
// Sets the mode of stroke direction 
// Called by the UI when a mode is chosen in the line choice
//-------------------------------------------------------------
void ImpressionistUI::cb_strokeChoice(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI = ((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	int mode = (int)v;
	
	pDoc->setStrokeDirection(mode);
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->clearCanvas();
}


//-----------------------------------------------------------
// Updates the brush size to use from the value of the size
// slider
// Called by the UI when the size slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_sizeSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nSize=int( ((Fl_Slider *)o)->value() ) ;
}

//-----------------------------------------------------------
// Updates the line width to use from the value of the size
// slider
// Called by the UI when the width slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_widthSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nLineWidth = int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_angleSlides(Fl_Widget * o, void * v)
{
	((ImpressionistUI*)(o->user_data()))->m_nLineAngle = int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_alphaSlides(Fl_Widget * o, void * v)
{
	((ImpressionistUI*)(o->user_data()))->m_nOpacity = double(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_RScaleSlides(Fl_Widget * o, void * v)
{
	((ImpressionistUI*)(o->user_data()))->m_RScale = double(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_GScaleSlides(Fl_Widget * o, void * v)
{
	((ImpressionistUI*)(o->user_data()))->m_GScale = double(((Fl_Slider *)o)->value());

}

void ImpressionistUI::cb_BScaleSlides(Fl_Widget * o, void * v)
{
	((ImpressionistUI*)(o->user_data()))->m_BScale = double(((Fl_Slider *)o)->value());

}

void ImpressionistUI::cb_SpacingSlides(Fl_Widget * o, void * v)
{
	((ImpressionistUI*)(o->user_data()))->m_nSpacing = int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_AutoPaintButton(Fl_Widget * o, void * v)
{
	((ImpressionistUI*)(o->user_data()))->m_paintView->AutoPaintTrigger();
}

void ImpressionistUI::cb_DimSlides(Fl_Widget * o, void * v)
{
	((ImpressionistUI*)(o->user_data()))->m_nDimvalue = double(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_DimButton(Fl_Widget * o, void * v)
{
	((ImpressionistUI*)(o->user_data()))->m_pDoc->applyDim();
}

//---------------------------------- per instance functions --------------------------------------

//------------------------------------------------
// Return the ImpressionistDoc used
//------------------------------------------------
ImpressionistDoc* ImpressionistUI::getDocument()
{
	return m_pDoc;
}

//------------------------------------------------
// Draw the main window
//------------------------------------------------
void ImpressionistUI::show() {
	m_mainWindow->show();
	m_paintView->show();
	m_origView->show();
}

//------------------------------------------------
// Change the paint and original window sizes to 
// w by h
//------------------------------------------------
void ImpressionistUI::resize_windows(int w, int h) {
	m_paintView->size(w,h);
	m_origView->size(w,h);
}

//------------------------------------------------ 
// Set the ImpressionistDoc used by the UI to 
// communicate with the brushes 
//------------------------------------------------
void ImpressionistUI::setDocument(ImpressionistDoc* doc)
{
	m_pDoc = doc;

	m_origView->m_pDoc = doc;
	m_paintView->m_pDoc = doc;
}

//------------------------------------------------
// Return the brush size
//------------------------------------------------
int ImpressionistUI::getSize()
{
	return m_nSize;
}

//-------------------------------------------------
// Set the brush size
//-------------------------------------------------
void ImpressionistUI::setSize( int size )
{
	m_nSize=size;

	if (size<=40) 
		m_BrushSizeSlider->value(m_nSize);
}

int ImpressionistUI::getLineWidth()
{
	return m_nLineWidth;
}

void ImpressionistUI::setLineWidth(int width)
{
	m_nLineWidth = width;
	if (width <= 40)
		m_LineWidthSlider->value(m_nLineWidth);
}

int ImpressionistUI::getLineAngle()
{
	return m_nLineAngle;
}

void ImpressionistUI::setLineAngle(int angle)
{
	m_nLineAngle = angle;
	if (angle <= 179)
		m_LineAngleSlider->value(m_nLineAngle);
}

double ImpressionistUI::getOpacity()
{
	return m_nOpacity;
}

// Main menu definition
Fl_Menu_Item ImpressionistUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU },
		{ "&Load Image...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load_image },
		{ "&Load Another Image...",	FL_ALT + 'n', (Fl_Callback *)ImpressionistUI::cb_load_another_image },
		{ "&Load Gradient/Disolve Image...",	FL_ALT + 'g', (Fl_Callback *)ImpressionistUI::cb_load_gradient_image },
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save_image },	
		{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
		{ 0 },
	{ "&Edit",		0, 0, 0, FL_SUBMENU },
		{ "&Undo", FL_CTRL + 'z', (Fl_Callback *)ImpressionistUI::cb_undo },
		{ "&Redo", FL_CTRL + 'y', (Fl_Callback *)ImpressionistUI::cb_redo },
		{ 0 },
	{ "&Display",	0, 0, 0, FL_SUBMENU },
		{ "&Show Original Image", FL_ALT + 'o', (Fl_Callback *)ImpressionistUI::cb_OriginalView },
		{ "&Show Edge Image", FL_ALT + 'e', (Fl_Callback *)ImpressionistUI::cb_EdgeView },
		{ "&Show Gradient/Disolve Image", FL_ALT + 'g', (Fl_Callback *)ImpressionistUI::cb_GradientView },
		{ "&Swap", FL_CTRL + 'w', (Fl_Callback *)ImpressionistUI::cb_swap },
		{ "&Clear Canvas", FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_clear_canvas, 0, FL_MENU_DIVIDER },
		{ 0 }, 
	{ "Draw",	0, 0, 0, FL_SUBMENU },
		{ "&Brushes...",	FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_brushes },
		{ "&RGB Config",	FL_ALT + 'p', (Fl_Callback *)ImpressionistUI::cb_RGBScale },
		{ "&Disolve",	FL_ALT + 'd', (Fl_Callback *)ImpressionistUI::cb_Disolve },
		{ 0 },
	{ "&Help",		0, 0, 0, FL_SUBMENU },
		{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
		{ 0 },
	{ 0 }
};

// Brush choice menu definition
Fl_Menu_Item ImpressionistUI::brushTypeMenu[NUM_BRUSH_TYPE+1] = {
  {"Points",			FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_POINTS},
  {"Lines",				FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_LINES},
  {"Circles",			FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_CIRCLES},
  {"Scattered Points",	FL_ALT+'q', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_POINTS},
  {"Scattered Lines",	FL_ALT+'m', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_LINES},
  {"Scattered Circles",	FL_ALT+'d', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_CIRCLES},
  { "Eraser",	FL_ALT + 'd', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_ERASER },
  { "Blur",	FL_ALT + 'd', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_BLUR },
  { "Sharpen",	FL_ALT + 'd', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SHARPEN },
  { "Warp",	FL_ALT + 'd', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_WARP },
  {0}
};

// Line Stroke Direction choice definition
Fl_Menu_Item ImpressionistUI::strokeMenu[5] = {
	{"Right Click/Angle Sidebar",	FL_ALT + '0', (Fl_Callback *)ImpressionistUI::cb_strokeChoice, (void *) 0},
	{"Gradient",					FL_ALT + '1', (Fl_Callback *)ImpressionistUI::cb_strokeChoice, (void *) 1},
	{"Cursor Driection",			FL_ALT + '2', (Fl_Callback *)ImpressionistUI::cb_strokeChoice, (void *) 2},
	{"Another Gradient",			FL_ALT + '3', (Fl_Callback *)ImpressionistUI::cb_strokeChoice, (void *) 3},
	{0}
};


//----------------------------------------------------
// Constructor.  Creates all of the widgets.
// Add new widgets here
//----------------------------------------------------
ImpressionistUI::ImpressionistUI() {
	// Create the main window
	m_mainWindow = new Fl_Window(600, 300, "Impressionist");
		m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
		// install menu bar
		m_menubar = new Fl_Menu_Bar(0, 0, 600, 25);
		m_menubar->menu(menuitems);

		// Create a group that will hold two sub windows inside the main
		// window
		Fl_Group* group = new Fl_Group(0, 25, 600, 275);

			// install paint view window
			m_paintView = new PaintView(300, 25, 300, 275, "This is the paint view");//0jon
			m_paintView->box(FL_DOWN_FRAME);

			// install original view window
			m_origView = new OriginalView(0, 25, 300, 275, "This is the orig view");//300jon
			m_origView->box(FL_DOWN_FRAME);
			m_origView->deactivate();

		group->end();
		Fl_Group::current()->resizable(group);
    m_mainWindow->end();

	// init values
	m_nSize = 10;
	m_nLineWidth = 1;
	m_nLineAngle = 90;
	m_nOpacity = 1.0;
	m_RScale = 1.0;
	m_GScale = 1.0;
	m_BScale = 1.0;
	m_nSpacing = 10;
	m_nDimvalue = 1.0;

	// brush dialog definition
	m_brushDialog = new Fl_Window(400, 325, "Brush Dialog");
		// Add a brush type choice to the dialog
		m_BrushTypeChoice = new Fl_Choice(50,10,150,25,"&Brush");
		m_BrushTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushTypeChoice->menu(brushTypeMenu);
		m_BrushTypeChoice->callback(cb_brushChoice);

		m_ClearCanvasButton = new Fl_Button(240,10,150,25,"&Clear Canvas");
		m_ClearCanvasButton->user_data((void*)(this));
		m_ClearCanvasButton->callback(cb_clear_canvas_button);

		// Add a stroke direction choice menu
		m_StrokeChoice = new Fl_Choice(145, 45, 210, 25, "&Line Stroke Direction");
		m_StrokeChoice->user_data((void*)(this));
		m_StrokeChoice->menu(strokeMenu);
		m_StrokeChoice->callback(cb_strokeChoice);
		


		// Add brush size slider to the dialog 
		m_BrushSizeSlider = new Fl_Value_Slider(10, 80, 300, 20, "Size");
		m_BrushSizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushSizeSlider->type(FL_HOR_NICE_SLIDER);
        m_BrushSizeSlider->labelfont(FL_COURIER);
        m_BrushSizeSlider->labelsize(12);
		m_BrushSizeSlider->minimum(1);
		m_BrushSizeSlider->maximum(40);
		m_BrushSizeSlider->step(1);
		m_BrushSizeSlider->value(m_nSize);
		m_BrushSizeSlider->align(FL_ALIGN_RIGHT);
		m_BrushSizeSlider->callback(cb_sizeSlides);

		// Add line width slider to the dialog
		m_LineWidthSlider = new Fl_Value_Slider(10, 110, 300, 20, "Line Width");
		m_LineWidthSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_LineWidthSlider->type(FL_HOR_NICE_SLIDER);
		m_LineWidthSlider->labelfont(FL_COURIER);
		m_LineWidthSlider->labelsize(12);
		m_LineWidthSlider->minimum(1);
		m_LineWidthSlider->maximum(40);
		m_LineWidthSlider->step(1);
		m_LineWidthSlider->value(m_nLineWidth);
		m_LineWidthSlider->align(FL_ALIGN_RIGHT);
		m_LineWidthSlider->callback(cb_widthSlides);

		// Add line angle slider to the dialog
		m_LineAngleSlider = new Fl_Value_Slider(10, 140, 300, 20, "Line Angle");
		m_LineAngleSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_LineAngleSlider->type(FL_HOR_NICE_SLIDER);
		m_LineAngleSlider->labelfont(FL_COURIER);
		m_LineAngleSlider->labelsize(12);
		m_LineAngleSlider->minimum(0);
		m_LineAngleSlider->maximum(179);
		m_LineAngleSlider->step(1);
		m_LineAngleSlider->value(m_nLineAngle);
		m_LineAngleSlider->align(FL_ALIGN_RIGHT);
		m_LineAngleSlider->callback(cb_angleSlides);

		// Add alpha slider to the dialog
		m_AlphaSlider = new Fl_Value_Slider(10, 170, 300, 20, "Alpha");
		m_AlphaSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_AlphaSlider->type(FL_HOR_NICE_SLIDER);
		m_AlphaSlider->labelfont(FL_COURIER);
		m_AlphaSlider->labelsize(12);
		m_AlphaSlider->minimum(0);
		m_AlphaSlider->maximum(1);
		m_AlphaSlider->step(0.01);
		m_AlphaSlider->value(m_nOpacity);
		m_AlphaSlider->align(FL_ALIGN_RIGHT);
		m_AlphaSlider->callback(cb_alphaSlides);

		// Spacing slider for auto paint
		m_SpacingSlider = new Fl_Value_Slider(10, 200, 200, 20, "Spacing");
		m_SpacingSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_SpacingSlider->type(FL_HOR_NICE_SLIDER);
		m_SpacingSlider->labelfont(FL_COURIER);
		m_SpacingSlider->labelsize(12);
		m_SpacingSlider->minimum(1);
		m_SpacingSlider->maximum(20);
		m_SpacingSlider->step(1);
		m_SpacingSlider->value(m_nSpacing);
		m_SpacingSlider->align(FL_ALIGN_RIGHT);
		m_SpacingSlider->callback(cb_SpacingSlides);

		// Auto Paint buttion
		m_AutoPaintButton = new Fl_Button(270, 196, 120, 25, "&Auto Paint");
		m_AutoPaintButton->user_data((void*)(this));
		m_AutoPaintButton->callback(cb_AutoPaintButton);

		// Dim slider
		m_DimSlider = new Fl_Value_Slider(10, 230, 200, 20, "Dimmed Background");
		m_DimSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_DimSlider->type(FL_HOR_NICE_SLIDER);
		m_DimSlider->labelfont(FL_COURIER);
		m_DimSlider->labelsize(12);
		m_DimSlider->minimum(0);
		m_DimSlider->maximum(1);
		m_DimSlider->step(0.01);
		m_DimSlider->value(m_nDimvalue);
		m_DimSlider->align(FL_ALIGN_RIGHT);
		m_DimSlider->callback(cb_DimSlides);

		// Dim Apply buttion
		m_DimButton = new Fl_Button(270, 226, 120, 25, "&Apply Dim");
		m_DimButton->user_data((void*)(this));
		m_DimButton->callback(cb_DimButton);

    m_brushDialog->end();	

	// RGB config dialog definition
	m_RGBConfigDialog = new Fl_Window(400, 150, "RGB Config");


		// Add R scale slider to the dialog 
		m_RScaleSlider = new Fl_Value_Slider(10, 10, 300, 20, "R Scale");
		m_RScaleSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_RScaleSlider->type(FL_HOR_NICE_SLIDER);
		m_RScaleSlider->labelfont(FL_COURIER);
		m_RScaleSlider->labelsize(12);
		m_RScaleSlider->minimum(0);
		m_RScaleSlider->maximum(1);
		m_RScaleSlider->step(0.01);
		m_RScaleSlider->value(m_RScale);
		m_RScaleSlider->align(FL_ALIGN_RIGHT);
		m_RScaleSlider->callback(cb_RScaleSlides);

		// Add G scale slider to the dialog 
		m_GScaleSlider = new Fl_Value_Slider(10, 40, 300, 20, "G Scale");
		m_GScaleSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_GScaleSlider->type(FL_HOR_NICE_SLIDER);
		m_GScaleSlider->labelfont(FL_COURIER);
		m_GScaleSlider->labelsize(12);
		m_GScaleSlider->minimum(0);
		m_GScaleSlider->maximum(1);
		m_GScaleSlider->step(0.01);
		m_GScaleSlider->value(m_GScale);
		m_GScaleSlider->align(FL_ALIGN_RIGHT);
		m_GScaleSlider->callback(cb_GScaleSlides);

		// Add B scale slider to the dialog 
		m_BScaleSlider = new Fl_Value_Slider(10, 70, 300, 20, "B Scale");
		m_BScaleSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BScaleSlider->type(FL_HOR_NICE_SLIDER);
		m_BScaleSlider->labelfont(FL_COURIER);
		m_BScaleSlider->labelsize(12);
		m_BScaleSlider->minimum(0);
		m_BScaleSlider->maximum(1);
		m_BScaleSlider->step(0.01);
		m_BScaleSlider->value(m_BScale);
		m_BScaleSlider->align(FL_ALIGN_RIGHT);
		m_BScaleSlider->callback(cb_BScaleSlides);

	m_RGBConfigDialog->end();

}
