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
	bool setFilenameAssetAsDisplayed(const QString &filename);
	void setCurrentToDefault(const ColorSet colorSet);
	QString getFilenameOfDisplayed();
	QString getPartTypeAssetStr();

	QColor getCurrentColor();
	void setColorToScene(const QColor &newColor, ColorSet colorSet);

private:
	// We store both img and altered img, so that we can display altered img,
	// and apply color changes to base img, to get consistent results when altering color.
	// Otherwise, we'd be applying changes to a modified img, which can result in inconsistent color changes
	// over time, depending on the type of composition mode used with QPainter.
	struct imgParts 
	{ 
		QPixmap imgBase; // Don't modify this after initial loading into it from assets folder contents.
		QPixmap imgAltered; // Modify this one for color changes.
		QString imgFilename;
		QColor currentColor = QColor("#FFFFFF");
		QColor defaultColor = QColor("#FFFFFF");
	};
	std::vector<imgParts> partsList;
	int currentPartsListIndex = 0;
	const QString appExecutablePath = QCoreApplication::applicationDirPath();
	PartType partTypeUnique = PartType::NONE; // There should only be one part with each type.
	QString partTypeAssetStr; // The corresponding asset folder path string for this unique part (ex: "Head").

	void populatePartsList(QStringList newParts, const QColor color, ColorSet colorSet);
	QStringList fileGetAssets(const QString subPath);
	void applyColorFill(imgParts &part, const QColor &color, bool applyToScene);
	void applyColorMultiply(imgParts &part, bool applyToScene);
};