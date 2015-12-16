# DSTDedicatedServerGUI
A Don't Starve Together Dedicated Server GUI.

**Still in heavy development. Backup your server before using it!**

![imgur](http://i.imgur.com/QkBdMd3.png)

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

### Starting a new server

**Still in heavy development. Backup your server before using it!**

1. Install Don't Starve Together dedicated server. (Downloading by Steam client is recommended.)
2. Make sure the template folder is in the same directory as the executable.
3. Open don't starve together and type "~" to open a console. After that, type ``TheNet:GenerateServerToken()``. The server_token.txt will be generate. (You can also assign the path manually if you already have one somewhere else.)
4. Open the DSTDedicatedServerGUI
5. Go to Basic Setup tab.
6. Setup the root directory of the dedicated server. (Where you can see folders like bin, mods...)
7. Check if the path of the server_token.txt is assigned.
8. Configure your new server.
  ![Imgur](http://i.imgur.com/KQ8ek5s.png)
9. Go to Console tab.
10. Press start server (啟動伺服器).
  ![Imgur](http://i.imgur.com/oXH7T2y.png)
11. Enjoy!
  ![Imgur](http://i.imgur.com/CASjY5R.png)

### Use an existing server

**Still in heavy development. Backup your server before using it!**

1. Install Don't Starve Together dedicated server. (Downloading by Steam client is recommended.)
2. Make sure the template folder is in the same directory as the executable.
3. Open don't starve together and type "~" to open a console. After that, type ``TheNet:GenerateServerToken()``. The server_token.txt will be generate. (You can also assign the path manually if you already have one somewhere else.)
4. Rename you existing server folder names (server and cave) to **gui_dst_server** and **gui_dst_cave**
5. Open the DSTDedicatedServerGUI
6. Go to Basic Setup tab.
7. Setup the root directory of the dedicated server. (Where you can see folders like bin, mods...)
8. Check if the path of the server_token.txt is assigned.
9. Go to Console tab.
10. Press start server (啟動伺服器).
  ![Imgur](http://i.imgur.com/oXH7T2y.png)
11. Enjoy!
  ![Imgur](http://i.imgur.com/CASjY5R.png)


## Q & A

Q: Does this GUI supports adding mods?

A: Currently adding mods when creating new server is supported, but you'll have to manually edit the modoverrides.lua and put it into the template folder. The app supports updating mods, but you'll have to modify the dedicated_server_mods_setup.lua located at your dedicated server mods directory.

Q: What's the language of the GUI?

A: Currently it supports Traditional Chinese and English. I use Qt's Locale to detect the system automatically.

## Future work

1. Able to manage mods in GUI.
2. Able to use custom server data folder path. ( Currently use the default path.)
