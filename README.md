# Zen Character Creator 2D
Zen Character Creator 2D is built from a deep love of character customization in video games and built with indie game developers in mind (though others of course are welcome to use it too!). As a developer, I'm aware of the limitations of resources that indie game developers are often working with. And as a fan, I want indie games to be able to match up to the experiences put out by larger studios.

With that in mind, this tool aims to offer a solid and flexible starting point for 2d character creation and customization, saving you the consideration of building your own in-house tool from scratch on limited time and budget.

## Features
### Backend (code/compiling customization)
* Species -> Gender -> Pose hierarchy, with the ability to set types of species, genders, and poses to what your game includes
* Set types of components (ex: Head, Shirt, Pants, Shoes), customized per Species
* Customize stylesheets and images for UI elements
* Expand or remove certain UI elements with little changes needed (ex: add a "middle name" input box or remove "last name" and only allow "first name")
* Linking of two components for special color needs (ex: link color change of Elf Ears to Body, so that changing color of Body changes color of Ears, allowing you to have multiple Ear piece choices in a component separate from Body)
### Frontend (user-facing features)
* Color customization using Qt's QColorDialog window, based on painting over a white 'fill' image to ensure color accuracy, along with an untouched outline painted over the fill
* Swap components and assets with a simple visual interface (click a component, e.g. Shirt, and then click from a list of Shirt thumbnails available, populated in the column to the right of the Shirt component image)
* Enter first name and last name
* Set Background Color or Image
* Save and load character data in a minimal-size human-readable format, using filenames and color codes
* Render character to a static PNG image, to be saved where the user desires
* Mute or unmute UI sound effects and background music, designed to be ignored if none are detected when loading the program
* Support in the folder system (see more on folder system below) for breaking an image's 'fill' into multiple pieces, so the user can recolor in more than one place (ex: left and right eye to support heterochromia)
### Folder System
Assets for Zen Character Creator 2D are read from the Assets folder in the application's executable path and look for PNG images (the image type can easily be changed in the code, if desired). Some examples of the folder hierarchy and how assets are looked for are as follows:
* Assets -> Species -> Human -> Female -> Front Facing -> Shirt -> tShirtBasic -> tShirtFill.png, tShirtOutline.png, tShirtThumbnail.png (note that Fill, Outline, and Thumbnail are special names programmed to be looked for in each asset)
* Assets -> Species -> Human -> Female -> Front Facing -> Jacket -> none -> noneFill.png, noneOutline.png, noneThumbnail.png (with some assets, you may want a default of 'none', meaning the character doesn't wear anything visible for that asset by default - the program will always place the 'none' named asset, if there is one, at the top of the populated list in the UI)
* Assets -> Species -> Human -> Female -> Front Facing -> Eyes -> eyesBasic -> multicolor -> eyeLeft.png, eyeRight.png (this is an example of supporting heterochromia - note you'll still need eyesBasicFill.png, eyesBasicOutline.png, and eyesBasicThumbnail.png in the eyesBasic folder - also note that image names in a multicolor folder are arbitrary and are what get displayed to the user when choosing what part of a multicolor image to recolor)
* Assets -> Species -> Human -> Female -> defaultCharacterTemplate.zen2dx (each gender within a species can be given a template to set what the default is when setting a character to new, ex: switching from Human species to Elf, or Male gender to Female - templates use the same format as saves, so you can create a template from a character save if desired)

Part of the goal of this open folder system is to make it easy to support mods and modding communities. Character assets can be painlessly added or removed. It also makes the testing of new assets easier on the development end. If you want assets that are more protected from the unwitting user, that is not currently supported, but it is something I'm considering as an addition to open adding/removing, provided I can find a sensible way to do it.
### Asset Design
Example of asset design: You know your character is never going to be larger than 250width/500height, so you set this as the "character frame" size in the program. You then make a 250width/500height image and use this with layers in image editor of your choice (ex: Paint.NET, GIMP, Photoshop) to design various assets for the character. For each asset you design, you make a pos.zen2dpos (plaintext) file along with it, with x=YourNumber on line 1 and y=YourNumber on line 2, where YourNumber is the position of the asset in the character frame. Then you crop the asset based on this position when saving it as a PNG in the image editing program. This allows you to:
* Make asset pieces no larger than their outline boundaries, if desired, while ensuring they are positioned accurately relative to a set "character frame" box
* Have relative size/pos of assets and "character frame" size/position for use in things like collision detection when using your assets in a game.

### Further Examples
If any of the explanations of usage here are too confusing or time-consuming to extrapolate based on words alone, you can see examples of usage in the latest release version of the program here: https://robertjaymyers.github.io/apps/character-creator-2d-prototype.html

The latest release version is based on the version of the code up to, and including, the commit made on July 22, 2020: "Fix save load to remove/add relevant gender & pose UI." Sometimes the release version may be behind the source code on github.

## License
Zen Character Creator 2D is licensed under GPL v3.0. You can read about the license here to decide if it's something you can work with in considering whether to use this tool for your project: https://www.gnu.org/licenses/gpl-3.0.en.html

## Build Info
Zen Character Creator 2D is built with C++ and Qt Widgets library, making it largely cross-compatible for PCs, so long as you have an OS of the desired type to compile it on. This build was made and tested on Windows 10 in Visual Studio 2017, using the Qt VS Tools extension.
