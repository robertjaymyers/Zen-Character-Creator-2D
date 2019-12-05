#pragma once
#include <QGraphicsPixmapItem>
#include <QDirIterator>
#include <QFileInfo>
#include <QCoreApplication>
#include <vector>

enum class PartType { HEAD, CHEST, BOTTOM, NONE };

// Companion string should correspond to a subdirectory
// found under Assets in the application directory path (e.g. AppPath/Assets/Head)
// We use this path to find assets from file for a particular character part.
// This map should only be modified along with the PartType enum class when adding or removing
// character parts to the design of the program as a whole.
// (ex: removing BOTTOM and adding PANTS and SHOES)
const std::map<PartType, QString> PartTypeMap = 
{
	{PartType::HEAD, "Head"},
	{PartType::CHEST, "Chest"},
	{PartType::BOTTOM, "Bottom"}
};

class CharacterPart : public QGraphicsPixmapItem
{
public:
	CharacterPart(QGraphicsItem *parent = NULL);

	// Creates a list of pixmap parts for a given type from image assets in app Asset folder
	// and sets the first one in the list as the initial display part.
	void init(PartType partType);

	void moveLeftInDisplay();
	void moveRightInDisplay();
	bool setUrlAsCurrent(const QString url);
	void setCurrentToDefault();
	QString getUrlOfDisplayed();

private:
	std::vector<std::pair<QPixmap, QString>> partsList;
	int currentPartsListIndex = 0;
	const QString appExecutablePath = QCoreApplication::applicationDirPath();

	void populatePartsList(QStringList newParts);
	QStringList fileGetAssets(const QString subPath);
};