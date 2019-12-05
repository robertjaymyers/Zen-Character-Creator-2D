#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CharacterCreator2d.h"
#include "GraphicsDisplay.h"

class CharacterCreator2d : public QMainWindow
{
	Q_OBJECT

public:
	CharacterCreator2d(QWidget *parent = Q_NULLPTR);
	void closeEvent(QCloseEvent *event);

private:
	Ui::CharacterCreator2dClass ui;

	std::unique_ptr<GraphicsDisplay> display = std::make_unique<GraphicsDisplay>();
};
