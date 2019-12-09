#include "CharacterPart.h"

CharacterPart::CharacterPart(QGraphicsItem* parent, const PartType partType, const QColor color, const ColorSet colorSet)
	: QGraphicsPixmapItem(parent)
{
	partTypeUnique = partType;
	partTypeAssetStr = partTypeMap.at(partType);

	// Creates a list of pixmap parts for a given type from image assets in app Asset folder
	// and sets the first one in the list as the initial display part.
	this->populatePartsList(fileGetAssets(partTypeMap.at(partType)), color, colorSet);
}

// public:

void CharacterPart::moveLeftInDisplay()
{
	if (currentPartsListIndex - 1 >= 0)
	{
		currentPartsListIndex--;
		this->setPixmap(partsList[currentPartsListIndex].imgAltered);
	}
	else
	{
		currentPartsListIndex = partsList.size() - 1;
		this->setPixmap(partsList[currentPartsListIndex].imgAltered);
	}
}

void CharacterPart::moveRightInDisplay()
{
	if (currentPartsListIndex + 1 <= partsList.size() - 1)
	{
		currentPartsListIndex++;
		this->setPixmap(partsList[currentPartsListIndex].imgAltered);
	}
	else
	{
		currentPartsListIndex = 0;
		this->setPixmap(partsList[currentPartsListIndex].imgAltered);
	}
}

bool CharacterPart::setFilenameAssetAsDisplayed(const QString &filename)
{
	for (int i = 0; i < partsList.size(); i++)
	{
		if (partsList[i].imgFilename == filename)
		{
			currentPartsListIndex = i;
			this->setPixmap(partsList[currentPartsListIndex].imgAltered);
			return true;
		}
	}
	return false;
}

void CharacterPart::setCurrentToDefault(const ColorSet colorSet)
{
	currentPartsListIndex = 0;
	if (colorSet == ColorSet::FILL)
	{
		applyColorFill(partsList[currentPartsListIndex], partsList[currentPartsListIndex].defaultColor, true);
	}
	else if (colorSet == ColorSet::MULTIPLY)
	{
		this->setPixmap(partsList[currentPartsListIndex].imgAltered);
		setColorToScene(partsList[currentPartsListIndex].defaultColor, colorSet);
	}
	else if (colorSet == ColorSet::NONE)
	{
		this->setPixmap(partsList[currentPartsListIndex].imgAltered);
	}
}

QString CharacterPart::getFilenameOfDisplayed()
{
	// Parts list stores Pixmap and QString(filepath) together, so
	// if you reorder parts list for some reason, reorder by container, not
	// by contents of container, to avoid messing up pairing of filepaths to Pixmaps.
	// We need them to match, so that creations can be saved/loaded by asset filepath.
	return partsList[currentPartsListIndex].imgFilename;
}

QString CharacterPart::getPartTypeAssetStr()
{
	return partTypeAssetStr;
}

QColor CharacterPart::getCurrentColor()
{
	return partsList[currentPartsListIndex].currentColor;
}

void CharacterPart::setColorToScene(const QColor &newColor, ColorSet colorSet)
{
	if (colorSet == ColorSet::FILL)
	{
		partsList[currentPartsListIndex].currentColor = newColor;
		applyColorFill(partsList[currentPartsListIndex], partsList[currentPartsListIndex].currentColor, true);
	}
	else if (colorSet == ColorSet::MULTIPLY)
	{
		partsList[currentPartsListIndex].currentColor = newColor;
		applyColorMultiply(partsList[currentPartsListIndex], true);
	}
}

// private:

void CharacterPart::populatePartsList(QStringList newParts, const QColor color, ColorSet colorSet)
{
	// Note: We store as filename only (e.g. NOT including full path), 
	// so that if exe moves, character saves can still be loaded correctly in relation to loaded assets.
	for (auto& part : newParts)
	{
		imgParts newImgParts;
		newImgParts.imgBase = QPixmap(part);
		newImgParts.imgAltered = newImgParts.imgBase;
		newImgParts.imgFilename = QFileInfo(part).fileName();
		newImgParts.currentColor = color;
		newImgParts.defaultColor = color;

		partsList.push_back(newImgParts);
	}
	if (!partsList.empty())
	{
		for (auto& part : partsList)
		{
			if (colorSet == ColorSet::FILL)
			{
				applyColorFill(part, part.defaultColor, false);
			}
			else if (colorSet == ColorSet::MULTIPLY)
			{
				// We can make all clothes have a white "fill" color normally
				// (in asset creation, not as part of this program)
				// With them all having white "fill" and black outline,
				// multiply should change their "fill" color accurately and completely,
				// without affecting the outline, no matter the color chosen.
				// Default colors can be applied on load, so that clothes aren't all
				// loading as white clothing on a white background.
				applyColorMultiply(part, false);
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
		this->setPixmap(partsList[0].imgAltered);
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

void CharacterPart::applyColorFill(imgParts &part, const QColor &color, bool applyToScene)
{
	this->setPixmap(part.imgBase);
	QPixmap newImage = part.imgBase;
	QPainter painter;
	painter.begin(&newImage);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.fillRect(newImage.rect(), color);
	painter.end();

	part.imgAltered = newImage;
	if (applyToScene)
		this->setPixmap(part.imgAltered);
}

void CharacterPart::applyColorMultiply(imgParts &part, bool applyToScene)
{
	QPixmap newImage = part.imgBase;
	QPixmap mask(newImage);

	QPainter painter;
	painter.begin(&mask);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.fillRect(newImage.rect(), part.currentColor);
	painter.end();

	painter.begin(&newImage);
	painter.setCompositionMode(QPainter::CompositionMode_Multiply);
	painter.drawPixmap(newImage.rect(), mask);
	painter.end();

	part.imgAltered = newImage;
	if (applyToScene)
		this->setPixmap(part.imgAltered);
}