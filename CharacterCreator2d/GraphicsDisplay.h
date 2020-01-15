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

struct uiBtnInvisibleSpacer
{
	int width;
	int height;
	QString style;
	std::vector<int> gridPlace;
	Qt::Alignment gridAlign;
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

	const QString appExecutablePath = QCoreApplication::applicationDirPath();

	struct characterPart
	{
		PartType partTypeUnique = PartType::NONE; // There should only be one part with each type.

		// Since we're working with 2D elements that can overlap, we use a display order
		// to ensure that the elements get added to the scene to overlap in the way we want.
		// For example, having HAIR added after CHEST so that long hair goes over clothing.
		int displayOrder;

		QString partTypeAssetStr; // The corresponding asset folder path string for this unique part (ex: "Head").
		QColor defaultInitialColor;
		ColorSetType colorSetType = ColorSetType::NONE;
		bool partHasBtnSwap;
		bool partHasBtnPicker;
		QStringList btnSwapLeftIcons;
		QStringList btnSwapRightIcons;
		QStringList btnPickerIcons;
		std::vector<int> gridPlacePicker;
		Qt::Alignment gridAlignPicker;
		std::vector<int> gridPlaceSwapLeft;
		Qt::Alignment gridAlignSwapLeft;
		std::vector<int> gridPlaceSwapRight;
		Qt::Alignment gridAlignSwapRight;
		int btnSwapWidth;
		int btnSwapHeight;
		int btnPickerWidth;
		int btnPickerHeight;

		std::unique_ptr<QGraphicsPixmapItem> item = std::make_unique<QGraphicsPixmapItem>(nullptr);
		std::unique_ptr<QPushButton> btnSwapLeft = std::make_unique<QPushButton>(nullptr);
		std::unique_ptr<QPushButton> btnSwapRight = std::make_unique<QPushButton>(nullptr);
		std::unique_ptr<QPushButton> btnPicker = std::make_unique<QPushButton>(nullptr);
		std::unique_ptr<QMenu> contextMenuColorPicker = std::make_unique<QMenu>();
		std::unique_ptr<QAction> actionCopyColor = std::make_unique<QAction>("Copy Color");
		std::unique_ptr<QAction> actionPasteColor = std::make_unique<QAction>("Paste Color");
		std::unique_ptr<QAction> actionApplyColorToAllInSet = std::make_unique<QAction>("Apply Current Color to All In Set");

		QString btnStyleSheetTemplate =
			"QPushButton{border: none; image: url(%1);}"
			"QPushButton:hover:!pressed{border: none; image: url(%2);}"
			"QPushButton:hover:pressed{border: none; image: url(%3);}"
			;

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
	};

	std::vector<characterPart> characterPartList;

	struct textInputSingleLine
	{
		TextInputSingleLineType inputType = TextInputSingleLineType::NONE;
		QString inputTypeStr;
		QString styleSheet;
		QString placeholderText;
		std::vector<int> gridPlace;
		Qt::Alignment gridAlign;
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
	static bool compareDisplayOrder(const characterPart &lhs, const characterPart &rhs);
	void pickerUpdatePasteIconColor(const QColor &color);
	void loadDefaultCharacterFromTemplate();
	void fileLoadSavedCharacter(const QString &filePath);
	void fileNew();
	void fileOpen();
	bool fileSave();
	void fileExportCharacter();
	int findPosOfFilenameInPartsList(const characterPart &cPart, const QString &filename);
	void setBackgroundColor(const QColor &color);
};