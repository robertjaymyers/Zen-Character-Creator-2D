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

#include "GraphicsDisplay.h"

GraphicsDisplay::GraphicsDisplay(QWidget* parent)
	: QGraphicsView(parent)
{
	contextMenu.get()->addAction(actionFileNew.get());
	contextMenu.get()->addAction(actionFileOpen.get());
	contextMenu.get()->addAction(actionFileSave.get());
	contextMenu.get()->addAction(actionFileExport.get());
	contextMenu.get()->addSeparator();
	contextMenu.get()->addAction(actionSetBackgroundColor.get());
	contextMenu.get()->addSeparator();
	contextMenu.get()->addMenu(speciesMenu.get());
	contextMenu.get()->addMenu(genderMenu.get());
	contextMenu.get()->addMenu(poseMenu.get());

	connect(actionFileNew.get(), &QAction::triggered, this, [=]() {
		if (fileSaveModifCheck())
			fileNew();
	});
	connect(actionFileOpen.get(), &QAction::triggered, this, [=]() {
		if (fileSaveModifCheck())
			fileOpen();
	});
	connect(actionFileSave.get(), &QAction::triggered, this, [=]() {
		if (fileSave())
			characterModified = false;
	});
	connect(actionFileExport.get(), &QAction::triggered, this, &GraphicsDisplay::fileExportCharacter);
	connect(actionSetBackgroundColor.get(), &QAction::triggered, this, [=]() {
		QColor colorNew = QColorDialog::getColor(backgroundColor, this->parentWidget(), "Choose Color");
		if (colorNew.isValid())
			setBackgroundColor(colorNew);
	});


	// We set up some elements to be initialized with a const value, where feasible.
	// This is both to protect against accidental modification of variables that shouldn't change,
	// and to make it easier to tweak values at a glance for customized usage of the program.

	// For example, if you had a species HUMAN and DOG, you'd have different parts to use.
	// The DOG would have a TAIL and the HUMAN wouldn't. 
	// So the DOG species can be initialized with TAIL as one of its "parts" without affecting HUMAN,
	// and without requiring special accommodations in the number or type of parts somewhere in the code.
	
	// This means some redundancy will need to be copy/pasted at times. For example, you probably want
	// the text input boxes for First Name and Last Name to have the same stylesheet.
	// However, if you want them to have different styles, you can do so without needing to modify
	// the design of the code to accommodate different styles.

	textInputSingleLineList.emplace_back
	(
		textInputSingleLine
		{
			TextInputSingleLineType::FIRST_NAME,
			"characterFirstName",
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
			"QLineEdit:hover{color: #A96539;}",
			"First Name",
			{0, 0}, // Row/Col placement in grid layout
			Qt::AlignLeft // Alignment in grid layout
		}
	);
	textInputSingleLineList.emplace_back
	(
		textInputSingleLine
		{
			TextInputSingleLineType::LAST_NAME,
			"characterLastName",
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
			"QLineEdit:hover{color: #A96539;}",
			"Last Name",
			{0, 1}, // Row/Col placement in grid layout
			Qt::AlignLeft // Alignment in grid layout
		}
	);

	//[&](std::vector<characterPart> &characterPartList) {
	//	characterPartList.emplace_back
	//	(
	//		characterPart
	//		{
	//			PartType::BODY,
	//			0, // Display order in scene (higher numbers overlap lower numbers)
	//			"Body",
	//			"#764c39",
	//			ColorSetType::FILL_WITH_OUTLINE,
	//			false, // SWAP BTN: Flag for whether part is expected to display btn
	//			true, // PICKER BTN: Flag for whether part is expected to display btn
	//			"QPushButton{border: none; image: url(%1);}"
	//			"QPushButton:hover:!pressed{border: none; image: url(%2);}"
	//			"QPushButton:hover:pressed{border: none; image: url(%3);}",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-skin-color.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-skin-color-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-skin-color-hover-pressed.png",
	//			{0, 0}, // PICKER BTN: Row/Col placement in grid layout
	//			Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
	//			{-1, -1}, // SWAP LEFT BTN: Row/Col placement in grid layout
	//			nullptr, // PICKER BTN: Alignment in grid layout
	//			{-1, -1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
	//			nullptr, // PICKER BTN: Alignment in grid layout
	//			75, // SWAP BTN: Width
	//			75, // SWAP BTN: Height
	//			75, // PICKER BTN: Width
	//			75, // PICKER BTN: Height
	//		}
	//	);
	//	characterPartList.emplace_back
	//	(
	//		characterPart
	//		{
	//			PartType::EYES,
	//			1, // Display order in scene (higher numbers overlap lower numbers)
	//			"Eyes",
	//			"#aaaa7f",
	//			ColorSetType::FILL_WITH_OUTLINE,
	//			false, // SWAP BTN: Flag for whether part is expected to display btn
	//			true, // PICKER BTN: Flag for whether part is expected to display btn
	//			"QPushButton{border: none; image: url(%1);}"
	//			"QPushButton:hover:!pressed{border: none; image: url(%2);}"
	//			"QPushButton:hover:pressed{border: none; image: url(%3);}",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-eye-color.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-eye-color-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-eye-color-hover-pressed.png",
	//			{1, 0}, // PICKER BTN: Row/Col placement in grid layout
	//			Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
	//			{-1, -1}, // SWAP LEFT BTN: Row/Col placement in grid layout
	//			nullptr, // PICKER BTN: Alignment in grid layout
	//			{-1, -1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
	//			nullptr, // PICKER BTN: Alignment in grid layout
	//			75, // SWAP BTN: Width
	//			75, // SWAP BTN: Height
	//			75, // PICKER BTN: Width
	//			75, // PICKER BTN: Height
	//		}
	//	);
	//	characterPartList.emplace_back
	//	(
	//		characterPart
	//		{
	//			PartType::LIPS,
	//			2, // Display order in scene (higher numbers overlap lower numbers)
	//			"Lips",
	//			"#555500",
	//			ColorSetType::FILL_WITH_OUTLINE,
	//			false, // SWAP BTN: Flag for whether part is expected to display btn
	//			true, // PICKER BTN: Flag for whether part is expected to display btn
	//			"QPushButton{border: none; image: url(%1);}"
	//			"QPushButton:hover:!pressed{border: none; image: url(%2);}"
	//			"QPushButton:hover:pressed{border: none; image: url(%3);}",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-lip-color.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-lip-color-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-lip-color-hover-pressed.png",
	//			{2, 0}, // PICKER BTN: Row/Col placement in grid layout
	//			Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
	//			{-1, -1}, // SWAP LEFT BTN: Row/Col placement in grid layout
	//			nullptr, // PICKER BTN: Alignment in grid layout
	//			{-1, -1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
	//			nullptr, // PICKER BTN: Alignment in grid layout
	//			75, // SWAP BTN: Width
	//			75, // SWAP BTN: Height
	//			75, // PICKER BTN: Width
	//			75, // PICKER BTN: Height
	//		}
	//	);
	//	characterPartList.emplace_back
	//	(
	//		characterPart
	//		{
	//			PartType::BLUSH,
	//			3, // Display order in scene (higher numbers overlap lower numbers)
	//			"Blush",
	//			"#555500",
	//			ColorSetType::FILL_NO_OUTLINE,
	//			false, // SWAP BTN: Flag for whether part is expected to display btn
	//			true, // PICKER BTN: Flag for whether part is expected to display btn
	//			"QPushButton{border: none; image: url(%1);}"
	//			"QPushButton:hover:!pressed{border: none; image: url(%2);}"
	//			"QPushButton:hover:pressed{border: none; image: url(%3);}",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-blush-color.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-blush-color-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-blush-color-hover-pressed.png",
	//			{3, 0}, // PICKER BTN: Row/Col placement in grid layout
	//			Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
	//			{-1, -1}, // SWAP LEFT BTN: Row/Col placement in grid layout
	//			nullptr, // PICKER BTN: Alignment in grid layout
	//			{-1, -1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
	//			nullptr, // PICKER BTN: Alignment in grid layout
	//			75, // SWAP BTN: Width
	//			75, // SWAP BTN: Height
	//			75, // PICKER BTN: Width
	//			75, // PICKER BTN: Height
	//		}
	//	);
	//	characterPartList.emplace_back
	//	(
	//		characterPart
	//		{
	//			PartType::HEAD,
	//			4, // Display order in scene (higher numbers overlap lower numbers)
	//			"Head",
	//			"#FFFFFF",
	//			ColorSetType::NONE,
	//			true, // SWAP BTN: Flag for whether part is expected to display btn
	//			false, // PICKER BTN: Flag for whether part is expected to display btn
	//			"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
	//			"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
	//			"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
	//			"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
	//			QStringList(), // Head color changing is controlled by skin color, so we leave this empty
	//			{-1, -1}, // PICKER BTN: Row/Col placement in grid layout
	//			nullptr, // PICKER BTN: Alignment in grid layout
	//			{2, 0}, // SWAP LEFT BTN: Row/Col placement in grid layout
	//			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
	//			{2, 1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
	//			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
	//			75, // SWAP BTN: Width
	//			75, // SWAP BTN: Height
	//			75, // PICKER BTN: Width
	//			75, // PICKER BTN: Height
	//		}
	//	);
	//	characterPartList.emplace_back
	//	(
	//		characterPart
	//		{
	//			PartType::BOTTOM,
	//			5, // Display order in scene (higher numbers overlap lower numbers)
	//			"Bottom",
	//			"#B5B5B5",
	//			ColorSetType::FILL_WITH_OUTLINE,
	//			true, // SWAP BTN: Flag for whether part is expected to display btn
	//			true, // PICKER BTN: Flag for whether part is expected to display btn
	//			"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
	//			"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
	//			"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
	//			"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartBottom.png"
	//			<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartBottomHover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartBottomHover.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-bottom-color.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-bottom-color-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-bottom-color-hover-pressed.png",
	//			{2, 1}, // PICKER BTN: Row/Col placement in grid layout
	//			Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
	//			{4, 0}, // SWAP LEFT BTN: Row/Col placement in grid layout
	//			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
	//			{4, 1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
	//			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
	//			75, // SWAP BTN: Width
	//			75, // SWAP BTN: Height
	//			75, // PICKER BTN: Width
	//			75, // PICKER BTN: Height
	//		}
	//	);
	//	characterPartList.emplace_back
	//	(
	//		characterPart
	//		{
	//			PartType::CHEST,
	//			6, // Display order in scene (higher numbers overlap lower numbers)
	//			"Chest",
	//			"#B5B5B5",
	//			ColorSetType::FILL_WITH_OUTLINE,
	//			true, // SWAP BTN: Flag for whether part is expected to display btn
	//			true, // PICKER BTN: Flag for whether part is expected to display btn
	//			"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
	//			"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
	//			"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
	//			"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartChest.png"
	//			<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartChestHover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartChestHover.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-chest-color.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-chest-color-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-chest-color-hover-pressed.png",
	//			{1, 1}, // PICKER BTN: Row/Col placement in grid layout
	//			Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
	//			{3, 0}, // SWAP LEFT BTN: Row/Col placement in grid layout
	//			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
	//			{3, 1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
	//			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
	//			75, // SWAP BTN: Width
	//			75, // SWAP BTN: Height
	//			75, // PICKER BTN: Width
	//			75, // PICKER BTN: Height
	//		}
	//	);
	//	characterPartList.emplace_back
	//	(
	//		characterPart
	//		{
	//			PartType::FEET,
	//			7, // Display order in scene (higher numbers overlap lower numbers)
	//			"Feet",
	//			"#000000",
	//			ColorSetType::FILL_WITH_OUTLINE,
	//			true, // SWAP BTN: Flag for whether part is expected to display btn
	//			true, // PICKER BTN: Flag for whether part is expected to display btn
	//			"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
	//			"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
	//			"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
	//			"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartFeet.png"
	//			<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartFeetHover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartFeetHover.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-feet-color.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-feet-color-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-feet-color-hover-pressed.png",
	//			{3, 1}, // PICKER BTN: Row/Col placement in grid layout
	//			Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
	//			{5, 0}, // SWAP LEFT BTN: Row/Col placement in grid layout
	//			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
	//			{5, 1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
	//			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
	//			75, // SWAP BTN: Width
	//			75, // SWAP BTN: Height
	//			75, // PICKER BTN: Width
	//			75, // PICKER BTN: Height
	//		}
	//	);
	//	characterPartList.emplace_back
	//	(
	//		characterPart
	//		{
	//			PartType::MASK,
	//			8, // Display order in scene (higher numbers overlap lower numbers)
	//			"Mask",
	//			"#000000",
	//			ColorSetType::FILL_WITH_OUTLINE,
	//			true, // SWAP BTN: Flag for whether part is expected to display btn
	//			true, // PICKER BTN: Flag for whether part is expected to display btn
	//			"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
	//			"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
	//			"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
	//			"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-feet-color.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-feet-color-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-feet-color-hover-pressed.png",
	//			{4, 0}, // PICKER BTN: Row/Col placement in grid layout
	//			nullptr, // PICKER BTN: Alignment in grid layout
	//			{0, 0}, // SWAP LEFT BTN: Row/Col placement in grid layout
	//			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
	//			{0, 1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
	//			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
	//			75, // SWAP BTN: Width
	//			75, // SWAP BTN: Height
	//			75, // PICKER BTN: Width
	//			75, // PICKER BTN: Height
	//		}
	//	);
	//	characterPartList.emplace_back
	//	(
	//		characterPart
	//		{
	//			PartType::HAIR,
	//			9, // Display order in scene (higher numbers overlap lower numbers)
	//			"Hair",
	//			"#000000",
	//			ColorSetType::FILL_WITH_OUTLINE,
	//			true, // SWAP BTN: Flag for whether part is expected to display btn
	//			true, // PICKER BTN: Flag for whether part is expected to display btn
	//			"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
	//			"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
	//			"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
	//			"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHair.png"
	//			<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHairHover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHairHover.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-hair-color.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-hair-color-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-hair-color-hover-pressed.png",
	//			{0, 1}, // PICKER BTN: Row/Col placement in grid layout
	//			Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
	//			{1, 0}, // SWAP LEFT BTN: Row/Col placement in grid layout
	//			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
	//			{1, 1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
	//			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
	//			75, // SWAP BTN: Width
	//			75, // SWAP BTN: Height
	//			75, // PICKER BTN: Width
	//			75, // PICKER BTN: Height
	//		}
	//	);
	//}(characterPartListHuman);

	//[&](std::vector<characterPart> &characterPartList) {
	//	characterPartList.emplace_back
	//	(
	//		characterPart
	//		{
	//			PartType::BODY,
	//			0, // Display order in scene (higher numbers overlap lower numbers)
	//			"Body",
	//			"#764c39",
	//			ColorSetType::FILL_WITH_OUTLINE,
	//			false, // SWAP BTN: Flag for whether part is expected to display btn
	//			true, // PICKER BTN: Flag for whether part is expected to display btn
	//			"QPushButton{border: none; image: url(%1);}"
	//			"QPushButton:hover:!pressed{border: none; image: url(%2);}"
	//			"QPushButton:hover:pressed{border: none; image: url(%3);}",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-skin-color.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-skin-color-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-skin-color-hover-pressed.png",
	//			{0, 0}, // PICKER BTN: Row/Col placement in grid layout
	//			Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
	//			{-1, -1}, // SWAP LEFT BTN: Row/Col placement in grid layout
	//			nullptr, // PICKER BTN: Alignment in grid layout
	//			{-1, -1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
	//			nullptr, // PICKER BTN: Alignment in grid layout
	//			75, // SWAP BTN: Width
	//			75, // SWAP BTN: Height
	//			75, // PICKER BTN: Width
	//			75, // PICKER BTN: Height
	//		}
	//	);
	//	characterPartList.emplace_back
	//	(
	//		characterPart
	//		{
	//			PartType::EYES,
	//			1, // Display order in scene (higher numbers overlap lower numbers)
	//			"Eyes",
	//			"#aaaa7f",
	//			ColorSetType::FILL_WITH_OUTLINE,
	//			false, // SWAP BTN: Flag for whether part is expected to display btn
	//			true, // PICKER BTN: Flag for whether part is expected to display btn
	//			"QPushButton{border: none; image: url(%1);}"
	//			"QPushButton:hover:!pressed{border: none; image: url(%2);}"
	//			"QPushButton:hover:pressed{border: none; image: url(%3);}",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-eye-color.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-eye-color-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-eye-color-hover-pressed.png",
	//			{1, 0}, // PICKER BTN: Row/Col placement in grid layout
	//			Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
	//			{-1, -1}, // SWAP LEFT BTN: Row/Col placement in grid layout
	//			nullptr, // PICKER BTN: Alignment in grid layout
	//			{-1, -1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
	//			nullptr, // PICKER BTN: Alignment in grid layout
	//			75, // SWAP BTN: Width
	//			75, // SWAP BTN: Height
	//			75, // PICKER BTN: Width
	//			75, // PICKER BTN: Height
	//		}
	//	);
	//	characterPartList.emplace_back
	//	(
	//		characterPart
	//		{
	//			PartType::LIPS,
	//			2, // Display order in scene (higher numbers overlap lower numbers)
	//			"Lips",
	//			"#555500",
	//			ColorSetType::FILL_WITH_OUTLINE,
	//			false, // SWAP BTN: Flag for whether part is expected to display btn
	//			true, // PICKER BTN: Flag for whether part is expected to display btn
	//			"QPushButton{border: none; image: url(%1);}"
	//			"QPushButton:hover:!pressed{border: none; image: url(%2);}"
	//			"QPushButton:hover:pressed{border: none; image: url(%3);}",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-lip-color.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-lip-color-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-lip-color-hover-pressed.png",
	//			{2, 0}, // PICKER BTN: Row/Col placement in grid layout
	//			Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
	//			{-1, -1}, // SWAP LEFT BTN: Row/Col placement in grid layout
	//			nullptr, // PICKER BTN: Alignment in grid layout
	//			{-1, -1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
	//			nullptr, // PICKER BTN: Alignment in grid layout
	//			75, // SWAP BTN: Width
	//			75, // SWAP BTN: Height
	//			75, // PICKER BTN: Width
	//			75, // PICKER BTN: Height
	//		}
	//	);
	//	characterPartList.emplace_back
	//	(
	//		characterPart
	//		{
	//			PartType::BLUSH,
	//			3, // Display order in scene (higher numbers overlap lower numbers)
	//			"Blush",
	//			"#555500",
	//			ColorSetType::FILL_NO_OUTLINE,
	//			false, // SWAP BTN: Flag for whether part is expected to display btn
	//			true, // PICKER BTN: Flag for whether part is expected to display btn
	//			"QPushButton{border: none; image: url(%1);}"
	//			"QPushButton:hover:!pressed{border: none; image: url(%2);}"
	//			"QPushButton:hover:pressed{border: none; image: url(%3);}",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-blush-color.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-blush-color-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-blush-color-hover-pressed.png",
	//			{3, 0}, // PICKER BTN: Row/Col placement in grid layout
	//			Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
	//			{-1, -1}, // SWAP LEFT BTN: Row/Col placement in grid layout
	//			nullptr, // PICKER BTN: Alignment in grid layout
	//			{-1, -1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
	//			nullptr, // PICKER BTN: Alignment in grid layout
	//			75, // SWAP BTN: Width
	//			75, // SWAP BTN: Height
	//			75, // PICKER BTN: Width
	//			75, // PICKER BTN: Height
	//		}
	//	);
	//	characterPartList.emplace_back
	//	(
	//		characterPart
	//		{
	//			PartType::HEAD,
	//			4, // Display order in scene (higher numbers overlap lower numbers)
	//			"Head",
	//			"#FFFFFF",
	//			ColorSetType::NONE,
	//			true, // SWAP BTN: Flag for whether part is expected to display btn
	//			false, // PICKER BTN: Flag for whether part is expected to display btn
	//			"QPushButton{border: none; image: url(%1);}"
	//			"QPushButton:hover:!pressed{border: none; image: url(%2);}"
	//			"QPushButton:hover:pressed{border: none; image: url(%3);}",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
	//			QStringList(), // Head color changing is controlled by skin color, so we leave this empty
	//			{-1, -1}, // PICKER BTN: Row/Col placement in grid layout
	//			nullptr, // PICKER BTN: Alignment in grid layout
	//			{1, 0}, // SWAP LEFT BTN: Row/Col placement in grid layout
	//			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
	//			{1, 1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
	//			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
	//			75, // SWAP BTN: Width
	//			75, // SWAP BTN: Height
	//			75, // PICKER BTN: Width
	//			75, // PICKER BTN: Height
	//		}
	//	);
	//	characterPartList.emplace_back
	//	(
	//		characterPart
	//		{
	//			PartType::BOTTOM,
	//			5, // Display order in scene (higher numbers overlap lower numbers)
	//			"Bottom",
	//			"#B5B5B5",
	//			ColorSetType::FILL_WITH_OUTLINE,
	//			true, // SWAP BTN: Flag for whether part is expected to display btn
	//			true, // PICKER BTN: Flag for whether part is expected to display btn
	//			"QPushButton{border: none; image: url(%1);}"
	//			"QPushButton:hover:!pressed{border: none; image: url(%2);}"
	//			"QPushButton:hover:pressed{border: none; image: url(%3);}",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-bottom-color.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-bottom-color-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-bottom-color-hover-pressed.png",
	//			{2, 1}, // PICKER BTN: Row/Col placement in grid layout
	//			Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
	//			{3, 0}, // SWAP LEFT BTN: Row/Col placement in grid layout
	//			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
	//			{3, 1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
	//			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
	//			75, // SWAP BTN: Width
	//			75, // SWAP BTN: Height
	//			75, // PICKER BTN: Width
	//			75, // PICKER BTN: Height
	//		}
	//	);
	//	characterPartList.emplace_back
	//	(
	//		characterPart
	//		{
	//			PartType::CHEST,
	//			6, // Display order in scene (higher numbers overlap lower numbers)
	//			"Chest",
	//			"#B5B5B5",
	//			ColorSetType::FILL_WITH_OUTLINE,
	//			true, // SWAP BTN: Flag for whether part is expected to display btn
	//			true, // PICKER BTN: Flag for whether part is expected to display btn
	//			"QPushButton{border: none; image: url(%1);}"
	//			"QPushButton:hover:!pressed{border: none; image: url(%2);}"
	//			"QPushButton:hover:pressed{border: none; image: url(%3);}",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-chest-color.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-chest-color-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-chest-color-hover-pressed.png",
	//			{1, 1}, // PICKER BTN: Row/Col placement in grid layout
	//			Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
	//			{2, 0}, // SWAP LEFT BTN: Row/Col placement in grid layout
	//			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
	//			{2, 1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
	//			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
	//			75, // SWAP BTN: Width
	//			75, // SWAP BTN: Height
	//			75, // PICKER BTN: Width
	//			75, // PICKER BTN: Height
	//		}
	//	);
	//	characterPartList.emplace_back
	//	(
	//		characterPart
	//		{
	//			PartType::FEET,
	//			7, // Display order in scene (higher numbers overlap lower numbers)
	//			"Feet",
	//			"#000000",
	//			ColorSetType::FILL_WITH_OUTLINE,
	//			true, // SWAP BTN: Flag for whether part is expected to display btn
	//			true, // PICKER BTN: Flag for whether part is expected to display btn
	//			"QPushButton{border: none; image: url(%1);}"
	//			"QPushButton:hover:!pressed{border: none; image: url(%2);}"
	//			"QPushButton:hover:pressed{border: none; image: url(%3);}",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-feet-color.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-feet-color-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-feet-color-hover-pressed.png",
	//			{3, 1}, // PICKER BTN: Row/Col placement in grid layout
	//			Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
	//			{4, 0}, // SWAP LEFT BTN: Row/Col placement in grid layout
	//			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
	//			{4, 1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
	//			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
	//			75, // SWAP BTN: Width
	//			75, // SWAP BTN: Height
	//			75, // PICKER BTN: Width
	//			75, // PICKER BTN: Height
	//		}
	//	);
	//	characterPartList.emplace_back
	//	(
	//		characterPart
	//		{
	//			PartType::HAIR,
	//			8, // Display order in scene (higher numbers overlap lower numbers)
	//			"Hair",
	//			"#000000",
	//			ColorSetType::FILL_WITH_OUTLINE,
	//			true, // SWAP BTN: Flag for whether part is expected to display btn
	//			true, // PICKER BTN: Flag for whether part is expected to display btn
	//			"QPushButton{border: none; image: url(%1);}"
	//			"QPushButton:hover:!pressed{border: none; image: url(%2);}"
	//			"QPushButton:hover:pressed{border: none; image: url(%3);}",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
	//			QStringList()
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-hair-color.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-hair-color-hover.png"
	//			<< ":/ZenCharacterCreator2D/Resources/button-pick-hair-color-hover-pressed.png",
	//			{0, 1}, // PICKER BTN: Row/Col placement in grid layout
	//			Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
	//			{0, 0}, // SWAP LEFT BTN: Row/Col placement in grid layout
	//			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
	//			{0, 1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
	//			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
	//			75, // SWAP BTN: Width
	//			75, // SWAP BTN: Height
	//			75, // PICKER BTN: Width
	//			75, // PICKER BTN: Height
	//		}
	//	);
	//}(characterPartListElf);

	//speciesMap.insert(std::pair<SpeciesType, speciesData>
	//	(
	//		SpeciesType::HUMAN,
	//		speciesData
	//		{
	//			"Human",
	//			std::move(characterPartListHuman)
	//		}
	//));

	//speciesMap.insert(std::pair<SpeciesType, speciesData>
	//	(
	//		SpeciesType::ELF,
	//		speciesData
	//		{
	//			"Elf",
	//			std::move(characterPartListElf)
	//		}
	//));

	//[&](std::vector<characterPart> &characterPartList) {
	//	for (auto& cPart : characterPartList)
	//	{
	//		cPart.genderMap.insert(std::pair<GenderType, characterPart::genderData>
	//			(
	//				GenderType::FEMALE,
	//				characterPart::genderData
	//				{
	//					"Female"
	//				}
	//		));
	//		cPart.genderMap.insert(std::pair<GenderType, characterPart::genderData>
	//			(
	//				GenderType::MALE,
	//				characterPart::genderData
	//				{
	//					"Male"
	//				}
	//		));
	//	}
	//}(speciesMap.at(SpeciesType::HUMAN).characterPartList);

	//[&](std::vector<characterPart> &characterPartList) {
	//	for (auto& cPart : characterPartList)
	//	{
	//		cPart.genderMap.insert(std::pair<GenderType, characterPart::genderData>
	//			(
	//				GenderType::FEMALE,
	//				characterPart::genderData
	//				{
	//					"Female"
	//				}
	//		));
	//		cPart.genderMap.insert(std::pair<GenderType, characterPart::genderData>
	//			(
	//				GenderType::MALE,
	//				characterPart::genderData
	//				{
	//					"Male"
	//				}
	//		));
	//	}
	//}(speciesMap.at(SpeciesType::ELF).characterPartList);

	this->setScene(scene.get());
	scene.get()->setParent(this->parent());

	this->setStyleSheet(styleSheetEditable.arg(backgroundColorDefault.name()));
	this->setLayout(layout.get());

	layout.get()->setMargin(50);

	// Nested order is: Species->Gender->Component->Pose->Assets

	// Create nested maps and data structures first.
	for (const auto& species : speciesTypeMap)
	{
		speciesMap.try_emplace(species.first, speciesData{ species.second.assetStr });
		for (const auto& gender : genderTypeMap)
		{
			speciesMap.at(species.first).genderMap.try_emplace(gender.first, genderData{ gender.second });
			for (const auto& pose : poseTypeMap)
			{
				speciesMap.at(species.first).genderMap.at(gender.first).poseMap.try_emplace(pose.first, poseData{ pose.second });

				for (const auto& component : species.second.componentMapRef)
				{
					speciesMap.at(species.first).genderMap.at(gender.first).poseMap.at(pose.first).componentMap.try_emplace
					(
						component.first, componentData{ component.second }
					);

					QStringList assetPathList = fileGetAssets(
						QCoreApplication::applicationDirPath() +
						"/Assets/Species/" +
						species.second.assetStr + "/" +
						gender.second + "/" +
						pose.second + "/" +
						component.second.assetStr
					);

					// Note: We store as filename only (e.g. NOT including full path), 
					// so that if exe moves, character saves can still be loaded correctly in relation to loaded assets.
					for (const auto& assetPath : assetPathList)
					{
						if (component.second.colorSetType == ColorSetType::FILL_WITH_OUTLINE)
						{
							QString outlinePath = assetPath;
							QString thumbnailPath = assetPath;
							speciesMap.at(species.first).genderMap.at(gender.first).poseMap.at(pose.first)
								.componentMap.at(component.first).assetsMap.try_emplace
							(
								QFileInfo(assetPath).baseName(),
								assetsData
								{
									assetPath,
									QFileInfo(assetPath).fileName(),
									component.second.defaultInitialColor,
									component.second.defaultInitialColor,
									outlinePath.replace("/" + QFileInfo(assetPath).fileName(), "/Outline/" + QFileInfo(assetPath).fileName()),
									thumbnailPath.replace("/" + QFileInfo(assetPath).fileName(), "/Thumbnail/" + QFileInfo(assetPath).fileName())
								}
							);
						}
						else if (component.second.colorSetType == ColorSetType::FILL_NO_OUTLINE ||
							component.second.colorSetType == ColorSetType::NONE)
						{
							QString thumbnailPath = assetPath;
							speciesMap.at(species.first).genderMap.at(gender.first).poseMap.at(pose.first)
								.componentMap.at(component.first).assetsMap.try_emplace
							(
								QFileInfo(assetPath).baseName(),
								assetsData
								{
									assetPath,
									QFileInfo(assetPath).fileName(),
									component.second.defaultInitialColor,
									component.second.defaultInitialColor,
									"",
									thumbnailPath.replace("/" + QFileInfo(assetPath).fileName(), "/Thumbnail/" + QFileInfo(assetPath).fileName())
								}
							);
						}
					}
				}
			}
		}
	}

	// Now we can start traversing through the nested maps and applying initial settings.
	for (auto& species : speciesMap)
	{
		species.second.actionSpecies.get()->setParent(this);
		species.second.actionSpecies.get()->setText(species.second.assetStr);
		species.second.actionSpecies.get()->setCheckable(true);
		actionSpeciesGroup.get()->addAction(species.second.actionSpecies.get());
		speciesMenu.get()->addAction(species.second.actionSpecies.get());

		connect(species.second.actionSpecies.get(), &QAction::triggered, this, [&]() {
			if (speciesCurrent != species.first)
			{
				if (fileSaveModifCheck())
				{
					removeCurrentSpeciesFromScene();
					for (auto& gender : speciesMap.at(speciesCurrent).genderMap)
						gender.second.actionGender.get()->setVisible(false);
					speciesCurrent = species.first;
					for (auto& gender : speciesMap.at(speciesCurrent).genderMap)
						gender.second.actionGender.get()->setVisible(true);
					genderCurrent = speciesMap.at(speciesCurrent).genderMap.begin()->first;
					speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).actionGender.get()->setChecked(true);
					applyCurrentSpeciesToScene();
					loadDefaultCharacterFromTemplate();
				}
			}
		});

		for (auto& gender : species.second.genderMap)
		{
			gender.second.actionGender.get()->setParent(this);
			gender.second.actionGender.get()->setText(gender.second.assetStr);
			gender.second.actionGender.get()->setCheckable(true);
			gender.second.actionGender.get()->setVisible(false);

			actionGenderGroup.get()->addAction(gender.second.actionGender.get());
			genderMenu.get()->addAction(gender.second.actionGender.get());

			connect(gender.second.actionGender.get(), &QAction::triggered, this, [&]() {
				if (genderCurrent != gender.first)
				{
					if (fileSaveModifCheck())
					{
						removeCurrentSpeciesFromScene();
						genderCurrent = gender.first;
						poseCurrent = speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.begin()->first;
						//for (auto& component : speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent).componentMap)
						//	updatePartInScene(component.second, component.second.assetsMap.begin()->second);
						applyCurrentSpeciesToScene();
						loadDefaultCharacterFromTemplate();
					}
				}
			});

			for (auto& pose : gender.second.poseMap)
			{
				pose.second.actionPose.get()->setParent(this);
				pose.second.actionPose.get()->setText(pose.second.assetStr);
				pose.second.actionPose.get()->setCheckable(true);
				pose.second.actionPose.get()->setVisible(false);

				actionPoseGroup.get()->addAction(pose.second.actionPose.get());
				poseMenu.get()->addAction(pose.second.actionPose.get());

				connect(pose.second.actionPose.get(), &QAction::triggered, this, [&]() {
					if (poseCurrent != pose.first)
					{
						if (fileSaveModifCheck())
						{
							removeCurrentSpeciesFromScene();
							poseCurrent = pose.first;
							//for (auto& component : speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent).componentMap)
							//	updatePartInScene(component.second, component.second.assetsMap.begin()->second);
							applyCurrentSpeciesToScene();
							loadDefaultCharacterFromTemplate();
						}
					}
				});

				for (auto& component : pose.second.componentMap)
				{
					if (component.second.settings.partHasBtnSwap)
					{
						component.second.btnSwapComponent.get()->setStyleSheet
						(
							component.second.settings.btnStyleSheetTemplate
							.arg(component.second.settings.btnSwapLeftIcons[0])
							.arg(component.second.settings.btnSwapLeftIcons[1])
							.arg(component.second.settings.btnSwapLeftIcons[2])
						);
						component.second.btnSwapComponent.get()->setParent(this);
						component.second.btnSwapComponent.get()->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
						component.second.btnSwapComponent.get()->setFixedSize
						(
							QSize
							(
								component.second.settings.btnSwapWidth,
								component.second.settings.btnSwapHeight
							)
						);
						component.second.btnSwapComponent.get()->setVisible(false);

						connect(component.second.btnSwapComponent.get(), &QPushButton::clicked, this, [&]() {
							species.second.genderMap.at(genderCurrent).poseMap.at(poseCurrent).
								componentMap.at(componentCurrent).btnSwapComponent.get()->setEnabled(true);

							for (auto& asset : species.second.genderMap.at(genderCurrent).poseMap.at(poseCurrent).
								componentMap.at(componentCurrent).assetsMap)
							{
								asset.second.btnSwapAsset.get()->setEnabled(true);
								asset.second.btnSwapAsset.get()->setVisible(false);
							}

							component.second.btnSwapComponent.get()->setEnabled(false);

							for (auto& asset : component.second.assetsMap)
							{
								asset.second.btnSwapAsset.get()->setEnabled(true);
								asset.second.btnSwapAsset.get()->setVisible(true);
							}

							partSwapScroll.get()->verticalScrollBar()->setValue(0);
							partSwapScroll.get()->horizontalScrollBar()->setValue(0);

							componentCurrent = component.first;

							species.second.genderMap.at(genderCurrent).poseMap.at(poseCurrent).
								componentMap.at(componentCurrent).assetsMap.at(component.second.displayedAssetKey)
								.btnSwapAsset.get()->setEnabled(false);
						});


						for (auto& asset : component.second.assetsMap)
						{
							asset.second.btnSwapAsset.get()->setStyleSheet
							(
								component.second.settings.btnStyleSheetTemplate
								.arg(asset.second.imgThumbnailPath)
								.arg(asset.second.imgThumbnailPath)
								.arg(asset.second.imgThumbnailPath)
							);
							asset.second.btnSwapAsset.get()->setParent(this);
							asset.second.btnSwapAsset.get()->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
							asset.second.btnSwapAsset.get()->setFixedSize
							(
								QSize
								(
									component.second.settings.btnSwapWidth,
									component.second.settings.btnSwapHeight
								)
							);
							asset.second.btnSwapAsset.get()->setVisible(false);

							connect(asset.second.btnSwapAsset.get(), &QPushButton::clicked, this, [&]() {
								component.second.assetsMap.at(component.second.displayedAssetKey).btnSwapAsset.get()->setEnabled(true);
								qDebug() << component.second.displayedAssetKey;
								component.second.displayedAssetKey = asset.first;
								qDebug() << component.second.displayedAssetKey;
								asset.second.btnSwapAsset.get()->setEnabled(false);
								updatePartInScene(component.second, asset.second);
								characterModified = true;
							});
						}
					}
					if (component.second.settings.partHasBtnPicker)
					{
						component.second.btnPicker.get()->setStyleSheet
						(
							component.second.settings.btnStyleSheetTemplate
							.arg(component.second.settings.btnPickerIcons[0])
							.arg(component.second.settings.btnPickerIcons[1])
							.arg(component.second.settings.btnPickerIcons[2])
						);
						component.second.btnPicker.get()->setParent(this);
						component.second.btnPicker.get()->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
						component.second.btnPicker.get()->setFixedSize
						(
							QSize
							(
								component.second.settings.btnPickerWidth,
								component.second.settings.btnPickerHeight
							)
						);
						component.second.btnPicker.get()->setVisible(false);

						component.second.contextMenuColorPicker.get()->addAction(component.second.actionCopyColor.get());
						component.second.contextMenuColorPicker.get()->addAction(component.second.actionPasteColor.get());
						component.second.contextMenuColorPicker.get()->addSeparator();
						component.second.contextMenuColorPicker.get()->addAction(component.second.actionApplyColorToAllInSet.get());

						component.second.btnPicker.get()->setContextMenuPolicy(Qt::CustomContextMenu);
						connect(component.second.btnPicker.get(), &QPushButton::customContextMenuRequested, this, [&](const QPoint &point) {
							QPoint globalPos = component.second.btnPicker.get()->mapToGlobal(point);
							component.second.contextMenuColorPicker.get()->exec(globalPos);
						});

						connect(component.second.actionCopyColor.get(), &QAction::triggered, this, [&]() {
							pickerCopiedColor = component.second.assetsMap.at(component.second.displayedAssetKey).colorAltered;
							pickerUpdatePasteIconColor(pickerCopiedColor);
						});

						connect(component.second.actionPasteColor.get(), &QAction::triggered, this, [&]() {
							if (pickerCopiedColor.isValid())
							{
								auto& currentAsset = component.second.assetsMap.at(component.second.displayedAssetKey);
								currentAsset.colorAltered = pickerCopiedColor;
								updatePartInScene(component.second, currentAsset);
								characterModified = true;
							}
						});

						connect(component.second.actionApplyColorToAllInSet.get(), &QAction::triggered, this, [&]() {
							if (component.second.settings.colorSetType == ColorSetType::FILL_NO_OUTLINE ||
								component.second.settings.colorSetType == ColorSetType::FILL_WITH_OUTLINE)
							{
								QColor currentColor = component.second.assetsMap.at(component.second.displayedAssetKey).colorAltered;
								for (auto& asset : component.second.assetsMap)
								{
									asset.second.colorAltered = currentColor;
									characterModified = true;
								}
							}
						});

						connect(component.second.btnPicker.get(), &QPushButton::clicked, this, [&]() {
							auto& currentAsset = component.second.assetsMap.at(component.second.displayedAssetKey);
							QColor colorNew = QColorDialog::getColor(currentAsset.colorAltered, this->parentWidget(), "Choose Color");
							if (colorNew.isValid())
							{
								currentAsset.colorAltered = colorNew;
								updatePartInScene(component.second, currentAsset);
								characterModified = true;
							}
						});
					}

					component.second.item.get()->setZValue(component.second.settings.displayOrderZ);
					component.second.actionPasteColor->setIcon(QIcon(pickerPasteColorIcon));
				}
			}
		}
	}

	// Now apply settings for the current species/gender/pose/components/assets
	// (since we're just starting the program, the "current" is the default)

	speciesMap.at(speciesCurrent).actionSpecies.get()->setChecked(true);
	speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).actionGender.get()->setChecked(true);
	speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent).actionPose.get()->setChecked(true);

	for (auto& gender : speciesMap.at(speciesCurrent).genderMap)
		gender.second.actionGender.get()->setVisible(true);
	for (auto& pose : speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap)
			pose.second.actionPose.get()->setVisible(true);
	
	for (auto& component : speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent).componentMap)
	{
		if (component.second.settings.partHasBtnSwap)
		{
			partSwapGroupLayout.get()->addWidget
			(
				component.second.btnSwapComponent.get(),
				component.second.settings.gridPlaceSwapLeft[0],
				component.second.settings.gridPlaceSwapLeft[1],
				component.second.settings.gridAlignSwapLeft
			);
			component.second.btnSwapComponent.get()->setVisible(true);
			int count = 0;
			for (auto& asset : component.second.assetsMap)
			{
				partSwapGroupLayout.get()->addWidget
				(
					asset.second.btnSwapAsset.get(),
					count,
					1,
					component.second.settings.gridAlignSwapRight
				);
			}

			if (componentCurrent == ComponentType::NONE && component.first != ComponentType::NONE)
			{
				componentCurrent = component.first;
				for (auto& asset : component.second.assetsMap)
				{
					asset.second.btnSwapAsset.get()->setEnabled(true);
					asset.second.btnSwapAsset.get()->setVisible(true);
				}
			}
		}
		if (component.second.settings.partHasBtnPicker)
		{
			partPickerGroupLayout.get()->addWidget
			(
				component.second.btnPicker.get(),
				component.second.settings.gridPlacePicker[0],
				component.second.settings.gridPlacePicker[1],
				component.second.settings.gridAlignPicker
			);
			component.second.btnPicker.get()->setVisible(true);
		}
		scene.get()->addItem(component.second.item.get());
	}

	// Now set up the rest of the UI.

	pickerUpdatePasteIconColor(pickerCopiedColor);

	for (const auto& textInputSL : textInputSingleLineList)
	{
		textInputSL.inputWidget.get()->setParent(this);
		textInputSL.inputWidget.get()->setStyleSheet(textInputSL.styleSheet);
		textInputSL.inputWidget.get()->setPlaceholderText(textInputSL.placeholderText);

		characterNameInputGroupLayout.get()->addWidget
		(
			textInputSL.inputWidget.get(),
			textInputSL.gridPlace[0],
			textInputSL.gridPlace[1],
			textInputSL.gridAlign
		);
	}

	/*for (const auto& poseSwap : poseSwapButtonList)
	{
		poseSwap.btn.get()->setParent(this);
		poseSwap.btn.get()->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		poseSwap.btn.get()->setFixedSize(QSize(poseSwap.width, poseSwap.height));
		poseSwap.btn.get()->setStyleSheet
		(
			poseSwap.styleSheetTemplate.arg(poseSwap.icons[0]).arg(poseSwap.icons[1]).arg(poseSwap.icons[2])
		);
		poseSwapGroupLayout.get()->addWidget
		(
			poseSwap.btn.get(),
			poseSwap.gridPlace[0],
			poseSwap.gridPlace[1],
			poseSwap.gridAlign
		);
	}

	connect(poseSwapButtonList[0].btn.get(), &QPushButton::released, this, [=]() {
		for (auto& cPartExternal : speciesMap.at(speciesCurrent).characterPartList)
		{
			for (auto& imgPart : cPartExternal.genderMap.at(genderCurrent).poseList[poseCurrentI].partsMap)
			{
				imgPart.second.btnSwapTo.get()->setVisible(false);
				partSwapGroupLayout.get()->removeWidget(imgPart.second.btnSwapTo.get());
			}
		}
		if (poseCurrentI - 1 >= 0)
		{
			poseCurrentI--;
			for (auto& cPart : speciesMap.at(speciesCurrent).characterPartList)
			{
				auto& currentPoseList = cPart.genderMap.at(genderCurrent).poseList;
				if (currentPoseList[poseCurrentI].partsMap.count(cPart.displayedPartKey) > 0)
					updatePartInScene(cPart, currentPoseList[poseCurrentI].partsMap.at(cPart.displayedPartKey));
				else
					updatePartInScene(cPart, currentPoseList[poseCurrentI].partsMap.begin()->second);
			}
		}
		else
		{
			poseCurrentI = speciesMap.at(speciesCurrent).characterPartList[0].genderMap.at(genderCurrent).poseList.size() - 1;
			for (auto& cPart : speciesMap.at(speciesCurrent).characterPartList)
			{
				auto& currentPoseList = cPart.genderMap.at(genderCurrent).poseList;
				if (currentPoseList[poseCurrentI].partsMap.count(cPart.displayedPartKey) > 0)
					updatePartInScene(cPart, currentPoseList[poseCurrentI].partsMap.at(cPart.displayedPartKey));
				else
					updatePartInScene(cPart, currentPoseList[poseCurrentI].partsMap.begin()->second);
			}
		}
		for (auto& cPartExternal : speciesMap.at(speciesCurrent).characterPartList)
		{
			cPartExternal.btnSwapCharacterPart.get()->setEnabled(true);
			int count = 0;
			for (auto& imgPart : cPartExternal.genderMap.at(genderCurrent).poseList[poseCurrentI].partsMap)
			{
				partSwapGroupLayout.get()->addWidget
				(
					imgPart.second.btnSwapTo.get(),
					count,
					1,
					cPartExternal.gridAlignSwapRight
				);
				count++;
			}
		}
		for (int i = 0; i < speciesMap.at(speciesCurrent).characterPartList.size(); i++)
		{
			if (speciesMap.at(speciesCurrent).characterPartList[i].partHasBtnSwap)
			{
				auto& cPart = speciesMap.at(speciesCurrent).characterPartList[i];
				for (auto& imgPart : cPart.genderMap.at(genderCurrent).poseList[poseCurrentI].partsMap)
				{
					imgPart.second.btnSwapTo.get()->setEnabled(true);
					imgPart.second.btnSwapTo.get()->setVisible(true);
				}
				break;
			}
		}
	});

	connect(poseSwapButtonList[1].btn.get(), &QPushButton::released, this, [=]() {
		for (auto& cPartExternal : speciesMap.at(speciesCurrent).characterPartList)
		{
			for (auto& imgPart : cPartExternal.genderMap.at(genderCurrent).poseList[poseCurrentI].partsMap)
			{
				imgPart.second.btnSwapTo.get()->setVisible(false);
				partSwapGroupLayout.get()->removeWidget(imgPart.second.btnSwapTo.get());
			}
		}
		if (poseCurrentI + 1 <= speciesMap.at(speciesCurrent).characterPartList[0].genderMap.at(genderCurrent).poseList.size() - 1)
		{
			poseCurrentI++;
			for (auto& cPart : speciesMap.at(speciesCurrent).characterPartList)
			{
				auto& currentPoseList = cPart.genderMap.at(genderCurrent).poseList;
				if (currentPoseList[poseCurrentI].partsMap.count(cPart.displayedPartKey) > 0)
					updatePartInScene(cPart, currentPoseList[poseCurrentI].partsMap.at(cPart.displayedPartKey));
				else
					updatePartInScene(cPart, currentPoseList[poseCurrentI].partsMap.begin()->second);
			}
		}
		else
		{
			poseCurrentI = 0;
			for (auto& cPart : speciesMap.at(speciesCurrent).characterPartList)
			{
				auto& currentPoseList = cPart.genderMap.at(genderCurrent).poseList;
				if (currentPoseList[poseCurrentI].partsMap.count(cPart.displayedPartKey) > 0)
					updatePartInScene(cPart, currentPoseList[poseCurrentI].partsMap.at(cPart.displayedPartKey));
				else
					updatePartInScene(cPart, currentPoseList[poseCurrentI].partsMap.begin()->second);
			}
		}
		for (auto& cPartExternal : speciesMap.at(speciesCurrent).characterPartList)
		{
			cPartExternal.btnSwapCharacterPart.get()->setEnabled(true);
			int count = 0;
			for (auto& imgPart : cPartExternal.genderMap.at(genderCurrent).poseList[poseCurrentI].partsMap)
			{
				partSwapGroupLayout.get()->addWidget
				(
					imgPart.second.btnSwapTo.get(),
					count,
					1,
					cPartExternal.gridAlignSwapRight
				);
				count++;
			}
		}
		for (int i = 0; i < speciesMap.at(speciesCurrent).characterPartList.size(); i++)
		{
			if (speciesMap.at(speciesCurrent).characterPartList[i].partHasBtnSwap)
			{
				auto& cPart = speciesMap.at(speciesCurrent).characterPartList[i];
				for (auto& imgPart : cPart.genderMap.at(genderCurrent).poseList[poseCurrentI].partsMap)
				{
					imgPart.second.btnSwapTo.get()->setEnabled(true);
					imgPart.second.btnSwapTo.get()->setVisible(true);
				}
				break;
			}
		}
	});*/

	/*for (const auto& uiSpacer : uiSpacerList)
	{
		uiSpacer.spacerBtn.get()->setParent(this);
		uiSpacer.spacerBtn.get()->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		uiSpacer.spacerBtn.get()->setFixedSize(QSize(uiSpacer.width, uiSpacer.height));
		uiSpacer.spacerBtn.get()->setStyleSheet(uiSpacer.style);
		layout.get()->addWidget
		(
			uiSpacer.spacerBtn.get(),
			uiSpacer.gridPlace[0],
			uiSpacer.gridPlace[1],
			uiSpacer.gridAlign
		);
	}*/

	partSwapGroupLayout.get()->setMargin(20);
	partSwapGroup.get()->setLayout(partSwapGroupLayout.get());
	partSwapGroup.get()->setFlat(true);
	layout.get()->addWidget(partSwapScroll.get(), 0, 2, Qt::AlignRight);
	partSwapScroll.get()->setWidgetResizable(true);
	partSwapScroll.get()->setWidget(partSwapGroup.get());
	partSwapScroll.get()->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	partSwapScroll.get()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	partPickerGroupLayout.get()->setMargin(20);
	partPickerGroup.get()->setLayout(partPickerGroupLayout.get());
	partPickerGroup.get()->setFlat(true);
	layout.get()->addWidget(partPickerScroll.get(), 0, 0, Qt::AlignLeft);
	partPickerScroll.get()->setWidgetResizable(true);
	partPickerScroll.get()->setWidget(partPickerGroup.get());
	partPickerScroll.get()->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	partPickerScroll.get()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	characterNameInputGroupLayout.get()->setMargin(10);
	characterNameInputGroup.get()->setLayout(characterNameInputGroupLayout.get());
	characterNameInputGroup.get()->setFlat(true);
	layout.get()->addWidget(characterNameInputGroup.get(), 1, 0, Qt::AlignLeft);

	/*poseSwapGroupLayout.get()->setMargin(10);
	poseSwapGroup.get()->setLayout(poseSwapGroupLayout.get());
	poseSwapGroup.get()->setFlat(true);
	layout.get()->addWidget(poseSwapGroup.get(), 2, 0, Qt::AlignLeft);*/

	// Template load should be last init operation in graphics display,
	// because it requires assets/parts to be ready (it acts identically to loading a saved character).
	loadDefaultCharacterFromTemplate();
}

