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

#include "GraphicsDisplay.h"

GraphicsDisplay::GraphicsDisplay(QWidget* parent, int width, int height)
	: QGraphicsView(parent)
{
	this->setMinimumSize(QSize(width, height));
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	contextMenu.get()->addAction(actionFileNew.get());
	contextMenu.get()->addAction(actionFileOpen.get());
	contextMenu.get()->addAction(actionFileSave.get());
	contextMenu.get()->addAction(actionFileRender.get());
	contextMenu.get()->addSeparator();
	contextMenu.get()->addAction(actionSetBackgroundColor.get());
	contextMenu.get()->addAction(actionSetBackgroundImage.get());
	contextMenu.get()->addAction(actionClearBackgroundImage.get());
	contextMenu.get()->addSeparator();
	contextMenu.get()->addMenu(speciesMenu.get());
	contextMenu.get()->addMenu(genderMenu.get());
	contextMenu.get()->addMenu(poseMenu.get());
	contextMenu.get()->addSeparator();
	contextMenu.get()->addMenu(colorChangeSettingsMenu.get());

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
			setCharacterModified(false);
	});
	connect(actionFileRender.get(), &QAction::triggered, this, &GraphicsDisplay::fileRenderCharacter);
	connect(actionSetBackgroundColor.get(), &QAction::triggered, this, [=]() {
		QColor colorNew = QColorDialog::getColor(backgroundColor, this->parentWidget(), "Choose Color");
		if (colorNew.isValid())
		{
			setBackgroundColor(colorNew);
			setCharacterModified(true);
		}
	});
	connect(actionSetBackgroundImage.get(), &QAction::triggered, this, [=]() {
		QString filePath = QFileDialog::getOpenFileName(this, tr("Open"), fileDirLastOpenedImage, tr("IMG Files (*.png *.gif *.jpg *.bmp)"));
		if (!filePath.isEmpty())
		{
			setBackgroundImage(filePath);
			fileDirLastOpenedImage = filePath;
			setCharacterModified(true);
		}
	});
	connect(actionClearBackgroundImage.get(), &QAction::triggered, this, [=]() {
		setBackgroundImage(backgroundImageDefault);
		setCharacterModified(true);
	});

	textInputSingleLineList.emplace_back
	(
		textInputSingleLine
		{
			TextInputSingleLineType::FIRST_NAME,
			"characterFirstName",
			"QLineEdit"
			"{"
			"color: #000000;"
			"background-color: #FFFFFF;"
			"selection-background-color: #A96539;"
			"border-width: 1px;"
			"border-style: solid;"
			"border-color: #000000;"
			"border-radius: 4px;"
			"font-size: 12px;"
			"padding: 5px;"
			"}"
			"QLineEdit:hover{color: #A96539;}",
			"First Name",
			{0, 0}, // Row/Col placement in grid layout
			Qt::AlignLeft // Alignment in grid layout
		}
	);
	textInputSingleLineList.emplace_back
	(
		textInputSingleLine
		{
			TextInputSingleLineType::LAST_NAME,
			"characterLastName",
			"QLineEdit"
			"{"
				"color: #000000;"
				"background-color: #FFFFFF;"
				"selection-background-color: #A96539;"
				"border-width: 1px;"
				"border-style: solid;"
				"border-color: #000000;"
				"border-radius: 4px;"
				"font-size: 12px;"
				"padding: 5px;"
			"}"
			"QLineEdit:hover{color: #A96539;}",
			"Last Name",
			{0, 1}, // Row/Col placement in grid layout
			Qt::AlignLeft // Alignment in grid layout
		}
	);

	this->setScene(scene.get());
	scene.get()->setParent(this->parent());

	this->setStyleSheet(styleSheetEditable.arg(backgroundColor.name()));
	scene.get()->addItem(backgroundImageItem.get());
	backgroundImageItem.get()->setZValue(backgroundImageItemZValue);
	backgroundImageItem.get()->setPixmap(backgroundImage);
	backgroundImageItem.get()->setPos(0, 0);
	this->setLayout(layout.get());

	layout.get()->setMargin(50);

	// Nested order is: Species->Gender->Component->Pose->Assets

	// Create nested maps and data structures first.
	for (const auto& species : speciesTypeMap)
	{
		speciesMap.try_emplace(species.first, speciesData{ species.second.assetStr });
		for (const auto& componentSettings : species.second.componentMapRef)
			speciesMap.at(species.first).componentUiMap.try_emplace(componentSettings.first, componentUiData{ componentSettings.second });
		for (const auto& gender : genderTypeMap)
		{
			speciesMap.at(species.first).genderMap.try_emplace(gender.first, genderData{ gender.second });
			for (const auto& pose : poseTypeMap)
			{
				speciesMap.at(species.first).genderMap.at(gender.first).poseMap.try_emplace(pose.first, poseData{ pose.second });

				for (const auto& componentSettings : species.second.componentMapRef)
				{
					speciesMap.at(species.first).genderMap.at(gender.first).poseMap.at(pose.first).componentMap.try_emplace
					(
						componentSettings.first, componentData{ }
					);

					QStringList assetFolderPathList = fileGetAssetDirectoriesOnStartup(
						QCoreApplication::applicationDirPath() +
						"/Assets/Species/" +
						species.second.assetStr + "/" +
						gender.second + "/" +
						pose.second + "/" +
						componentSettings.second.assetStr
					);

					// Note: We store as filename only (e.g. NOT including full path), 
					// so that if exe moves, character saves can still be loaded correctly in relation to loaded assets.
					for (const auto& assetFolderPath : assetFolderPathList)
					{
						QString outlinePath = assetFolderPath;
						QString thumbnailPath = assetFolderPath;
						speciesMap.at(species.first).genderMap.at(gender.first).poseMap.at(pose.first)
							.componentMap.at(componentSettings.first).assetsMap.try_emplace
							(
								QDir(assetFolderPath).dirName(),
								assetsData
								{
									QDir(assetFolderPath).dirName(),
									getPathIfExists(assetFolderPath, AssetImgType::FILL),
									getPathIfExists(assetFolderPath, AssetImgType::OUTLINE),
									getPathIfExists(assetFolderPath, AssetImgType::THUMBNAIL),
									componentSettings.second.defaultInitialColor,
									componentSettings.second.defaultInitialColor,
								}
						);
						//qDebug() << assetFolderPath;

						QString multicolorPath = assetFolderPath + "/multicolor";
						if (QDir(multicolorPath).exists())
						{
							QStringList multicolorPathList = fileGetAssets(multicolorPath);
							if (multicolorPathList.isEmpty())
								continue;
							for (const auto& colorPath : multicolorPathList)
							{
								auto& currentAssetsMap = speciesMap.at(species.first).genderMap.at(gender.first).poseMap.at(pose.first)
									.componentMap.at(componentSettings.first).assetsMap;
								currentAssetsMap.at(QDir(assetFolderPath).dirName()).subColorsMap.try_emplace
								(
									QFileInfo(colorPath).baseName(),
									subColorData
									{
										QFileInfo(colorPath).baseName(),
										colorPath,
										componentSettings.second.defaultInitialColor,
										componentSettings.second.defaultInitialColor,
									}
								);
								currentAssetsMap.at(QDir(assetFolderPath).dirName()).subColorsKeyList.append
								(QFileInfo(colorPath).baseName());
							}
						}
					}
				}
			}
		}
	}

	// Now we can start traversing through the nested maps and applying initial settings.
	for (auto& species : speciesMap)
	{
		species.second.actionSpecies.get()->setParent(this);
		species.second.actionSpecies.get()->setText(species.second.assetStr);
		species.second.actionSpecies.get()->setCheckable(true);
		actionSpeciesGroup.get()->addAction(species.second.actionSpecies.get());
		speciesMenu.get()->addAction(species.second.actionSpecies.get());

		connect(species.second.actionSpecies.get(), &QAction::triggered, this, [&]() {
			if (speciesCurrent != species.first)
			{
				if (fileSaveModifCheck())
				{
					removeCurrentSpeciesFromScene();

					for (auto& gender : speciesCurrentSecond().genderMap)
						gender.second.actionGender.get()->setVisible(false);
					for (auto& pose : genderCurrentSecond().poseMap)
						pose.second.actionPose.get()->setVisible(false);

					speciesCurrent = species.first;
					for (auto& gender : speciesCurrentSecond().genderMap)
						gender.second.actionGender.get()->setVisible(true);

					genderCurrent = speciesCurrentSecond().genderMap.begin()->first;
					genderCurrentSecond().actionGender.get()->setChecked(true);
					for (auto& pose : genderCurrentSecond().poseMap)
						pose.second.actionPose.get()->setVisible(true);

					poseCurrent = genderCurrentSecond().poseMap.begin()->first;
					poseCurrentSecond().actionPose.get()->setChecked(true);

					applyCurrentSpeciesToScene();
					loadDefaultCharacterFromTemplate();
				}
			}
		});

		for (auto& componentUi : species.second.componentUiMap)
		{
			if (componentUi.second.settings.partHasBtnSwap)
			{
				componentUi.second.btnSwapComponentStyle =
					componentUi.second.settings.btnStyleSheetTemplate
					.arg(componentUi.second.settings.btnSwapIcons[0])
					.arg(componentUi.second.settings.btnSwapIcons[1])
					.arg(componentUi.second.settings.btnSwapIcons[2])
					;

				componentUi.second.btnSwapComponentChosenStyle =
					componentUi.second.settings.btnStyleSheetTemplateChosen
					.arg(componentUi.second.settings.btnSwapIcons[1])
					;

				componentUi.second.btnSwapComponent.get()->setStyleSheet(componentUi.second.btnSwapComponentStyle);
				componentUi.second.btnSwapComponent.get()->setParent(this);
				componentUi.second.btnSwapComponent.get()->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
				componentUi.second.btnSwapComponent.get()->setFixedSize
				(
					QSize
					(
						componentUi.second.settings.btnSwapWidth,
						componentUi.second.settings.btnSwapHeight
					)
				);
				componentUi.second.btnSwapComponent.get()->setVisible(false);

				connect(componentUi.second.btnSwapComponent.get(), &QPushButton::clicked, this, [&]() {
					if (!componentUi.second.btnComponentChosen)
					{
						setChosen(false, componentUiCurrentSecond());
						//componentUiCurrentSecond().btnSwapComponent.get()->setEnabled(true);

						for (auto& asset : componentCurrentSecond().assetsMap)
						{
							setChosen(false, asset.second);
							//asset.second.btnSwapAsset.get()->setEnabled(true);
							asset.second.btnSwapAsset.get()->setVisible(false);
						}

						setChosen(true, componentUi.second);
						//componentUi.second.btnSwapComponent.get()->setEnabled(false);

						for (auto& asset : poseCurrentSecond().componentMap.at(componentUi.first).assetsMap)
						{
							setChosen(false, asset.second);
							//asset.second.btnSwapAsset.get()->setEnabled(true);
							asset.second.btnSwapAsset.get()->setVisible(true);
						}

						partSwapScroll.get()->verticalScrollBar()->setValue(0);
						partSwapScroll.get()->horizontalScrollBar()->setValue(0);

						componentCurrent = componentUi.first;

						setChosen(true, assetCurrentSecond());
						//assetCurrentSecond().btnSwapAsset.get()->setEnabled(false);
					}
				});
			}
			if (componentUi.second.settings.partHasBtnPickColor)
			{
				componentUi.second.btnPickColorStyle =
					componentUi.second.settings.btnStyleSheetTemplate
					.arg(componentUi.second.settings.btnPickColorIcons[0])
					.arg(componentUi.second.settings.btnPickColorIcons[1])
					.arg(componentUi.second.settings.btnPickColorIcons[2])
					;

				componentUi.second.btnPickColor.get()->setStyleSheet(componentUi.second.btnPickColorStyle);
				componentUi.second.btnPickColor.get()->setParent(this);
				componentUi.second.btnPickColor.get()->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
				componentUi.second.btnPickColor.get()->setFixedSize
				(
					QSize
					(
						componentUi.second.settings.btnPickColorWidth,
						componentUi.second.settings.btnPickColorHeight
					)
				);
				componentUi.second.btnPickColor.get()->setVisible(false);

				componentUi.second.contextMenuForBtnPickColor.get()->addAction(componentUi.second.actionCopyColor.get());
				componentUi.second.contextMenuForBtnPickColor.get()->addAction(componentUi.second.actionPasteColor.get());
				componentUi.second.contextMenuForBtnPickColor.get()->addSeparator();
				componentUi.second.contextMenuForBtnPickColor.get()->addAction(componentUi.second.actionApplyColorToAllInSet.get());

				componentUi.second.btnPickColor.get()->setContextMenuPolicy(Qt::CustomContextMenu);
				connect(componentUi.second.btnPickColor.get(), &QPushButton::customContextMenuRequested, this, [&](const QPoint &point) {
					QPoint globalPos = componentUi.second.btnPickColor.get()->mapToGlobal(point);
					componentUi.second.contextMenuForBtnPickColor.get()->exec(globalPos);
				});

				connect(componentUi.second.actionCopyColor.get(), &QAction::triggered, this, [&]() {
					auto& componentCurrentSecondLocal = poseCurrentSecond().componentMap.at(componentUi.first);
					auto& assetCurrentSecondLocal = componentCurrentSecondLocal.assetsMap.at(componentCurrentSecondLocal.displayedAssetKey);
					
					if (assetCurrentSecondLocal.subColorsMap.empty())
						pickerCopiedColor = assetCurrentSecondLocal.colorAltered;
					else
					{
						QStringList dropdownList = assetCurrentSecondLocal.subColorsKeyList;
						dropdownList.prepend(componentCurrentSecondLocal.displayedAssetKey);
						bool ok;
						QString pickedKey = QInputDialog::getItem(
							this->parentWidget(),
							"Pick Part To Copy Color From",
							tr("Part Name:"),
							dropdownList,
							0,
							false,
							&ok,
							Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

						if (ok && !pickedKey.isEmpty())
						{
							if (pickedKey == componentCurrentSecondLocal.displayedAssetKey)
								pickerCopiedColor = assetCurrentSecondLocal.colorAltered;
							else
								pickerCopiedColor = assetCurrentSecondLocal.subColorsMap.at(pickedKey).colorAltered;
						}
					}
					pickerUpdatePasteIconColor(pickerCopiedColor);
				});

				connect(componentUi.second.actionPasteColor.get(), &QAction::triggered, this, [&]() {
					auto& componentCurrentSecondLocal = poseCurrentSecond().componentMap.at(componentUi.first);
					auto& assetCurrentSecondLocal = componentCurrentSecondLocal.assetsMap.at(componentCurrentSecondLocal.displayedAssetKey);
					
					if (pickerCopiedColor.isValid())
					{
						if (assetCurrentSecondLocal.subColorsMap.empty())
						{
							assetCurrentSecondLocal.colorAltered = pickerCopiedColor;

							if (actionColorChangeSettingsApplyToAllOnPicker.get()->isChecked())
							{
								for (auto& asset : componentCurrentSecondLocal.assetsMap)
								{
									asset.second.colorAltered = pickerCopiedColor;
								}
								for (auto& sub : componentUi.second.settings.sharedColoringSubList)
								{
									for (auto& assetSub : poseCurrentSecond().componentMap.at(sub).assetsMap)
									{
										assetSub.second.colorAltered = pickerCopiedColor;
									}
								}
							}
						}
						else
						{
							QStringList dropdownList = assetCurrentSecondLocal.subColorsKeyList;
							dropdownList.prepend(componentCurrentSecondLocal.displayedAssetKey);
							bool ok;
							QString pickedKey = QInputDialog::getItem(
								this->parentWidget(),
								"Pick Part To Paste Color To",
								tr("Part Name:"),
								dropdownList,
								0,
								false,
								&ok,
								Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

							if (ok && !pickedKey.isEmpty())
							{
								if (pickedKey == componentCurrentSecondLocal.displayedAssetKey)
								{
									assetCurrentSecondLocal.colorAltered = pickerCopiedColor;
									for (auto& subColor : assetCurrentSecondLocal.subColorsMap)
										subColor.second.colorAltered = pickerCopiedColor;
									if (actionColorChangeSettingsApplyToAllOnPicker.get()->isChecked())
									{
										for (auto& asset : componentCurrentSecondLocal.assetsMap)
										{
											asset.second.colorAltered = pickerCopiedColor;
										}
									}
								}
								else
									assetCurrentSecondLocal.subColorsMap.at(pickedKey).colorAltered = pickerCopiedColor;
							}
						}
						for (auto& sub : componentUi.second.settings.sharedColoringSubList)
						{
							auto& subCompCurrentSecondLocal = poseCurrentSecond().componentMap.at(sub);
							subCompCurrentSecondLocal.assetsMap.at(subCompCurrentSecondLocal.displayedAssetKey)
								.colorAltered = pickerCopiedColor;
							updatePartInScene
							(
								speciesCurrentSecond().componentUiMap.at(sub),
								subCompCurrentSecondLocal.assetsMap.at(subCompCurrentSecondLocal.displayedAssetKey)
							);

							for (auto& assetSub : poseCurrentSecond().componentMap.at(sub).assetsMap)
							{
								assetSub.second.colorAltered = pickerCopiedColor;
							}
						}
						updatePartInScene(componentUi.second, assetCurrentSecondLocal);
						setCharacterModified(true);
					}
				});

				connect(componentUi.second.actionApplyColorToAllInSet.get(), &QAction::triggered, this, [&]() {
					auto& componentCurrentSecondLocal = poseCurrentSecond().componentMap.at(componentUi.first);
					auto& assetCurrentSecondLocal = componentCurrentSecondLocal.assetsMap.at(componentCurrentSecondLocal.displayedAssetKey);
					
					if (componentUi.second.settings.colorSetType == ColorSetType::FILL_NO_OUTLINE ||
						componentUi.second.settings.colorSetType == ColorSetType::FILL_WITH_OUTLINE)
					{
						QColor currentColor;
						if (assetCurrentSecondLocal.subColorsMap.empty())
							currentColor = assetCurrentSecondLocal.colorAltered;
						else
						{
							QStringList dropdownList = assetCurrentSecondLocal.subColorsKeyList;
							dropdownList.prepend(componentCurrentSecondLocal.displayedAssetKey);
							bool ok;
							QString pickedKey = QInputDialog::getItem(
								this->parentWidget(),
								"Pick Part To Apply Color From",
								tr("Part Name:"),
								dropdownList,
								0,
								false,
								&ok,
								Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

							if (ok && !pickedKey.isEmpty())
							{
								if (pickedKey == componentCurrentSecondLocal.displayedAssetKey)
									currentColor = assetCurrentSecondLocal.colorAltered;
								else
									currentColor = assetCurrentSecondLocal.subColorsMap.at(pickedKey).colorAltered;
								for (auto& subColor : assetCurrentSecondLocal.subColorsMap)
									subColor.second.colorAltered = currentColor;
								updatePartInScene(componentUi.second, assetCurrentSecondLocal);
							}
						}
						for (auto& sub : componentUi.second.settings.sharedColoringSubList)
						{
							for (auto& assetSub : poseCurrentSecond().componentMap.at(sub).assetsMap)
							{
								assetSub.second.colorAltered = currentColor;
							}
						}
						for (auto& asset : componentCurrentSecondLocal.assetsMap)
							asset.second.colorAltered = currentColor;
						setCharacterModified(true);
					}
				});

				connect(componentUi.second.btnPickColor.get(), &QPushButton::clicked, this, [&]() {
					auto& componentCurrentSecondLocal = poseCurrentSecond().componentMap.at(componentUi.first);
					auto& assetCurrentSecondLocal = componentCurrentSecondLocal.assetsMap.at(componentCurrentSecondLocal.displayedAssetKey);
					
					if (!assetCurrentSecondLocal.subColorsMap.empty())
					{
						QStringList dropdownList = assetCurrentSecondLocal.subColorsKeyList;
						dropdownList.prepend(componentCurrentSecondLocal.displayedAssetKey);
						bool ok;
						QString pickedKey = QInputDialog::getItem(
							this->parentWidget(),
							"Pick Part To Recolor",
							tr("Part Name:"),
							dropdownList,
							0,
							false,
							&ok,
							Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

						if (ok && !pickedKey.isEmpty())
						{
							if (pickedKey == componentCurrentSecondLocal.displayedAssetKey)
							{
								QColor colorNew = QColorDialog::getColor(assetCurrentSecondLocal.colorAltered, this->parentWidget(), "Choose Color");
								if (colorNew.isValid())
								{
									assetCurrentSecondLocal.colorAltered = colorNew;
									for (auto& subColor : assetCurrentSecondLocal.subColorsMap)
										subColor.second.colorAltered = colorNew;
									updatePartInScene(componentUi.second, assetCurrentSecondLocal);
									setCharacterModified(true);
									if (actionColorChangeSettingsApplyToAllOnPicker.get()->isChecked())
									{
										for (auto& asset : componentCurrentSecondLocal.assetsMap)
										{
											asset.second.colorAltered = colorNew;
										}
									}
								}
							}
							else
							{
								auto& pickedColorObj = assetCurrentSecondLocal.subColorsMap.at(pickedKey);
								QColor colorNew = QColorDialog::getColor(pickedColorObj.colorAltered, this->parentWidget(), "Choose Color");
								if (colorNew.isValid())
								{
									pickedColorObj.colorAltered = colorNew;
									updatePartInScene(componentUi.second, assetCurrentSecondLocal);
									setCharacterModified(true);
									if (actionColorChangeSettingsApplyToAllOnPicker.get()->isChecked())
									{
										for (auto& asset : componentCurrentSecondLocal.assetsMap)
										{
											asset.second.colorAltered = colorNew;
										}
									}
								}
							}
						}
					}
					else
					{
						QColor colorNew = QColorDialog::getColor(assetCurrentSecondLocal.colorAltered, this->parentWidget(), "Choose Color");
						if (colorNew.isValid())
						{
							assetCurrentSecondLocal.colorAltered = colorNew;
							updatePartInScene(componentUi.second, assetCurrentSecondLocal);
							setCharacterModified(true);
							for (auto& sub : componentUi.second.settings.sharedColoringSubList)
							{
								auto& subCompCurrentSecondLocal = poseCurrentSecond().componentMap.at(sub);
								subCompCurrentSecondLocal.assetsMap.at(subCompCurrentSecondLocal.displayedAssetKey)
									.colorAltered = colorNew;
								updatePartInScene
								(
									speciesCurrentSecond().componentUiMap.at(sub),
									subCompCurrentSecondLocal.assetsMap.at(subCompCurrentSecondLocal.displayedAssetKey)
								);

								for (auto& assetSub : poseCurrentSecond().componentMap.at(sub).assetsMap)
								{
									assetSub.second.colorAltered = colorNew;
								}
							}
							if (actionColorChangeSettingsApplyToAllOnPicker.get()->isChecked())
							{
								for (auto& asset : componentCurrentSecondLocal.assetsMap)
								{
									asset.second.colorAltered = colorNew;
								}
							}
						}
					}
				});
			}

			componentUi.second.item.get()->setZValue(componentUi.second.settings.displayOrderZ);
			componentUi.second.actionPasteColor->setIcon(QIcon(pickerPasteColorIcon));
		}

		for (auto& gender : species.second.genderMap)
		{
			gender.second.actionGender.get()->setParent(this);
			gender.second.actionGender.get()->setText(gender.second.assetStr);
			gender.second.actionGender.get()->setCheckable(true);
			gender.second.actionGender.get()->setVisible(false);

			actionGenderGroup.get()->addAction(gender.second.actionGender.get());
			genderMenu.get()->addAction(gender.second.actionGender.get());

			connect(gender.second.actionGender.get(), &QAction::triggered, this, [&]() {
				if (genderCurrent != gender.first)
				{
					if (fileSaveModifCheck())
					{
						removeCurrentSpeciesFromScene();
						for (auto& pose : genderCurrentSecond().poseMap)
							pose.second.actionPose.get()->setVisible(false);
						genderCurrent = gender.first;
						for (auto& pose : genderCurrentSecond().poseMap)
							pose.second.actionPose.get()->setVisible(true);
						poseCurrent = speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.begin()->first;
						applyCurrentSpeciesToScene();
						loadDefaultCharacterFromTemplate();
					}
				}
			});

			for (auto& pose : gender.second.poseMap)
			{
				pose.second.actionPose.get()->setParent(this);
				pose.second.actionPose.get()->setText(pose.second.assetStr);
				pose.second.actionPose.get()->setCheckable(true);
				pose.second.actionPose.get()->setVisible(false);

				actionPoseGroup.get()->addAction(pose.second.actionPose.get());
				poseMenu.get()->addAction(pose.second.actionPose.get());

				connect(pose.second.actionPose.get(), &QAction::triggered, this, [&]() {
					if (poseCurrent != pose.first)
					{
						if (fileSaveModifCheck())
						{
							setChosen(false, componentUiCurrentSecond());
							//componentUiCurrentSecond().btnSwapComponent.get()->setEnabled(true);
							if (!componentCurrentSecond().displayedAssetKey.isEmpty())
								setChosen(false, assetCurrentSecond());
								//assetCurrentSecond().btnSwapAsset.get()->setEnabled(true);
							removeCurrentSpeciesFromScene();
							for (auto& component : poseCurrentSecond().componentMap)
							{
								// We check if a matching asset exists for the new pose.
								// If it does, we use the matching asset. Otherwise, we default to first available asset.
								// We do this so that, where possible, the experience of pose changing is ONLY a 
								// change of pose. As opposed to user having to redo a bunch of customizations.
								auto& newPoseSecond = genderCurrentSecond().poseMap.at(pose.first);
								auto& newPoseComponentSecond = newPoseSecond.componentMap.at(component.first);
								auto& assetCurrentSecondLocal = component.second.assetsMap.at(component.second.displayedAssetKey);
								if (newPoseComponentSecond.assetsMap.count(component.second.displayedAssetKey) > 0)
								{
									newPoseComponentSecond.displayedAssetKey = component.second.displayedAssetKey;
									auto& newPoseAssetSecond = newPoseComponentSecond.assetsMap.at(newPoseComponentSecond.displayedAssetKey);
									newPoseAssetSecond.colorAltered = assetCurrentSecondLocal.colorAltered;
									if (!assetCurrentSecondLocal.subColorsMap.empty() && !newPoseAssetSecond.subColorsMap.empty())
									{
										for (auto& subColor : newPoseAssetSecond.subColorsMap)
										{
											if (assetCurrentSecondLocal.subColorsMap.count(subColor.first) > 0)
											{
												subColor.second.colorAltered = assetCurrentSecondLocal.subColorsMap.at(subColor.first).colorAltered;
												updatePartInScene(speciesCurrentSecond().componentUiMap.at(component.first), newPoseAssetSecond);
											}
										}
									}
									updatePartInScene(speciesCurrentSecond().componentUiMap.at(component.first), newPoseAssetSecond);

									if (actionColorChangeSettingsApplyToAllOnPicker.get()->isChecked())
									{
										for (auto& asset : newPoseComponentSecond.assetsMap)
										{
											asset.second.colorAltered = assetCurrentSecondLocal.colorAltered;;
										}
									}
								}
								else
								{
									newPoseComponentSecond.displayedAssetKey = newPoseComponentSecond.assetsMap.begin()->first;
									auto& newPoseAssetSecond = newPoseComponentSecond.assetsMap.at(newPoseComponentSecond.displayedAssetKey);
									updatePartInScene(speciesCurrentSecond().componentUiMap.at(component.first), newPoseAssetSecond);
								}
							}
							poseCurrent = pose.first;
							applyCurrentSpeciesToScene();
							setChosen(true, componentUiCurrentSecond());
							setChosen(true, assetCurrentSecond());
							//componentUiCurrentSecond().btnSwapComponent.get()->setEnabled(false);
							//assetCurrentSecond().btnSwapAsset.get()->setEnabled(false);
							setCharacterModified(false);
						}
					}
				});

				for (auto& component : pose.second.componentMap)
				{
					auto& componentUi = species.second.componentUiMap.at(component.first);
					if (componentUi.settings.partHasBtnSwap)
					{
						for (auto& asset : component.second.assetsMap)
						{
							asset.second.btnSwapAssetStyle = 
								componentUi.settings.btnStyleSheetTemplate
								.arg(asset.second.imgThumbnailPath)
								.arg(asset.second.imgThumbnailPath)
								.arg(asset.second.imgThumbnailPath)
								;

							asset.second.btnSwapAssetChosenStyle =
								componentUi.settings.btnStyleSheetTemplateChosen
								.arg(asset.second.imgThumbnailPath)
								;

							asset.second.btnSwapAsset.get()->setStyleSheet(asset.second.btnSwapAssetStyle);
							asset.second.btnSwapAsset.get()->setParent(this);
							asset.second.btnSwapAsset.get()->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
							asset.second.btnSwapAsset.get()->setFixedSize
							(
								QSize
								(
									componentUi.settings.btnSwapWidth,
									componentUi.settings.btnSwapHeight
								)
							);
							asset.second.btnSwapAsset.get()->setVisible(false);

							connect(asset.second.btnSwapAsset.get(), &QPushButton::clicked, this, [&]() {
								if (!asset.second.btnAssetChosen)
								{
									setChosen(false, component.second.assetsMap.at(component.second.displayedAssetKey));
									//component.second.assetsMap.at(component.second.displayedAssetKey).btnSwapAsset.get()->setEnabled(true);

									component.second.displayedAssetKey = asset.first;

									setChosen(true, asset.second);
									//asset.second.btnSwapAsset.get()->setEnabled(false);

									updatePartInScene(componentUi, asset.second);
									setCharacterModified(true);
								}
							});
						}
					}
				}
			}
		}
	}

	// Now apply settings for the current species/gender/pose/components/assets
	// (since we're just starting the program, the "current" is the default)

	speciesMap.at(speciesCurrent).actionSpecies.get()->setChecked(true);
	speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).actionGender.get()->setChecked(true);
	speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent).actionPose.get()->setChecked(true);

	for (auto& gender : speciesMap.at(speciesCurrent).genderMap)
		gender.second.actionGender.get()->setVisible(true);
	for (auto& pose : speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap)
			pose.second.actionPose.get()->setVisible(true);
	
	for (auto& componentUi : speciesMap.at(speciesCurrent).componentUiMap)
	{
		if (componentUi.second.settings.partHasBtnSwap)
		{
			partSwapGroupLayout.get()->addWidget
			(
				componentUi.second.btnSwapComponent.get(),
				componentUi.second.settings.gridPlaceSwapComponent[0],
				componentUi.second.settings.gridPlaceSwapComponent[1],
				componentUi.second.settings.gridAlignSwapComponent
			);
			componentUi.second.btnSwapComponent.get()->setVisible(true);

			int count = componentUi.second.settings.gridPlaceSwapAssetOrigin[0];
			auto& currentComponentAt = speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent)
				.componentMap.at(componentUi.first);
			if (currentComponentAt.assetsMap.count("none") > 0)
				count++;
			for (auto& asset : currentComponentAt.assetsMap)
			{
				if (asset.first == "none")
				{
					partSwapGroupLayout.get()->addWidget
					(
						asset.second.btnSwapAsset.get(),
						componentUi.second.settings.gridPlaceSwapAssetOrigin[0],
						componentUi.second.settings.gridPlaceSwapAssetOrigin[1],
						componentUi.second.settings.gridAlignSwapAsset
					);
				}
				else
				{
					partSwapGroupLayout.get()->addWidget
					(
						asset.second.btnSwapAsset.get(),
						count,
						componentUi.second.settings.gridPlaceSwapAssetOrigin[1],
						componentUi.second.settings.gridAlignSwapAsset
					);
					//qDebug() << componentUi.second.settings.assetStr;
					//qDebug() << count;
					count++;
				}
			}

			if (componentCurrent == ComponentType::NONE && componentUi.first != ComponentType::NONE)
			{
				componentCurrent = componentUi.first;
				for (auto& asset : currentComponentAt.assetsMap)
				{
					setChosen(false, asset.second);
					//asset.second.btnSwapAsset.get()->setEnabled(true);
					asset.second.btnSwapAsset.get()->setVisible(true);
				}
			}
		}
		if (componentUi.second.settings.partHasBtnPickColor)
		{
			partPickerGroupLayout.get()->addWidget
			(
				componentUi.second.btnPickColor.get(),
				componentUi.second.settings.gridPlacePickColor[0],
				componentUi.second.settings.gridPlacePickColor[1],
				componentUi.second.settings.gridAlignPickColor
			);
			componentUi.second.btnPickColor.get()->setVisible(true);
		}
		scene.get()->addItem(componentUi.second.item.get());
	}

	// Now set up the rest of the UI.

	pickerUpdatePasteIconColor(pickerCopiedColor);

	for (const auto& textInputSL : textInputSingleLineList)
	{
		textInputSL.inputWidget.get()->setParent(this);
		textInputSL.inputWidget.get()->setStyleSheet(textInputSL.styleSheet);
		textInputSL.inputWidget.get()->setPlaceholderText(textInputSL.placeholderText);

		characterNameInputGroupLayout.get()->addWidget
		(
			textInputSL.inputWidget.get(),
			textInputSL.gridPlace[0],
			textInputSL.gridPlace[1],
			textInputSL.gridAlign
		);
	}

	partSwapGroupLayout.get()->setMargin(20);
	partSwapGroup.get()->setLayout(partSwapGroupLayout.get());
	partSwapGroup.get()->setFlat(true);
	layout.get()->addWidget(partSwapScroll.get(), 0, 2, Qt::AlignRight);
	partSwapScroll.get()->setWidgetResizable(true);
	partSwapScroll.get()->setWidget(partSwapGroup.get());
	partSwapScroll.get()->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	partSwapScroll.get()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	partPickerGroupLayout.get()->setMargin(20);
	partPickerGroup.get()->setLayout(partPickerGroupLayout.get());
	partPickerGroup.get()->setFlat(true);
	layout.get()->addWidget(partPickerScroll.get(), 0, 0, Qt::AlignLeft);
	partPickerScroll.get()->setWidgetResizable(true);
	partPickerScroll.get()->setWidget(partPickerGroup.get());
	partPickerScroll.get()->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	partPickerScroll.get()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	characterNameInputGroupLayout.get()->setMargin(10);
	characterNameInputGroup.get()->setLayout(characterNameInputGroupLayout.get());
	characterNameInputGroup.get()->setFlat(true);
	layout.get()->addWidget(characterNameInputGroup.get(), 1, 0, Qt::AlignLeft);

	/*fullscreenBtn.get()->setText("Fullscreen");
	fullscreenBtn.get()->setStyleSheet(utilityBtnStyle);
	layout.get()->addWidget(fullscreenBtn.get(), 1, 2, Qt::AlignRight);*/

	utilityBtnExit.get()->setText("Exit");
	utilityBtnExit.get()->setStyleSheet(utilityBtnStyle);
	layout.get()->addWidget(utilityBtnExit.get(), 1, 2, Qt::AlignRight);
	connect(utilityBtnExit.get(), &QPushButton::clicked, this, [=]() {
		this->parentWidget()->parentWidget()->close();
	});

	actionColorChangeSettingsApplyToAllOnPicker.get()->setParent(this);
	actionColorChangeSettingsApplyToAllOnPicker.get()->setCheckable(true);
	actionColorChangeSettingsDontApplyToAllOnPicker.get()->setParent(this);
	actionColorChangeSettingsDontApplyToAllOnPicker.get()->setCheckable(true);
	actionColorChangeSettingsApplyToAllOnPicker.get()->setChecked(true);

	actioColorChangeSettingsGroup.get()->addAction(actionColorChangeSettingsApplyToAllOnPicker.get());
	actioColorChangeSettingsGroup.get()->addAction(actionColorChangeSettingsDontApplyToAllOnPicker.get());

	colorChangeSettingsMenu.get()->addAction(actionColorChangeSettingsApplyToAllOnPicker.get());
	colorChangeSettingsMenu.get()->addAction(actionColorChangeSettingsDontApplyToAllOnPicker.get());

	// Template load should be last init operation in graphics display,
	// because it requires assets/parts to be ready (it acts identically to loading a saved character).
	loadDefaultCharacterFromTemplate();
}

