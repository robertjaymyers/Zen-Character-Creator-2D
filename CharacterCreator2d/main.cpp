#include "CharacterCreator2d.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CharacterCreator2d w;
	w.show();
	return a.exec();
}
