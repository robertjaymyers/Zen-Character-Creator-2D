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
enum class PoseType { FRONT_FACING, FRONT_FACING_ATTN_STANCE };
enum class ComponentType { BODY, EYES, LIPS, BLUSH, HEAD, EARS, NECK, CHEST, BOTTOM, FEET, MASK, HAIR, NONE };
enum class ColorSetType { FILL_NO_OUTLINE, FILL_WITH_OUTLINE, NONE };


// With subcolors, we allow assets to be split up into different "fill" image parts that can be recolored individually.
// Any assets that are NOT split up are ignored by subcolor code and recolored as normal.
// Folder structure and placement of images in them determines whether an asset is split up, which means there are no
// built-in limits to the number of "fill" parts for a specific image. It is worth considering in design, though, that
// too many parts may be a pain for the user to recolor.
struct subColorData
{
	const QString imgFilename; // We use this for saving/loading by filename of an img part.
	const QString imgPath;
	const QColor colorDefault;
	QColor colorAltered;
};

struct assetsData
{
	const QString imgFilename; // We use this for saving/loading by filename of an img part.
	const QString imgFillPath; // This is the part that is affected by color changes.
	const QString imgOutlinePath; // Outline for related fill img. Assets with no color change use Outline only (no fill).
	const QString imgThumbnailPath; // The thumbnail for showing the asset in the swap UI.
	const QColor colorDefault; // We revert to this when "resetting" with, for example, "new character."
	QColor colorAltered; // Color changes are stored here and applied to the scene on part swap.
	std::unique_ptr<QPushButton> btnSwapAsset = std::make_unique<QPushButton>(nullptr);
	std::map<QString, subColorData> subColorsMap;
	QStringList subColorsKeyList; // Used to quickly populate the dropdown list.
};

struct componentDataSettings
{
	// Since we're working with 2D elements that can overlap, we use a display order
	// to ensure that the elements get added to the scene to overlap in the way we want.
	// For example, we give HAIR a higher value than CHEST so that long hair goes on top of shirts.
	// Note: 0 is reserved for background image, so it will go under everything else.
	const int displayOrderZ;

	const QString assetStr; // The corresponding asset folder path string for this unique part (ex: "Head").
	const QColor defaultInitialColor;
	const ColorSetType colorSetType = ColorSetType::NONE;
	const bool partHasBtnSwap;
	const bool partHasBtnPickColor;
	const QString btnStyleSheetTemplate;
	const QStringList btnSwapIcons;
	const QStringList btnPickColorIcons;
	const std::vector<int> gridPlacePickColor;
	const Qt::Alignment gridAlignPickColor;
	const std::vector<int> gridPlaceSwapComponent;
	const Qt::Alignment gridAlignSwapComponent;
	const std::vector<int> gridPlaceSwapAssetOrigin; // Where placement starts; the rest are incremented down as needed.
	const Qt::Alignment gridAlignSwapAsset;
	const int btnSwapWidth;
	const int btnSwapHeight;
	const int btnPickColorWidth;
	const int btnPickColorHeight;

	// Components that this one bases its colors on.
	// Ex: Elf "Ears" bases its colors on the color of Body
	// NOTE: Components that have a Dom list should NOT have a color picker button.
	const std::vector<ComponentType> sharedColoringDomList;

	// Components that get their color changed when this one is changed.
	// Ex: When Body color is changed, the Elf "Ears" also has its color changed to the same thing.
	const std::vector<ComponentType> sharedColoringSubList;
};

struct componentData
{
	std::map<QString, assetsData> assetsMap;
	QString displayedAssetKey;
};

struct componentUiData
{
	const componentDataSettings settings;
	std::unique_ptr<QGraphicsPixmapItem> item = std::make_unique<QGraphicsPixmapItem>(nullptr);
	std::unique_ptr<QPushButton> btnSwapComponent = std::make_unique<QPushButton>(nullptr);
	std::unique_ptr<QPushButton> btnPickColor = std::make_unique<QPushButton>(nullptr);
	std::unique_ptr<QMenu> contextMenuForBtnPickColor = std::make_unique<QMenu>();
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
	std::map<ComponentType, componentUiData> componentUiMap;
	std::map<GenderType, genderData> genderMap;
	std::unique_ptr<QAction> actionSpecies = std::make_unique<QAction>();
};