// public:

bool GraphicsDisplay::fileSaveModifCheck()
{
	if (!characterModified)
		return true;

	const QMessageBox::StandardButton ret
		= QMessageBox::warning(this->parentWidget(), tr("Zen Character Creator 2D"),
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

void GraphicsDisplay::resizeEvent(QResizeEvent *event)
{
	setBackgroundImage(backgroundImage);
	for (auto& item : scene.get()->items())
	{
		if (item->zValue() != backgroundImageItemZValue)
		{
			item->setPos
			(
				(this->size().width() - item->boundingRect().width()) / 2,
				(this->size().height() - item->boundingRect().height()) / 2
			);
		}
	}
	QWidget::resizeEvent(event);
}

// private:

QString GraphicsDisplay::extractSubstringInbetweenQt(const QString strBegin, const QString strEnd, const QString &strExtractFrom)
{
	QString extracted = "";
	int posFound = 0;

	if (!strBegin.isEmpty() && !strEnd.isEmpty())
	{
		int posBegin = strExtractFrom.indexOf(strBegin, posFound, Qt::CaseSensitive) + strBegin.length();
		int posEnd = strExtractFrom.indexOf(strEnd, posBegin, Qt::CaseSensitive);
		extracted += strExtractFrom.mid(posBegin, posEnd - posBegin);
		posFound = posEnd;
	}
	else if (strBegin.isEmpty() && !strEnd.isEmpty())
	{
		int posBegin = 0;
		int posEnd = strExtractFrom.indexOf(strEnd, posBegin, Qt::CaseSensitive);
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

QString GraphicsDisplay::extractSubstringInbetweenRevFind(const QString strBegin, const QString strEnd, const QString &strExtractFrom)
{
	QString extracted = "";
	int posFound = -1;

	if (!strBegin.isEmpty() && !strEnd.isEmpty())
	{
		while (strExtractFrom.lastIndexOf(strBegin, posFound, Qt::CaseSensitive) != -1)
		{
			int posBegin = strExtractFrom.lastIndexOf(strBegin, posFound, Qt::CaseSensitive) + strBegin.length();
			int posEnd = strExtractFrom.lastIndexOf(strEnd, posBegin, Qt::CaseSensitive);
			extracted += strExtractFrom.mid(posBegin, posEnd - posBegin);
			posFound--;
		}
	}
	else if (strBegin.isEmpty() && !strEnd.isEmpty())
	{
		int posBegin = 0;
		int posEnd = strExtractFrom.lastIndexOf(strEnd, posBegin, Qt::CaseSensitive);
		extracted += strExtractFrom.mid(posBegin, posEnd - posBegin);
		posFound = posEnd;
	}
	else if (!strBegin.isEmpty() && strEnd.isEmpty())
	{
		int posBegin = strExtractFrom.lastIndexOf(strBegin, posFound, Qt::CaseSensitive) + strBegin.length();
		int posEnd = strExtractFrom.length();
		extracted += strExtractFrom.mid(posBegin, posEnd - posBegin);
		posFound = posEnd;
	}
	return extracted;
}

QStringList GraphicsDisplay::extractSubstringInbetweenLoopList(const QString strBegin, const QString strEnd, const QString &strExtractFrom)
{
	QStringList extracted;
	int posFound = 0;

	if (!strBegin.isEmpty() && !strEnd.isEmpty())
	{
		while (strExtractFrom.indexOf(strBegin, posFound, Qt::CaseSensitive) != -1)
		{
			int posBegin = strExtractFrom.indexOf(strBegin, posFound, Qt::CaseSensitive) + strBegin.length();
			int posEnd = strExtractFrom.indexOf(strEnd, posBegin, Qt::CaseSensitive);
			extracted.append(strExtractFrom.mid(posBegin, posEnd - posBegin));
			posFound = posEnd;
		}
	}
	else if (strBegin.isEmpty() && !strEnd.isEmpty())
	{
		int posBegin = 0;
		int posEnd = strExtractFrom.indexOf(strEnd, posBegin, Qt::CaseSensitive);
		extracted.append(strExtractFrom.mid(posBegin, posEnd - posBegin));
		posFound = posEnd;
	}
	else if (!strBegin.isEmpty() && strEnd.isEmpty())
	{
		int posBegin = strExtractFrom.indexOf(strBegin, posFound, Qt::CaseSensitive) + strBegin.length();
		int posEnd = strExtractFrom.length();
		extracted.append(strExtractFrom.mid(posBegin, posEnd - posBegin));
		posFound = posEnd;
	}
	return extracted;
}

QStringList GraphicsDisplay::fileGetAssetDirectoriesOnStartup(const QString &path)
{
	QStringList assetPathList;
	QDirIterator dirIt(path, QDir::AllDirs | QDir::NoDotAndDotDot);
	while (dirIt.hasNext())
	{
		QString assetPath = dirIt.next();
		if (QFileInfo(assetPath).isDir())
			assetPathList.append(assetPath);
	}
	return assetPathList;
}

QStringList GraphicsDisplay::fileGetAssets(const QString &path)
{
	QStringList assetPathList;
	QDirIterator dirIt(path);
	while (dirIt.hasNext())
	{
		QString assetPath = dirIt.next();
		if (QFileInfo(assetPath).suffix() == "png")
			assetPathList.append(assetPath);
	}
	return assetPathList;
}

QString GraphicsDisplay::getPathIfExists(const QString &assetFolderPath, const AssetImgType &assetImgType)
{
	QString imgPath = assetFolderPath + "/" + QDir(assetFolderPath).dirName();
	for (const auto& naming : assetImgTypeMap.at(assetImgType))
	{
		QString namePath = imgPath + naming + imgExtensionStandard;
		if (QFile(namePath).exists())
			return namePath;
	}
	return imgErrorPath;
}

void GraphicsDisplay::updatePartInScene(const componentUiData &componentUi, const assetsData &asset)
{
	auto setNewPixmapAndPos = [&](const QPixmap &newPix) {
		componentUi.item.get()->setPixmap(newPix);
		/*componentUi.item.get()->setPos
		(
			(this->size().width() - newPix.width()) / 2,
			(this->size().height() - newPix.height()) / 2
		);*/
	};

	if (asset.subColorsMap.empty())
	{
		if (componentUi.settings.colorSetType == ColorSetType::FILL_WITH_OUTLINE)
		{
			QPixmap newPix = recolorPixmapSolidWithOutline(asset, PaintType::SINGLE);
			setNewPixmapAndPos(newPix);
		}
		else if (componentUi.settings.colorSetType == ColorSetType::FILL_NO_OUTLINE)
		{
			QPixmap newPix = recolorPixmapSolid(asset, PaintType::SINGLE);
			setNewPixmapAndPos(newPix);
		}
		else
		{
			QPixmap newPix = asset.imgOutlinePath;
			setNewPixmapAndPos(newPix);
		}
	}
	else
	{
		if (componentUi.settings.colorSetType == ColorSetType::FILL_WITH_OUTLINE)
		{
			QPixmap newPix = recolorPixmapSolidWithOutline(asset, PaintType::COMBINED);
			setNewPixmapAndPos(newPix);
		}
		else if (componentUi.settings.colorSetType == ColorSetType::FILL_NO_OUTLINE)
		{
			QPixmap newPix = recolorPixmapSolid(asset, PaintType::COMBINED);
			setNewPixmapAndPos(newPix);
		}
		else
		{
			QPixmap newPix = asset.imgOutlinePath;
			setNewPixmapAndPos(newPix);
		}
	}
}

QPixmap GraphicsDisplay::recolorPixmapSolid(const QPixmap &img, const QColor &color)
{
	QPixmap newImage = QPixmap(img);
	QPainter painter;
	painter.begin(&newImage);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.fillRect(newImage.rect(), color);
	painter.end();
	return newImage;
}

QPixmap GraphicsDisplay::recolorPixmapSolid(const assetsData &asset, const PaintType &paintType)
{
	if (paintType == PaintType::SINGLE)
	{
		QPixmap newImage = QPixmap(asset.imgFillPath);
		QPainter painter;
		painter.begin(&newImage);
		painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
		painter.fillRect(newImage.rect(), asset.colorAltered);
		painter.end();
		return newImage;
	}
	else if (paintType == PaintType::COMBINED)
	{
		std::vector<QPixmap> recoloredParts;
		for (const auto& subColor : asset.subColorsMap)
		{
			QPixmap recoloredImg = QPixmap(subColor.second.imgPath);
			QPainter painter;
			painter.begin(&recoloredImg);
			painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
			painter.fillRect
			(
				QPixmap(subColor.second.imgPath).rect(),
				subColor.second.colorAltered
			);
			painter.end();
			recoloredParts.emplace_back(recoloredImg);
		}
		QPixmap newImage = QPixmap(asset.imgFillPath);
		QPainter painter;
		painter.begin(&newImage);
		painter.setCompositionMode(QPainter::CompositionMode_Source);
		painter.fillRect(newImage.rect(), Qt::transparent);
		painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
		for (const auto& part : recoloredParts)
		{
			painter.drawPixmap(part.rect(), part);
		}
		painter.end();
		return newImage;
	}
	return imgError;
}

QPixmap GraphicsDisplay::recolorPixmapSolidWithOutline(const assetsData &asset, const PaintType &paintType)
{
	if (paintType == PaintType::SINGLE)
	{
		QPixmap newImage = QPixmap(asset.imgFillPath);
		QPainter painter;
		painter.begin(&newImage);
		painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
		painter.fillRect(newImage.rect(), asset.colorAltered);
		painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
		painter.drawPixmap(QPixmap(asset.imgOutlinePath).rect(), QPixmap(asset.imgOutlinePath));
		painter.end();
		return newImage;
	}
	else if (paintType == PaintType::COMBINED)
	{
		std::vector<QPixmap> recoloredParts;
		for (const auto& subColor : asset.subColorsMap)
		{
			QPixmap recoloredImg = QPixmap(subColor.second.imgPath);
			QPainter painter;
			painter.begin(&recoloredImg);
			painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
			painter.fillRect
			(
				QPixmap(subColor.second.imgPath).rect(),
				subColor.second.colorAltered
			);
			painter.end();
			recoloredParts.emplace_back(recoloredImg);
		}
		QPixmap newImage = QPixmap(asset.imgFillPath);
		QPainter painter;
		painter.begin(&newImage);
		painter.setCompositionMode(QPainter::CompositionMode_Source);
		painter.fillRect(newImage.rect(), Qt::transparent);
		painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
		for (const auto& part : recoloredParts)
		{
			painter.drawPixmap(part.rect(), part);
		}
		painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
		painter.drawPixmap(QPixmap(asset.imgOutlinePath).rect(), QPixmap(asset.imgOutlinePath));
		painter.end();
		return newImage;
	}
	return imgError;
}

void GraphicsDisplay::pickerUpdatePasteIconColor(const QColor &color)
{
	for (auto& componentUi : speciesCurrentSecond().componentUiMap)
	{
		QPixmap newIcon = recolorPixmapSolid(componentUi.second.actionPasteColor->icon().pixmap(QSize(20, 20)), color);
		componentUi.second.actionPasteColor->setIcon(newIcon);
	}
}

void GraphicsDisplay::loadDefaultCharacterFromTemplate()
{
	// To provide a little more control over default character settings,
	// a default character can be loaded from a template file.
	// Template is built identical to a saved character, so the loading logic can be reused.
	const QString templatePath =
		QCoreApplication::applicationDirPath() +
		"/Assets/Species/" +
		speciesMap.at(speciesCurrent).assetStr +
		"/" +
		speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).assetStr +
		"/" +
		speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent).assetStr +
		"/CharacterTemplate/default-character-template.zen2dx"
		;
	if (QFile::exists(templatePath))
	{
		fileLoadSavedCharacter(templatePath);
	}
}

void GraphicsDisplay::fileLoadSavedCharacter(const QString &filePath)
{
	setChosen(false, componentUiCurrentSecond());
	//speciesMap.at(speciesCurrent).componentUiMap.at(componentCurrent).btnSwapComponent.get()->setEnabled(true);

	if (!componentCurrentSecond().displayedAssetKey.isEmpty())
		setChosen(false, assetCurrentSecond());
		//assetCurrentSecond().btnSwapAsset.get()->setEnabled(true);

	QString fileContents;
	QFile fileRead(filePath);
	if (fileRead.open(QIODevice::ReadOnly))
	{
		QString missingParts;
		QTextStream qStream(&fileRead);
		while (!qStream.atEnd())
		{
			QString line = qStream.readLine();
			if (line.contains("::Species=") && line.contains("::Gender=") && line.contains("::Pose="))
			{
				QString speciesStr = extractSubstringInbetweenQt("::Species=", "::", line);
				QString genderStr = extractSubstringInbetweenQt("::Gender=", "::", line);
				QString poseStr = extractSubstringInbetweenQt("::Pose=", "::", line);

				for (auto& species : speciesMap)
				{
					if (species.second.assetStr == speciesStr)
					{
						for (auto& gender : species.second.genderMap)
						{
							if (gender.second.assetStr == genderStr)
							{
								for (auto& pose : gender.second.poseMap)
								{
									if (pose.second.assetStr == poseStr)
									{
										removeCurrentSpeciesFromScene();
										speciesCurrent = species.first;
										genderCurrent = gender.first;
										poseCurrent = pose.first;
										applyCurrentSpeciesToScene();
										speciesMap.at(speciesCurrent).actionSpecies.get()->setChecked(true);
										speciesMap.at(speciesCurrent).genderMap.at(genderCurrent)
											.actionGender->setChecked(true);
										speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent)
											.actionPose->setChecked(true);
										break;
									}
								}
								break;
							}
						}
						break;
					}
				}
			}
			for (auto& component : speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent).componentMap)
			{
				auto& currentComponentUiAt = speciesMap.at(speciesCurrent).componentUiMap.at(component.first);
				if (line.contains(currentComponentUiAt.settings.assetStr + "="))
				{
					if (line.contains(currentComponentUiAt.settings.assetStr + "=[Single]"))
					{
						QString assetKey = extractSubstringInbetweenQt("=[Single]", ",", line);
						if (component.second.assetsMap.count(assetKey) > 0)
						{
							component.second.displayedAssetKey = assetKey;
							component.second.assetsMap.at(assetKey).colorAltered = QColor(extractSubstringInbetweenQt(",", "", line));
							updatePartInScene(currentComponentUiAt, component.second.assetsMap.at(assetKey));
							if (component.first == componentCurrent)
							{
								setChosen(true, currentComponentUiAt);
								setChosen(true, component.second.assetsMap.at(assetKey));
								//currentComponentUiAt.btnSwapComponent.get()->setEnabled(false);
								//component.second.assetsMap.at(assetKey).btnSwapAsset.get()->setEnabled(false);
							}

							if (actionColorChangeSettingsApplyToAllOnPicker.get()->isChecked())
							{
								QColor colorToApply = component.second.assetsMap.at(assetKey).colorAltered;
								for (auto& asset : component.second.assetsMap)
								{
									asset.second.colorAltered = colorToApply;
								}
							}

							for (auto& sub : currentComponentUiAt.settings.sharedColoringSubList)
							{
								auto& subCompCurrentSecondLocal = poseCurrentSecond().componentMap.at(sub);
								if (!subCompCurrentSecondLocal.displayedAssetKey.isEmpty())
								{
									subCompCurrentSecondLocal.assetsMap.at(subCompCurrentSecondLocal.displayedAssetKey)
										.colorAltered = component.second.assetsMap.at(assetKey).colorAltered;
									updatePartInScene
									(
										speciesCurrentSecond().componentUiMap.at(sub),
										subCompCurrentSecondLocal.assetsMap.at(subCompCurrentSecondLocal.displayedAssetKey)
									);
								}

								for (auto& assetSub : poseCurrentSecond().componentMap.at(sub).assetsMap)
								{
									assetSub.second.colorAltered = component.second.assetsMap.at(assetKey).colorAltered;
								}
							}
						}
						else
							missingParts.append(" | " + assetKey);
					}
					else if (line.contains(currentComponentUiAt.settings.assetStr + "=[Combined]"))
					{
						QString assetKey = extractSubstringInbetweenQt("=[Combined]", ",", line);
						if (component.second.assetsMap.count(assetKey) > 0)
						{
							component.second.displayedAssetKey = assetKey;
							component.second.assetsMap.at(assetKey).colorAltered = QColor(extractSubstringInbetweenQt(",", "[Parts]", line));
							if (component.first == componentCurrent)
							{
								setChosen(true, currentComponentUiAt);
								setChosen(true, component.second.assetsMap.at(assetKey));
								//currentComponentUiAt.btnSwapComponent.get()->setEnabled(false);
								//component.second.assetsMap.at(assetKey).btnSwapAsset.get()->setEnabled(false);
							}

							QString subPartsStr = extractSubstringInbetweenQt("[Parts]=", "", line);
							QStringList subPartsStrList = extractSubstringInbetweenLoopList("[", "]", subPartsStr);
							std::map<QString, QString> subPartsStrMap;
							for (const auto& partsStr : subPartsStrList)
							{
								subPartsStrMap.try_emplace
								(
									extractSubstringInbetweenQt("[", ",", partsStr),
									extractSubstringInbetweenQt(",", "]", partsStr)
								);
							}
							for (auto& subColor : component.second.assetsMap.at(assetKey).subColorsMap)
							{
								subColor.second.colorAltered = QColor(
									subPartsStrMap.at(subColor.second.imgFilename)
								);
							}
							updatePartInScene(currentComponentUiAt, component.second.assetsMap.at(assetKey));
						}
						else
							missingParts.append(" | " + assetKey);
					}
					break;
				}
			}
			if (line.contains("backgroundColor="))
			{
				setBackgroundColor(QColor(extractSubstringInbetweenQt("=", "", line)));
			}
			else if (line.contains("backgroundImage="))
			{
				setBackgroundImage(extractSubstringInbetweenQt("=", "", line));
			}
			for (auto& textInputSL : textInputSingleLineList)
			{
				if (line.contains(textInputSL.inputTypeStr + "="))
				{
					textInputSL.inputWidget.get()->setText(extractSubstringInbetweenQt("=", "", line));
				}
			}
		}
		fileRead.close();
		setCharacterModified(false);
		if (!missingParts.isEmpty())
		{
			QMessageBox::information
			(
				this->parentWidget(), 
				tr("Parts Missing"), 
				"One or more parts was not found when trying to load from file reference.\r\nSave file only saves references to assets, so if they are moved or deleted, loading may fail.\r\nParts not found are:\r\n" + missingParts
			);
		}
	}
}