// public:

bool GraphicsDisplay::fileSaveModifCheck()
{
	if (!characterModified)
		return true;

	const QMessageBox::StandardButton ret
		= QMessageBox::warning(this->parentWidget(), tr("Zen Character Creator 2D"),
			tr("The character has been modified.\nDo you want to save your character changes?"),
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

	switch (ret)
	{
	case QMessageBox::Save:
		if (fileSave())
			return true;
		else
			return false;
	case QMessageBox::Discard:
		return true;
	case QMessageBox::Cancel:
		return false;
	default:
		break;
	}

	return true;
}

// protected:

void GraphicsDisplay::contextMenuEvent(QContextMenuEvent *event)
{
	contextMenu.get()->exec(event->globalPos());
}

// private:

QString GraphicsDisplay::extractSubstringInbetweenQt(const QString strBegin, const QString strEnd, const QString &strExtractFrom)
{
	QString extracted = "";
	int posFound = 0;

	if (!strBegin.isEmpty() && !strEnd.isEmpty())
	{
		while (strExtractFrom.indexOf(strBegin, posFound, Qt::CaseSensitive) != -1)
		{
			int posBegin = strExtractFrom.indexOf(strBegin, posFound, Qt::CaseSensitive) + strBegin.length();
			int posEnd = strExtractFrom.indexOf(strEnd, posBegin, Qt::CaseSensitive);
			extracted += strExtractFrom.mid(posBegin, posEnd - posBegin);
			posFound = posEnd;
		}
	}
	else if (strBegin.isEmpty() && !strEnd.isEmpty())
	{
		int posBegin = 0;
		int posEnd = strExtractFrom.indexOf(strEnd, posBegin, Qt::CaseSensitive);
		extracted += strExtractFrom.mid(posBegin, posEnd - posBegin);
		posFound = posEnd;
	}
	else if (!strBegin.isEmpty() && strEnd.isEmpty())
	{
		int posBegin = strExtractFrom.indexOf(strBegin, posFound, Qt::CaseSensitive) + strBegin.length();
		int posEnd = strExtractFrom.length();
		extracted += strExtractFrom.mid(posBegin, posEnd - posBegin);
		posFound = posEnd;
	}
	return extracted;
}

QString GraphicsDisplay::extractSubstringInbetweenRevFind(const QString strBegin, const QString strEnd, const QString &strExtractFrom)
{
	QString extracted = "";
	int posFound = -1;

	if (!strBegin.isEmpty() && !strEnd.isEmpty())
	{
		while (strExtractFrom.lastIndexOf(strBegin, posFound, Qt::CaseSensitive) != -1)
		{
			int posBegin = strExtractFrom.lastIndexOf(strBegin, posFound, Qt::CaseSensitive) + strBegin.length();
			int posEnd = strExtractFrom.lastIndexOf(strEnd, posBegin, Qt::CaseSensitive);
			extracted += strExtractFrom.mid(posBegin, posEnd - posBegin);
			posFound--;
		}
	}
	else if (strBegin.isEmpty() && !strEnd.isEmpty())
	{
		int posBegin = 0;
		int posEnd = strExtractFrom.lastIndexOf(strEnd, posBegin, Qt::CaseSensitive);
		extracted += strExtractFrom.mid(posBegin, posEnd - posBegin);
		posFound = posEnd;
	}
	else if (!strBegin.isEmpty() && strEnd.isEmpty())
	{
		int posBegin = strExtractFrom.lastIndexOf(strBegin, posFound, Qt::CaseSensitive) + strBegin.length();
		int posEnd = strExtractFrom.length();
		extracted += strExtractFrom.mid(posBegin, posEnd - posBegin);
		posFound = posEnd;
	}
	return extracted;
}

QStringList GraphicsDisplay::fileGetAssetDirectories(const QString &subPath)
{
	QStringList assetPathList;
	QDirIterator dirIt(appExecutablePath + "/Assets/" + subPath, QDir::AllDirs | QDir::NoDotAndDotDot);
	while (dirIt.hasNext())
	{
		QString assetPath = dirIt.next();
		if (QFileInfo(assetPath).isDir())
			assetPathList.append(assetPath);
	}
	return assetPathList;
}

QStringList GraphicsDisplay::fileGetAssets(const QString &path)
{
	QStringList assetPathList;
	QDirIterator dirIt(path);
	while (dirIt.hasNext())
	{
		QString assetPath = dirIt.next();
		if (QFileInfo(assetPath).suffix() == "png")
			assetPathList.append(assetPath);
	}
	return assetPathList;
}

void GraphicsDisplay::updatePartInScene(const componentData &component, const assetsData &asset)
{
	if (component.settings.colorSetType == ColorSetType::FILL_WITH_OUTLINE)
		component.item.get()->setPixmap(recolorPixmapSolidWithOutline(asset));
	else if (component.settings.colorSetType == ColorSetType::FILL_NO_OUTLINE)
		component.item.get()->setPixmap(recolorPixmapSolid(asset));
	else
		component.item.get()->setPixmap(asset.imgBasePath);
}

QPixmap GraphicsDisplay::recolorPixmapSolid(const QPixmap &img, const QColor &color)
{
	QPixmap newImage = QPixmap(img);
	QPainter painter;
	painter.begin(&newImage);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.fillRect(newImage.rect(), color);
	painter.end();
	return newImage;
}

QPixmap GraphicsDisplay::recolorPixmapSolid(const assetsData &asset)
{
	QPixmap newImage = QPixmap(asset.imgBasePath);
	QPainter painter;
	painter.begin(&newImage);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.fillRect(newImage.rect(), asset.colorAltered);
	painter.end();
	return newImage;
}

QPixmap GraphicsDisplay::recolorPixmapSolidWithOutline(const assetsData &asset)
{
	QPixmap newImage = QPixmap(asset.imgBasePath);
	QPainter painter;
	painter.begin(&newImage);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.fillRect(newImage.rect(), asset.colorAltered);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawPixmap(QPixmap(asset.imgOutlinePath).rect(), QPixmap(asset.imgOutlinePath));
	painter.end();
	return newImage;
}

void GraphicsDisplay::pickerUpdatePasteIconColor(const QColor &color)
{
	for (auto& component : speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent).componentMap)
	{
		QPixmap newIcon = recolorPixmapSolid(component.second.actionPasteColor->icon().pixmap(QSize(20, 20)), color);
		component.second.actionPasteColor->setIcon(newIcon);
	}
}

