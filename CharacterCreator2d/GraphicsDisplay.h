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
#include "CharacterPart.h"
#include "PartSwapButton.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QPushButton>
#include <QGridLayout>
#include <QDebug>
#include <QFileDialog>
#include <QShortcut>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QColorDialog>
#include <vector>

class GraphicsDisplay : public QGraphicsView
{
	Q_OBJECT

public:
	GraphicsDisplay(QWidget *parent = NULL);
	bool fileSaveModifCheck();

protected:
	void contextMenuEvent(QContextMenuEvent *event) override;

private:
	QString fileDirLastOpened = QCoreApplication::applicationDirPath() + "/Saves";
	QString fileDirLastSaved = QCoreApplication::applicationDirPath() + "/Saves";
	QString fileDirLastExported = QCoreApplication::applicationDirPath() + "/Exports";
	bool characterModified = false;
	QString styleSheetEditable = "border: none; background-color: %1";
	const QColor backgroundColorDefault = QColor("#FFFFFF");
	QColor backgroundColor = backgroundColorDefault;

	std::unique_ptr<QMenu> contextMenu = std::make_unique<QMenu>();
	std::unique_ptr<QAction> actionFileNew = std::make_unique<QAction>("New Character");
	std::unique_ptr<QAction> actionFileOpen = std::make_unique<QAction>("Open Character");
	std::unique_ptr<QAction> actionFileSave = std::make_unique<QAction>("Save Character");
	std::unique_ptr<QAction> actionFileExport = std::make_unique<QAction>("Export Character");
	std::unique_ptr<QAction> actionSetBackgroundColor = std::make_unique<QAction>("Set Background Color");

	std::unique_ptr<QGridLayout> layout = std::make_unique<QGridLayout>();
	std::unique_ptr<QGraphicsScene> scene = std::make_unique<QGraphicsScene>();
	std::unique_ptr<CharacterPart> characterSkinColor = std::make_unique<CharacterPart>(nullptr, PartType::SKIN_COLOR, QColor("#764c39"), ColorSetType::FILL_WITH_OUTLINE);
	std::unique_ptr<CharacterPart> characterEyeColor = std::make_unique<CharacterPart>(nullptr, PartType::EYE_COLOR, QColor("#aaaa7f"), ColorSetType::FILL_WITH_OUTLINE);
	std::unique_ptr<CharacterPart> characterLipColor = std::make_unique<CharacterPart>(nullptr, PartType::LIP_COLOR, QColor("#555500"), ColorSetType::FILL_WITH_OUTLINE);
	std::unique_ptr<CharacterPart> characterBlushColor = std::make_unique<CharacterPart>(nullptr, PartType::BLUSH_COLOR, QColor("#555500"), ColorSetType::FILL_NO_OUTLINE);
	std::unique_ptr<CharacterPart> characterHead = std::make_unique<CharacterPart>(nullptr, PartType::HEAD, QColor("#FFFFFF"), ColorSetType::NONE);
	std::unique_ptr<CharacterPart> characterChest = std::make_unique<CharacterPart>(nullptr, PartType::CHEST, QColor("#B5B5B5"), ColorSetType::FILL_WITH_OUTLINE);
	std::unique_ptr<CharacterPart> characterBottom = std::make_unique<CharacterPart>(nullptr, PartType::BOTTOM, QColor("#B5B5B5"), ColorSetType::FILL_WITH_OUTLINE);
	std::unique_ptr<CharacterPart> characterFeet = std::make_unique<CharacterPart>(nullptr, PartType::FEET, QColor("#000000"), ColorSetType::FILL_WITH_OUTLINE);
	std::unique_ptr<CharacterPart> characterHair = std::make_unique<CharacterPart>(nullptr, PartType::HAIR, QColor("#000000"), ColorSetType::FILL_WITH_OUTLINE);

