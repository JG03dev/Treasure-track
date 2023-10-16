// stdafx.h: archivo de inclusi�n para archivos de inclusi�n est�ndar del sistema,
// o archivos de inclusi�n espec�ficos del proyecto utilizados frecuentemente,
// pero cambiados rara vez

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Excluir material rara vez utilizado de encabezados de Windows
#endif

/* ------------------------------------------------------------------------- */
/*                                INCLUDES                                   */
/* ------------------------------------------------------------------------- */
// VGI: Includes llibreria GLEW 2.1.0 per a Win32
//#define GLEW_STATIC
#define GLEW_NO_GLU	// Sense usar llibreria GLU
#include <gl/glew.h>
#include <gl/wglew.h>
//#pragma comment(lib, "../lib/glew32.lib")	// Link against GLEW library 
//#pragma comment(lib, "../lib/glew32s.lib")// Link against GLEW library 

// VGI: Include llibreria GLFW
#include <GLFW/glfw3.h>

// Include all GLM core / GLSL features
#include <glm/glm.hpp>		// perspective, translate, rotate
// Include all GLM extensions
#include <glm/ext.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
//#include <glm/gtx/norm.hpp>
using namespace glm;

// VGI: Llibreries OpenGL
//#include <gl/gl.h>
//#pragma comment(lib, "../lib/opengl32.lib")	// Link against GL library  
//#include <gl/glaux.h>
//#include <gl/glu.h>
//#pragma comment(lib, "../lib/glu32.lib")	// Link against GLU library 
//#include <gl/glut.h>
#include "glut_geometry.h"			// Fonts dels Objectes de la llibreria freeglut

/*
// Include AntTweakBar
#include <AntTweakBar.h>
*/

// VGI: Llibreria SOIL2 (actualitzaci� de SOIL) per llegir imatges de diferents formats 
//     (BMP,JPG,TIF,GIF,etc.) en la funci� loadIMA (visualitzacio.cpp)
//#include <gl/SOIL.h>
#include <SOIL2/SOIL2.h>

// VGI: Includes per lectura fitxers, funcions trigonom�triques i nombres aleatoris.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <string>

// VGI: Constants de l'aplicaci� EntornVGI
#include "constants.h"

// Desactivar en llistat compilaci� warning C4244: 'argumento': conversi�n de 'double' a 'GLfloat'; posible p�rdida de datos
#  pragma warning (disable:4244)  // Disable bogus VC++ 4.2 conversion warnings.
#  pragma warning (disable:4305)  // VC++ 5.0 version of above warning.
#  pragma warning (disable:4473)  // Disable strpcpy deprecated command in objLoader.cpp
#  pragma warning (disable:4099)  // Warning 'no se encontro vc120.pdb
#  pragma warning (disable:4996)  // Disable strpcpy deprecated command in objLoader.cpp