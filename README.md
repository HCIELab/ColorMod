# ColorMod
Code from ColorMod: Recoloring 3D Printed Objects using Photochromic Inks (ACM CHI2018)

Please refer to **our project [website]**(http://hcie.csail.mit.edu/research/colormod/colormod.html) for more details. 

### Overview
This code has TWO seperate parts, **printing** and **painting** (included controller for rotating platform). 

*Printing*: this section provided the code for preparing the 3D model for multi-materials 3D printing. Specifically, we convert the triangular mesh to voxel-based representation. 
Then, assign each voxel with the different type of material. 

*Painting*: this part of the code provided a user interface (UI) that integrated with Blender software that allows users to paint the color they want on to the 3D model. 
Our algorithm match the painted color with the color voxel of a 3D printed object, and assign the projection-mapping pattern to activate the color of specific part. 

### Software
1. Blender 2.7 (or later)
1. Python (work either 2 and 3)
1. Visual Studio 2017 (or later)