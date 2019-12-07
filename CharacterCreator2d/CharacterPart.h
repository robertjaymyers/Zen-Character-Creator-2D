#pragma once
#include <QGraphicsPixmapItem>
#include <QDirIterator>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDebug>
#include <vector>

#include <QPainter>

enum class ColorSet { FILL, MULTIPLY, NONE };

enum class PartType { SKIN_COLOR, EYE_COLOR, LIP_COLOR, BLUSH_COLOR, HEAD, CHEST, BOTTOM, FEET, HAIR, NONE };

// Companion string should correspond to a subdirectory
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
	CharacterPart(QGraphicsItem *parent = NULL);

	// Creates a list of pixmap parts for a given type from image assets in app Asset folder
	// and sets the first one in the list as the initial display part.
	void init(PartType partType, const QColor color, ColorSet colorSet);

	void moveLeftInDisplay();
	void moveRightInDisplay();
	bool setUrlAsCurrent(const QString url);
	void setCurrentToDefault(const ColorSet colorSet);
	QString getUrlOfDisplayed();
	QString getPartTypeAssetStr();

	QColor getCurrentColor();
	void setColorToScene(const QColor &newColor, ColorSet colorSet);

private:
	struct imgParts 
	{ 
		QPixmap img;
		QPixmap imgAltered;
		QString imgUrl;
		QColor currentColor = QColor("#FFFFFF");
		QColor defaultColor = QColor("#FFFFFF");
	};
	std::vector<imgParts> partsList;
	//std::vector<std::pair<QPixmap, QString>> partsList;
	int currentPartsListIndex = 0;
	const QString appExecutablePath = QCoreApplication::applicationDirPath();
	PartType partTypeUnique = PartType::NONE; // There should only be one part with each type.
	QString partTypeAssetStr; // The corresponding asset path string for this unique part.
	//QColor currentColor = QColor(255, 255, 255);

	void populatePartsList(QStringList newParts, const QColor color, ColorSet colorSet);
	QStringList fileGetAssets(const QString subPath);
	void applyColorFillDefault(imgParts &part);
};