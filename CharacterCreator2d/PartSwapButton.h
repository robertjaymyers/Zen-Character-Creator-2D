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
enum class BtnGeometry { SWAP, PICKER, PICKER_SPACER, NONE };

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
	const QStringList btnPickerSkinIcons = QStringList()
		<< ":/CharacterCreator2d/Resources/button-pick-skin-color.png"
		<< ":/CharacterCreator2d/Resources/button-pick-skin-color-hover.png"
		<< ":/CharacterCreator2d/Resources/button-pick-skin-color-hover-pressed.png"
		;
	const QStringList btnPickerEyeIcons = QStringList()
		<< ":/CharacterCreator2d/Resources/button-pick-eye-color.png"
		<< ":/CharacterCreator2d/Resources/button-pick-eye-color-hover.png"
		<< ":/CharacterCreator2d/Resources/button-pick-eye-color-hover-pressed.png"
		;
	const QStringList btnPickerLipIcons = QStringList()
		<< ":/CharacterCreator2d/Resources/button-pick-lip-color.png"
		<< ":/CharacterCreator2d/Resources/button-pick-lip-color-hover.png"
		<< ":/CharacterCreator2d/Resources/button-pick-lip-color-hover-pressed.png"
		;
	const QStringList btnPickerBlushIcons = QStringList()
		<< ":/CharacterCreator2d/Resources/button-pick-blush-color.png"
		<< ":/CharacterCreator2d/Resources/button-pick-blush-color-hover.png"
		<< ":/CharacterCreator2d/Resources/button-pick-blush-color-hover-pressed.png"
		;
	const QStringList btnPickerChestIcons = QStringList()
		<< ":/CharacterCreator2d/Resources/button-pick-chest-color.png"
		<< ":/CharacterCreator2d/Resources/button-pick-chest-color-hover.png"
		<< ":/CharacterCreator2d/Resources/button-pick-chest-color-hover-pressed.png"
		;
	const QStringList btnPickerBottomIcons = QStringList()
		<< ":/CharacterCreator2d/Resources/button-pick-bottom-color.png"
		<< ":/CharacterCreator2d/Resources/button-pick-bottom-color-hover.png"
		<< ":/CharacterCreator2d/Resources/button-pick-bottom-color-hover-pressed.png"
		;
	const QStringList btnPickerFeetIcons = QStringList()
		<< ":/CharacterCreator2d/Resources/button-pick-feet-color.png"
		<< ":/CharacterCreator2d/Resources/button-pick-feet-color-hover.png"
		<< ":/CharacterCreator2d/Resources/button-pick-feet-color-hover-pressed.png"
		;
	const QStringList btnPickerHairIcons = QStringList()
		<< ":/CharacterCreator2d/Resources/button-pick-hair-color.png"
		<< ":/CharacterCreator2d/Resources/button-pick-hair-color-hover.png"
		<< ":/CharacterCreator2d/Resources/button-pick-hair-color-hover-pressed.png"
		;

	// Buttons can be resized here to accommodate more parts, if necessary.
	// Complexity of button layout would probably need to be changed to accommodate beyond
	// parts localized to a range of space on the Y axis.
	// For example, skin color could visually correspond to a button placed just about anyway,
	// so it's not necessarily intuitive which button would correspond to skin color in the eyes of the user.
	const int btnSwapWidth = 75;
	const int btnSwapHeight = 75;

	const int btnPickerWidth = 75;
	const int btnPickerHeight = 75;
};