	const int characterViewWidth = 500;
	const int characterViewHeight = 550;
	std::unique_ptr<PartSwapButton> characterHeadBtnLeft = std::make_unique<PartSwapButton>(this, BtnIcon::SWAP_LEFT, BtnGeometry::SWAP);
	std::unique_ptr<PartSwapButton> characterHeadBtnRight = std::make_unique<PartSwapButton>(this, BtnIcon::SWAP_RIGHT, BtnGeometry::SWAP);
	std::unique_ptr<PartSwapButton> characterChestBtnLeft = std::make_unique<PartSwapButton>(this, BtnIcon::SWAP_LEFT, BtnGeometry::SWAP);
	std::unique_ptr<PartSwapButton> characterChestBtnRight = std::make_unique<PartSwapButton>(this, BtnIcon::SWAP_RIGHT, BtnGeometry::SWAP);
	std::unique_ptr<PartSwapButton> characterBottomBtnLeft = std::make_unique<PartSwapButton>(this, BtnIcon::SWAP_LEFT, BtnGeometry::SWAP);
	std::unique_ptr<PartSwapButton> characterBottomBtnRight = std::make_unique<PartSwapButton>(this, BtnIcon::SWAP_RIGHT, BtnGeometry::SWAP);
	std::unique_ptr<PartSwapButton> characterFeetBtnLeft = std::make_unique<PartSwapButton>(this, BtnIcon::SWAP_LEFT, BtnGeometry::SWAP);
	std::unique_ptr<PartSwapButton> characterFeetBtnRight = std::make_unique<PartSwapButton>(this, BtnIcon::SWAP_RIGHT, BtnGeometry::SWAP);
	std::unique_ptr<PartSwapButton> characterHairBtnLeft = std::make_unique<PartSwapButton>(this, BtnIcon::SWAP_LEFT, BtnGeometry::SWAP);
	std::unique_ptr<PartSwapButton> characterHairBtnRight = std::make_unique<PartSwapButton>(this, BtnIcon::SWAP_RIGHT, BtnGeometry::SWAP);
	
	std::unique_ptr<PartSwapButton> characterSkinBtnPicker = std::make_unique<PartSwapButton>(this, BtnIcon::COLOR_PICKER_SKIN, BtnGeometry::PICKER);
	std::unique_ptr<PartSwapButton> characterEyeBtnPicker = std::make_unique<PartSwapButton>(this, BtnIcon::COLOR_PICKER_EYE, BtnGeometry::PICKER);
	std::unique_ptr<PartSwapButton> characterLipBtnPicker = std::make_unique<PartSwapButton>(this, BtnIcon::COLOR_PICKER_LIP, BtnGeometry::PICKER);
	std::unique_ptr<PartSwapButton> characterBlushBtnPicker = std::make_unique<PartSwapButton>(this, BtnIcon::COLOR_PICKER_BLUSH, BtnGeometry::PICKER);
	
	std::unique_ptr<PartSwapButton> characterChestBtnPicker = std::make_unique<PartSwapButton>(this, BtnIcon::COLOR_PICKER_CHEST, BtnGeometry::PICKER);
	std::unique_ptr<PartSwapButton> characterBottomBtnPicker = std::make_unique<PartSwapButton>(this, BtnIcon::COLOR_PICKER_BOTTOM, BtnGeometry::PICKER);
	std::unique_ptr<PartSwapButton> characterFeetBtnPicker = std::make_unique<PartSwapButton>(this, BtnIcon::COLOR_PICKER_FEET, BtnGeometry::PICKER);
	std::unique_ptr<PartSwapButton> characterHairBtnPicker = std::make_unique<PartSwapButton>(this, BtnIcon::COLOR_PICKER_HAIR, BtnGeometry::PICKER);

	// These exist only to fill space and affect spacing of UI layout. They should be invisible and do nothing.
	std::unique_ptr<PartSwapButton> characterBtnSpacerPicker1 = std::make_unique<PartSwapButton>(this, BtnIcon::NONE, BtnGeometry::PICKER_SPACER);
	std::unique_ptr<PartSwapButton> characterBtnSpacerPicker2 = std::make_unique<PartSwapButton>(this, BtnIcon::NONE, BtnGeometry::PICKER_SPACER);
	std::unique_ptr<PartSwapButton> characterBtnSpacerPicker3 = std::make_unique<PartSwapButton>(this, BtnIcon::NONE, BtnGeometry::PICKER_SPACER);
	std::unique_ptr<PartSwapButton> characterBtnSpacerPicker4 = std::make_unique<PartSwapButton>(this, BtnIcon::NONE, BtnGeometry::PICKER_SPACER);

	// temporary shortcut for testing color change in image(s)
	//std::unique_ptr<QShortcut> shortcutChangeColor = std::make_unique<QShortcut>(QKeySequence(tr("E", "Change Color")), this);

	QString extractSubstringInbetweenQt(const QString strBegin, const QString strEnd, const QString &strExtractFrom);
	void fileNew();
	void fileOpen();
	bool fileSave();
	void fileExportCombination();
	void setBackgroundColor(const QColor &color);
};