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
#include "theme.h"
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
#include <QGroupBox>
#include <QScrollArea>
#include <QScrollBar>
#include <vector>
#include <algorithm>
#include <iterator>
#include <map>

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

//enum class SpeciesType { HUMAN, ELF };
//enum class GenderType { FEMALE, MALE };
//enum class PartType { BODY, EYES, LIPS, BLUSH, HEAD, CHEST, BOTTOM, FEET, MASK, HAIR, NONE };
//enum class ColorSetType { FILL_NO_OUTLINE, FILL_WITH_OUTLINE, NONE };

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

	std::unique_ptr<QScrollArea> partSwapScroll = std::make_unique<QScrollArea>(this);
	std::unique_ptr<QGroupBox> partSwapGroup = std::make_unique<QGroupBox>(this);
	std::unique_ptr<QGridLayout> partSwapGroupLayout = std::make_unique<QGridLayout>();

	std::unique_ptr<QScrollArea> partPickerScroll = std::make_unique<QScrollArea>(this);
	std::unique_ptr<QGroupBox> partPickerGroup = std::make_unique<QGroupBox>(this);
	std::unique_ptr<QGridLayout> partPickerGroupLayout = std::make_unique<QGridLayout>();

	std::unique_ptr<QGroupBox> characterNameInputGroup = std::make_unique<QGroupBox>(this);
	std::unique_ptr<QGridLayout> characterNameInputGroupLayout = std::make_unique<QGridLayout>();

	/*std::unique_ptr<QGroupBox> poseSwapGroup = std::make_unique<QGroupBox>(this);
	std::unique_ptr<QGridLayout> poseSwapGroupLayout = std::make_unique<QGridLayout>();*/

	const std::unique_ptr<QMenu> contextMenu = std::make_unique<QMenu>();
	const std::unique_ptr<QAction> actionFileNew = std::make_unique<QAction>("New Character");
	const std::unique_ptr<QAction> actionFileOpen = std::make_unique<QAction>("Open Character");
	const std::unique_ptr<QAction> actionFileSave = std::make_unique<QAction>("Save Character");
	const std::unique_ptr<QAction> actionFileExport = std::make_unique<QAction>("Export Character");
	const std::unique_ptr<QAction> actionSetBackgroundColor = std::make_unique<QAction>("Set Background Color");
	std::unique_ptr<QMenu> speciesMenu = std::make_unique<QMenu>("Species", contextMenu.get());
	std::unique_ptr<QActionGroup> actionSpeciesGroup = std::make_unique<QActionGroup>(this);
	std::unique_ptr<QMenu> genderMenu = std::make_unique<QMenu>("Gender", contextMenu.get());
	std::unique_ptr<QActionGroup> actionGenderGroup = std::make_unique<QActionGroup>(this);
	std::unique_ptr<QMenu> poseMenu = std::make_unique<QMenu>("Pose", contextMenu.get());
	std::unique_ptr<QActionGroup> actionPoseGroup = std::make_unique<QActionGroup>(this);

	const QString appExecutablePath = QCoreApplication::applicationDirPath();

	//struct characterPart
	//{
	//	const PartType partTypeUnique = PartType::NONE; // There should only be one part with each type.

	//	// Since we're working with 2D elements that can overlap, we use a display order
	//	// to ensure that the elements get added to the scene to overlap in the way we want.
	//	// With Qt's QGraphicsScene class, we can set the Z value of an item to determine
	//	// what it's on top of (per Qt documentation: higher Z values overlap lower Z values).
	//	// For example, we give HAIR a higher value than CHEST so that long hair goes over shirts.
	//	const int displayOrder;

	//	const QString assetStr; // The corresponding asset folder path string for this unique part (ex: "Head").
	//	const QColor defaultInitialColor;
	//	const ColorSetType colorSetType = ColorSetType::NONE;
	//	const bool partHasBtnSwap;
	//	const bool partHasBtnPicker;
	//	const QString btnStyleSheetTemplate;
	//	const QStringList btnSwapLeftIcons;
	//	const QStringList btnSwapRightIcons;
	//	const QStringList btnPickerIcons;
	//	const std::vector<int> gridPlacePicker;
	//	const Qt::Alignment gridAlignPicker;
	//	const std::vector<int> gridPlaceSwapLeft;
	//	const Qt::Alignment gridAlignSwapLeft;
	//	const std::vector<int> gridPlaceSwapRight;
	//	const Qt::Alignment gridAlignSwapRight;
	//	const int btnSwapWidth;
	//	const int btnSwapHeight;
	//	const int btnPickerWidth;
	//	const int btnPickerHeight;

	//	struct imgPart
	//	{
	//		const QString imgBasePath; // Can be a fill img or outline, depending on whether asset is meant to be colorable.
	//		const QString imgFilename; // We use this for saving/loading by filename of an img part.
	//		const QColor colorDefault; // We revert to this when "resetting" with, for example, "new character."
	//		QColor colorAltered; // Color changes are stored here and applied to the scene on part swap.
	//		const QString imgOutlinePath; // Outline for related fill img (fill is always stored in base img if there is a fill).
	//		const QString imgThumbnailPath; // The thumbnail for showing the asset in the swap UI.
	//		std::unique_ptr<QPushButton> btnSwapTo = std::make_unique<QPushButton>(nullptr);
	//	};
	//	QString displayedPartKey;
	//	//int displayedPartI = 0;

	//	struct poseData
	//	{
	//		const QString assetStr;
	//		std::map<QString, imgPart> partsMap;
	//		//std::vector<imgPart> partsList;
	//	};

	//	struct genderData
	//	{
	//		const QString assetStr;
	//		std::vector<poseData> poseList;
	//		std::unique_ptr<QAction> actionGender = std::make_unique<QAction>();
	//	};

	//	std::map<GenderType, genderData> genderMap;

	//	std::unique_ptr<QGraphicsPixmapItem> item = std::make_unique<QGraphicsPixmapItem>(nullptr);
	//	std::unique_ptr<QPushButton> btnSwapCharacterPart = std::make_unique<QPushButton>(nullptr);
	//	std::unique_ptr<QPushButton> btnPicker = std::make_unique<QPushButton>(nullptr);
	//	std::unique_ptr<QMenu> contextMenuColorPicker = std::make_unique<QMenu>();
	//	std::unique_ptr<QAction> actionCopyColor = std::make_unique<QAction>("Copy Color");
	//	std::unique_ptr<QAction> actionPasteColor = std::make_unique<QAction>("Paste Color");
	//	std::unique_ptr<QAction> actionApplyColorToAllInSet = std::make_unique<QAction>("Apply Current Color to All In Set");
	//};

	//std::vector<characterPart> characterPartListHuman;
	//std::vector<characterPart> characterPartListElf;

	//struct speciesData
	//{
	//	const QString assetStr;
	//	std::vector<characterPart> characterPartList;
	//	std::unique_ptr<QAction> actionSpecies = std::make_unique<QAction>();
	//};

	// MAKING NEW NESTED DESIGN

	//struct assetsData
	//{
	//	QString imgBasePath; // Can be a fill img or outline, depending on whether asset is meant to be colorable.
	//	QString imgFilename; // We use this for saving/loading by filename of an img part.
	//	QColor colorDefault; // We revert to this when "resetting" with, for example, "new character."
	//	QColor colorAltered; // Color changes are stored here and applied to the scene on part swap.
	//	QString imgOutlinePath; // Outline for related fill img (fill is always stored in base img if there is a fill).
	//	QString imgThumbnailPath; // The thumbnail for showing the asset in the swap UI.
	//	std::unique_ptr<QPushButton> btnSwapAsset = std::make_unique<QPushButton>(nullptr);
	//};

	//struct componentData
	//{
	//	componentDataInit settings;
	//	std::map<QString, assetsData> assetsMap;
	//	QString displayedAssetKey;
	//	std::unique_ptr<QGraphicsPixmapItem> item = std::make_unique<QGraphicsPixmapItem>(nullptr);
	//	std::unique_ptr<QPushButton> btnSwapComponent = std::make_unique<QPushButton>(nullptr);
	//	std::unique_ptr<QPushButton> btnPicker = std::make_unique<QPushButton>(nullptr);
	//	std::unique_ptr<QMenu> contextMenuColorPicker = std::make_unique<QMenu>();
	//	std::unique_ptr<QAction> actionCopyColor = std::make_unique<QAction>("Copy Color");
	//	std::unique_ptr<QAction> actionPasteColor = std::make_unique<QAction>("Paste Color");
	//	std::unique_ptr<QAction> actionApplyColorToAllInSet = std::make_unique<QAction>("Apply Current Color to All In Set");
	//};

	//struct poseData
	//{
	//	QString assetStr;
	//	std::map<ComponentType, componentData> componentMap;
	//	std::unique_ptr<QAction> actionPose = std::make_unique<QAction>();
	//};

	//struct genderData
	//{
	//	QString assetStr;
	//	std::map<PoseType, poseData> poseMap;
	//	std::unique_ptr<QAction> actionGender = std::make_unique<QAction>();
	//};

	//struct speciesData
	//{
	//	QString assetStr;
	//	std::map<GenderType, genderData> genderMap;
	//	std::unique_ptr<QAction> actionSpecies = std::make_unique<QAction>();
	//};

	// ---------------------------

	std::map<SpeciesType, speciesData> speciesMap;
	SpeciesType speciesCurrent = SpeciesType::HUMAN;
	GenderType genderCurrent = GenderType::FEMALE;
	PoseType poseCurrent = PoseType::FRONT_FACING;
	ComponentType componentCurrent = ComponentType::NONE;

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

	//struct poseSwapButton
	//{
	//	const QString styleSheetTemplate;
	//	const QStringList icons;
	//	const std::vector<int> gridPlace;
	//	const Qt::Alignment gridAlign;
	//	const int width;
	//	const int height;
	//	std::unique_ptr<QPushButton> btn = std::make_unique<QPushButton>(nullptr);
	//};

	//const poseSwapButton poseSwapButtonList[2] =
	//{
	//	{
	//		"QPushButton{border: none; image: url(%1);}"
	//		"QPushButton:hover:!pressed{border: none; image: url(%2);}"
	//		"QPushButton:hover:pressed{border: none; image: url(%3);}",
	//		QStringList()
	//		<< ":/ZenCharacterCreator2D/Resources/button-pose-swap-left-normal.png"
	//		<< ":/ZenCharacterCreator2D/Resources/button-pose-swap-left-hover.png"
	//		<< ":/ZenCharacterCreator2D/Resources/button-pose-swap-left-hover-pressed.png",
	//		{0, 0}, // Row/Col placement in grid layout
	//		Qt::AlignLeft | Qt::AlignBottom, // Alignment in grid layout
	//		50,
	//		50
	//	},
	//	{
	//		"QPushButton{border: none; image: url(%1);}"
	//		"QPushButton:hover:!pressed{border: none; image: url(%2);}"
	//		"QPushButton:hover:pressed{border: none; image: url(%3);}",
	//		QStringList()
	//		<< ":/ZenCharacterCreator2D/Resources/button-pose-swap-right-normal.png"
	//		<< ":/ZenCharacterCreator2D/Resources/button-pose-swap-right-hover.png"
	//		<< ":/ZenCharacterCreator2D/Resources/button-pose-swap-right-hover-pressed.png",
	//		{0, 1}, // Row/Col placement in grid layout
	//		Qt::AlignLeft | Qt::AlignBottom, // Alignment in grid layout
	//		50,
	//		50
	//	}
	//};

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
		}
	};

	QColor pickerCopiedColor = QColor("#000000");
	const QPixmap pickerPasteColorIcon = QPixmap(":/ZenCharacterCreator2D/Resources/clipboardColorIcon.png");

	QString extractSubstringInbetweenQt(const QString strBegin, const QString strEnd, const QString &strExtractFrom);
	QString extractSubstringInbetweenRevFind(const QString strBegin, const QString strEnd, const QString &strExtractFrom);
	QStringList fileGetAssetDirectories(const QString &subPath);
	QStringList fileGetAssets(const QString &path);
	void updatePartInScene(const componentData &component, const assetsData &asset);
	QPixmap recolorPixmapSolid(const QPixmap &img, const QColor &color);
	QPixmap recolorPixmapSolid(const assetsData &asset);
	QPixmap recolorPixmapSolidWithOutline(const assetsData &asset);
	void pickerUpdatePasteIconColor(const QColor &color);
	void loadDefaultCharacterFromTemplate();
	void fileLoadSavedCharacter(const QString &filePath);
	void fileNew();
	void fileOpen();
	bool fileSave();
	void fileExportCharacter();
	//int findPosOfFilenameInPartsList(const characterPart &cPart, const QString &filename);
	void setBackgroundColor(const QColor &color);
	void removeCurrentSpeciesFromScene();
	void applyCurrentSpeciesToScene();
};