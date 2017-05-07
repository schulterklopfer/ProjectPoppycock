# Project Poppycock (pre alpha)
## Abstract
A visual editor for quickly creating lighting effects on arbitrary, 3d structures using point lights.
![Demo Project Poppycock pre alpha](https://github.com/schulterklopfer/ProjectPoppycock/raw/master/demo.gif)
## Installation
put this repo into your openframeworks 0.9.8 apps/myApps folder.
You will need the following addons:
* ofxAssets
* ofxImGui ()
* ofxXmlSettings
## Usage
* Hit space or enter to open a popup menu. Select one of the options in the menu by mouse click or arrow key following Enter.
Its also possible to type a search string for filtering the list.
* Hold down shortcut key (Windows: Ctrl, Mac: CMD) and drag outside of a node to scroll the patch.
* Use mousewheel to zoom in and out.
* Hold down shortcut key and drag onto another node to create a connection. The connection always flows from the srouce node to the target node.
Observer nodes only take inputs and have no outputs.
Effect nodes take one input and can have multiple outputs.
* Use backspace to delete either a node or a connection.
* As soon as you connect an effect to an observer, the effect will have an area of effect. Move the little handle to resize the area.