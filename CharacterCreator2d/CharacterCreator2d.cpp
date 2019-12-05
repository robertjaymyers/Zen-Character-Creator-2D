#include "CharacterCreator2d.h"

// General concept: Display 2d "parts" in QGraphicsView from a list of available parts.
// Parts are separated into things like: head, chest, bottom
// User can:
// - Rotate through parts in different sections
// - Save or Load a particular combination of parts
// - Export their combination as a full PNG image if desired.

// Parts are all loaded into the QGraphicsView with the same image size.
// This means that each part should be the same image size, but only take up part of the overall picture.
// (e.g. Head image is full size, but the Head itself only takes up a small part of the image file
//  and the rest of the Head image file is transparent)
// This saves some headache in fiddling around with precise placement of images on the Y axis.
// It also means that you can concept out parts as a whole picture in an image editor, with layers,
// and then simply hide the layers that don't belong to the part when you're ready to save it for usage in the program.

CharacterCreator2d::CharacterCreator2d(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	display.get()->setParent(this);
	display.get()->show();
}

// public:

void CharacterCreator2d::closeEvent(QCloseEvent *event)
{
	if (display.get()->fileSaveModifCheck())
	{
		event->accept();
	}
	else
	{
		event->ignore();
	}
}