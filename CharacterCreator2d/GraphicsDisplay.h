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
#include <QHBoxLayout>
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
#include <QInputDialog>
#include <QLineEdit>
#include <QDateTime>
#include <QMessageBox>
#include <QGroupBox>
#include <QScrollArea>
#include <QScrollBar>
#include <QShortcut>
#include <QTimer>
#include <QSound>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <vector>
#include <algorithm>
#include <iterator>
#include <map>

// Setting for painting recolored image as whole, or as multicolored parts.
enum class PaintType { SINGLE, COMBINED };

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

class GraphicsDisplay : public QGraphicsView
{
	Q_OBJECT

public:
	GraphicsDisplay(QWidget *parent = nullptr, int width = 800, int height = 600);
	bool fileSaveModifCheck();
	//std::unique_ptr<QShortcut> fullscreenShortcutExit = std::make_unique<QShortcut>(QKeySequence(tr("ESC", "Exit Fullscreen")), this);
	//std::unique_ptr<QPushButton> fullscreenBtn = std::make_unique<QPushButton>(this);

protected:
	void contextMenuEvent(QContextMenuEvent *event) override;
	void resizeEvent(QResizeEvent *event);

private:
	const QString appExecutablePath = QCoreApplication::applicationDirPath();
	QString fileDirLastOpened = appExecutablePath + "/Saves";
	QString fileDirLastSaved = appExecutablePath + "/Saves";
	QString fileDirLastRendered = appExecutablePath + "/Renders";
	QString fileDirLastOpenedImage = appExecutablePath + "/Backgrounds";
	bool characterModified = false;
	QString styleSheetEditable = "border: none; background-color: %1;";
	const QColor backgroundColorDefault = QColor("#FFFFFF");
	QColor backgroundColor = backgroundColorDefault;

	bool soundEnabled = true;
	const QString soundEffectsPath = appExecutablePath + "/SoundEffects";
	const QString soundEffectComponentSwap = soundEffectsPath + "/componentSwap.wav";
	const QString soundEffectAssetSwap = soundEffectsPath + "/assetSwap.wav";
	const QString soundEffectPickColor = soundEffectsPath + "/pickColor.wav";

	const QString soundtrackPath = appExecutablePath + "/Soundtrack";
	std::unique_ptr<QMediaPlayer> soundtrackPlayer = std::make_unique<QMediaPlayer>();
	std::unique_ptr<QMediaPlaylist> soundtrackPlaylist = std::make_unique<QMediaPlaylist>();

	std::unique_ptr<QGridLayout> layout = std::make_unique<QGridLayout>();
	std::unique_ptr<QGraphicsScene> scene = std::make_unique<QGraphicsScene>();

	const QString backgroundImageDefault = ":/ZenCharacterCreator2D/Resources/invisible.png";
	QString backgroundImage = backgroundImageDefault;
	std::unique_ptr<QGraphicsPixmapItem> backgroundImageItem = std::make_unique<QGraphicsPixmapItem>(nullptr);
	const int backgroundImageItemZValue = 0;

	std::unique_ptr<QScrollArea> partSwapScroll = std::make_unique<QScrollArea>(this);
	std::unique_ptr<QGroupBox> partSwapGroup = std::make_unique<QGroupBox>(this);
	std::unique_ptr<QGridLayout> partSwapGroupLayout = std::make_unique<QGridLayout>();

	std::unique_ptr<QScrollArea> partPickerScroll = std::make_unique<QScrollArea>(this);
	std::unique_ptr<QGroupBox> partPickerGroup = std::make_unique<QGroupBox>(this);
	std::unique_ptr<QGridLayout> partPickerGroupLayout = std::make_unique<QGridLayout>();

	std::unique_ptr<QGroupBox> characterNameInputGroup = std::make_unique<QGroupBox>(this);
	std::unique_ptr<QGridLayout> characterNameInputGroupLayout = std::make_unique<QGridLayout>();

	std::unique_ptr<QGroupBox> utilityBtnGroup = std::make_unique<QGroupBox>(this);
	std::unique_ptr<QHBoxLayout> utilityBtnGroupLayout = std::make_unique<QHBoxLayout>();

	std::unique_ptr<QPushButton> utilityBtnVolume = std::make_unique<QPushButton>(this);
	const QIcon utilityBtnVolumeIcon = QIcon(":/ZenCharacterCreator2D/Resources/utilityIconVolume.png");
	const QIcon utilityBtnVolumeMutedIcon = QIcon(":/ZenCharacterCreator2D/Resources/utilityIconVolumeMuted.png");

	std::unique_ptr<QPushButton> utilityBtnExit = std::make_unique<QPushButton>(this);
	const QString utilityBtnStyle = 
	{
		"QPushButton"
		"{"
			"color: #000000;"
			"background-color: #FFFFFF;"
			"border-width: 1px;"
			"border-style: solid;"
			"border-color: #000000;"
			"border-radius: 4px;"
			"font-size: 12px;"
			"padding: 5px;"
		"}"
		"QPushButton:hover"
		"{"
			"color: #000000;"
			"background-color: #E5884E;"
			"border-width: 1px;"
			"border-style: solid;"
			"border-color: #000000;"
			"border-radius: 4px;"
			"font-size: 12px;"
			"padding: 5px;"
		"}"
	};

