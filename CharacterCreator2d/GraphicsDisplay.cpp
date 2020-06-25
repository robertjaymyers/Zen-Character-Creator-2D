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

GraphicsDisplay::GraphicsDisplay(QWidget* parent)
	: QGraphicsView(parent)
{
	contextMenu.get()->addAction(actionFileNew.get());
	contextMenu.get()->addAction(actionFileOpen.get());
	contextMenu.get()->addAction(actionFileSave.get());
	contextMenu.get()->addAction(actionFileRender.get());
	contextMenu.get()->addSeparator();
	contextMenu.get()->addAction(actionSetBackgroundColor.get());
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
			characterModified = false;
	});
	connect(actionFileRender.get(), &QAction::triggered, this, &GraphicsDisplay::fileRenderCharacter);
	connect(actionSetBackgroundColor.get(), &QAction::triggered, this, [=]() {
		QColor colorNew = QColorDialog::getColor(backgroundColor, this->parentWidget(), "Choose Color");
		if (colorNew.isValid())
			setBackgroundColor(colorNew);
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

	this->setStyleSheet(styleSheetEditable.arg(backgroundColorDefault.name()));
	this->setLayout(layout.get());

	layout.get()->setMargin(50);

	// Nested order is: Species->Gender->Component->Pose->Assets

	// Create nested maps and data structures first.
	for (const auto& species : speciesTypeMap)
	{
		speciesMap.try_emplace(species.first, speciesData{ species.second.assetStr });
		for (const auto& gender : genderTypeMap)
		{
			speciesMap.at(species.first).genderMap.try_emplace(gender.first, genderData{ gender.second });
			for (const auto& pose : poseTypeMap)
			{
				speciesMap.at(species.first).genderMap.at(gender.first).poseMap.try_emplace(pose.first, poseData{ pose.second });

				for (const auto& component : species.second.componentMapRef)
				{
					speciesMap.at(species.first).genderMap.at(gender.first).poseMap.at(pose.first).componentMap.try_emplace
					(
						component.first, componentData{ component.second }
					);

					QStringList assetPathList = fileGetAssets(
						QCoreApplication::applicationDirPath() +
						"/Assets/Species/" +
						species.second.assetStr + "/" +
						gender.second + "/" +
						pose.second + "/" +
						component.second.assetStr
					);

					// Note: We store as filename only (e.g. NOT including full path), 
					// so that if exe moves, character saves can still be loaded correctly in relation to loaded assets.
					for (const auto& assetPath : assetPathList)
					{
						if (component.second.colorSetType == ColorSetType::FILL_WITH_OUTLINE)
						{
							QString outlinePath = assetPath;
							QString thumbnailPath = assetPath;
							speciesMap.at(species.first).genderMap.at(gender.first).poseMap.at(pose.first)
								.componentMap.at(component.first).assetsMap.try_emplace
							(
								QFileInfo(assetPath).baseName(),
								assetsData
								{
									assetPath,
									QFileInfo(assetPath).fileName(),
									component.second.defaultInitialColor,
									component.second.defaultInitialColor,
									outlinePath.replace("/" + QFileInfo(assetPath).fileName(), "/Outline/" + QFileInfo(assetPath).fileName()),
									thumbnailPath.replace("/" + QFileInfo(assetPath).fileName(), "/Thumbnail/" + QFileInfo(assetPath).fileName())
								}
							);

							QString multicolorPath = assetPath;
							multicolorPath.replace("/" + QFileInfo(assetPath).fileName(), "/Multicolor/" + QFileInfo(assetPath).baseName());
							if (QDir(multicolorPath).exists())
							{
								QStringList multicolorPathList = fileGetAssets(multicolorPath);
								if (multicolorPathList.isEmpty())
									continue;
								for (const auto& colorPath : multicolorPathList)
								{
									auto& currentAssetsMap = speciesMap.at(species.first).genderMap.at(gender.first).poseMap.at(pose.first)
										.componentMap.at(component.first).assetsMap;
									currentAssetsMap.at(QFileInfo(assetPath).baseName()).subColorsMap.try_emplace
									(
										QFileInfo(colorPath).baseName(),
										subColorData
										{
											colorPath,
											QFileInfo(colorPath).fileName(),
											component.second.defaultInitialColor,
											component.second.defaultInitialColor,
										}
									);
									currentAssetsMap.at(QFileInfo(assetPath).baseName()).subColorsKeyList.append
									(QFileInfo(colorPath).baseName());
								}
							}
						}
						else if (component.second.colorSetType == ColorSetType::FILL_NO_OUTLINE ||
							component.second.colorSetType == ColorSetType::NONE)
						{
							QString thumbnailPath = assetPath;
							speciesMap.at(species.first).genderMap.at(gender.first).poseMap.at(pose.first)
								.componentMap.at(component.first).assetsMap.try_emplace
							(
								QFileInfo(assetPath).baseName(),
								assetsData
								{
									assetPath,
									QFileInfo(assetPath).fileName(),
									component.second.defaultInitialColor,
									component.second.defaultInitialColor,
									"",
									thumbnailPath.replace("/" + QFileInfo(assetPath).fileName(), "/Thumbnail/" + QFileInfo(assetPath).fileName())
								}
							);
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
					for (auto& gender : speciesMap.at(speciesCurrent).genderMap)
						gender.second.actionGender.get()->setVisible(false);
					speciesCurrent = species.first;
					for (auto& gender : speciesMap.at(speciesCurrent).genderMap)
						gender.second.actionGender.get()->setVisible(true);
					genderCurrent = speciesMap.at(speciesCurrent).genderMap.begin()->first;
					speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).actionGender.get()->setChecked(true);
					applyCurrentSpeciesToScene();
					loadDefaultCharacterFromTemplate();
				}
			}
		});

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
						genderCurrent = gender.first;
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
							removeCurrentSpeciesFromScene();
							poseCurrent = pose.first;
							applyCurrentSpeciesToScene();
							loadDefaultCharacterFromTemplate();
						}
					}
				});

				for (auto& component : pose.second.componentMap)
				{
					if (component.second.settings.partHasBtnSwap)
					{
						component.second.btnSwapComponent.get()->setStyleSheet
						(
							component.second.settings.btnStyleSheetTemplate
							.arg(component.second.settings.btnSwapIcons[0])
							.arg(component.second.settings.btnSwapIcons[1])
							.arg(component.second.settings.btnSwapIcons[2])
						);
						component.second.btnSwapComponent.get()->setParent(this);
						component.second.btnSwapComponent.get()->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
						component.second.btnSwapComponent.get()->setFixedSize
						(
							QSize
							(
								component.second.settings.btnSwapWidth,
								component.second.settings.btnSwapHeight
							)
						);
						component.second.btnSwapComponent.get()->setVisible(false);

						connect(component.second.btnSwapComponent.get(), &QPushButton::clicked, this, [&]() {
							species.second.genderMap.at(genderCurrent).poseMap.at(poseCurrent).
								componentMap.at(componentCurrent).btnSwapComponent.get()->setEnabled(true);

							for (auto& asset : species.second.genderMap.at(genderCurrent).poseMap.at(poseCurrent).
								componentMap.at(componentCurrent).assetsMap)
							{
								asset.second.btnSwapAsset.get()->setEnabled(true);
								asset.second.btnSwapAsset.get()->setVisible(false);
							}

							component.second.btnSwapComponent.get()->setEnabled(false);

							for (auto& asset : component.second.assetsMap)
							{
								asset.second.btnSwapAsset.get()->setEnabled(true);
								asset.second.btnSwapAsset.get()->setVisible(true);
							}

							partSwapScroll.get()->verticalScrollBar()->setValue(0);
							partSwapScroll.get()->horizontalScrollBar()->setValue(0);

							componentCurrent = component.first;

							species.second.genderMap.at(genderCurrent).poseMap.at(poseCurrent).
								componentMap.at(componentCurrent).assetsMap.at(component.second.displayedAssetKey)
								.btnSwapAsset.get()->setEnabled(false);
						});


						for (auto& asset : component.second.assetsMap)
						{
							asset.second.btnSwapAsset.get()->setStyleSheet
							(
								component.second.settings.btnStyleSheetTemplate
								.arg(asset.second.imgThumbnailPath)
								.arg(asset.second.imgThumbnailPath)
								.arg(asset.second.imgThumbnailPath)
							);
							asset.second.btnSwapAsset.get()->setParent(this);
							asset.second.btnSwapAsset.get()->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
							asset.second.btnSwapAsset.get()->setFixedSize
							(
								QSize
								(
									component.second.settings.btnSwapWidth,
									component.second.settings.btnSwapHeight
								)
							);
							asset.second.btnSwapAsset.get()->setVisible(false);

							connect(asset.second.btnSwapAsset.get(), &QPushButton::clicked, this, [&]() {
								component.second.assetsMap.at(component.second.displayedAssetKey).btnSwapAsset.get()->setEnabled(true);
								component.second.displayedAssetKey = asset.first;
								asset.second.btnSwapAsset.get()->setEnabled(false);
								updatePartInScene(component.second, asset.second);
								characterModified = true;
							});
						}
					}
					if (component.second.settings.partHasBtnPicker)
					{
						component.second.btnPicker.get()->setStyleSheet
						(
							component.second.settings.btnStyleSheetTemplate
							.arg(component.second.settings.btnPickerIcons[0])
							.arg(component.second.settings.btnPickerIcons[1])
							.arg(component.second.settings.btnPickerIcons[2])
						);
						component.second.btnPicker.get()->setParent(this);
						component.second.btnPicker.get()->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
						component.second.btnPicker.get()->setFixedSize
						(
							QSize
							(
								component.second.settings.btnPickerWidth,
								component.second.settings.btnPickerHeight
							)
						);
						component.second.btnPicker.get()->setVisible(false);

						component.second.contextMenuColorPicker.get()->addAction(component.second.actionCopyColor.get());
						component.second.contextMenuColorPicker.get()->addAction(component.second.actionPasteColor.get());
						component.second.contextMenuColorPicker.get()->addSeparator();
						component.second.contextMenuColorPicker.get()->addAction(component.second.actionApplyColorToAllInSet.get());

						component.second.btnPicker.get()->setContextMenuPolicy(Qt::CustomContextMenu);
						connect(component.second.btnPicker.get(), &QPushButton::customContextMenuRequested, this, [&](const QPoint &point) {
							QPoint globalPos = component.second.btnPicker.get()->mapToGlobal(point);
							component.second.contextMenuColorPicker.get()->exec(globalPos);
						});

						connect(component.second.actionCopyColor.get(), &QAction::triggered, this, [&]() {
							if (component.second.assetsMap.at(component.second.displayedAssetKey).subColorsMap.empty())
								pickerCopiedColor = component.second.assetsMap.at(component.second.displayedAssetKey).colorAltered;
							else
							{
								QStringList dropdownList = component.second.assetsMap.at(component.second.displayedAssetKey).subColorsKeyList;
								dropdownList.prepend(component.second.displayedAssetKey);
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
									if (pickedKey == component.second.displayedAssetKey)
										pickerCopiedColor = component.second.assetsMap.at(component.second.displayedAssetKey).colorAltered;
									else
										pickerCopiedColor = component.second.assetsMap.at(component.second.displayedAssetKey)
											.subColorsMap.at(pickedKey).colorAltered;
								}
							}
							pickerUpdatePasteIconColor(pickerCopiedColor);
						});

						connect(component.second.actionPasteColor.get(), &QAction::triggered, this, [&]() {
							if (pickerCopiedColor.isValid())
							{
								auto& currentAsset = component.second.assetsMap.at(component.second.displayedAssetKey);
								if (currentAsset.subColorsMap.empty())
								{
									currentAsset.colorAltered = pickerCopiedColor;
									if (actionColorChangeSettingsApplyToAllOnPicker.get()->isChecked())
									{
										for (auto& asset : component.second.assetsMap)
										{
											asset.second.colorAltered = pickerCopiedColor;
										}
									}
								}
								else
								{
									QStringList dropdownList = currentAsset.subColorsKeyList;
									dropdownList.prepend(component.second.displayedAssetKey);
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
										if (pickedKey == component.second.displayedAssetKey)
										{
											currentAsset.colorAltered = pickerCopiedColor;
											for (auto& subColor : currentAsset.subColorsMap)
												subColor.second.colorAltered = pickerCopiedColor;
											if (actionColorChangeSettingsApplyToAllOnPicker.get()->isChecked())
											{
												for (auto& asset : component.second.assetsMap)
												{
													asset.second.colorAltered = pickerCopiedColor;
												}
											}
										}
										else
											currentAsset.subColorsMap.at(pickedKey).colorAltered = pickerCopiedColor;
									}
								}
								updatePartInScene(component.second, currentAsset);
								characterModified = true;
							}
						});

						connect(component.second.actionApplyColorToAllInSet.get(), &QAction::triggered, this, [&]() {
							if (component.second.settings.colorSetType == ColorSetType::FILL_NO_OUTLINE ||
								component.second.settings.colorSetType == ColorSetType::FILL_WITH_OUTLINE)
							{
								auto& currentAsset = component.second.assetsMap.at(component.second.displayedAssetKey);
								QColor currentColor;
								if (currentAsset.subColorsMap.empty())
									currentColor = currentAsset.colorAltered;
								else
								{
									QStringList dropdownList = currentAsset.subColorsKeyList;
									dropdownList.prepend(component.second.displayedAssetKey);
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
										if (pickedKey == component.second.displayedAssetKey)
											currentColor = currentAsset.colorAltered;
										else
											currentColor = currentAsset.subColorsMap.at(pickedKey).colorAltered;
										for (auto& subColor : currentAsset.subColorsMap)
											subColor.second.colorAltered = currentColor;
										updatePartInScene(component.second, currentAsset);
									}
								}
								for (auto& asset : component.second.assetsMap)
									asset.second.colorAltered = currentColor;
								characterModified = true;
							}
						});

						connect(component.second.btnPicker.get(), &QPushButton::clicked, this, [&]() {
							auto& currentAsset = component.second.assetsMap.at(component.second.displayedAssetKey);
							if (!currentAsset.subColorsMap.empty())
							{
								QStringList dropdownList = currentAsset.subColorsKeyList;
								dropdownList.prepend(component.second.displayedAssetKey);
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
									if (pickedKey == component.second.displayedAssetKey)
									{
										QColor colorNew = QColorDialog::getColor(currentAsset.colorAltered, this->parentWidget(), "Choose Color");
										if (colorNew.isValid())
										{
											currentAsset.colorAltered = colorNew;
											for (auto& subColor : currentAsset.subColorsMap)
												subColor.second.colorAltered = colorNew;
											updatePartInScene(component.second, currentAsset);
											characterModified = true;
											if (actionColorChangeSettingsApplyToAllOnPicker.get()->isChecked())
											{
												for (auto& asset : component.second.assetsMap)
												{
													asset.second.colorAltered = colorNew;
												}
											}
										}
									}
									else
									{
										auto& pickedColorObj = currentAsset.subColorsMap.at(pickedKey);
										QColor colorNew = QColorDialog::getColor(pickedColorObj.colorAltered, this->parentWidget(), "Choose Color");
										if (colorNew.isValid())
										{
											pickedColorObj.colorAltered = colorNew;
											updatePartInScene(component.second, currentAsset);
											characterModified = true;
											if (actionColorChangeSettingsApplyToAllOnPicker.get()->isChecked())
											{
												for (auto& asset : component.second.assetsMap)
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
								QColor colorNew = QColorDialog::getColor(currentAsset.colorAltered, this->parentWidget(), "Choose Color");
								if (colorNew.isValid())
								{
									currentAsset.colorAltered = colorNew;
									updatePartInScene(component.second, currentAsset);
									characterModified = true;
									if (actionColorChangeSettingsApplyToAllOnPicker.get()->isChecked())
									{
										for (auto& asset : component.second.assetsMap)
										{
											asset.second.colorAltered = colorNew;
										}
									}
								}
							}
						});
					}

					component.second.item.get()->setZValue(component.second.settings.displayOrderZ);
					component.second.actionPasteColor->setIcon(QIcon(pickerPasteColorIcon));
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
	
	for (auto& component : speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent).componentMap)
	{
		if (component.second.settings.partHasBtnSwap)
		{
			partSwapGroupLayout.get()->addWidget
			(
				component.second.btnSwapComponent.get(),
				component.second.settings.gridPlaceSwapLeft[0],
				component.second.settings.gridPlaceSwapLeft[1],
				component.second.settings.gridAlignSwapLeft
			);
			component.second.btnSwapComponent.get()->setVisible(true);
			int count = 0;
			for (auto& asset : component.second.assetsMap)
			{
				partSwapGroupLayout.get()->addWidget
				(
					asset.second.btnSwapAsset.get(),
					count,
					1,
					component.second.settings.gridAlignSwapRight
				);
			}

			if (componentCurrent == ComponentType::NONE && component.first != ComponentType::NONE)
			{
				componentCurrent = component.first;
				for (auto& asset : component.second.assetsMap)
				{
					asset.second.btnSwapAsset.get()->setEnabled(true);
					asset.second.btnSwapAsset.get()->setVisible(true);
				}
			}
		}
		if (component.second.settings.partHasBtnPicker)
		{
			partPickerGroupLayout.get()->addWidget
			(
				component.second.btnPicker.get(),
				component.second.settings.gridPlacePicker[0],
				component.second.settings.gridPlacePicker[1],
				component.second.settings.gridAlignPicker
			);
			component.second.btnPicker.get()->setVisible(true);
		}
		scene.get()->addItem(component.second.item.get());
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

QStringList GraphicsDisplay::fileGetAssetDirectories(const QString &subPath)
{
	QStringList assetPathList;
	QDirIterator dirIt(appExecutablePath + "/Assets/" + subPath, QDir::AllDirs | QDir::NoDotAndDotDot);
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

void GraphicsDisplay::updatePartInScene(const componentData &component, const assetsData &asset)
{
	if (asset.subColorsMap.empty())
	{
		if (component.settings.colorSetType == ColorSetType::FILL_WITH_OUTLINE)
			component.item.get()->setPixmap(recolorPixmapSolidWithOutline(asset, PaintType::SINGLE));
		else if (component.settings.colorSetType == ColorSetType::FILL_NO_OUTLINE)
			component.item.get()->setPixmap(recolorPixmapSolid(asset, PaintType::SINGLE));
		else
			component.item.get()->setPixmap(asset.imgBasePath);
	}
	else
	{
		if (component.settings.colorSetType == ColorSetType::FILL_WITH_OUTLINE)
			component.item.get()->setPixmap(recolorPixmapSolidWithOutline(asset, PaintType::COMBINED));
		else if (component.settings.colorSetType == ColorSetType::FILL_NO_OUTLINE)
			component.item.get()->setPixmap(recolorPixmapSolid(asset, PaintType::COMBINED));
		else
			component.item.get()->setPixmap(asset.imgBasePath);
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
		QPixmap newImage = QPixmap(asset.imgBasePath);
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
		QPixmap newImage = QPixmap(asset.imgBasePath);
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
		QPixmap newImage = QPixmap(asset.imgBasePath);
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
		QPixmap newImage = QPixmap(asset.imgBasePath);
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
	for (auto& component : speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent).componentMap)
	{
		QPixmap newIcon = recolorPixmapSolid(component.second.actionPasteColor->icon().pixmap(QSize(20, 20)), color);
		component.second.actionPasteColor->setIcon(newIcon);
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
	auto& componentCurrentObj = speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap
		.at(poseCurrent).componentMap.at(componentCurrent);

	componentCurrentObj.btnSwapComponent.get()->setEnabled(true);

	if (!componentCurrentObj.displayedAssetKey.isEmpty())
		componentCurrentObj.assetsMap.at(componentCurrentObj.displayedAssetKey).btnSwapAsset.get()->setEnabled(true);

	QString fileContents;
	QFile fileRead(filePath);
	if (fileRead.open(QIODevice::ReadOnly))
	{
		bool partsMissing = false;
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
				if (line.contains(component.second.settings.assetStr + "="))
				{
					if (line.contains(component.second.settings.assetStr + "=[Single]"))
					{
						QString assetKey = QFileInfo(extractSubstringInbetweenQt("=[Single]", ",", line)).baseName();
						if (component.second.assetsMap.count(assetKey) > 0)
						{
							component.second.displayedAssetKey = assetKey;
							component.second.assetsMap.at(assetKey).colorAltered = QColor(extractSubstringInbetweenQt(",", "", line));
							updatePartInScene(component.second, component.second.assetsMap.at(assetKey));
							if (component.first == componentCurrent)
							{
								component.second.btnSwapComponent.get()->setEnabled(false);
								component.second.assetsMap.at(assetKey).btnSwapAsset.get()->setEnabled(false);
							}

							if (actionColorChangeSettingsApplyToAllOnPicker.get()->isChecked())
							{
								QColor colorToApply = component.second.assetsMap.at(assetKey).colorAltered;
								for (auto& asset : component.second.assetsMap)
								{
									asset.second.colorAltered = colorToApply;
								}
							}
						}
						else
							partsMissing = true;
					}
					else if (line.contains(component.second.settings.assetStr + "=[Combined]"))
					{
						QString assetKey = QFileInfo(extractSubstringInbetweenQt("=[Combined]", ",", line)).baseName();
						if (component.second.assetsMap.count(assetKey) > 0)
						{
							component.second.displayedAssetKey = assetKey;
							component.second.assetsMap.at(assetKey).colorAltered = QColor(extractSubstringInbetweenQt(",", "[Parts]", line));
							if (component.first == componentCurrent)
							{
								component.second.btnSwapComponent.get()->setEnabled(false);
								component.second.assetsMap.at(assetKey).btnSwapAsset.get()->setEnabled(false);
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
							updatePartInScene(component.second, component.second.assetsMap.at(assetKey));
						}
						else
							partsMissing = true;
					}
					break;
				}
			}
			if (line.contains("backgroundColor="))
			{
				setBackgroundColor(QColor(extractSubstringInbetweenQt("=", "", line)));
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
		characterModified = false;
		if (partsMissing)
			QMessageBox::information(this->parentWidget(), tr("Parts Missing"), tr("One or more parts was not found when trying to load from file reference.\r\nSave file only saves references to assets, so if they are moved or deleted, loading may fail."));
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
	setBackgroundColor(backgroundColor);
	for (auto& textInputSL : textInputSingleLineList)
	{
		textInputSL.inputWidget.get()->setText("");
	}
	characterModified = false;

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
				auto& currentPart = component.second.assetsMap.at(component.second.displayedAssetKey);
				if (currentPart.subColorsMap.empty())
				{
					qStream << component.second.settings.assetStr +
						"=[Single]" + currentPart.imgFilename +
						"," + currentPart.colorAltered.name() + "\r\n";
				}
				else
				{
					qStream << component.second.settings.assetStr +
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
	characterModified = true;
}

void GraphicsDisplay::removeCurrentSpeciesFromScene()
{
	for (auto& component : speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent).componentMap)
	{
		if (component.second.settings.partHasBtnSwap)
		{
			component.second.btnSwapComponent.get()->setVisible(false);
			partSwapGroupLayout.get()->removeWidget(component.second.btnSwapComponent.get());
			for (auto& asset : component.second.assetsMap)
			{
				asset.second.btnSwapAsset.get()->setVisible(false);
				partSwapGroupLayout.get()->removeWidget(asset.second.btnSwapAsset.get());
			}
		}
		if (component.second.settings.partHasBtnPicker)
		{
			component.second.btnPicker.get()->setVisible(false);
			partPickerGroupLayout.get()->removeWidget(component.second.btnPicker.get());
		}

		// We remove rather than using QGraphicsScene clear() function,
		// to avoid deleting the items (they need to be reusable).
		scene.get()->removeItem(component.second.item.get());
	}
	componentCurrent = ComponentType::NONE;
}

void GraphicsDisplay::applyCurrentSpeciesToScene()
{
	// Note: We make the widgets visible AFTER adding them to layout.
	// Otherwise, they may be briefly visible outside of the layout
	// and then get added to it, causing a flickering effect from the adjustment of position.
	for (auto& component : speciesMap.at(speciesCurrent).genderMap.at(genderCurrent).poseMap.at(poseCurrent).componentMap)
	{
		if (component.second.settings.partHasBtnSwap)
		{
			partSwapGroupLayout.get()->addWidget
			(
				component.second.btnSwapComponent.get(),
				component.second.settings.gridPlaceSwapLeft[0],
				component.second.settings.gridPlaceSwapLeft[1],
				component.second.settings.gridAlignSwapLeft
			);
			int count = 0;
			for (auto& asset : component.second.assetsMap)
			{
				partSwapGroupLayout.get()->addWidget
				(
					asset.second.btnSwapAsset.get(),
					count,
					1,
					component.second.settings.gridAlignSwapRight
				);
				count++;
			}
			component.second.btnSwapComponent.get()->setVisible(true);

			if (componentCurrent == ComponentType::NONE && component.first != ComponentType::NONE)
			{
				componentCurrent = component.first;
				for (auto& asset : component.second.assetsMap)
				{
					asset.second.btnSwapAsset.get()->setEnabled(true);
					asset.second.btnSwapAsset.get()->setVisible(true);
				}
			}
		}
		if (component.second.settings.partHasBtnPicker)
		{
			partPickerGroupLayout.get()->addWidget
			(
				component.second.btnPicker.get(),
				component.second.settings.gridPlacePicker[0],
				component.second.settings.gridPlacePicker[1],
				component.second.settings.gridAlignPicker
			);
			component.second.btnPicker.get()->setVisible(true);
		}
		scene.get()->addItem(component.second.item.get());
	}
}