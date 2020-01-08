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

#include "PartSwapButton.h"

PartSwapButton::PartSwapButton(QWidget *parent, BtnIcon btnIcon, BtnGeometry btnGeometry)
	: QPushButton(parent)
{
	if (btnIcon == BtnIcon::SWAP_LEFT)
		this->setStyleSheet(styleSheetTemplate.arg(btnSwapLeftIcons[0]).arg(btnSwapLeftIcons[1]).arg(btnSwapLeftIcons[2]));
	else if (btnIcon == BtnIcon::SWAP_RIGHT)
		this->setStyleSheet(styleSheetTemplate.arg(btnSwapRightIcons[0]).arg(btnSwapRightIcons[1]).arg(btnSwapRightIcons[2]));
	else if (btnIcon == BtnIcon::COLOR_PICKER_SKIN)
		this->setStyleSheet(styleSheetTemplate.arg(btnPickerSkinIcons[0]).arg(btnPickerSkinIcons[1]).arg(btnPickerSkinIcons[2]));
	else if (btnIcon == BtnIcon::COLOR_PICKER_EYE)
		this->setStyleSheet(styleSheetTemplate.arg(btnPickerEyeIcons[0]).arg(btnPickerEyeIcons[1]).arg(btnPickerEyeIcons[2]));
	else if (btnIcon == BtnIcon::COLOR_PICKER_LIP)
		this->setStyleSheet(styleSheetTemplate.arg(btnPickerLipIcons[0]).arg(btnPickerLipIcons[1]).arg(btnPickerLipIcons[2]));
	else if (btnIcon == BtnIcon::COLOR_PICKER_BLUSH)
		this->setStyleSheet(styleSheetTemplate.arg(btnPickerBlushIcons[0]).arg(btnPickerBlushIcons[1]).arg(btnPickerBlushIcons[2]));
	else if (btnIcon == BtnIcon::COLOR_PICKER_CHEST)
		this->setStyleSheet(styleSheetTemplate.arg(btnPickerChestIcons[0]).arg(btnPickerChestIcons[1]).arg(btnPickerChestIcons[2]));
	else if (btnIcon == BtnIcon::COLOR_PICKER_BOTTOM)
		this->setStyleSheet(styleSheetTemplate.arg(btnPickerBottomIcons[0]).arg(btnPickerBottomIcons[1]).arg(btnPickerBottomIcons[2]));
	else if (btnIcon == BtnIcon::COLOR_PICKER_FEET)
		this->setStyleSheet(styleSheetTemplate.arg(btnPickerFeetIcons[0]).arg(btnPickerFeetIcons[1]).arg(btnPickerFeetIcons[2]));
	else if (btnIcon == BtnIcon::COLOR_PICKER_HAIR)
		this->setStyleSheet(styleSheetTemplate.arg(btnPickerHairIcons[0]).arg(btnPickerHairIcons[1]).arg(btnPickerHairIcons[2]));
	else if (btnIcon == BtnIcon::NONE)
		this->setStyleSheet("QPushButton{background-color: rgba(255, 255, 255, 0); border: none; image: url(:/CharacterCreator2d/Resources/picker-spacer-transparent-background.png);}");
	
	if (btnGeometry == BtnGeometry::SWAP)
		this->setFixedSize(QSize(btnSwapWidth, btnSwapHeight));
	else if (btnGeometry == BtnGeometry::PICKER || btnGeometry == BtnGeometry::PICKER_SPACER)
		this->setFixedSize(QSize(btnPickerWidth, btnPickerHeight));

	btnTypeForRef = btnGeometry;

	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	contextMenuPicker.get()->addAction(actionCopyColor.get());
	contextMenuPicker.get()->addAction(actionPasteColor.get());
	contextMenuPicker.get()->addSeparator();
	contextMenuPicker.get()->addAction(actionApplyColorToAllInSet.get());
}

// protected:

void PartSwapButton::contextMenuEvent(QContextMenuEvent *event)
{
	if (btnTypeForRef == BtnGeometry::PICKER)
		contextMenuPicker.get()->exec(event->globalPos());
}