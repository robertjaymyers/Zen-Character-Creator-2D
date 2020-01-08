/*
This file is part of CharacterCreator2d.
	CharacterCreator2d is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	CharacterCreator2d is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with CharacterCreator2d.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once
#include <QLineEdit>

class TextInputSingleLine : public QLineEdit
{
	Q_OBJECT

public:
	TextInputSingleLine(QWidget *parent = nullptr, QString placeholderText = "");

private:
	const QString styleSheet = 
		"QLineEdit"
		"{"
			"color: #000000;"
			"background-color: #FFFFFF;"
			"selection-background-color: #A96539;"
			"border-width: 1px;"
			"border-style: solid;"
			"border-color: #000000;"
			"border-radius: 4px;"
			"font-size: 12px;"
			"padding: 5px;"
		"}"
		"QLineEdit:hover{color: #A96539;}"
		;
};