void GraphicsDisplay::fileNew()
{
	for (auto& component : speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent).componentMap)
	{
		component.second.displayedAssetKey = component.second.assetsMap.begin()->first;
		for (auto& asset : component.second.assetsMap)
			asset.second.colorAltered = asset.second.colorDefault;
	}
	backgroundColor = backgroundColorDefault;
	backgroundImage = backgroundImageDefault;
	setBackgroundColor(backgroundColor);
	setBackgroundImage(backgroundImage);
	for (auto& textInputSL : textInputSingleLineList)
	{
		textInputSL.inputWidget.get()->setText("");
	}
	setCharacterModified(false);

	loadDefaultCharacterFromTemplate();
}

void GraphicsDisplay::fileOpen()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open"), fileDirLastOpened, tr("Zen Character Creator 2D Files (*.zen2dx)"));
	if (!filename.isEmpty())
	{
		fileLoadSavedCharacter(filename);
		fileDirLastOpened = QFileInfo(filename).path();
	}
}

bool GraphicsDisplay::fileSave()
{
	QString proposedSaveName;
	proposedSaveName += fileDirLastSaved + "/";
	for (const auto& textInputSL : textInputSingleLineList)
	{
		if (textInputSL.inputType == TextInputSingleLineType::FIRST_NAME)
			proposedSaveName += textInputSL.inputWidget.get()->text();
		else if (textInputSL.inputType == TextInputSingleLineType::LAST_NAME)
			proposedSaveName += textInputSL.inputWidget.get()->text();
	}
	proposedSaveName += QDateTime::currentDateTime().toString("_yyyy_MM_dd_HH_mm_ss");
	QFileDialog dialog(this, tr("Save As"), proposedSaveName, tr("Zen Character Creator 2D Files (*.zen2dx)"));
	dialog.setWindowModality(Qt::WindowModal);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	if (dialog.exec() == QFileDialog::Accepted)
	{
		QString fpath = dialog.selectedFiles().first();
		QFile fileWrite(fpath);
		if (fileWrite.open(QIODevice::WriteOnly))
		{
			QTextStream qStream(&fileWrite);

			qStream <<
				"::"
				"Species=" + speciesMap.at(speciesCurrent).assetStr +
				"::" +
				"Gender=" + speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).assetStr +
				"::" +
				"Pose=" + speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent).assetStr +
				"::"
				"\r\n";

			for (auto& component : speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent).componentMap)
			{
				auto& currentComponentUiAt = speciesMap.at(speciesCurrent).componentUiMap.at(component.first);
				auto& currentPart = component.second.assetsMap.at(component.second.displayedAssetKey);
				if (currentPart.subColorsMap.empty())
				{
					qStream << currentComponentUiAt.settings.assetStr +
						"=[Single]" + currentPart.imgFilename +
						"," + currentPart.colorAltered.name() + "\r\n";
				}
				else
				{
					qStream << currentComponentUiAt.settings.assetStr +
						"=[Combined]" + currentPart.imgFilename +
						"," + currentPart.colorAltered.name();

					qStream << "[Parts]=";
					for (const auto& subColor : currentPart.subColorsMap)
					{
						qStream <<
							"[" +
							subColor.second.imgFilename +
							"," +
							subColor.second.colorAltered.name() +
							"]"
							;
					}

					qStream << "\r\n";
				}
			}

			qStream << "backgroundColor=" + backgroundColor.name() + "\r\n";
			qStream << "backgroundImage=" + backgroundImage + "\r\n";

			for (const auto& textInputSL : textInputSingleLineList)
			{
				qStream << textInputSL.inputTypeStr + "=" + textInputSL.inputWidget.get()->text() + "\r\n";
			}

			fileWrite.close();
			fileDirLastSaved = QFileInfo(fpath).path();
			return true;
		}
	}
	return false;
}

