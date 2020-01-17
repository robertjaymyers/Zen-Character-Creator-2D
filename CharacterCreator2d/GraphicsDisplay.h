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
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGridLayout>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QCoreApplication>
#include <QGraphicsPixmapItem>
#include <QDirIterator>
#include <QPushButton>
#include <QDebug>
#include <QColorDialog>
#include <QPainter>
#include <QFileDialog>
#include <QLineEdit>
#include <QDateTime>
#include <QMessageBox>
#include <vector>
#include <algorithm>
#include <iterator>

enum class SpeciesType { HUMAN, ELF };

struct uiBtnInvisibleSpacer
{
	const int width;
	const int height;
	const QString style;
	const std::vector<int> gridPlace;
	const Qt::Alignment gridAlign;
	std::unique_ptr<QPushButton> spacerBtn = std::make_unique<QPushButton>(nullptr);
};

enum class TextInputSingleLineType { FIRST_NAME, LAST_NAME, NONE };

enum class ColorSetType { FILL_NO_OUTLINE, FILL_WITH_OUTLINE, NONE };

enum class PartType { SKIN_COLOR, EYE_COLOR, LIP_COLOR, BLUSH_COLOR, HEAD, CHEST, BOTTOM, FEET, HAIR, NONE };

class GraphicsDisplay : public QGraphicsView
{
	Q_OBJECT

public:
	GraphicsDisplay(QWidget *parent = nullptr);
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

	std::unique_ptr<QGridLayout> layout = std::make_unique<QGridLayout>();
	std::unique_ptr<QGraphicsScene> scene = std::make_unique<QGraphicsScene>();

	const std::unique_ptr<QMenu> contextMenu = std::make_unique<QMenu>();
	const std::unique_ptr<QAction> actionFileNew = std::make_unique<QAction>("New Character");
	const std::unique_ptr<QAction> actionFileOpen = std::make_unique<QAction>("Open Character");
	const std::unique_ptr<QAction> actionFileSave = std::make_unique<QAction>("Save Character");
	const std::unique_ptr<QAction> actionFileExport = std::make_unique<QAction>("Export Character");
	const std::unique_ptr<QAction> actionSetBackgroundColor = std::make_unique<QAction>("Set Background Color");
	std::unique_ptr<QMenu> speciesMenu = std::make_unique<QMenu>("Species", contextMenu.get());
	std::unique_ptr<QActionGroup> actionSpeciesGroup = std::make_unique<QActionGroup>(this);

	const QString appExecutablePath = QCoreApplication::applicationDirPath();

	struct characterPart
	{
		const PartType partTypeUnique = PartType::NONE; // There should only be one part with each type.

		// Since we're working with 2D elements that can overlap, we use a display order
		// to ensure that the elements get added to the scene to overlap in the way we want.
		// With Qt's QGraphicsScene class, we can set the Z value of an item to determine
		// what it's on top of (per Qt documentation: higher Z values overlap lower Z values).
		// For example, we give HAIR a higher value than CHEST so that long hair goes over shirts.
		const int displayOrder;

		const QString partTypeAssetStr; // The corresponding asset folder path string for this unique part (ex: "Head").
		const QColor defaultInitialColor;
		const ColorSetType colorSetType = ColorSetType::NONE;
		const bool partHasBtnSwap;
		const bool partHasBtnPicker;
		const QString btnStyleSheetTemplate;
		const QStringList btnSwapLeftIcons;
		const QStringList btnSwapRightIcons;
		const QStringList btnPickerIcons;
		const std::vector<int> gridPlacePicker;
		const Qt::Alignment gridAlignPicker;
		const std::vector<int> gridPlaceSwapLeft;
		const Qt::Alignment gridAlignSwapLeft;
		const std::vector<int> gridPlaceSwapRight;
		const Qt::Alignment gridAlignSwapRight;
		const int btnSwapWidth;
		const int btnSwapHeight;
		const int btnPickerWidth;
		const int btnPickerHeight;

		// We store both base img and altered img, so that we can display altered img,
		// and apply color changes to base img, to get consistent results when altering color.
		// Otherwise, we'd be applying changes to a modified img, which can result in inconsistent color changes
		// over time, depending on the type of composition mode used with QPainter.
		struct imgParts
		{
			QPixmap imgBase; // Don't modify this after initial loading into it from assets folder contents.
			QPixmap imgOutline; // Don't modify this after initial loading into it from assets folder contents.
			QPixmap imgAltered; // Modify this one for color changes.
			QString imgFilename;
			QColor colorBase; // Don't modify this after initial setting it from parts map
			QColor colorAltered; // Modify this one for color changes.
		};
		int displayedPartI = 0;
		std::vector<imgParts> partsList;

