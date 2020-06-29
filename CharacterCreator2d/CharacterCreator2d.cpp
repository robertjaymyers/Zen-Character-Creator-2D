/*
This file is part of Zen Character Creator 2D.
	Zen Character Creator 2D is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	Zen Character Creator 2D is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with Zen Character Creator 2D.  If not, see <https://www.gnu.org/licenses/>.
*/

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

	ui.centralWidget->setLayout(baseLayout.get());
	baseLayout.get()->setMargin(0);

	display = std::make_unique<GraphicsDisplay>(this, this->width(), this->height());

	baseLayout.get()->addWidget(display.get(), 0, 0);

	connect(display.get()->fullscreenShortcutExit.get(), &QShortcut::activated, this, [=]() {
		if (this->isFullScreen())
		{
			exitFullscreen();
		}
	});
	connect(display.get()->fullscreenBtn.get(), &QPushButton::clicked, this, [=]() {
		if (this->isFullScreen())
		{
			exitFullscreen();
		}
		else
		{
			enterFullscreen();
		}
	});

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

// private:

void CharacterCreator2d::enterFullscreen()
{
	if (windowState() == Qt::WindowMaximized)
		preFullscreenWindowState = PreFullscreenWindowState::MAXIMIZED;
	else
		preFullscreenWindowState = PreFullscreenWindowState::NORMAL;
	this->showFullScreen();
}

void CharacterCreator2d::exitFullscreen()
{
	if (preFullscreenWindowState == PreFullscreenWindowState::MAXIMIZED)
		this->showMaximized();
	else if (preFullscreenWindowState == PreFullscreenWindowState::NORMAL)
		this->showNormal();
}