void GraphicsDisplay::fileRenderCharacter()
{
	QString proposedExportName;
	proposedExportName += fileDirLastRendered + "/";
	for (const auto& textInputSL : textInputSingleLineList)
	{
		if (textInputSL.inputType == TextInputSingleLineType::FIRST_NAME)
			proposedExportName += textInputSL.inputWidget.get()->text();
		else if (textInputSL.inputType == TextInputSingleLineType::LAST_NAME)
			proposedExportName += textInputSL.inputWidget.get()->text();
	}
	proposedExportName += QDateTime::currentDateTime().toString("_yyyy_MM_dd_HH_mm_ss");
	QFileDialog dialog(this, tr("Save As"), proposedExportName, tr("Image Files (*.png)"));
	dialog.setWindowModality(Qt::WindowModal);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	if (dialog.exec() == QFileDialog::Accepted)
	{
		QString selectedFile = dialog.selectedFiles().first();
		QFile fileWrite(selectedFile);
		fileWrite.open(QIODevice::WriteOnly);
		QImage composite(scene.get()->sceneRect().size().toSize(), QImage::Format_ARGB32_Premultiplied);
		composite.fill(backgroundColor);
		QPainter painter(&composite);
		scene->render(&painter);
		composite.save(&fileWrite, "PNG");
		fileDirLastRendered = QFileInfo(selectedFile).path();
	}
}