void GraphicsDisplay::loadDefaultCharacterFromTemplate()
{
	// To provide a little more control over default character settings,
	// a default character can be loaded from a template file.
	// Template is built identical to a saved character, so the loading logic can be reused.
	const QString templatePath =
		QCoreApplication::applicationDirPath() +
		"/Assets/Species/" +
		speciesMap.at(speciesCurrent).assetStr +
		"/" +
		speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).assetStr +
		"/" +
		speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent).assetStr +
		"/CharacterTemplate/default-character-template.zen2dx"
		;
	if (QFile::exists(templatePath))
	{
		fileLoadSavedCharacter(templatePath);
	}
}

void GraphicsDisplay::fileLoadSavedCharacter(const QString &filePath)
{
	QString fileContents;
	QFile fileRead(filePath);
	if (fileRead.open(QIODevice::ReadOnly))
	{
		bool partsMissing = false;
		QTextStream qStream(&fileRead);
		while (!qStream.atEnd())
		{
			QString line = qStream.readLine();
			if (line.contains("::Species=") && line.contains("::Gender=") && line.contains("::Pose="))
			{
				QString speciesStr = extractSubstringInbetweenQt("::Species=", "::", line);
				QString genderStr = extractSubstringInbetweenQt("::Gender=", "::", line);
				QString poseStr = extractSubstringInbetweenQt("::Pose=", "::", line);

				for (auto& species : speciesMap)
				{
					if (species.second.assetStr == speciesStr)
					{
						for (auto& gender : species.second.genderMap)
						{
							if (gender.second.assetStr == genderStr)
							{
								for (auto& pose : gender.second.poseMap)
								{
									if (pose.second.assetStr == poseStr)
									{
										removeCurrentSpeciesFromScene();
										speciesCurrent = species.first;
										genderCurrent = gender.first;
										poseCurrent = pose.first;
										applyCurrentSpeciesToScene();
										speciesMap.at(speciesCurrent).actionSpecies.get()->setChecked(true);
										break;
									}
								}
								break;
							}
						}
						break;
					}
				}
			}
			for (auto& component : speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent).componentMap)
			{
				if (line.contains(component.second.settings.assetStr + "="))
				{
					QString key = QFileInfo(extractSubstringInbetweenQt("=", ",", line)).baseName();
					if (component.second.assetsMap.count(key) > 0)
					{
						component.second.displayedAssetKey = key;
						component.second.assetsMap.at(key).colorAltered = QColor(extractSubstringInbetweenQt(",", "", line));
						updatePartInScene(component.second, component.second.assetsMap.at(key));
						if (component.first == componentCurrent)
						{
							component.second.btnSwapComponent.get()->setEnabled(false);
							component.second.assetsMap.at(key).btnSwapAsset.get()->setEnabled(false);
						}
					}
					else
						partsMissing = true;
					break;
				}
			}
			if (line.contains("backgroundColor="))
			{
				setBackgroundColor(QColor(extractSubstringInbetweenQt("=", "", line)));
			}
			for (auto& textInputSL : textInputSingleLineList)
			{
				if (line.contains(textInputSL.inputTypeStr + "="))
				{
					textInputSL.inputWidget.get()->setText(extractSubstringInbetweenQt("=", "", line));
				}
			}
		}
		fileRead.close();
		characterModified = false;
		if (partsMissing)
			QMessageBox::information(this->parentWidget(), tr("Parts Missing"), tr("One or more parts was not found when trying to load from file reference.\r\nSave file only saves references to assets, so if they are moved or deleted, loading may fail."));
	}
}

