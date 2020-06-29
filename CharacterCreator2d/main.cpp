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
#include <QtWidgets/QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setWindowIcon(QIcon(":/ZenCharacterCreator2D/Resources/ProgramIcon.ico"));
	
	QSplashScreen loadScreen(QPixmap(":/ZenCharacterCreator2D/Resources/splashLoadScreenStatic.png"));
	loadScreen.show();
	app.processEvents();

	CharacterCreator2d window;
	window.show();
	loadScreen.finish(&window);
	return app.exec();
}