void GraphicsDisplay::setBackgroundColor(const QColor &color)
{
	backgroundColor = color;
	this->setStyleSheet(styleSheetEditable.arg(backgroundColor.name()));
}

void GraphicsDisplay::setBackgroundImage(const QString &imgPath)
{
	backgroundImage = imgPath;
	backgroundImageItem.get()->setPixmap
	(
		QPixmap(backgroundImage).scaled(QSize(this->size().width(), this->size().height()), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)
	);
	backgroundImageItem.get()->setPos(0, 0);
}

void GraphicsDisplay::removeCurrentSpeciesFromScene()
{
	for (auto& componentUi : speciesMap.at(speciesCurrent).componentUiMap)
	{
		if (componentUi.second.settings.partHasBtnSwap)
		{
			componentUi.second.btnSwapComponent.get()->setVisible(false);
			partSwapGroupLayout.get()->removeWidget(componentUi.second.btnSwapComponent.get());
			for (auto& asset : poseCurrentSecond().componentMap.at(componentUi.first).assetsMap)
			{
				asset.second.btnSwapAsset.get()->setVisible(false);
				partSwapGroupLayout.get()->removeWidget(asset.second.btnSwapAsset.get());
			}
		}
		if (componentUi.second.settings.partHasBtnPickColor)
		{
			componentUi.second.btnPickColor.get()->setVisible(false);
			partPickerGroupLayout.get()->removeWidget(componentUi.second.btnPickColor.get());
		}

		// We remove rather than using QGraphicsScene clear() function,
		// to avoid deleting the items (they need to be reusable).
		scene.get()->removeItem(componentUi.second.item.get());
	}
	componentCurrent = ComponentType::NONE;
}