void GraphicsDisplay::fileNew()
{
	for (auto& component : speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent).componentMap)
	{
		component.second.displayedAssetKey = component.second.assetsMap.begin()->first;
		for (auto& asset : component.second.assetsMap)
			asset.second.colorAltered = asset.second.colorDefault;
	}
	backgroundColor = backgroundColorDefault;
	setBackgroundColor(backgroundColor);
	for (auto& textInputSL : textInputSingleLineList)
	{
		textInputSL.inputWidget.get()->setText("");
	}
	characterModified = false;

	loadDefaultCharacterFromTemplate();
}

void GraphicsDisplay::fileOpen()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open"), fileDirLastOpened, tr("Zen Character Creator 2D Files (*.zen2dx)"));
	if (!filename.isEmpty())
	{
		fileLoadSavedCharacter(filename);
		fileDirLastOpened = QFileInfo(filename).path();
	}
}

bool GraphicsDisplay::fileSave()
{
	QString proposedSaveName;
	proposedSaveName += fileDirLastSaved + "/";
	for (const auto& textInputSL : textInputSingleLineList)
	{
		if (textInputSL.inputType == TextInputSingleLineType::FIRST_NAME)
			proposedSaveName += textInputSL.inputWidget.get()->text();
		else if (textInputSL.inputType == TextInputSingleLineType::LAST_NAME)
			proposedSaveName += textInputSL.inputWidget.get()->text();
	}
	proposedSaveName += QDateTime::currentDateTime().toString("_yyyy_MM_dd_HH_mm_ss");
	QFileDialog dialog(this, tr("Save As"), proposedSaveName, tr("Zen Character Creator 2D Files (*.zen2dx)"));
	dialog.setWindowModality(Qt::WindowModal);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	if (dialog.exec() == QFileDialog::Accepted)
	{
		QString fpath = dialog.selectedFiles().first();
		QFile fileWrite(fpath);
		if (fileWrite.open(QIODevice::WriteOnly))
		{
			QTextStream qStream(&fileWrite);

			qStream <<
				"::"
				"Species=" + speciesMap.at(speciesCurrent).assetStr +
				"::" +
				"Gender=" + speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).assetStr +
				"::" +
				"Pose=" + speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent).assetStr +
				"::"
				"\r\n";

			for (auto& component : speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent).componentMap)
			{
				auto& currentPart = component.second.assetsMap.at(component.second.displayedAssetKey);
				qStream << component.second.settings.assetStr +
					"=" + currentPart.imgFilename +
					"," + currentPart.colorAltered.name() + "\r\n";
			}

			qStream << "backgroundColor=" + backgroundColor.name() + "\r\n";

			for (const auto& textInputSL : textInputSingleLineList)
			{
				qStream << textInputSL.inputTypeStr + "=" + textInputSL.inputWidget.get()->text() + "\r\n";
			}

			fileWrite.close();
			fileDirLastSaved = QFileInfo(fpath).path();
			return true;
		}
	}
	return false;
}