struct mapInitData
{
	const QString assetStr;
	const std::map<ComponentType, componentDataSettings>& componentMapRef;
};

const std::map<ComponentType, componentDataSettings> componentTypeMapForHuman =
{
	{ComponentType::BODY,
	componentDataSettings
	{
		1, // Display order in scene (higher numbers overlap lower numbers)
		"Body",
		"#764c39",
		ColorSetType::FILL_WITH_OUTLINE,
		false, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICK COLOR BTN: Flag for whether part is expected to display btn
		"QPushButton{border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{border: none; image: url(%2);}"
		"QPushButton:hover:pressed{border: none; image: url(%3);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnPickerBody.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerBodyHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerBodyHover.png",
		{0, 0}, // PICK COLOR BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICK COLOR BTN: Alignment in grid layout
		{-1, -1}, // SWAP COMPONENT BTN: Row/Col placement in grid layout
		nullptr, // SWAP COMPONENT BTN: Alignment in grid layout
		{-1, -1}, // SWAP ASSET BTN: Row/Col placement in grid layout
		nullptr, // SWAP ASSET BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICK COLOR BTN: Width
		75, // PICK COLOR BTN: Height
	}
	},
	{ComponentType::EYES,
	componentDataSettings
	{
		2, // Display order in scene (higher numbers overlap lower numbers)
		"Eyes",
		"#aaaa7f",
		ColorSetType::FILL_WITH_OUTLINE,
		false, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICK COLOR BTN: Flag for whether part is expected to display btn
		"QPushButton{border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{border: none; image: url(%2);}"
		"QPushButton:hover:pressed{border: none; image: url(%3);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnPickerEyes.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerEyesHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerEyesHover.png",
		{1, 0}, // PICK COLOR BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICK COLOR BTN: Alignment in grid layout
		{-1, -1}, // SWAP COMPONENT BTN: Row/Col placement in grid layout
		nullptr, // SWAP COMPONENT BTN: Alignment in grid layout
		{-1, -1}, // SWAP ASSET BTN: Row/Col placement in grid layout
		nullptr, // SWAP ASSET BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICK COLOR BTN: Width
		75, // PICK COLOR BTN: Height
	}
	},
	{ComponentType::LIPS,
	componentDataSettings
	{
		3, // Display order in scene (higher numbers overlap lower numbers)
		"Lips",
		"#555500",
		ColorSetType::FILL_WITH_OUTLINE,
		false, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICK COLOR BTN: Flag for whether part is expected to display btn
		"QPushButton{border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{border: none; image: url(%2);}"
		"QPushButton:hover:pressed{border: none; image: url(%3);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnPickerLips.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerLipsHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerLipsHover.png",
		{2, 0}, // PICK COLOR BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICK COLOR BTN: Alignment in grid layout
		{-1, -1}, // SWAP COMPONENT BTN: Row/Col placement in grid layout
		nullptr, // SWAP COMPONENT BTN: Alignment in grid layout
		{-1, -1}, // SWAP ASSET BTN: Row/Col placement in grid layout
		nullptr, // SWAP ASSET BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICK COLOR BTN: Width
		75, // PICK COLOR BTN: Height
	}
	},
	{ComponentType::BLUSH,
	componentDataSettings
	{
		4, // Display order in scene (higher numbers overlap lower numbers)
		"Blush",
		"#555500",
		ColorSetType::FILL_NO_OUTLINE,
		false, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICK COLOR BTN: Flag for whether part is expected to display btn
		"QPushButton{border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{border: none; image: url(%2);}"
		"QPushButton:hover:pressed{border: none; image: url(%3);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnPickerBlush.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerBlushHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerBlushHover.png",
		{3, 0}, // PICK COLOR BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICK COLOR BTN: Alignment in grid layout
		{-1, -1}, // SWAP COMPONENT BTN: Row/Col placement in grid layout
		nullptr, // SWAP COMPONENT BTN: Alignment in grid layout
		{-1, -1}, // SWAP ASSET BTN: Row/Col placement in grid layout
		nullptr, // SWAP ASSET BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICK COLOR BTN: Width
		75, // PICK COLOR BTN: Height
	}
	},
	{ComponentType::HEAD,
	componentDataSettings
	{
		5, // Display order in scene (higher numbers overlap lower numbers)
		"Head",
		"#FFFFFF",
		ColorSetType::NONE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		false, // PICK COLOR BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHead.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHeadHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHeadHover.png",
		QStringList(), // Head color changing is controlled by skin color, so we leave this empty
		{-1, -1}, // PICK COLOR BTN: Row/Col placement in grid layout
		nullptr, // PICK COLOR BTN: Alignment in grid layout
		{2, 0}, // SWAP COMPONENT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP COMPONENT BTN: Alignment in grid layout
		{0, 1}, // SWAP ASSET BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP ASSET BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICK COLOR BTN: Width
		75, // PICK COLOR BTN: Height
	}
	},
	{ComponentType::NECK,
	componentDataSettings
	{
		6, // Display order in scene (higher numbers overlap lower numbers)
		"Neck",
		"#b5b5b5",
		ColorSetType::FILL_WITH_OUTLINE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICK COLOR BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartNeck.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartNeckHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartNeckHover.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnPickerNeck.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerNeckHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerNeckHover.png",
		{6, 0}, // PICK COLOR BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICK COLOR BTN: Alignment in grid layout
		{3, 0}, // SWAP COMPONENT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP COMPONENT BTN: Alignment in grid layout
		{0, 1}, // SWAP ASSET BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP ASSET BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICK COLOR BTN: Width
		75, // PICK COLOR BTN: Height
	}
	},
	{ComponentType::BOTTOM,
	componentDataSettings
	{
		7, // Display order in scene (higher numbers overlap lower numbers)
		"Bottom",
		"#B5B5B5",
		ColorSetType::FILL_WITH_OUTLINE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICK COLOR BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartBottom.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartBottomHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartBottomHover.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnPickerBottom.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerBottomHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerBottomHover.png",
		{8, 0}, // PICK COLOR BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICK COLOR BTN: Alignment in grid layout
		{5, 0}, // SWAP COMPONENT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP COMPONENT BTN: Alignment in grid layout
		{0, 1}, // SWAP ASSET BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP ASSET BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICK COLOR BTN: Width
		75, // PICK COLOR BTN: Height
	}
	},
	{ComponentType::CHEST,
	componentDataSettings
	{
		8, // Display order in scene (higher numbers overlap lower numbers)
		"Chest",
		"#B5B5B5",
		ColorSetType::FILL_WITH_OUTLINE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICK COLOR BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartChest.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartChestHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartChestHover.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnPickerChest.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerChestHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerChestHover.png",
		{7, 0}, // PICK COLOR BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICK COLOR BTN: Alignment in grid layout
		{4, 0}, // SWAP COMPONENT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP COMPONENT BTN: Alignment in grid layout
		{0, 1}, // SWAP ASSET BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP ASSET BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICK COLOR BTN: Width
		75, // PICK COLOR BTN: Height
	}
	},
	{ComponentType::FEET,
	componentDataSettings
	{
		9, // Display order in scene (higher numbers overlap lower numbers)
		"Feet",
		"#000000",
		ColorSetType::FILL_WITH_OUTLINE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICK COLOR BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartFeet.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartFeetHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartFeetHover.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnPickerFeet.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerFeetHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerFeetHover.png",
		{9, 0}, // PICK COLOR BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICK COLOR BTN: Alignment in grid layout
		{6, 0}, // SWAP COMPONENT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP COMPONENT BTN: Alignment in grid layout
		{0, 1}, // SWAP ASSET BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP ASSET BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICK COLOR BTN: Width
		75, // PICK COLOR BTN: Height
	}
	},
	{ComponentType::MASK,
	componentDataSettings
	{
		10, // Display order in scene (higher numbers overlap lower numbers)
		"Mask",
		"#B5B5B5",
		ColorSetType::FILL_WITH_OUTLINE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICK COLOR BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartMask.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartMaskHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartMaskHover.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnPickerMask.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerMaskHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerMaskHover.png",
		{4, 0}, // PICK COLOR BTN: Row/Col placement in grid layout
		nullptr, // PICK COLOR BTN: Alignment in grid layout
		{0, 0}, // SWAP COMPONENT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP COMPONENT BTN: Alignment in grid layout
		{0, 1}, // SWAP ASSET BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP ASSET BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICK COLOR BTN: Width
		75, // PICK COLOR BTN: Height
	}
	},
	{ComponentType::HAIR,
	componentDataSettings
	{
		11, // Display order in scene (higher numbers overlap lower numbers)
		"Hair",
		"#000000",
		ColorSetType::FILL_WITH_OUTLINE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICK COLOR BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHair.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHairHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHairHover.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnPickerHair.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerHairHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerHairHover.png",
		{5, 0}, // PICK COLOR BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICK COLOR BTN: Alignment in grid layout
		{1, 0}, // SWAP COMPONENT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP COMPONENT BTN: Alignment in grid layout
		{0, 1}, // SWAP ASSET BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP ASSET BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICK COLOR BTN: Width
		75, // PICK COLOR BTN: Height
	}
	},
};

