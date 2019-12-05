#pragma once
#include <QGraphicsPixmapItem>
#include <QDirIterator>
#include <QFileInfo>
#include <QCoreApplication>
#include <vector>

enum class PartType { HEAD, CHEST, BOTTOM, NONE };

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