void GraphicsDisplay::fileExportCharacter()
{
	QString proposedExportName;
	proposedExportName += fileDirLastExported + "/";
	for (const auto& textInputSL : textInputSingleLineList)
	{
		if (textInputSL.inputType == TextInputSingleLineType::FIRST_NAME)
			proposedExportName += textInputSL.inputWidget.get()->text();
		else if (textInputSL.inputType == TextInputSingleLineType::LAST_NAME)
			proposedExportName += textInputSL.inputWidget.get()->text();
	}
	proposedExportName += QDateTime::currentDateTime().toString("_yyyy_MM_dd_HH_mm_ss");
	QFileDialog dialog(this, tr("Save As"), proposedExportName, tr("Image Files (*.png)"));
	dialog.setWindowModality(Qt::WindowModal);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	if (dialog.exec() == QFileDialog::Accepted)
	{
		QString selectedFile = dialog.selectedFiles().first();
		QFile fileWrite(selectedFile);
		fileWrite.open(QIODevice::WriteOnly);
		QImage composite(scene.get()->sceneRect().size().toSize(), QImage::Format_ARGB32_Premultiplied);
		composite.fill(backgroundColor);
		QPainter painter(&composite);
		scene->render(&painter);
		composite.save(&fileWrite, "PNG");
		fileDirLastExported = QFileInfo(selectedFile).path();
	}
}

