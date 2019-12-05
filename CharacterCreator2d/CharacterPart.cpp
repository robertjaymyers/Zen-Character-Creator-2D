#include "CharacterPart.h"

CharacterPart::CharacterPart(QGraphicsItem* parent)
	: QGraphicsPixmapItem(parent)
{
	
}

// public:

void CharacterPart::init(PartType partType)
{
	// String passed to fileGetAssets function should correspond to a subdirectory
	// found under Assets in the application directory path (e.g. AppPath/Assets/Head)
	if (partType == PartType::HEAD)
		this->populatePartsList(fileGetAssets("Head"));
	else if (partType == PartType::CHEST)
		this->populatePartsList(fileGetAssets("Chest"));
	else if (partType == PartType::BOTTOM)
		this->populatePartsList(fileGetAssets("Bottom"));
}

void CharacterPart::moveLeftInDisplay()
{
	if (currentPartsListIndex - 1 >= 0)
	{
		currentPartsListIndex--;
		this->setPixmap(partsList[currentPartsListIndex].first);
	}
	else
	{
		currentPartsListIndex = partsList.size() - 1;
		this->setPixmap(partsList[currentPartsListIndex].first);
	}
	//qDebug() << "Current index is: " + QString::number(currentPartsListIndex);
}

void CharacterPart::moveRightInDisplay()
{
	if (currentPartsListIndex + 1 <= partsList.size() - 1)
	{
		currentPartsListIndex++;
		this->setPixmap(partsList[currentPartsListIndex].first);
	}
	else
	{
		currentPartsListIndex = 0;
		this->setPixmap(partsList[currentPartsListIndex].first);
	}
	//qDebug() << "Current index is: " + QString::number(currentPartsListIndex);
}

bool CharacterPart::setUrlAsCurrent(const QString url)
{
	for (int i = 0; i < partsList.size(); i++)
	{
		if (partsList[i].second == url)
		{
			currentPartsListIndex = i;
			this->setPixmap(partsList[currentPartsListIndex].first);
			return true;
		}
	}
	return false;
}

void CharacterPart::setCurrentToDefault()
{
	currentPartsListIndex = 0;
	this->setPixmap(partsList[currentPartsListIndex].first);
}

QString CharacterPart::getUrlOfDisplayed()
{
	// Parts list stores as pair of Pixmap and QString(filepath), so
	// if you reorder parts list for some reason, reorder by pair, not
	// by contents of pair, to avoid messing up pairing of filepaths to Pixmaps.
	// We need them to match, so that creations can be saved/loaded by asset filepath.
	return partsList[currentPartsListIndex].second;
}

// private:

void CharacterPart::populatePartsList(QStringList newParts)
{
	// Note: Url is stored as filename only (e.g. NOT including full path), 
	// so that if exe moves, character saves can still be loaded correctly in relation to loaded assets.
	for (auto& part : newParts)
	{
		partsList.push_back(std::pair<QPixmap, QString>(QPixmap(part), QFileInfo(part).fileName()));
	}
	if (!partsList.empty())
		this->setPixmap(partsList[0].first);
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