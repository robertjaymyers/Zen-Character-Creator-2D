#pragma once
#include <QPushButton>

enum class BtnFacing { LEFT, RIGHT, NONE };

class PartSwapButton : public QPushButton
{
	Q_OBJECT

public:
	PartSwapButton(QWidget *parent = NULL, BtnFacing btnFacing = BtnFacing::NONE);

private:
	QString styleSheetTemplate = 
		"QPushButton{border: none; image: url(%1);}"
		"QPushButton:hover:!pressed{border: none; image: url(%2);}"
		"QPushButton:hover:pressed{border: none; image: url(%3);}"
		;
	const QStringList btnLeftImages = QStringList()
		<< ":/CharacterCreator2d/Resources/button-left-pencil-sketch.png"
		<< ":/CharacterCreator2d/Resources/button-left-pencil-sketch-hover.png"
		<< ":/CharacterCreator2d/Resources/button-left-pencil-sketch-hover-pressed.png"
		;
	const QStringList btnRightImages = QStringList()
		<< ":/CharacterCreator2d/Resources/button-right-pencil-sketch.png"
		<< ":/CharacterCreator2d/Resources/button-right-pencil-sketch-hover.png"
		<< ":/CharacterCreator2d/Resources/button-right-pencil-sketch-hover-pressed.png"
		;

	// Buttons can be resized here to accommodate more parts, if necessary.
	// Complexity of button layout would probably need to be changed to accommodate beyond
	// parts localized to a range of space on the Y axis.
	// For example, skin color could visually correspond to a button placed just about anyway,
	// so it's not necessarily intuitive which button would correspond to skin color in the eyes of the user.
	const int btnWidth = 100;
	const int btnHeight = 100;
};