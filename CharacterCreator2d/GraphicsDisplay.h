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
#include <QInputDialog>
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
	GraphicsDisplay(QWidget *parent = nullptr);
	bool fileSaveModifCheck();

protected:
	void contextMenuEvent(QContextMenuEvent *event) override;

private:
	QString fileDirLastOpened = QCoreApplication::applicationDirPath() + "/Saves";
	QString fileDirLastSaved = QCoreApplication::applicationDirPath() + "/Saves";
	QString fileDirLastRendered = QCoreApplication::applicationDirPath() + "/Renders";
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

	const std::unique_ptr<QMenu> contextMenu = std::make_unique<QMenu>();
	const std::unique_ptr<QAction> actionFileNew = std::make_unique<QAction>("New Character");
	const std::unique_ptr<QAction> actionFileOpen = std::make_unique<QAction>("Open Character");
	const std::unique_ptr<QAction> actionFileSave = std::make_unique<QAction>("Save Character");
	const std::unique_ptr<QAction> actionFileRender = std::make_unique<QAction>("Render Character");
	const std::unique_ptr<QAction> actionSetBackgroundColor = std::make_unique<QAction>("Set Background Color");
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

	const QString appExecutablePath = QCoreApplication::applicationDirPath();

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

	const QPixmap imgError = QPixmap(":/ZenCharacterCreator2D/Resources/error.png");

	// private functions:
	QString extractSubstringInbetweenQt(const QString strBegin, const QString strEnd, const QString &strExtractFrom);
	QString extractSubstringInbetweenRevFind(const QString strBegin, const QString strEnd, const QString &strExtractFrom);
	QStringList extractSubstringInbetweenLoopList(const QString strBegin, const QString strEnd, const QString &strExtractFrom);
	QStringList fileGetAssetDirectories(const QString &subPath);
	QStringList fileGetAssets(const QString &path);
	void updatePartInScene(const componentData &component, const assetsData &asset);
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
	void removeCurrentSpeciesFromScene();
	void applyCurrentSpeciesToScene();
};