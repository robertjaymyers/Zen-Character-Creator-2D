#include "GraphicsDisplay.h"

GraphicsDisplay::GraphicsDisplay(QWidget* parent)
	: QGraphicsView(parent)
{
	/*actionFileOpen.get()->setShortcut(QKeySequence(tr("Ctrl+O", "Open Character")));
	actionFileSave.get()->setShortcut(QKeySequence(tr("Ctrl+S", "Save Character")));
	actionFileExport.get()->setShortcut(QKeySequence(Qt::Key_E));*/
	contextMenu.get()->addAction(actionFileNew.get());
	contextMenu.get()->addAction(actionFileOpen.get());
	contextMenu.get()->addAction(actionFileSave.get());
	contextMenu.get()->addAction(actionFileExport.get());

	this->setScene(scene.get());
	scene.get()->setParent(this->parent());

	this->setStyleSheet("border: none; background-color: #FFFFFF");
	this->setMouseTracking(true);
	this->setFixedSize(600, 600);
	this->setLayout(layout.get());
	//this->setAttribute(Qt::WA_TransparentForMouseEvents);
	//scene.get()->setSceneRect(QRectF(QPoint(0, 0), QSize(600, 600)));

	layout.get()->setMargin(50);
	layout.get()->addWidget(characterHeadBtnLeft.get(), 0, 0, Qt::AlignLeft);
	layout.get()->addWidget(characterChestBtnLeft.get(), 1, 0, Qt::AlignLeft);
	layout.get()->addWidget(characterBottomBtnLeft.get(), 2, 0, Qt::AlignLeft);
	layout.get()->addWidget(characterHeadBtnRight.get(), 0, 1, Qt::AlignRight);
	layout.get()->addWidget(characterChestBtnRight.get(), 1, 1, Qt::AlignRight);
	layout.get()->addWidget(characterBottomBtnRight.get(), 2, 1, Qt::AlignRight);

	scene.get()->addItem(characterHead.get());
	scene.get()->addItem(characterChest.get());
	scene.get()->addItem(characterBottom.get());

	characterHead.get()->init(PartType::HEAD);
	characterChest.get()->init(PartType::CHEST);
	characterBottom.get()->init(PartType::BOTTOM);

	connect(characterHeadBtnLeft.get(), &QPushButton::clicked, this, [=]() {
		characterHead.get()->moveLeftInDisplay();
		characterModified = true;
	});
	connect(characterHeadBtnRight.get(), &QPushButton::clicked, this, [=]() {
		characterHead.get()->moveRightInDisplay();
		characterModified = true;
	});
	connect(characterChestBtnLeft.get(), &QPushButton::clicked, this, [=]() {
		characterChest.get()->moveLeftInDisplay();
		characterModified = true;
	});
	connect(characterChestBtnRight.get(), &QPushButton::clicked, this, [=]() {
		characterChest.get()->moveRightInDisplay();
		characterModified = true;
	});
	connect(characterBottomBtnLeft.get(), &QPushButton::clicked, this, [=]() {
		characterBottom.get()->moveLeftInDisplay();
		characterModified = true;
	});
	connect(characterBottomBtnRight.get(), &QPushButton::clicked, this, [=]() {
		characterBottom.get()->moveRightInDisplay();
		characterModified = true;
	});

	connect(actionFileNew.get(), &QAction::triggered, this, [=]() {
		if (fileSaveModifCheck())
			fileNew();
	});

	connect(actionFileOpen.get(), &QAction::triggered, this, [=]() {
		if (fileSaveModifCheck())
			fileOpen();
	});
	connect(actionFileSave.get(), &QAction::triggered, this, [=]() {
		if (fileSave())
			characterModified = false;
	});
	connect(actionFileExport.get(), &QAction::triggered, this, &GraphicsDisplay::fileExportCombination);
	//connect(shortcutExport.get(), &QShortcut::activated, this, &GraphicsDisplay::fileExportCombination);
}

// public:

