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

#include "CharacterPart.h"

CharacterPart::CharacterPart(QGraphicsItem* parent, const PartType partType, const QColor color, const ColorSetType colorSetType)
	: QGraphicsPixmapItem(parent)
{
	partTypeUnique = partType;
	partTypeAssetStr = partTypeMap.at(partType);
	partColorSetType = colorSetType;

	// Creates a list of pixmap parts for a given type from image assets in app Asset folder
	// and sets the first one in the list as the initial display part.
	this->populatePartsList(fileGetAssets(partTypeMap.at(partType)), color);
}

// public:

void CharacterPart::moveLeftInDisplay()
{
	if (displayedPartI - 1 >= 0)
	{
		displayedPartI--;
		this->setPixmap(partsList[displayedPartI].imgAltered);
	}
	else
	{
		displayedPartI = partsList.size() - 1;
		this->setPixmap(partsList[displayedPartI].imgAltered);
	}
}

void CharacterPart::moveRightInDisplay()
{
	if (displayedPartI + 1 <= partsList.size() - 1)
	{
		displayedPartI++;
		this->setPixmap(partsList[displayedPartI].imgAltered);
	}
	else
	{
		displayedPartI = 0;
		this->setPixmap(partsList[displayedPartI].imgAltered);
	}
}

bool CharacterPart::setFilenameAssetAsDisplayed(const QString &filename)
{
	for (int i = 0; i < partsList.size(); i++)
	{
		if (partsList[i].imgFilename == filename)
		{
			displayedPartI = i;
			this->setPixmap(partsList[displayedPartI].imgAltered);
			return true;
		}
	}
	return false;
}

void CharacterPart::setDisplayedToDefault()
{
	displayedPartI = 0;
	for (auto& part : partsList)
	{
		part.currentColor = part.defaultColor;
		if (partColorSetType == ColorSetType::FILL_NO_OUTLINE ||
			partColorSetType == ColorSetType::FILL_WITH_OUTLINE)
		{
			applyColorFill(part, false);
		}
	}
	this->setPixmap(partsList[displayedPartI].imgAltered);
}

QString CharacterPart::getFilenameOfDisplayed()
{
	// Parts list stores Pixmap and QString(filepath) together, so
	// if you reorder parts list for some reason, reorder by container, not
	// by contents of container, to avoid messing up pairing of filepaths to Pixmaps.
	// We need them to match, so that creations can be saved/loaded by asset filepath.
	return partsList[displayedPartI].imgFilename;
}

QString CharacterPart::getPartTypeAssetStr()
{
	return partTypeAssetStr;
}

QColor CharacterPart::getColorOfDisplayed()
{
	return partsList[displayedPartI].currentColor;
}

void CharacterPart::setColorToScene(const QColor &newColor)
{
	if (partColorSetType == ColorSetType::FILL_NO_OUTLINE ||
		partColorSetType == ColorSetType::FILL_WITH_OUTLINE)
	{
		partsList[displayedPartI].currentColor = newColor;
		applyColorFill(partsList[displayedPartI], true);
	}
}

void CharacterPart::applyCurrentColorToAll()
{
	if (partColorSetType == ColorSetType::FILL_NO_OUTLINE ||
		partColorSetType == ColorSetType::FILL_WITH_OUTLINE)
	{
		QColor current = partsList[displayedPartI].currentColor;
		for (auto& part : partsList)
		{
			part.currentColor = current;
			applyColorFill(part, false);
		}
	}
}

// private:

void CharacterPart::populatePartsList(QStringList newParts, const QColor color)
{
	// Note: We store as filename only (e.g. NOT including full path), 
	// so that if exe moves, character saves can still be loaded correctly in relation to loaded assets.
	for (auto& part : newParts)
	{
		imgParts newImgParts;
		newImgParts.imgBase = QPixmap(part);
		newImgParts.imgAltered = newImgParts.imgBase;
		newImgParts.imgFilename = QFileInfo(part).fileName();
		newImgParts.defaultColor = color;
		newImgParts.currentColor = newImgParts.defaultColor;

		if (partColorSetType == ColorSetType::FILL_WITH_OUTLINE)
			newImgParts.imgOutline = QPixmap(part.replace("/" + QFileInfo(part).fileName(), "/Outline/" + QFileInfo(part).fileName()));

		partsList.push_back(newImgParts);
	}
	if (!partsList.empty())
	{
		for (auto& part : partsList)
		{
			if (partColorSetType == ColorSetType::FILL_NO_OUTLINE ||
				partColorSetType == ColorSetType::FILL_WITH_OUTLINE)
			{
				applyColorFill(part, false);
			}
		}

		// For simplicity, we make the default the first part in the list.
		// (ex: the first Chest part in the list of Chest parts)
		// This means default will be defined by load order, based on how folder traversal works.
		// (so probably alphabetical)
		// Note that an instance of this class initialized with a color set type of none still uses
		// the altered image variable for display to avoid unnecessary complexity.
		// It gets its altered image set to the normal image at the beginning, so despite never
		// having color setting applied, it'll still have an image to display.
		this->setPixmap(partsList[displayedPartI].imgAltered);
	}
}

QStringList CharacterPart::fileGetAssets(const QString subPath)
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

void CharacterPart::applyColorFill(imgParts &part, bool applyToScene)
{
	QPixmap newImage = part.imgBase;
	QPainter painter;
	painter.begin(&newImage);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.fillRect(newImage.rect(), part.currentColor);
	if (partColorSetType == ColorSetType::FILL_WITH_OUTLINE)
	{
		painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
		painter.drawPixmap(part.imgOutline.rect(), part.imgOutline);
	}
	painter.end();

	part.imgAltered = newImage;
	if (applyToScene)
		this->setPixmap(part.imgAltered);
}