//int GraphicsDisplay::findPosOfFilenameInPartsList(const characterPart &cPart, const QString &filename)
//{
//	auto& gc = cPart.genderMap.at(genderCurrent);
//	for (int i = 0; i < gc.poseList[poseCurrentI].partsList.size(); i++)
//	{
//		if (gc.poseList[poseCurrentI].partsList[i].imgFilename == filename)
//		{
//			return i;
//		}
//	}
//	return -1;
//}

void GraphicsDisplay::setBackgroundColor(const QColor &color)
{
	backgroundColor = color;
	this->setStyleSheet(styleSheetEditable.arg(backgroundColor.name()));
	characterModified = true;
}

void GraphicsDisplay::removeCurrentSpeciesFromScene()
{
	for (auto& component : speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent).componentMap)
	{
		if (component.second.settings.partHasBtnSwap)
		{
			component.second.btnSwapComponent.get()->setVisible(false);
			partSwapGroupLayout.get()->removeWidget(component.second.btnSwapComponent.get());
			for (auto& asset : component.second.assetsMap)
			{
				asset.second.btnSwapAsset.get()->setVisible(false);
				partSwapGroupLayout.get()->removeWidget(asset.second.btnSwapAsset.get());
			}
		}
		if (component.second.settings.partHasBtnPicker)
		{
			component.second.btnPicker.get()->setVisible(false);
			partPickerGroupLayout.get()->removeWidget(component.second.btnPicker.get());
		}

		// We remove rather than using QGraphicsScene clear() function,
		// to avoid deleting the items (they need to be reusable).
		scene.get()->removeItem(component.second.item.get());
	}
	componentCurrent = ComponentType::NONE;
}