const std::map<ComponentType, componentDataSettings> componentTypeMapForElf =
{
	{ComponentType::BODY,
	componentDataSettings
	{
		1, // Display order in scene (higher numbers overlap lower numbers)
		"Body",
		"#764c39",
		ColorSetType::FILL_WITH_OUTLINE,
		false, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICK COLOR BTN: Flag for whether part is expected to display btn
		"QPushButton{border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{border: none; image: url(%2);}"
		"QPushButton:hover:pressed{border: none; image: url(%3);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnPickerBody.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerBodyHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerBodyHover.png",
		{0, 0}, // PICK COLOR BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICK COLOR BTN: Alignment in grid layout
		{-1, -1}, // SWAP COMPONENT BTN: Row/Col placement in grid layout
		nullptr, // SWAP COMPONENT BTN: Alignment in grid layout
		{-1, -1}, // SWAP ASSET BTN: Row/Col placement in grid layout
		nullptr, // SWAP ASSET BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICK COLOR BTN: Width
		75, // PICK COLOR BTN: Height
		{},
		{ComponentType::EARS} // Components that get their color changed when this one is changed.
	}
	},
	{ComponentType::EYES,
	componentDataSettings
	{
		2, // Display order in scene (higher numbers overlap lower numbers)
		"Eyes",
		"#aaaa7f",
		ColorSetType::FILL_WITH_OUTLINE,
		false, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICK COLOR BTN: Flag for whether part is expected to display btn
		"QPushButton{border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{border: none; image: url(%2);}"
		"QPushButton:hover:pressed{border: none; image: url(%3);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnPickerEyes.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerEyesHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerEyesHover.png",
		{1, 0}, // PICK COLOR BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICK COLOR BTN: Alignment in grid layout
		{-1, -1}, // SWAP COMPONENT BTN: Row/Col placement in grid layout
		nullptr, // SWAP COMPONENT BTN: Alignment in grid layout
		{-1, -1}, // SWAP ASSET BTN: Row/Col placement in grid layout
		nullptr, // SWAP ASSET BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICK COLOR BTN: Width
		75, // PICK COLOR BTN: Height
	}
	},
	{ComponentType::LIPS,
	componentDataSettings
	{
		3, // Display order in scene (higher numbers overlap lower numbers)
		"Lips",
		"#555500",
		ColorSetType::FILL_WITH_OUTLINE,
		false, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICK COLOR BTN: Flag for whether part is expected to display btn
		"QPushButton{border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{border: none; image: url(%2);}"
		"QPushButton:hover:pressed{border: none; image: url(%3);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnPickerLips.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerLipsHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerLipsHover.png",
		{2, 0}, // PICK COLOR BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICK COLOR BTN: Alignment in grid layout
		{-1, -1}, // SWAP COMPONENT BTN: Row/Col placement in grid layout
		nullptr, // SWAP COMPONENT BTN: Alignment in grid layout
		{-1, -1}, // SWAP ASSET BTN: Row/Col placement in grid layout
		nullptr, // SWAP ASSET BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICK COLOR BTN: Width
		75, // PICK COLOR BTN: Height
	}
	},
	{ComponentType::BLUSH,
	componentDataSettings
	{
		4, // Display order in scene (higher numbers overlap lower numbers)
		"Blush",
		"#555500",
		ColorSetType::FILL_NO_OUTLINE,
		false, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICK COLOR BTN: Flag for whether part is expected to display btn
		"QPushButton{border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{border: none; image: url(%2);}"
		"QPushButton:hover:pressed{border: none; image: url(%3);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover.png"
		<< ":/ZenCharacterCreator2D/Resources/button-left-pencil-sketch-hover-pressed.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnPickerBlush.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerBlushHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerBlushHover.png",
		{3, 0}, // PICK COLOR BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICK COLOR BTN: Alignment in grid layout
		{-1, -1}, // SWAP COMPONENT BTN: Row/Col placement in grid layout
		nullptr, // SWAP COMPONENT BTN: Alignment in grid layout
		{-1, -1}, // SWAP ASSET BTN: Row/Col placement in grid layout
		nullptr, // SWAP ASSET BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICK COLOR BTN: Width
		75, // PICK COLOR BTN: Height
	}
	},
	{ ComponentType::EARS,
	componentDataSettings
	{
		1, // Display order in scene (higher numbers overlap lower numbers)
		"Ears",
		"#FFFFFF",
		ColorSetType::FILL_WITH_OUTLINE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		false, // PICK COLOR BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartElfEars.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartElfEarsHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartElfEarsHover.png",
		QStringList(), // Head color changing is controlled by skin color, so we leave this empty
		{-1, -1}, // PICK COLOR BTN: Row/Col placement in grid layout
		nullptr, // PICK COLOR BTN: Alignment in grid layout
		{2, 0}, // SWAP COMPONENT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP COMPONENT BTN: Alignment in grid layout
		{0, 1}, // SWAP ASSET BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP ASSET BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICK COLOR BTN: Width
		75, // PICK COLOR BTN: Height
		{ComponentType::BODY} // Components that this one bases its colors on.
	}
	},
	{ComponentType::HEAD,
	componentDataSettings
	{
		5, // Display order in scene (higher numbers overlap lower numbers)
		"Head",
		"#FFFFFF",
		ColorSetType::NONE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		false, // PICK COLOR BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHead.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHeadHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHeadHover.png",
		QStringList(), // Head color changing is controlled by skin color, so we leave this empty
		{-1, -1}, // PICK COLOR BTN: Row/Col placement in grid layout
		nullptr, // PICK COLOR BTN: Alignment in grid layout
		{3, 0}, // SWAP COMPONENT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP COMPONENT BTN: Alignment in grid layout
		{0, 1}, // SWAP ASSET BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP ASSET BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICK COLOR BTN: Width
		75, // PICK COLOR BTN: Height
	}
	},
	{ComponentType::NECK,
	componentDataSettings
	{
		6, // Display order in scene (higher numbers overlap lower numbers)
		"Neck",
		"#b5b5b5",
		ColorSetType::FILL_WITH_OUTLINE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICK COLOR BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartNeck.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartNeckHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartNeckHover.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnPickerNeck.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerNeckHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerNeckHover.png",
		{6, 0}, // PICK COLOR BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICK COLOR BTN: Alignment in grid layout
		{4, 0}, // SWAP COMPONENT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP COMPONENT BTN: Alignment in grid layout
		{0, 1}, // SWAP ASSET BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP ASSET BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICK COLOR BTN: Width
		75, // PICK COLOR BTN: Height
	}
	},
	{ComponentType::BOTTOM,
	componentDataSettings
	{
		7, // Display order in scene (higher numbers overlap lower numbers)
		"Bottom",
		"#B5B5B5",
		ColorSetType::FILL_WITH_OUTLINE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICK COLOR BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartBottom.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartBottomHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartBottomHover.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnPickerBottom.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerBottomHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerBottomHover.png",
		{8, 0}, // PICK COLOR BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICK COLOR BTN: Alignment in grid layout
		{6, 0}, // SWAP COMPONENT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP COMPONENT BTN: Alignment in grid layout
		{0, 1}, // SWAP ASSET BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP ASSET BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICK COLOR BTN: Width
		75, // PICK COLOR BTN: Height
	}
	},
	{ComponentType::CHEST,
	componentDataSettings
	{
		8, // Display order in scene (higher numbers overlap lower numbers)
		"Chest",
		"#B5B5B5",
		ColorSetType::FILL_WITH_OUTLINE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICK COLOR BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartChest.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartChestHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartChestHover.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnPickerChest.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerChestHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerChestHover.png",
		{7, 0}, // PICK COLOR BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICK COLOR BTN: Alignment in grid layout
		{5, 0}, // SWAP COMPONENT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP COMPONENT BTN: Alignment in grid layout
		{0, 1}, // SWAP ASSET BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP ASSET BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICK COLOR BTN: Width
		75, // PICK COLOR BTN: Height
	}
	},
	{ComponentType::FEET,
	componentDataSettings
	{
		9, // Display order in scene (higher numbers overlap lower numbers)
		"Feet",
		"#000000",
		ColorSetType::FILL_WITH_OUTLINE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICK COLOR BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartFeet.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartFeetHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartFeetHover.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnPickerFeet.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerFeetHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerFeetHover.png",
		{9, 0}, // PICK COLOR BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICK COLOR BTN: Alignment in grid layout
		{7, 0}, // SWAP COMPONENT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP COMPONENT BTN: Alignment in grid layout
		{0, 1}, // SWAP ASSET BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP ASSET BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICK COLOR BTN: Width
		75, // PICK COLOR BTN: Height
	}
	},
	{ComponentType::MASK,
	componentDataSettings
	{
		10, // Display order in scene (higher numbers overlap lower numbers)
		"Mask",
		"#B5B5B5",
		ColorSetType::FILL_WITH_OUTLINE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICK COLOR BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartMask.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartMaskHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartMaskHover.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnPickerMask.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerMaskHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerMaskHover.png",
		{4, 0}, // PICK COLOR BTN: Row/Col placement in grid layout
		nullptr, // PICK COLOR BTN: Alignment in grid layout
		{0, 0}, // SWAP COMPONENT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP COMPONENT BTN: Alignment in grid layout
		{0, 1}, // SWAP ASSET BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP ASSET BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICK COLOR BTN: Width
		75, // PICK COLOR BTN: Height
	}
	},
	{ComponentType::HAIR,
	componentDataSettings
	{
		11, // Display order in scene (higher numbers overlap lower numbers)
		"Hair",
		"#000000",
		ColorSetType::FILL_WITH_OUTLINE,
		true, // SWAP BTN: Flag for whether part is expected to display btn
		true, // PICK COLOR BTN: Flag for whether part is expected to display btn
		"QPushButton{background: #FFFFFF; border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{background: #F8F1E6; border: none; image: url(%2);}"
		"QPushButton:hover:pressed{background: #F8F1E6; border: none; image: url(%3);}"
		"QPushButton:disabled{background: #E5884E; border: none; image: url(%2);}",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHair.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHairHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnSwapCharacterPartHairHover.png",
		QStringList()
		<< ":/ZenCharacterCreator2D/Resources/btnPickerHair.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerHairHover.png"
		<< ":/ZenCharacterCreator2D/Resources/btnPickerHairHover.png",
		{5, 0}, // PICK COLOR BTN: Row/Col placement in grid layout
		Qt::AlignLeft | Qt::AlignTop, // PICK COLOR BTN: Alignment in grid layout
		{1, 0}, // SWAP COMPONENT BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP COMPONENT BTN: Alignment in grid layout
		{0, 1}, // SWAP ASSET BTN: Row/Col placement in grid layout
		Qt::AlignRight, // SWAP ASSET BTN: Alignment in grid layout
		75, // SWAP BTN: Width
		75, // SWAP BTN: Height
		75, // PICK COLOR BTN: Width
		75, // PICK COLOR BTN: Height
	}
	},
};

const std::map<PoseType, QString> poseTypeMap =
{
	{ PoseType::FRONT_FACING, "Front Facing" },
	{ PoseType::FRONT_FACING_ATTN_STANCE, "Front Facing Attn Stance" }
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