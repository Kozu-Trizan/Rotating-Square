# The Background
Inspired from the famous donut.c project featuring a spinning donut (torus) in console using C programming language I decide to give something similar (less cooler) a try. Similar projects already exist on the internet.
However this project is built from scratch (so if you find it clumsy or unprofessional at some point; it was inevitable) as this was all done for the purpose of learning only (and to do something cool ><)

# Compatibility
This project uses windows API via <code><windows.h></code> header file hence this is only compatible for windows operating system.

# Features
The <code>DIMENSION</code> feature allows to change the size of the cube
The <code>x_offset</code>, <code>y_offset</code>, <code>z_offset</code> values determine the cube's position. Initialized at <code>0</code> the cube is positioned
with its one corner at the origin and its base resting on the XZ plane. 
<br>I have assumed the following coordinate system:
<br>Horizontal -> X-axis
<br>Vertical -> Y-axis
<br>Outwards from screen -> Z-axis

<b>All transformations are made on this assumptions.</b>

Also you can animate the offset of the cube by uncommenting the animation code at the beginning and end of the <code>while(1)</code> loop.
