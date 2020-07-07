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

#pragma once

#include <QtWidgets/QMainWindow>
#include <QGridLayout>
#include <QTimer>
#include "ui_CharacterCreator2d.h"
#include "GraphicsDisplay.h"

class CharacterCreator2d : public QMainWindow
{
	Q_OBJECT

public:
	CharacterCreator2d(QWidget *parent = Q_NULLPTR);
	void closeEvent(QCloseEvent *event);

private:
	Ui::CharacterCreator2dClass ui;

	std::unique_ptr<QGridLayout> baseLayout = std::make_unique<QGridLayout>();
	std::unique_ptr<GraphicsDisplay> display;

	/*enum class PreFullscreenWindowState { NORMAL, MAXIMIZED };
	PreFullscreenWindowState preFullscreenWindowState = PreFullscreenWindowState::NORMAL;

	void enterFullscreen();
	void exitFullscreen();*/
};
