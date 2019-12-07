#pragma once
#include <QPushButton>

enum class BtnIcon 
{ 
	SWAP_LEFT, 
	SWAP_RIGHT, 
	COLOR_PICKER_SKIN, 
	COLOR_PICKER_EYE, 
	COLOR_PICKER_LIP, 
	COLOR_PICKER_BLUSH,
	COLOR_PICKER_CHEST,
	COLOR_PICKER_BOTTOM,
	COLOR_PICKER_FEET,
	COLOR_PICKER_HAIR,
	NONE 
};
enum class BtnGeometry { SWAP, PICKER, NONE };

class PartSwapButton : public QPushButton
{
	Q_OBJECT

public:
	PartSwapButton(QWidget *parent = NULL, BtnIcon btnIcon = BtnIcon::NONE, BtnGeometry btnGeometry = BtnGeometry::NONE);

private:
	QString styleSheetTemplate = 
		"QPushButton{border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{border: none; image: url(%2);}"
		"QPushButton:hover:pressed{border: none; image: url(%3);}"
		;
	const QStringList btnSwapLeftIcons = QStringList()
		<< ":/CharacterCreator2d/Resources/button-left-pencil-sketch.png"
		<< ":/CharacterCreator2d/Resources/button-left-pencil-sketch-hover.png"
		<< ":/CharacterCreator2d/Resources/button-left-pencil-sketch-hover-pressed.png"
		;
	const QStringList btnSwapRightIcons = QStringList()
		<< ":/CharacterCreator2d/Resources/button-right-pencil-sketch.png"
		<< ":/CharacterCreator2d/Resources/button-right-pencil-sketch-hover.png"
		<< ":/CharacterCreator2d/Resources/button-right-pencil-sketch-hover-pressed.png"
		;
	// The following need to be replaced with image resources, rather than placeholders:
	const QStringList btnPickerSkinIcons = QStringList()
		<< "C:/Users/Robert/source/repos/CharacterCreator2d/x64/Assets/button-pick-skin-color.png"
		<< "C:/Users/Robert/source/repos/CharacterCreator2d/x64/Assets/button-pick-skin-color-hover.png"
		<< "C:/Users/Robert/source/repos/CharacterCreator2d/x64/Assets/button-pick-skin-color-hover-pressed.png"
		;
	const QStringList btnPickerEyeIcons = QStringList()
		<< "C:/Users/Robert/source/repos/CharacterCreator2d/x64/Assets/button-pick-eye-color.png"
		<< ":/CharacterCreator2d/Resources/button-right-pencil-sketch-hover.png"
		<< ":/CharacterCreator2d/Resources/button-right-pencil-sketch-hover-pressed.png"
		;
	const QStringList btnPickerLipIcons = QStringList()
		<< "C:/Users/Robert/source/repos/CharacterCreator2d/x64/Assets/button-pick-eye-color.png"
		<< ":/CharacterCreator2d/Resources/button-right-pencil-sketch-hover.png"
		<< ":/CharacterCreator2d/Resources/button-right-pencil-sketch-hover-pressed.png"
		;
	const QStringList btnPickerBlushIcons = QStringList()
		<< "C:/Users/Robert/source/repos/CharacterCreator2d/x64/Assets/button-pick-eye-color.png"
		<< ":/CharacterCreator2d/Resources/button-right-pencil-sketch-hover.png"
		<< ":/CharacterCreator2d/Resources/button-right-pencil-sketch-hover-pressed.png"
		;
	const QStringList btnPickerChestIcons = QStringList()
		<< "C:/Users/Robert/source/repos/CharacterCreator2d/x64/Assets/button-pick-eye-color.png"
		<< ":/CharacterCreator2d/Resources/button-right-pencil-sketch-hover.png"
		<< ":/CharacterCreator2d/Resources/button-right-pencil-sketch-hover-pressed.png"
		;
	const QStringList btnPickerBottomIcons = QStringList()
		<< "C:/Users/Robert/source/repos/CharacterCreator2d/x64/Assets/button-pick-eye-color.png"
		<< ":/CharacterCreator2d/Resources/button-right-pencil-sketch-hover.png"
		<< ":/CharacterCreator2d/Resources/button-right-pencil-sketch-hover-pressed.png"
		;
	const QStringList btnPickerFeetIcons = QStringList()
		<< "C:/Users/Robert/source/repos/CharacterCreator2d/x64/Assets/button-pick-eye-color.png"
		<< ":/CharacterCreator2d/Resources/button-right-pencil-sketch-hover.png"
		<< ":/CharacterCreator2d/Resources/button-right-pencil-sketch-hover-pressed.png"
		;
	const QStringList btnPickerHairIcons = QStringList()
		<< "C:/Users/Robert/source/repos/CharacterCreator2d/x64/Assets/button-pick-eye-color.png"
		<< ":/CharacterCreator2d/Resources/button-right-pencil-sketch-hover.png"
		<< ":/CharacterCreator2d/Resources/button-right-pencil-sketch-hover-pressed.png"
		;

	// Buttons can be resized here to accommodate more parts, if necessary.
	// Complexity of button layout would probably need to be changed to accommodate beyond
	// parts localized to a range of space on the Y axis.
	// For example, skin color could visually correspond to a button placed just about anyway,
	// so it's not necessarily intuitive which button would correspond to skin color in the eyes of the user.
	const int btnSwapWidth = 75;
	const int btnSwapHeight = 75;

	const int btnPickerWidth = 50;
	const int btnPickerHeight = 50;
};