bool GraphicsDisplay::fileSaveModifCheck()
{
	if (!characterModified)
		return true;

	const QMessageBox::StandardButton ret
		= QMessageBox::warning(this->parentWidget(), tr("Character Creator 2D"),
			tr("The character has been modified.\nDo you want to save your character changes?"),
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

	switch (ret)
	{
	case QMessageBox::Save:
		if (fileSave())
			return true;
		else
			return false;
	case QMessageBox::Discard:
		return true;
	case QMessageBox::Cancel:
		return false;
	default:
		break;
	}

	return true;
}

// protected:

void GraphicsDisplay::contextMenuEvent(QContextMenuEvent *event)
{
	contextMenu.get()->exec(event->globalPos());
}

// private:

QString GraphicsDisplay::extractSubstringInbetweenQt(const QString strBegin, const QString strEnd, const QString &strExtractFrom)
{
	QString extracted = "";
	int posFound = 0;

	if (!strBegin.isEmpty() && !strEnd.isEmpty())
	{
		while (strExtractFrom.indexOf(strBegin, posFound, Qt::CaseSensitive) != -1)
		{
			int posBegin = strExtractFrom.indexOf(strBegin, posFound, Qt::CaseSensitive) + strBegin.length();
			int posEnd = strExtractFrom.indexOf(strEnd, posBegin, Qt::CaseSensitive) + 1 - strEnd.length();
			extracted += strExtractFrom.mid(posBegin, posEnd - posBegin);
			posFound = posEnd;
		}
	}
	else if (strBegin.isEmpty() && !strEnd.isEmpty())
	{
		int posBegin = 0;
		int posEnd = strExtractFrom.indexOf(strEnd, posBegin, Qt::CaseSensitive) + 1 - strEnd.length();
		extracted += strExtractFrom.mid(posBegin, posEnd - posBegin);
		posFound = posEnd;
	}
	else if (!strBegin.isEmpty() && strEnd.isEmpty())
	{
		int posBegin = strExtractFrom.indexOf(strBegin, posFound, Qt::CaseSensitive) + strBegin.length();
		int posEnd = strExtractFrom.length();
		extracted += strExtractFrom.mid(posBegin, posEnd - posBegin);
		posFound = posEnd;
	}
	return extracted;
}

void GraphicsDisplay::fileNew()
{
	characterHead.get()->setCurrentToDefault();
	characterChest.get()->setCurrentToDefault();
	characterBottom.get()->setCurrentToDefault();
	characterModified = false;
}

void GraphicsDisplay::fileOpen()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open"), fileDirLastOpened, tr("Text Files (*.txt)"));
	if (!filename.isEmpty())
	{
		QString fileContents;
		QFile fileRead(filename);
		if (fileRead.open(QIODevice::ReadOnly))
		{
			bool partsMissing = false;
			QTextStream qStream(&fileRead);
			while (!qStream.atEnd())
			{
				QString line = qStream.readLine();
				if (line.contains("Head="))
				{
					if (!characterHead.get()->setUrlAsCurrent(extractSubstringInbetweenQt("=", "", line)))
						partsMissing = true;
				}
				else if (line.contains("Chest="))
				{
					if (!characterChest.get()->setUrlAsCurrent(extractSubstringInbetweenQt("=", "", line)))
						partsMissing = true;
				}
				else if (line.contains("Bottom="))
				{
					if (!characterBottom.get()->setUrlAsCurrent(extractSubstringInbetweenQt("=", "", line)))
						partsMissing = true;
				}
			}
			fileRead.close();
			characterModified = false;
			if (partsMissing)
				QMessageBox::information(this->parentWidget(), tr("Parts Missing"), tr("One or more parts was not found when trying to load from file reference.\r\nSave file only saves references to assets, so if they are moved or deleted, loading may fail."));
		}
	}
}

bool GraphicsDisplay::fileSave()
{
	QFileDialog dialog(this, tr("Save As"), fileDirLastSaved, tr("Text Files (*.txt)"));
	dialog.setWindowModality(Qt::WindowModal);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	if (dialog.exec() == QFileDialog::Accepted)
	{
		QString fpath = dialog.selectedFiles().first();
		QFile fileWrite(fpath);
		if (fileWrite.open(QIODevice::WriteOnly))
		{
			QTextStream qStream(&fileWrite);
			qStream << "Head=" + characterHead.get()->getUrlOfDisplayed() + "\r\n";
			qStream << "Chest=" + characterChest.get()->getUrlOfDisplayed() + "\r\n";
			qStream << "Bottom=" + characterBottom.get()->getUrlOfDisplayed();
			fileWrite.close();
			fileDirLastSaved = fpath;
			return true;
		}
	}
	return false;
}

void GraphicsDisplay::fileExportCombination()
{
	QFileDialog dialog(this, tr("Save As"), fileDirLastSaved, tr("Image Files (*.png)"));
	dialog.setWindowModality(Qt::WindowModal);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	if (dialog.exec() == QFileDialog::Accepted)
	{
		QString selectedFile = dialog.selectedFiles().first();
		QFile fileWrite(selectedFile);
		fileWrite.open(QIODevice::WriteOnly);
		QImage composite(scene.get()->sceneRect().size().toSize(), QImage::Format_ARGB32_Premultiplied);
		composite.fill(Qt::white);
		QPainter painter(&composite);
		scene->render(&painter);
		composite.save(&fileWrite, "PNG");
		fileDirLastSaved = selectedFile;
	}
}