#include "CharacterPart.h"

CharacterPart::CharacterPart(QGraphicsItem* parent)
	: QGraphicsPixmapItem(parent)
{
	
}

// public:

void CharacterPart::init(PartType partType, const QColor color, ColorSet colorSet)
{
	partTypeUnique = partType;
	partTypeAssetStr = partTypeMap.at(partType);
	this->populatePartsList(fileGetAssets(partTypeMap.at(partType)), color, colorSet);
}

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
	//qDebug() << "Current index is: " + QString::number(currentPartsListIndex);
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
	//qDebug() << "Current index is: " + QString::number(currentPartsListIndex);
}

bool CharacterPart::setUrlAsCurrent(const QString url)
{
	for (int i = 0; i < partsList.size(); i++)
	{
		if (partsList[i].imgUrl == url)
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
		applyColorFillDefault(partsList[currentPartsListIndex]);
	}
	else if (colorSet == ColorSet::MULTIPLY)
	{
		this->setPixmap(partsList[currentPartsListIndex].imgAltered);
		setColorToScene(partsList[currentPartsListIndex].defaultColor, colorSet);
	}
	else if (colorSet == ColorSet::NONE)
	{
		this->setPixmap(partsList[currentPartsListIndex].img);
	}
}

QString CharacterPart::getUrlOfDisplayed()
{
	// Parts list stores as pair of Pixmap and QString(filepath), so
	// if you reorder parts list for some reason, reorder by pair, not
	// by contents of pair, to avoid messing up pairing of filepaths to Pixmaps.
	// We need them to match, so that creations can be saved/loaded by asset filepath.
	return partsList[currentPartsListIndex].imgUrl;
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
		QPixmap newImage = partsList[currentPartsListIndex].img;
		QPainter painter(&newImage);
		painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
		painter.fillRect(newImage.rect(), partsList[currentPartsListIndex].currentColor);
		painter.end();
		this->setPixmap(newImage);
	}
	else if (colorSet == ColorSet::MULTIPLY)
	{
		// We can make all clothes have a white "fill" color normally
		// (through image editing, not through code)
		// With them all having white "fill" and black outline,
		// multiply should change their "fill" color accurately and completely,
		// without affecting the outline.
		// Another option would be to store 2 versions of a given piece of clothing:
		// one regular, one with white fill
		// then the one with white fill gets called on for the overlay effect
		partsList[currentPartsListIndex].currentColor = newColor;
		QPixmap newImage = partsList[currentPartsListIndex].img;
		QPixmap mask(newImage);

		QPainter painter;
		painter.begin(&mask);
		painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
		painter.fillRect(newImage.rect(), partsList[currentPartsListIndex].currentColor);
		painter.end();

		painter.begin(&newImage);
		painter.setCompositionMode(QPainter::CompositionMode_Multiply);
		painter.drawPixmap(newImage.rect(), mask);
		painter.end();

		this->setPixmap(newImage);
		partsList[currentPartsListIndex].imgAltered = newImage;
	}
}

// private:

void CharacterPart::populatePartsList(QStringList newParts, const QColor color, ColorSet colorSet)
{
	// Note: Url is stored as filename only (e.g. NOT including full path), 
	// so that if exe moves, character saves can still be loaded correctly in relation to loaded assets.
	for (auto& part : newParts)
	{
		imgParts newImgParts;
		newImgParts.img = QPixmap(part);
		newImgParts.imgAltered = newImgParts.img;
		newImgParts.imgUrl = QFileInfo(part).fileName();
		newImgParts.currentColor = color;
		newImgParts.defaultColor = color;

		partsList.push_back(newImgParts);
		//partsList.push_back(std::pair<QPixmap, QString>(QPixmap(part), QFileInfo(part).fileName()));
	}
	if (!partsList.empty())
	{
		for (auto& part : partsList)
		{
			if (colorSet == ColorSet::FILL)
			{
				applyColorFillDefault(part);
			}
			else if (colorSet == ColorSet::MULTIPLY)
			{
				QPixmap newImage = part.img;
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

				//this->setPixmap(newImage);
				part.imgAltered = newImage;
				this->setPixmap(partsList[0].imgAltered);
			}
			else if (colorSet == ColorSet::NONE)
				this->setPixmap(partsList[0].img);
		}
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

void CharacterPart::applyColorFillDefault(imgParts &part)
{
	this->setPixmap(part.img);
	QPixmap newImage = part.img;
	QPainter painter;
	painter.begin(&newImage);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.fillRect(newImage.rect(), part.defaultColor);
	painter.end();
	this->setPixmap(newImage);
}