void GraphicsDisplay::applyCurrentSpeciesToScene()
{
	// Note: We make the widgets visible AFTER adding them to layout.
	// Otherwise, they may be briefly visible outside of the layout
	// and then get added to it, causing a flickering effect from the adjustment of position.
	for (auto& componentUi : speciesMap.at(speciesCurrent).componentUiMap)
	{
		if (componentUi.second.settings.partHasBtnSwap)
		{
			partSwapGroupLayout.get()->addWidget
			(
				componentUi.second.btnSwapComponent.get(),
				componentUi.second.settings.gridPlaceSwapComponent[0],
				componentUi.second.settings.gridPlaceSwapComponent[1],
				componentUi.second.settings.gridAlignSwapComponent
			);
			int count = componentUi.second.settings.gridPlaceSwapAssetOrigin[0];
			auto& currentComponentAt = poseCurrentSecond().componentMap.at(componentUi.first);
			if (currentComponentAt.assetsMap.count("none") > 0)
				count++;
			for (auto& asset : currentComponentAt.assetsMap)
			{
				if (asset.first == "none")
				{
					partSwapGroupLayout.get()->addWidget
					(
						asset.second.btnSwapAsset.get(),
						componentUi.second.settings.gridPlaceSwapAssetOrigin[0],
						componentUi.second.settings.gridPlaceSwapAssetOrigin[1],
						componentUi.second.settings.gridAlignSwapAsset
					);
				}
				else
				{
					partSwapGroupLayout.get()->addWidget
					(
						asset.second.btnSwapAsset.get(),
						count,
						componentUi.second.settings.gridPlaceSwapAssetOrigin[1],
						componentUi.second.settings.gridAlignSwapAsset
					);
					count++;
				}
			}
			componentUi.second.btnSwapComponent.get()->setVisible(true);

			if (componentCurrent == ComponentType::NONE && componentUi.first != ComponentType::NONE)
			{
				componentCurrent = componentUi.first;
				for (auto& asset : currentComponentAt.assetsMap)
				{
					setChosen(false, asset.second);
					//asset.second.btnSwapAsset.get()->setEnabled(true);
					asset.second.btnSwapAsset.get()->setVisible(true);
				}
			}
		}
		if (componentUi.second.settings.partHasBtnPickColor)
		{
			partPickerGroupLayout.get()->addWidget
			(
				componentUi.second.btnPickColor.get(),
				componentUi.second.settings.gridPlacePickColor[0],
				componentUi.second.settings.gridPlacePickColor[1],
				componentUi.second.settings.gridAlignPickColor
			);
			componentUi.second.btnPickColor.get()->setVisible(true);
		}
		scene.get()->addItem(componentUi.second.item.get());
	}
}

