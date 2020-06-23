#pragma once
#include <map>
#include <vector>
#include <memory>
#include <QString>
#include <QColor>
#include <QStringList>
#include <QPushButton>
#include <QGraphicsPixmapItem>
#include <QMenu>
#include <QAction>

// We define creator theme properties here that can be defined without needing information from startup.
// For example, what are the possible species? We can define that here.
// Information like how many chest pieces there are requires reading files, so that isn't included here.

// The purpose of organizing this information here is to be able to easily locate the parts of the program 
// that can be customized for compile to the preference of the person using the creator for a specific game.
// For example, if you want a game with Human, Elf, and Orc, you'd make those species types.
// If you want customizable Head, Chest, Bottom, and Feet, you'd make those component types.

// In this way, you can make broad custom changes without requiring significant in-depth tinkering.

enum class SpeciesType { HUMAN, ELF };
enum class GenderType { FEMALE, MALE };
enum class PoseType { FRONT_FACING };
enum class ComponentType { BODY, EYES, LIPS, BLUSH, HEAD, CHEST, BOTTOM, FEET, MASK, HAIR, NONE };
enum class ColorSetType { FILL_NO_OUTLINE, FILL_WITH_OUTLINE, NONE };

struct assetsData
{
	const QString imgBasePath; // Can be a fill img or outline, depending on whether asset is meant to be colorable.
	const QString imgFilename; // We use this for saving/loading by filename of an img part.
	const QColor colorDefault; // We revert to this when "resetting" with, for example, "new character."
	QColor colorAltered; // Color changes are stored here and applied to the scene on part swap.
	const QString imgOutlinePath; // Outline for related fill img (fill is always stored in base img if there is a fill).
	const QString imgThumbnailPath; // The thumbnail for showing the asset in the swap UI.
	std::unique_ptr<QPushButton> btnSwapAsset = std::make_unique<QPushButton>(nullptr);
};

struct componentDataInit
{
	// Since we're working with 2D elements that can overlap, we use a display order
	// to ensure that the elements get added to the scene to overlap in the way we want.
	// For example, we give HAIR a higher value than CHEST so that long hair goes on top of shirts.
	const int displayOrderZ;

	const QString assetStr; // The corresponding asset folder path string for this unique part (ex: "Head").
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
};

struct componentData
{
	const componentDataInit settings;
	std::map<QString, assetsData> assetsMap;
	QString displayedAssetKey;
	std::unique_ptr<QGraphicsPixmapItem> item = std::make_unique<QGraphicsPixmapItem>(nullptr);
	std::unique_ptr<QPushButton> btnSwapComponent = std::make_unique<QPushButton>(nullptr);
	std::unique_ptr<QPushButton> btnPicker = std::make_unique<QPushButton>(nullptr);
	std::unique_ptr<QMenu> contextMenuColorPicker = std::make_unique<QMenu>();
	std::unique_ptr<QAction> actionCopyColor = std::make_unique<QAction>("Copy Color");
	std::unique_ptr<QAction> actionPasteColor = std::make_unique<QAction>("Paste Color");
	std::unique_ptr<QAction> actionApplyColorToAllInSet = std::make_unique<QAction>("Apply Current Color to All In Set");
};

struct poseData
{
	const QString assetStr;
	std::map<ComponentType, componentData> componentMap;
	std::unique_ptr<QAction> actionPose = std::make_unique<QAction>();
};

struct genderData
{
	const QString assetStr;
	std::map<PoseType, poseData> poseMap;
	std::unique_ptr<QAction> actionGender = std::make_unique<QAction>();
};

struct speciesData
{
	const QString assetStr;
	std::map<GenderType, genderData> genderMap;
	std::unique_ptr<QAction> actionSpecies = std::make_unique<QAction>();
};

struct mapInitData
{
	const QString assetStr;
	const std::map<ComponentType, componentDataInit>& componentMapRef;
};