void GraphicsDisplay::applyCurrentSpeciesToScene()
{
	// Note: We make the widgets visible AFTER adding them to layout.
	// Otherwise, they may be briefly visible outside of the layout
	// and then get added to it, causing a flickering effect from the adjustment of position.
	for (auto& component : speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent).componentMap)
	{
		if (component.second.settings.partHasBtnSwap)
		{
			partSwapGroupLayout.get()->addWidget
			(
				component.second.btnSwapComponent.get(),
				component.second.settings.gridPlaceSwapLeft[0],
				component.second.settings.gridPlaceSwapLeft[1],
				component.second.settings.gridAlignSwapLeft
			);
			int count = 0;
			for (auto& asset : component.second.assetsMap)
			{
				partSwapGroupLayout.get()->addWidget
				(
					asset.second.btnSwapAsset.get(),
					count,
					1,
					component.second.settings.gridAlignSwapRight
				);
				count++;
			}
			component.second.btnSwapComponent.get()->setVisible(true);

			if (componentCurrent == ComponentType::NONE && component.first != ComponentType::NONE)
			{
				componentCurrent = component.first;
				for (auto& asset : component.second.assetsMap)
				{
					asset.second.btnSwapAsset.get()->setEnabled(true);
					asset.second.btnSwapAsset.get()->setVisible(true);
				}
			}
		}
		if (component.second.settings.partHasBtnPicker)
		{
			partPickerGroupLayout.get()->addWidget
			(
				component.second.btnPicker.get(),
				component.second.settings.gridPlacePicker[0],
				component.second.settings.gridPlacePicker[1],
				component.second.settings.gridAlignPicker
			);
			component.second.btnPicker.get()->setVisible(true);
		}
		scene.get()->addItem(component.second.item.get());
	}
}