void GraphicsDisplay::setChosen(bool isChosen, assetsData &asset)
{
	if (isChosen)
	{
		asset.btnSwapAsset.get()->setStyleSheet(asset.btnSwapAssetChosenStyle);
		asset.btnAssetChosen = true;
	}
	else
	{
		asset.btnSwapAsset.get()->setStyleSheet(asset.btnSwapAssetStyle);
		asset.btnAssetChosen = false;
	}
}

void GraphicsDisplay::setChosen(bool isChosen, componentUiData &componentUi)
{
	if (isChosen)
	{
		componentUi.btnSwapComponent.get()->setStyleSheet(componentUi.btnSwapComponentChosenStyle);
		componentUi.btnComponentChosen = true;
	}
	else
	{
		componentUi.btnSwapComponent.get()->setStyleSheet(componentUi.btnSwapComponentStyle);
		componentUi.btnComponentChosen = false;
	}
}

void GraphicsDisplay::setCharacterModified(const bool newState)
{
	characterModified = newState;
	/*if (characterModified)
		qDebug() << "Character modified state set to: TRUE";
	else
		qDebug() << "Character modified state set to: FALSE";*/
}

speciesData& GraphicsDisplay::speciesCurrentSecond()
{
	return speciesMap.at(speciesCurrent);
}

genderData& GraphicsDisplay::genderCurrentSecond()
{
	return speciesMap.at(speciesCurrent).genderMap.at(genderCurrent);
}

poseData& GraphicsDisplay::poseCurrentSecond()
{
	return speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent);
}

componentUiData& GraphicsDisplay::componentUiCurrentSecond()
{
	return speciesMap.at(speciesCurrent).componentUiMap.at(componentCurrent);
}

componentData& GraphicsDisplay::componentCurrentSecond()
{
	return speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent)
		.componentMap.at(componentCurrent);
}

assetsData& GraphicsDisplay::assetCurrentSecond()
{
	return speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent)
		.componentMap.at(componentCurrent).assetsMap.at
		(
			speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent)
			.componentMap.at(componentCurrent).displayedAssetKey
		);
}