const std::map<ComponentType, componentDataInit> componentTypeMapForHuman =
{
	{ComponentType::BODY,
	componentDataInit
	{
		0, // Display order in scene (higher numbers overlap lower numbers)
		"Body",
		"#764c39",
		ColorSetType::FILL_WITH_OUTLINE,
		false, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICKER BTN: Flag for whether part is expected to display btn
		"QPushButton{border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{border: none; image: url(%2);}"
		"QPushButton:hover:pressed{border: none; image: url(%3);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-pick-skin-color.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-skin-color-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-skin-color-hover-pressed.png",
		{0, 0}, // PICKER BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
		{-1, -1}, // SWAP LEFT BTN: Row/Col placement in grid layout
		nullptr, // PICKER BTN: Alignment in grid layout
		{-1, -1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
		nullptr, // PICKER BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICKER BTN: Width
		75, // PICKER BTN: Height
	}
	},
	{ComponentType::EYES,
	componentDataInit
	{
		1, // Display order in scene (higher numbers overlap lower numbers)
		"Eyes",
		"#aaaa7f",
		ColorSetType::FILL_WITH_OUTLINE,
		false, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICKER BTN: Flag for whether part is expected to display btn
		"QPushButton{border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{border: none; image: url(%2);}"
		"QPushButton:hover:pressed{border: none; image: url(%3);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-pick-eye-color.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-eye-color-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-eye-color-hover-pressed.png",
		{1, 0}, // PICKER BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
		{-1, -1}, // SWAP LEFT BTN: Row/Col placement in grid layout
		nullptr, // PICKER BTN: Alignment in grid layout
		{-1, -1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
		nullptr, // PICKER BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICKER BTN: Width
		75, // PICKER BTN: Height
	}
	},
	{ComponentType::LIPS,
	componentDataInit
	{
		2, // Display order in scene (higher numbers overlap lower numbers)
		"Lips",
		"#555500",
		ColorSetType::FILL_WITH_OUTLINE,
		false, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICKER BTN: Flag for whether part is expected to display btn
		"QPushButton{border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{border: none; image: url(%2);}"
		"QPushButton:hover:pressed{border: none; image: url(%3);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-pick-lip-color.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-lip-color-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-lip-color-hover-pressed.png",
		{2, 0}, // PICKER BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
		{-1, -1}, // SWAP LEFT BTN: Row/Col placement in grid layout
		nullptr, // PICKER BTN: Alignment in grid layout
		{-1, -1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
		nullptr, // PICKER BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICKER BTN: Width
		75, // PICKER BTN: Height
	}
	},
	{ ComponentType::BLUSH,
	componentDataInit
	{
		3, // Display order in scene (higher numbers overlap lower numbers)
		"Blush",
		"#555500",
		ColorSetType::FILL_NO_OUTLINE,
		false, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICKER BTN: Flag for whether part is expected to display btn
		"QPushButton{border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{border: none; image: url(%2);}"
		"QPushButton:hover:pressed{border: none; image: url(%3);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-pick-blush-color.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-blush-color-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-blush-color-hover-pressed.png",
		{3, 0}, // PICKER BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
		{-1, -1}, // SWAP LEFT BTN: Row/Col placement in grid layout
		nullptr, // PICKER BTN: Alignment in grid layout
		{-1, -1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
		nullptr, // PICKER BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICKER BTN: Width
		75, // PICKER BTN: Height
	}
	},
	{ ComponentType::HEAD,
	componentDataInit
	{
		4, // Display order in scene (higher numbers overlap lower numbers)
		"Head",
		"#FFFFFF",
		ColorSetType::NONE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		false, // PICKER BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHead.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHeadHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHeadHover.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
		QStringList(), // Head color changing is controlled by skin color, so we leave this empty
		{-1, -1}, // PICKER BTN: Row/Col placement in grid layout
		nullptr, // PICKER BTN: Alignment in grid layout
		{2, 0}, // SWAP LEFT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // PICKER BTN: Alignment in grid layout
		{2, 1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // PICKER BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICKER BTN: Width
		75, // PICKER BTN: Height
	}
	},
	{ ComponentType::BOTTOM,
	componentDataInit
	{
		5, // Display order in scene (higher numbers overlap lower numbers)
		"Bottom",
		"#B5B5B5",
		ColorSetType::FILL_WITH_OUTLINE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICKER BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartBottom.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartBottomHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartBottomHover.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-pick-bottom-color.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-bottom-color-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-bottom-color-hover-pressed.png",
		{2, 1}, // PICKER BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
		{4, 0}, // SWAP LEFT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // PICKER BTN: Alignment in grid layout
		{4, 1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // PICKER BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICKER BTN: Width
		75, // PICKER BTN: Height
	}
	},
	{ ComponentType::CHEST,
	componentDataInit
	{
		6, // Display order in scene (higher numbers overlap lower numbers)
		"Chest",
		"#B5B5B5",
		ColorSetType::FILL_WITH_OUTLINE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICKER BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartChest.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartChestHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartChestHover.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-pick-chest-color.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-chest-color-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-chest-color-hover-pressed.png",
		{1, 1}, // PICKER BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
		{3, 0}, // SWAP LEFT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // PICKER BTN: Alignment in grid layout
		{3, 1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // PICKER BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICKER BTN: Width
		75, // PICKER BTN: Height
	}
	},
	{ ComponentType::FEET,
	componentDataInit
	{
		7, // Display order in scene (higher numbers overlap lower numbers)
		"Feet",
		"#000000",
		ColorSetType::FILL_WITH_OUTLINE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICKER BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartFeet.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartFeetHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartFeetHover.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-pick-feet-color.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-feet-color-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-feet-color-hover-pressed.png",
		{3, 1}, // PICKER BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
		{5, 0}, // SWAP LEFT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // PICKER BTN: Alignment in grid layout
		{5, 1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // PICKER BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICKER BTN: Width
		75, // PICKER BTN: Height
	}
	},
	{ ComponentType::MASK,
	componentDataInit
	{
		8, // Display order in scene (higher numbers overlap lower numbers)
		"Mask",
		"#000000",
		ColorSetType::FILL_WITH_OUTLINE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICKER BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartMask.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartMaskHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartMaskHover.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-pick-feet-color.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-feet-color-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-feet-color-hover-pressed.png",
		{4, 0}, // PICKER BTN: Row/Col placement in grid layout
		nullptr, // PICKER BTN: Alignment in grid layout
		{0, 0}, // SWAP LEFT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // PICKER BTN: Alignment in grid layout
		{0, 1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // PICKER BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICKER BTN: Width
		75, // PICKER BTN: Height
	}
	},
	{ ComponentType::HAIR,
	componentDataInit
	{
		9, // Display order in scene (higher numbers overlap lower numbers)
		"Hair",
		"#000000",
		ColorSetType::FILL_WITH_OUTLINE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICKER BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHair.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHairHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHairHover.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-pick-hair-color.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-hair-color-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-hair-color-hover-pressed.png",
		{0, 1}, // PICKER BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
		{1, 0}, // SWAP LEFT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // PICKER BTN: Alignment in grid layout
		{1, 1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // PICKER BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICKER BTN: Width
		75, // PICKER BTN: Height
	}
	},
};

const std::map<ComponentType, componentDataInit> componentTypeMapForElf =
{
	{ComponentType::BODY,
	componentDataInit
	{
		0, // Display order in scene (higher numbers overlap lower numbers)
		"Body",
		"#764c39",
		ColorSetType::FILL_WITH_OUTLINE,
		false, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICKER BTN: Flag for whether part is expected to display btn
		"QPushButton{border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{border: none; image: url(%2);}"
		"QPushButton:hover:pressed{border: none; image: url(%3);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-pick-skin-color.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-skin-color-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-skin-color-hover-pressed.png",
		{0, 0}, // PICKER BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
		{-1, -1}, // SWAP LEFT BTN: Row/Col placement in grid layout
		nullptr, // PICKER BTN: Alignment in grid layout
		{-1, -1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
		nullptr, // PICKER BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICKER BTN: Width
		75, // PICKER BTN: Height
	}
	},
	{ComponentType::EYES,
	componentDataInit
	{
		1, // Display order in scene (higher numbers overlap lower numbers)
		"Eyes",
		"#aaaa7f",
		ColorSetType::FILL_WITH_OUTLINE,
		false, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICKER BTN: Flag for whether part is expected to display btn
		"QPushButton{border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{border: none; image: url(%2);}"
		"QPushButton:hover:pressed{border: none; image: url(%3);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-pick-eye-color.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-eye-color-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-eye-color-hover-pressed.png",
		{1, 0}, // PICKER BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
		{-1, -1}, // SWAP LEFT BTN: Row/Col placement in grid layout
		nullptr, // PICKER BTN: Alignment in grid layout
		{-1, -1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
		nullptr, // PICKER BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICKER BTN: Width
		75, // PICKER BTN: Height
	}
	},
	{ComponentType::LIPS,
	componentDataInit
	{
		2, // Display order in scene (higher numbers overlap lower numbers)
		"Lips",
		"#555500",
		ColorSetType::FILL_WITH_OUTLINE,
		false, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICKER BTN: Flag for whether part is expected to display btn
		"QPushButton{border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{border: none; image: url(%2);}"
		"QPushButton:hover:pressed{border: none; image: url(%3);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-pick-lip-color.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-lip-color-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-lip-color-hover-pressed.png",
		{2, 0}, // PICKER BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
		{-1, -1}, // SWAP LEFT BTN: Row/Col placement in grid layout
		nullptr, // PICKER BTN: Alignment in grid layout
		{-1, -1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
		nullptr, // PICKER BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICKER BTN: Width
		75, // PICKER BTN: Height
	}
	},
	{ ComponentType::BLUSH,
	componentDataInit
	{
		3, // Display order in scene (higher numbers overlap lower numbers)
		"Blush",
		"#555500",
		ColorSetType::FILL_NO_OUTLINE,
		false, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICKER BTN: Flag for whether part is expected to display btn
		"QPushButton{border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{border: none; image: url(%2);}"
		"QPushButton:hover:pressed{border: none; image: url(%3);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-pick-blush-color.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-blush-color-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-blush-color-hover-pressed.png",
		{3, 0}, // PICKER BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
		{-1, -1}, // SWAP LEFT BTN: Row/Col placement in grid layout
		nullptr, // PICKER BTN: Alignment in grid layout
		{-1, -1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
		nullptr, // PICKER BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICKER BTN: Width
		75, // PICKER BTN: Height
	}
	},
	{ ComponentType::HEAD,
	componentDataInit
	{
		4, // Display order in scene (higher numbers overlap lower numbers)
		"Head",
		"#FFFFFF",
		ColorSetType::NONE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		false, // PICKER BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHead.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHeadHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHeadHover.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
		QStringList(), // Head color changing is controlled by skin color, so we leave this empty
		{-1, -1}, // PICKER BTN: Row/Col placement in grid layout
		nullptr, // PICKER BTN: Alignment in grid layout
		{2, 0}, // SWAP LEFT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // PICKER BTN: Alignment in grid layout
		{2, 1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // PICKER BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICKER BTN: Width
		75, // PICKER BTN: Height
	}
	},
	{ ComponentType::BOTTOM,
	componentDataInit
	{
		5, // Display order in scene (higher numbers overlap lower numbers)
		"Bottom",
		"#B5B5B5",
		ColorSetType::FILL_WITH_OUTLINE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICKER BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartBottom.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartBottomHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartBottomHover.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-pick-bottom-color.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-bottom-color-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-bottom-color-hover-pressed.png",
		{2, 1}, // PICKER BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
		{4, 0}, // SWAP LEFT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // PICKER BTN: Alignment in grid layout
		{4, 1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // PICKER BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICKER BTN: Width
		75, // PICKER BTN: Height
	}
	},
	{ ComponentType::CHEST,
	componentDataInit
	{
		6, // Display order in scene (higher numbers overlap lower numbers)
		"Chest",
		"#B5B5B5",
		ColorSetType::FILL_WITH_OUTLINE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICKER BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartChest.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartChestHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartChestHover.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-pick-chest-color.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-chest-color-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-chest-color-hover-pressed.png",
		{1, 1}, // PICKER BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
		{3, 0}, // SWAP LEFT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // PICKER BTN: Alignment in grid layout
		{3, 1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // PICKER BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICKER BTN: Width
		75, // PICKER BTN: Height
	}
	},
	{ ComponentType::FEET,
	componentDataInit
	{
		7, // Display order in scene (higher numbers overlap lower numbers)
		"Feet",
		"#000000",
		ColorSetType::FILL_WITH_OUTLINE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICKER BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartFeet.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartFeetHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartFeetHover.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-pick-feet-color.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-feet-color-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-feet-color-hover-pressed.png",
		{3, 1}, // PICKER BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
		{5, 0}, // SWAP LEFT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // PICKER BTN: Alignment in grid layout
		{5, 1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // PICKER BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICKER BTN: Width
		75, // PICKER BTN: Height
	}
	},
	{ ComponentType::MASK,
	componentDataInit
	{
		8, // Display order in scene (higher numbers overlap lower numbers)
		"Mask",
		"#000000",
		ColorSetType::FILL_WITH_OUTLINE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICKER BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartMask.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartMaskHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartMaskHover.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-pick-feet-color.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-feet-color-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-feet-color-hover-pressed.png",
		{4, 0}, // PICKER BTN: Row/Col placement in grid layout
		nullptr, // PICKER BTN: Alignment in grid layout
		{0, 0}, // SWAP LEFT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // PICKER BTN: Alignment in grid layout
		{0, 1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // PICKER BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICKER BTN: Width
		75, // PICKER BTN: Height
	}
	},
	{ ComponentType::HAIR,
	componentDataInit
	{
		9, // Display order in scene (higher numbers overlap lower numbers)
		"Hair",
		"#000000",
		ColorSetType::FILL_WITH_OUTLINE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICKER BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHair.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHairHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHairHover.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-right-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-pick-hair-color.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-hair-color-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-pick-hair-color-hover-pressed.png",
		{0, 1}, // PICKER BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICKER BTN: Alignment in grid layout
		{1, 0}, // SWAP LEFT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // PICKER BTN: Alignment in grid layout
		{1, 1}, // SWAP RIGHT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // PICKER BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICKER BTN: Width
		75, // PICKER BTN: Height
	}
	},
};

const std::map<PoseType, QString> poseTypeMap =
{
	{ PoseType::FRONT_FACING, "Front Facing" }
};

const std::map<GenderType, QString> genderTypeMap =
{
	{ GenderType::MALE, "Male" },
	{ GenderType::FEMALE, "Female" }
};

const std::map<SpeciesType, mapInitData> speciesTypeMap =
{
	{ SpeciesType::HUMAN, mapInitData{"Human", componentTypeMapForHuman} },
	{ SpeciesType::ELF, mapInitData{"Elf", componentTypeMapForElf} }
};