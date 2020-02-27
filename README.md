# Gallex
**Solar system simulator.**\
This is my school project that allows you to create a simulation of the solar system. \
![create planet menu](https://github.com/archylex/Gallex/blob/master/screenshots/2.png)\
**The project uses libraries:**
* GLUT - a library for managing the window manager and input interface
* GLEW - an OpenGL extension library for using shaders
* GLM - OpenGL Mathematics (This library completely replaced my mathematical functions from the project)
* FreeImage - a library for support graphics image formats like PNG, BMP, JPEG...
## Settings
* settings.conf - configuration file, which sets the screen resolution
* stars.xml - base of stars that surround the solar system \
Add your pictures to the appropriate folders. \
**Folders:**
* data/textures/planet/ - planet textures
* data/textures/satellite/ - satellite textures
* data/textures/ring/ - planet rings
* data/textures/system/ - background, bokeh and cursor
* data/fonts/ - font files can be changed as desired
## Controls
**Mouse:**
* Right Button - camera rotation around the system
* Middle Button - zoom in and out
* Left Button - select an object

**Keyboard:**
* f - focus on the selected object
* r - reset camera position
* i - show/hide info on selected object

***To call up the menu press 'Esc' or click on the bottom button.*** \
\
![create planet menu](https://github.com/archylex/Gallex/blob/master/screenshots/3.png)
 \
 \
**In the Settings menu:**
* Time - set time step per second (sec)
* System scale - scale radius and distance (km)
* Distance scale - scale the distances between celestial bodies, but not their radius (km)
