# 3D-Viewer
Implementation of  3D Viewer on C/C++

## Contents

0. [Information](#information)
1. [3D Viewer Interface](#3d-viewer-interface)



## Information

In this project I implement a program to view 3D wireframe models (3D Viewer) in the C programming language. The models themselves must be loaded from .obj files and be viewable on the screen with the ability to rotate, scale and translate.

- The program was developed in C language of C11 standard using gcc compiler, using additional QT libraries and modules
- The program code located in the src folder
- The program built with Makefile which contains standard set of targets for GNU-programs: all, install, uninstall, clean, dvi, dist, tests, gcov. Installation directory could be arbitrary, except the building one
- The program developed according to the principles of structured programming
- The program code written in Google style
- Full coverage of modules related to model loading and affine transformations with unit-tests
- Only one model on the screen at a time
- The program provides the ability to:
    - Load a wireframe model from an obj file (vertices and surfaces list support only).
    - Translate the model by a given distance in relation to the X, Y, Z axes.
    - Rotate the model by a given angle relative to its X, Y, Z axes.
    - Scale the model by a given value.
- GUI implementation, based on any GUI library `Qt` with API for C89/C99/C11
- The graphical user interface contains:
    - A button to select the model file and a field to output its name.
    - A visualisation area for the wireframe model.
    - Button/buttons and input fields for translating the model.
    - Button/buttons and input fields for rotating the model.
    - Button/buttons and input fields for scaling the model.
    - Information about the uploaded model - file name, number of vertices and edges.
- The program correctly process and allow user to view models with details up to 100, 1000, 10,000, 100,000, 1,000,000  vertices without freezing (a freeze is an interface inactivity of more than 0.5 seconds)
- The program allows customizing the type of projection (parallel and central)
- The program allows setting up the type (solid, dashed), color and thickness of the edges, display method (none, circle, square), color and size of the vertices
- The program allows choosing the background color
- Settings are saved between program restarts
- The program allows saving the captured (rendered) images as bmp and jpeg files.
- The program allows recording small screencasts by a special button - the current custom affine transformation of the loaded object into gif-animation (640x480, 10fps, 5s)


## 3D Viewer Interface

![alt text](<docs/viewer.gif>)