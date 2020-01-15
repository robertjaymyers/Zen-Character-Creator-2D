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

	{
		textInputSingleLine textInputSL =
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
			{4, 0}, // Row/Col placement in grid layout
			Qt::AlignLeft | Qt::AlignBottom // Alignment in grid layout
		};
		textInputSingleLineList.push_back(std::move(textInputSL));
	}
	{
		textInputSingleLine textInputSL =
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
			{4, 1}, // Row/Col placement in grid layout
			Qt::AlignLeft | Qt::AlignBottom // Alignment in grid layout
		};
		textInputSingleLineList.push_back(std::move(textInputSL));
	}

	{
		characterPart cPart =
		{
			PartType::SKIN_COLOR,
			0, // Display order in scene (higher numbers overlap lower numbers)
			"SkinColor",
			"#764c39",
			ColorSetType::FILL_WITH_OUTLINE,
			false, // SWAP BTN: Flag for whether part is expected to display btn
			true, // PICKER BTN: Flag for whether part is expected to display btn
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-pick-skin-color.png"
			<< ":/ZenCharacterCreator2D/Resources/button-pick-skin-color-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-pick-skin-color-hover-pressed.png",
			{0, 0}, // PICKER BTN: Row/Col placement in grid layout
			Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
			{-1, -1}, // SWAP LEFT BTN: Row/Col placement in grid layout
			nullptr, // PICKER BTN: Alignment in grid layout
			{-1, -1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
			nullptr, // PICKER BTN: Alignment in grid layout
			75, // SWAP BTN: Width
			75, // SWAP BTN: Height
			75, // PICKER BTN: Width
			75, // PICKER BTN: Height
		};
		characterPartList.push_back(std::move(cPart));
	}
	{
		characterPart cPart =
		{
			PartType::EYE_COLOR,
			1, // Display order in scene (higher numbers overlap lower numbers)
			"EyeColor",
			"#aaaa7f",
			ColorSetType::FILL_WITH_OUTLINE,
			false, // SWAP BTN: Flag for whether part is expected to display btn
			true, // PICKER BTN: Flag for whether part is expected to display btn
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-pick-eye-color.png"
			<< ":/ZenCharacterCreator2D/Resources/button-pick-eye-color-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-pick-eye-color-hover-pressed.png",
			{1, 0}, // PICKER BTN: Row/Col placement in grid layout
			Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
			{-1, -1}, // SWAP LEFT BTN: Row/Col placement in grid layout
			nullptr, // PICKER BTN: Alignment in grid layout
			{-1, -1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
			nullptr, // PICKER BTN: Alignment in grid layout
			75, // SWAP BTN: Width
			75, // SWAP BTN: Height
			75, // PICKER BTN: Width
			75, // PICKER BTN: Height
		};
		characterPartList.push_back(std::move(cPart));
	}
	{
		characterPart cPart =
		{
			PartType::LIP_COLOR,
			2, // Display order in scene (higher numbers overlap lower numbers)
			"LipColor",
			"#555500",
			ColorSetType::FILL_WITH_OUTLINE,
			false, // SWAP BTN: Flag for whether part is expected to display btn
			true, // PICKER BTN: Flag for whether part is expected to display btn
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-pick-lip-color.png"
			<< ":/ZenCharacterCreator2D/Resources/button-pick-lip-color-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-pick-lip-color-hover-pressed.png",
			{2, 0}, // PICKER BTN: Row/Col placement in grid layout
			Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
			{-1, -1}, // SWAP LEFT BTN: Row/Col placement in grid layout
			nullptr, // PICKER BTN: Alignment in grid layout
			{-1, -1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
			nullptr, // PICKER BTN: Alignment in grid layout
			75, // SWAP BTN: Width
			75, // SWAP BTN: Height
			75, // PICKER BTN: Width
			75, // PICKER BTN: Height
		};
		characterPartList.push_back(std::move(cPart));
	}
	{
		characterPart cPart =
		{
			PartType::BLUSH_COLOR,
			3, // Display order in scene (higher numbers overlap lower numbers)
			"BlushColor",
			"#555500",
			ColorSetType::FILL_NO_OUTLINE,
			false, // SWAP BTN: Flag for whether part is expected to display btn
			true, // PICKER BTN: Flag for whether part is expected to display btn
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-pick-blush-color.png"
			<< ":/ZenCharacterCreator2D/Resources/button-pick-blush-color-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-pick-blush-color-hover-pressed.png",
			{3, 0}, // PICKER BTN: Row/Col placement in grid layout
			Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
			{-1, -1}, // SWAP LEFT BTN: Row/Col placement in grid layout
			nullptr, // PICKER BTN: Alignment in grid layout
			{-1, -1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
			nullptr, // PICKER BTN: Alignment in grid layout
			75, // SWAP BTN: Width
			75, // SWAP BTN: Height
			75, // PICKER BTN: Width
			75, // PICKER BTN: Height
		};
		characterPartList.push_back(std::move(cPart));
	}
	{
		characterPart cPart =
		{
			PartType::HEAD,
			4, // Display order in scene (higher numbers overlap lower numbers)
			"Head",
			"#FFFFFF",
			ColorSetType::NONE,
			true, // SWAP BTN: Flag for whether part is expected to display btn
			false, // PICKER BTN: Flag for whether part is expected to display btn
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
			QStringList(), // Head color changing is controlled by skin color, so we leave this empty
			{-1, -1}, // PICKER BTN: Row/Col placement in grid layout
			nullptr, // PICKER BTN: Alignment in grid layout
			{1, 3}, // SWAP LEFT BTN: Row/Col placement in grid layout
			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
			{1, 4}, // SWAP RIGHT BTN: Row/Col placement in grid layout
			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
			75, // SWAP BTN: Width
			75, // SWAP BTN: Height
			75, // PICKER BTN: Width
			75, // PICKER BTN: Height
		};
		characterPartList.push_back(std::move(cPart));
	}
	{
		characterPart cPart =
		{
			PartType::BOTTOM,
			5, // Display order in scene (higher numbers overlap lower numbers)
			"Bottom",
			"#B5B5B5",
			ColorSetType::FILL_WITH_OUTLINE,
			true, // SWAP BTN: Flag for whether part is expected to display btn
			true, // PICKER BTN: Flag for whether part is expected to display btn
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-pick-bottom-color.png"
			<< ":/ZenCharacterCreator2D/Resources/button-pick-bottom-color-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-pick-bottom-color-hover-pressed.png",
			{2, 1}, // PICKER BTN: Row/Col placement in grid layout
			Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
			{3, 3}, // SWAP LEFT BTN: Row/Col placement in grid layout
			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
			{3, 4}, // SWAP RIGHT BTN: Row/Col placement in grid layout
			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
			75, // SWAP BTN: Width
			75, // SWAP BTN: Height
			75, // PICKER BTN: Width
			75, // PICKER BTN: Height
		};
		characterPartList.push_back(std::move(cPart));
	}
	{
		characterPart cPart =
		{
			PartType::CHEST,
			6, // Display order in scene (higher numbers overlap lower numbers)
			"Chest",
			"#B5B5B5",
			ColorSetType::FILL_WITH_OUTLINE,
			true, // SWAP BTN: Flag for whether part is expected to display btn
			true, // PICKER BTN: Flag for whether part is expected to display btn
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-pick-chest-color.png"
			<< ":/ZenCharacterCreator2D/Resources/button-pick-chest-color-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-pick-chest-color-hover-pressed.png",
			{1, 1}, // PICKER BTN: Row/Col placement in grid layout
			Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
			{2, 3}, // SWAP LEFT BTN: Row/Col placement in grid layout
			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
			{2, 4}, // SWAP RIGHT BTN: Row/Col placement in grid layout
			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
			75, // SWAP BTN: Width
			75, // SWAP BTN: Height
			75, // PICKER BTN: Width
			75, // PICKER BTN: Height
		};
		characterPartList.push_back(std::move(cPart));
	}
	{
		characterPart cPart =
		{
			PartType::FEET,
			7, // Display order in scene (higher numbers overlap lower numbers)
			"Feet",
			"#000000",
			ColorSetType::FILL_WITH_OUTLINE,
			true, // SWAP BTN: Flag for whether part is expected to display btn
			true, // PICKER BTN: Flag for whether part is expected to display btn
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-pick-feet-color.png"
			<< ":/ZenCharacterCreator2D/Resources/button-pick-feet-color-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-pick-feet-color-hover-pressed.png",
			{3, 1}, // PICKER BTN: Row/Col placement in grid layout
			Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
			{4, 3}, // SWAP LEFT BTN: Row/Col placement in grid layout
			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
			{4, 4}, // SWAP RIGHT BTN: Row/Col placement in grid layout
			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
			75, // SWAP BTN: Width
			75, // SWAP BTN: Height
			75, // PICKER BTN: Width
			75, // PICKER BTN: Height
		};
		characterPartList.push_back(std::move(cPart));
	}
	{
		characterPart cPart =
		{
			PartType::HAIR,
			8, // Display order in scene (higher numbers overlap lower numbers)
			"Hair",
			"#000000",
			ColorSetType::FILL_WITH_OUTLINE,
			true, // SWAP BTN: Flag for whether part is expected to display btn
			true, // PICKER BTN: Flag for whether part is expected to display btn
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
			QStringList()
			<< ":/ZenCharacterCreator2D/Resources/button-pick-hair-color.png"
			<< ":/ZenCharacterCreator2D/Resources/button-pick-hair-color-hover.png"
			<< ":/ZenCharacterCreator2D/Resources/button-pick-hair-color-hover-pressed.png",
			{0, 1}, // PICKER BTN: Row/Col placement in grid layout
			Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
			{0, 3}, // SWAP LEFT BTN: Row/Col placement in grid layout
			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
			{0, 4}, // SWAP RIGHT BTN: Row/Col placement in grid layout
			Qt::AlignRight, // PICKER BTN: Alignment in grid layout
			75, // SWAP BTN: Width
			75, // SWAP BTN: Height
			75, // PICKER BTN: Width
			75, // PICKER BTN: Height
		};
		characterPartList.push_back(std::move(cPart));
	}

	this->setScene(scene.get());
	scene.get()->setParent(this->parent());

	this->setStyleSheet(styleSheetEditable.arg(backgroundColorDefault.name()));
	this->setLayout(layout.get());

	layout.get()->setMargin(50);

	// Before adding to scene, we need to sort the stored part types to match their display order.
	// Depending on how parts are added to the list, this may be unnecessary, but we do it anyway
	// as a safeguard against programmer error in ordering how parts get added to the list.
	std::sort(characterPartList.begin(), characterPartList.end(), compareDisplayOrder);

	for (auto& cPart : characterPartList)
	{
		QStringList partsFromFile = fileGetAssets(cPart.partTypeAssetStr);

		// Note: We store as filename only (e.g. NOT including full path), 
		// so that if exe moves, character saves can still be loaded correctly in relation to loaded assets.
		for (auto& part : partsFromFile)
		{
			characterPart::imgParts newImgParts;
			newImgParts.imgBase = QPixmap(part);
			newImgParts.imgAltered = newImgParts.imgBase;
			newImgParts.imgFilename = QFileInfo(part).fileName();
			newImgParts.colorBase = cPart.defaultInitialColor;
			newImgParts.colorAltered = newImgParts.colorBase;

			if (cPart.colorSetType == ColorSetType::FILL_WITH_OUTLINE)
				newImgParts.imgOutline = QPixmap(part.replace("/" + QFileInfo(part).fileName(), "/Outline/" + QFileInfo(part).fileName()));

			cPart.partsList.push_back(newImgParts);
		}
		if (!cPart.partsList.empty())
		{
			for (auto& p : cPart.partsList)
			{
				if (cPart.colorSetType == ColorSetType::FILL_WITH_OUTLINE)
					p.imgAltered = recolorPixmapSolidWithOutline(p.imgBase, p.imgOutline, p.colorAltered);
				else if (cPart.colorSetType == ColorSetType::FILL_NO_OUTLINE)
					p.imgAltered = recolorPixmapSolid(p.imgBase, p.colorAltered);
			}

			// For simplicity, we make the default the first part in the list.
			// (ex: the first Chest part in the list of Chest parts)
			// This means default will be defined by load order, based on how folder traversal works.
			// (so probably alphabetical)
			// Note that an instance of this class initialized with a color set type of none still uses
			// the altered image variable for display to avoid unnecessary complexity.
			// It gets its altered image set to the normal image at the beginning, so despite never
			// having color setting applied, it'll still have an image to display.
			cPart.item.get()->setPixmap(cPart.partsList[cPart.displayedPartI].imgAltered);
		}

		if (cPart.partHasBtnSwap)
		{
			cPart.btnSwapLeft.get()->setStyleSheet
			(
				cPart.btnStyleSheetTemplate
				.arg(cPart.btnSwapLeftIcons[0])
				.arg(cPart.btnSwapLeftIcons[1])
				.arg(cPart.btnSwapLeftIcons[2])
			);
			cPart.btnSwapLeft.get()->setParent(this);
			cPart.btnSwapLeft.get()->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
			cPart.btnSwapLeft.get()->setFixedSize(QSize(cPart.btnSwapWidth, cPart.btnSwapHeight));

			layout.get()->addWidget
			(
				cPart.btnSwapLeft.get(),
				cPart.gridPlaceSwapLeft[0],
				cPart.gridPlaceSwapLeft[1],
				cPart.gridAlignSwapLeft
			);

			connect(cPart.btnSwapLeft.get(), &QPushButton::clicked, this, [&]() {
				if (cPart.displayedPartI - 1 >= 0)
				{
					cPart.displayedPartI--;
					cPart.item.get()->setPixmap(cPart.partsList[cPart.displayedPartI].imgAltered);
				}
				else
				{
					cPart.displayedPartI = cPart.partsList.size() - 1;
					cPart.item.get()->setPixmap(cPart.partsList[cPart.displayedPartI].imgAltered);
				}
				characterModified = true;
			});

			cPart.btnSwapRight.get()->setStyleSheet
			(
				cPart.btnStyleSheetTemplate
				.arg(cPart.btnSwapRightIcons[0])
				.arg(cPart.btnSwapRightIcons[1])
				.arg(cPart.btnSwapRightIcons[2])
			);
			cPart.btnSwapRight.get()->setParent(this);
			cPart.btnSwapRight.get()->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
			cPart.btnSwapRight.get()->setFixedSize(QSize(cPart.btnSwapWidth, cPart.btnSwapHeight));

			layout.get()->addWidget
			(
				cPart.btnSwapRight.get(),
				cPart.gridPlaceSwapRight[0],
				cPart.gridPlaceSwapRight[1],
				cPart.gridAlignSwapRight
			);

			connect(cPart.btnSwapRight.get(), &QPushButton::clicked, this, [&]() {
				if (cPart.displayedPartI + 1 <= cPart.partsList.size() - 1)
				{
					cPart.displayedPartI++;
					cPart.item.get()->setPixmap(cPart.partsList[cPart.displayedPartI].imgAltered);
				}
				else
				{
					cPart.displayedPartI = 0;
					cPart.item.get()->setPixmap(cPart.partsList[cPart.displayedPartI].imgAltered);
				}
				characterModified = true;
			});
		}
		if (cPart.partHasBtnPicker)
		{
			cPart.btnPicker.get()->setStyleSheet
			(
				cPart.btnStyleSheetTemplate
				.arg(cPart.btnPickerIcons[0])
				.arg(cPart.btnPickerIcons[1])
				.arg(cPart.btnPickerIcons[2])
			);
			cPart.btnPicker.get()->setParent(this);
			cPart.btnPicker.get()->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
			cPart.btnPicker.get()->setFixedSize(QSize(cPart.btnPickerWidth, cPart.btnPickerHeight));

			layout.get()->addWidget
			(
				cPart.btnPicker.get(),
				cPart.gridPlacePicker[0],
				cPart.gridPlacePicker[1],
				cPart.gridAlignPicker
			);

			cPart.contextMenuColorPicker.get()->addAction(cPart.actionCopyColor.get());
			cPart.contextMenuColorPicker.get()->addAction(cPart.actionPasteColor.get());
			cPart.contextMenuColorPicker.get()->addSeparator();
			cPart.contextMenuColorPicker.get()->addAction(cPart.actionApplyColorToAllInSet.get());

			cPart.btnPicker.get()->setContextMenuPolicy(Qt::CustomContextMenu);
			connect(cPart.btnPicker.get(), &QPushButton::customContextMenuRequested, this, [&](const QPoint &point) {
				QPoint globalPos = cPart.btnPicker.get()->mapToGlobal(point);
				cPart.contextMenuColorPicker.get()->exec(globalPos);
			});

			connect(cPart.actionCopyColor.get(), &QAction::triggered, this, [&]() {
				pickerCopiedColor = cPart.partsList[cPart.displayedPartI].colorAltered;
				pickerUpdatePasteIconColor(pickerCopiedColor);
			});

			connect(cPart.actionPasteColor.get(), &QAction::triggered, this, [&]() {
				if (pickerCopiedColor.isValid())
				{
					auto& currentPart = cPart.partsList[cPart.displayedPartI];
					currentPart.colorAltered = pickerCopiedColor;
					if (cPart.colorSetType == ColorSetType::FILL_WITH_OUTLINE)
						currentPart.imgAltered = recolorPixmapSolidWithOutline(currentPart.imgBase, currentPart.imgOutline, currentPart.colorAltered);
					else if (cPart.colorSetType == ColorSetType::FILL_NO_OUTLINE)
						currentPart.imgAltered = recolorPixmapSolid(currentPart.imgBase, currentPart.colorAltered);
					cPart.item.get()->setPixmap(currentPart.imgAltered);
					characterModified = true;
				}
			});

			connect(cPart.actionApplyColorToAllInSet.get(), &QAction::triggered, this, [&]() {
				if (cPart.colorSetType == ColorSetType::FILL_NO_OUTLINE ||
					cPart.colorSetType == ColorSetType::FILL_WITH_OUTLINE)
				{
					QColor current = cPart.partsList[cPart.displayedPartI].colorAltered;
					for (auto& p : cPart.partsList)
					{
						p.colorAltered = current;
						if (cPart.colorSetType == ColorSetType::FILL_WITH_OUTLINE)
							p.imgAltered = recolorPixmapSolidWithOutline(p.imgBase, p.imgOutline, p.colorAltered);
						else if (cPart.colorSetType == ColorSetType::FILL_NO_OUTLINE)
							p.imgAltered = recolorPixmapSolid(p.imgBase, p.colorAltered);
						characterModified = true;
					}
				}
			});

			connect(cPart.btnPicker.get(), &QPushButton::clicked, this, [&]() {
				auto& currentPart = cPart.partsList[cPart.displayedPartI];
				QColor colorNew = QColorDialog::getColor(currentPart.colorAltered, this->parentWidget(), "Choose Color");
				if (colorNew.isValid())
				{
					currentPart.colorAltered = colorNew;
					if (cPart.colorSetType == ColorSetType::FILL_WITH_OUTLINE)
						currentPart.imgAltered = recolorPixmapSolidWithOutline(currentPart.imgBase, currentPart.imgOutline, currentPart.colorAltered);
					else if (cPart.colorSetType == ColorSetType::FILL_NO_OUTLINE)
						currentPart.imgAltered = recolorPixmapSolid(currentPart.imgBase, currentPart.colorAltered);
					cPart.item.get()->setPixmap(currentPart.imgAltered);
					characterModified = true;
				}
			});
		}

		scene.get()->addItem(cPart.item.get());

		cPart.actionPasteColor->setIcon(QIcon(pickerPasteColorIcon));
	}
	pickerUpdatePasteIconColor(pickerCopiedColor);

	for (const auto& textInputSL : textInputSingleLineList)
	{
		textInputSL.inputWidget.get()->setParent(this);
		textInputSL.inputWidget.get()->setStyleSheet(textInputSL.styleSheet);
		textInputSL.inputWidget.get()->setPlaceholderText(textInputSL.placeholderText);

		layout.get()->addWidget
		(
			textInputSL.inputWidget.get(),
			textInputSL.gridPlace[0],
			textInputSL.gridPlace[1],
			textInputSL.gridAlign
		);
	}

	for (const auto& uiSpacer : uiSpacerList)
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
	}

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
		= QMessageBox::warning(this->parentWidget(), tr("Character Creator 2D"),
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

QStringList GraphicsDisplay::fileGetAssets(const QString &subPath)
{
	QStringList assetPathList;
	QDirIterator dirIt(appExecutablePath + "/Assets/" + subPath);
	while (dirIt.hasNext())
	{
		QString assetPath = dirIt.next();
		if (QFileInfo(assetPath).suffix() == "png")
			assetPathList.append(assetPath);
	}
	return assetPathList;
}

QPixmap GraphicsDisplay::recolorPixmapSolid(const QPixmap &imgSolid, const QColor &color)
{
	QPixmap newImage = imgSolid;
	QPainter painter;
	painter.begin(&newImage);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.fillRect(newImage.rect(), color);
	painter.end();
	return newImage;
}

QPixmap GraphicsDisplay::recolorPixmapSolidWithOutline(const QPixmap &imgSolid, const QPixmap &imgOutline, const QColor &color)
{
	QPixmap newImage = imgSolid;
	QPainter painter;
	painter.begin(&newImage);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.fillRect(newImage.rect(), color);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawPixmap(imgOutline.rect(), imgOutline);
	painter.end();
	return newImage;
}

/*static*/ bool GraphicsDisplay::compareDisplayOrder(const characterPart &lhs, const characterPart &rhs)
{
	return lhs.displayOrder < rhs.displayOrder;
}

void GraphicsDisplay::pickerUpdatePasteIconColor(const QColor &color)
{
	for (auto& cPart : characterPartList)
	{
		QPixmap newIcon = recolorPixmapSolid(cPart.actionPasteColor->icon().pixmap(QSize(20, 20)), color);
		cPart.actionPasteColor->setIcon(newIcon);
	}
}

void GraphicsDisplay::loadDefaultCharacterFromTemplate()
{
	// To provide a little more control over default character settings,
	// a default character can be loaded from a template file.
	// Template is built identical to a saved character, so the loading logic can be reused.
	const QString templatePath = QCoreApplication::applicationDirPath() + "/Assets" + "/default-character-template.zen2dx";
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
			for (auto& cPart : characterPartList)
			{
				if (line.contains(cPart.partTypeAssetStr + "="))
				{
					int pos = findPosOfFilenameInPartsList(cPart, extractSubstringInbetweenQt("=", ",", line));
					if (pos != -1)
					{
						cPart.displayedPartI = pos;
						if (cPart.colorSetType != ColorSetType::NONE)
						{
							auto& currentPart = cPart.partsList[cPart.displayedPartI];
							currentPart.colorAltered = QColor(extractSubstringInbetweenQt(",", "", line));
							if (cPart.colorSetType == ColorSetType::FILL_WITH_OUTLINE)
								currentPart.imgAltered = recolorPixmapSolidWithOutline(currentPart.imgBase, currentPart.imgOutline, currentPart.colorAltered);
							else if (cPart.colorSetType == ColorSetType::FILL_NO_OUTLINE)
								currentPart.imgAltered = recolorPixmapSolid(currentPart.imgBase, currentPart.colorAltered);
						}
						cPart.item.get()->setPixmap(cPart.partsList[cPart.displayedPartI].imgAltered);
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
	for (auto& cPart : characterPartList)
	{
		cPart.displayedPartI = 0;
		for (auto& p : cPart.partsList)
		{
			p.colorAltered = p.colorBase;
			if (cPart.colorSetType == ColorSetType::FILL_NO_OUTLINE ||
				cPart.colorSetType == ColorSetType::FILL_WITH_OUTLINE)
			{
				if (cPart.colorSetType == ColorSetType::FILL_WITH_OUTLINE)
					p.imgAltered = recolorPixmapSolidWithOutline(p.imgBase, p.imgOutline, p.colorAltered);
				else if (cPart.colorSetType == ColorSetType::FILL_NO_OUTLINE)
					p.imgAltered = recolorPixmapSolid(p.imgBase, p.colorAltered);
				cPart.item.get()->setPixmap(cPart.partsList[cPart.displayedPartI].imgAltered);
			}
		}
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

			for (const auto& cPart : characterPartList)
			{
				qStream << cPart.partTypeAssetStr +
					"=" + cPart.partsList[cPart.displayedPartI].imgFilename +
					"," + cPart.partsList[cPart.displayedPartI].colorAltered.name() + "\r\n";
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

int GraphicsDisplay::findPosOfFilenameInPartsList(const characterPart &cPart, const QString &filename)
{
	for (int i = 0; i < cPart.partsList.size(); i++)
	{
		if (cPart.partsList[i].imgFilename == filename)
		{
			return i;
		}
	}
	return -1;
}

void GraphicsDisplay::setBackgroundColor(const QColor &color)
{
	backgroundColor = color;
	this->setStyleSheet(styleSheetEditable.arg(backgroundColor.name()));
	characterModified = true;
}