		std::unique_ptr<QGraphicsPixmapItem> item = std::make_unique<QGraphicsPixmapItem>(nullptr);
		std::unique_ptr<QPushButton> btnSwapLeft = std::make_unique<QPushButton>(nullptr);
		std::unique_ptr<QPushButton> btnSwapRight = std::make_unique<QPushButton>(nullptr);
		std::unique_ptr<QPushButton> btnPicker = std::make_unique<QPushButton>(nullptr);
		std::unique_ptr<QMenu> contextMenuColorPicker = std::make_unique<QMenu>();
		std::unique_ptr<QAction> actionCopyColor = std::make_unique<QAction>("Copy Color");
		std::unique_ptr<QAction> actionPasteColor = std::make_unique<QAction>("Paste Color");
		std::unique_ptr<QAction> actionApplyColorToAllInSet = std::make_unique<QAction>("Apply Current Color to All In Set");
	};

	std::vector<characterPart> characterPartListHuman;
	std::vector<characterPart> characterPartListElf;

	struct speciesData
	{
		const QString speciesTypeAssetStr;
		std::vector<characterPart> characterPartList;
		std::unique_ptr<QAction> actionSpecies = std::make_unique<QAction>();
	};

	std::map<SpeciesType, speciesData> speciesMap;
	SpeciesType speciesCurrent = SpeciesType::HUMAN;

	struct textInputSingleLine
	{
		const TextInputSingleLineType inputType = TextInputSingleLineType::NONE;
		const QString inputTypeStr;
		const QString styleSheet;
		const QString placeholderText;
		const std::vector<int> gridPlace;
		const Qt::Alignment gridAlign;
		std::unique_ptr<QLineEdit> inputWidget = std::make_unique<QLineEdit>();
	};

	std::vector<textInputSingleLine> textInputSingleLineList;

	const uiBtnInvisibleSpacer uiSpacerList[4] =
	{
		{
			75,
			75,
			"QPushButton"
			"{"
				"background-color: rgba(255, 255, 255, 0);"
				"border: none;"
				"image: url(:/ZenCharacterCreator2D/Resources/picker-spacer-transparent-background.png);"
			"}",
			{0, 2},
			Qt::AlignLeft | Qt::AlignTop
		},
		{
			75,
			75,
			"QPushButton"
			"{"
				"background-color: rgba(255, 255, 255, 0);"
				"border: none;"
				"image: url(:/ZenCharacterCreator2D/Resources/picker-spacer-transparent-background.png);"
			"}",
			{1, 2},
			Qt::AlignLeft | Qt::AlignTop
		},
		{
			75,
			75,
			"QPushButton"
			"{"
				"background-color: rgba(255, 255, 255, 0);"
				"border: none;"
				"image: url(:/ZenCharacterCreator2D/Resources/picker-spacer-transparent-background.png);"
			"}",
			{2, 2},
			Qt::AlignLeft | Qt::AlignTop
		},
		{
			75,
			75,
			"QPushButton"
			"{"
				"background-color: rgba(255, 255, 255, 0);"
				"border: none;"
				"image: url(:/ZenCharacterCreator2D/Resources/picker-spacer-transparent-background.png);"
			"}",
			{3, 2},
			Qt::AlignLeft | Qt::AlignTop
		},
	};

	QColor pickerCopiedColor = QColor("#000000");
	const QPixmap pickerPasteColorIcon = QPixmap(":/ZenCharacterCreator2D/Resources/clipboardColorIcon.png");

	QString extractSubstringInbetweenQt(const QString strBegin, const QString strEnd, const QString &strExtractFrom);
	QStringList fileGetAssets(const QString &subPath);
	QPixmap recolorPixmapSolid(const QPixmap &imgSolid, const QColor &color);
	QPixmap recolorPixmapSolidWithOutline(const QPixmap &imgSolid, const QPixmap &imgOutline, const QColor &color);
	void pickerUpdatePasteIconColor(const QColor &color);
	void loadDefaultCharacterFromTemplate();
	void fileLoadSavedCharacter(const QString &filePath);
	void fileNew();
	void fileOpen();
	bool fileSave();
	void fileExportCharacter();
	int findPosOfFilenameInPartsList(const characterPart &cPart, const QString &filename);
	void setBackgroundColor(const QColor &color);
	void removeCurrentSpeciesFromScene();
	void applyCurrentSpeciesToScene();
};