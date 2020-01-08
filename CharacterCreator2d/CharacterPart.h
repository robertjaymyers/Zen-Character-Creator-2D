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
#include <QGraphicsPixmapItem>
#include <QDirIterator>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDebug>
#include <QPainter>
#include <vector>

enum class ColorSetType { FILL_NO_OUTLINE, FILL_WITH_OUTLINE, NONE };

enum class PartType { SKIN_COLOR, EYE_COLOR, LIP_COLOR, BLUSH_COLOR, HEAD, CHEST, BOTTOM, FEET, HAIR, NONE };

// Paired string should correspond to a subdirectory
// found under Assets in the application directory path (e.g. AppPath/Assets/Head)
// We use this path to find assets from file for a particular character part.
// This map should only be modified along with the PartType enum class when adding or removing
// character parts to the design of the program as a whole.
// (ex: removing BOTTOM and adding PANTS and SHOES)
const std::map<PartType, QString> partTypeMap = 
{
	{PartType::SKIN_COLOR, "SkinColor"},
	{PartType::EYE_COLOR, "EyeColor"},
	{PartType::LIP_COLOR, "LipColor"},
	{PartType::BLUSH_COLOR, "BlushColor"},
	{PartType::HEAD, "Head"},
	{PartType::CHEST, "Chest"},
	{PartType::BOTTOM, "Bottom"},
	{PartType::FEET, "Feet"},
	{PartType::HAIR, "Hair"},
};

class CharacterPart : public QGraphicsPixmapItem
{
public:
	CharacterPart(QGraphicsItem *parent = nullptr, const PartType partType = PartType::NONE, const QColor color = QColor("#FFFFFF"), ColorSetType colorSetType = ColorSetType::NONE);

	void moveLeftInDisplay();
	void moveRightInDisplay();
	bool setFilenameAssetAsDisplayed(const QString &filename);
	void setDisplayedToDefault();
	QString getFilenameOfDisplayed();
	QString getPartTypeAssetStr();

	QColor getColorOfDisplayed();
	void setColorToScene(const QColor &newColor);
	void applyCurrentColorToAll();

private:
	// We store both img and altered img, so that we can display altered img,
	// and apply color changes to base img, to get consistent results when altering color.
	// Otherwise, we'd be applying changes to a modified img, which can result in inconsistent color changes
	// over time, depending on the type of composition mode used with QPainter.
	struct imgParts 
	{ 
		QPixmap imgBase; // Don't modify this after initial loading into it from assets folder contents.
		QPixmap imgAltered; // Modify this one for color changes.
		QPixmap imgOutline;
		QString imgFilename;
		QColor currentColor = QColor("#FFFFFF");
		QColor defaultColor = QColor("#FFFFFF");
	};
	std::vector<imgParts> partsList;
	int displayedPartI = 0;
	const QString appExecutablePath = QCoreApplication::applicationDirPath();
	PartType partTypeUnique = PartType::NONE; // There should only be one part with each type.
	QString partTypeAssetStr; // The corresponding asset folder path string for this unique part (ex: "Head").
	ColorSetType partColorSetType;

	void populatePartsList(QStringList newParts, const QColor color);
	QStringList fileGetAssets(const QString subPath);
	void applyColorFill(imgParts &part, bool applyToScene);
};