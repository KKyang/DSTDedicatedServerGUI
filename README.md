# DSTDedicatedServerGUI
A Don't Starve Together Dedicated Server GUI.

**Still in heavy development. Backup your server before using it!**

## Minimum Requirements

1. A PC running Windows
2. A dedicated server with cave or after.

## What can it do

1. One click to start a new server.
2. Able to change basic server settings. ( ex. Server name, descriptions...)
3. Able to change world settings in GUI. ( ex. The amount of flint...)

## Install

You may need the Visual Studio 2013 Redistributable if lack of vc related DLL: https://www.microsoft.com/zh-TW/download/details.aspx?id=40784

### Pre-built binaries

1. Download the app from Github.
2. Extract somewhere else in your PC.

### Build it yourself

1. Download and install Qt (> 5.5) and Visual Studio (> 2013).
2. Download the source code.
3. Double-click the *.pro file.
4. Configure your QT settings.
5. Compile!

## Usage

1. Make sure the template folder is in the same directory as the executable.
2. Currently adding mods when creating new server is supported, but you'll have to manually edit the modoverrides.lua and put it into the template folder. The app supports updating mods, but you'll have to modify the dedicated_server_mods_setup.lua located at your dedicated server mods directory.
3. The app automatically search the token file in the default path. You can also assign the path manually.
4. You'll have to tell the app where the dedicated server's root directory location is.

## Future work

1. Able to manage mods in GUI.
2. Able to use custom server data folder path. ( Currently use the default path.)