	const std::unique_ptr<QMenu> contextMenu = std::make_unique<QMenu>();
	const std::unique_ptr<QAction> actionFileNew = std::make_unique<QAction>("New Character");
	const std::unique_ptr<QAction> actionFileOpen = std::make_unique<QAction>("Open Character");
	const std::unique_ptr<QAction> actionFileSave = std::make_unique<QAction>("Save Character");
	const std::unique_ptr<QAction> actionFileRender = std::make_unique<QAction>("Render Character");
	const std::unique_ptr<QAction> actionSetBackgroundColor = std::make_unique<QAction>("Set Background Color");
	const std::unique_ptr<QAction> actionSetBackgroundImage = std::make_unique<QAction>("Set Background Image");
	const std::unique_ptr<QAction> actionClearBackgroundImage = std::make_unique<QAction>("Clear Background Image");
	std::unique_ptr<QMenu> speciesMenu = std::make_unique<QMenu>("Species", contextMenu.get());
	std::unique_ptr<QActionGroup> actionSpeciesGroup = std::make_unique<QActionGroup>(this);
	std::unique_ptr<QMenu> genderMenu = std::make_unique<QMenu>("Gender", contextMenu.get());
	std::unique_ptr<QActionGroup> actionGenderGroup = std::make_unique<QActionGroup>(this);
	std::unique_ptr<QMenu> poseMenu = std::make_unique<QMenu>("Pose", contextMenu.get());
	std::unique_ptr<QActionGroup> actionPoseGroup = std::make_unique<QActionGroup>(this);

	std::unique_ptr<QMenu> colorChangeSettingsMenu = std::make_unique<QMenu>("Color Change Settings", contextMenu.get());
	std::unique_ptr<QActionGroup> actioColorChangeSettingsGroup = std::make_unique<QActionGroup>(this);
	std::unique_ptr<QAction> actionColorChangeSettingsApplyToAllOnPicker = std::make_unique<QAction>("Apply Color Change To All In Set");
	std::unique_ptr<QAction> actionColorChangeSettingsDontApplyToAllOnPicker = std::make_unique<QAction>("Apply Color Change To Current Item Only");

	std::map<SpeciesType, speciesData> speciesMap;
	SpeciesType speciesCurrent = SpeciesType::HUMAN;
	GenderType genderCurrent = GenderType::FEMALE;
	PoseType poseCurrent = PoseType::FRONT_FACING;
	ComponentType componentCurrent = ComponentType::NONE;

	// We can support multiple naming conventions for files here.
	// Ex: If asset folder is called "tShirt", camel or pascal case will work.
	// If asset folder is called "t_shirt", snake case will work.
	const QString imgExtensionStandard = ".png";
	enum class AssetImgType { FILL, OUTLINE, THUMBNAIL };
	std::map<AssetImgType, QStringList> assetImgTypeMap =
	{
		{AssetImgType::FILL, QStringList{ "Fill", "_fill", "-fill" } },
		{AssetImgType::OUTLINE, QStringList{ "Outline", "_outline", "-outline" } },
		{AssetImgType::THUMBNAIL, QStringList{ "Thumbnail", "_thumbnail", "-thumbnail" } },
	};

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

	QColor pickerCopiedColor = QColor("#000000");
	const QPixmap pickerPasteColorIcon = QPixmap(":/ZenCharacterCreator2D/Resources/clipboardColorIcon.png");

	const QPixmap imgError = QPixmap(":/ZenCharacterCreator2D/Resources/error.png");
	const QString imgErrorPath = ":/ZenCharacterCreator2D/Resources/error.png";

	// private functions:
	QString extractSubstringInbetweenQt(const QString strBegin, const QString strEnd, const QString &strExtractFrom);
	QString extractSubstringInbetweenRevFind(const QString strBegin, const QString strEnd, const QString &strExtractFrom);
	QStringList extractSubstringInbetweenLoopList(const QString strBegin, const QString strEnd, const QString &strExtractFrom);
	QStringList fileGetAssetDirectoriesOnStartup(const QString &path);
	QStringList fileGetAssets(const QString &path);
	QString getPathIfExists(const QString &assetFolderPath, const AssetImgType &assetImgType);
	void updatePartInScene(const componentUiData &componentUi, const assetsData &asset);
	QPixmap recolorPixmapSolid(const QPixmap &img, const QColor &color);
	QPixmap recolorPixmapSolid(const assetsData &asset, const PaintType &paintType);
	QPixmap recolorPixmapSolidWithOutline(const assetsData &asset, const PaintType &paintType);
	void pickerUpdatePasteIconColor(const QColor &color);
	void loadDefaultCharacterFromTemplate();
	void fileLoadSavedCharacter(const QString &filePath);
	void fileNew();
	void fileOpen();
	bool fileSave();
	void fileRenderCharacter();
	void setBackgroundColor(const QColor &color);
	void setBackgroundImage(const QString &imgPath);
	void removeCurrentSpeciesFromScene();
	void applyCurrentSpeciesToScene();
	void setChosen(bool isChosen, assetsData &asset);
	void setChosen(bool isChosen, componentUiData &componentUi);
	void setCharacterModified(const bool newState);
	const QString getDropdownListItem(const QString &title, const QString &label, const QStringList &items, bool &ok);
	void applyDisplayOrder();
	speciesData& speciesCurrentSecond();
	genderData& genderCurrentSecond();
	poseData& poseCurrentSecond();
	componentUiData& componentUiCurrentSecond();
	componentData& componentCurrentSecond();
	assetsData& assetCurrentSecond();
};