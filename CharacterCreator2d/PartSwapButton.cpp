#include "PartSwapButton.h"

PartSwapButton::PartSwapButton(QWidget *parent, BtnFacing btnFacing)
	: QPushButton(parent)
{
	if (btnFacing == BtnFacing::LEFT)
		this->setStyleSheet(styleSheetTemplate.arg(btnLeftImages[0]).arg(btnLeftImages[1]).arg(btnLeftImages[2]));
	else if (btnFacing == BtnFacing::RIGHT)
		this->setStyleSheet(styleSheetTemplate.arg(btnRightImages[0]).arg(btnRightImages[1]).arg(btnRightImages[2]));
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	this->setFixedSize(QSize(btnWidth, btnHeight));
}