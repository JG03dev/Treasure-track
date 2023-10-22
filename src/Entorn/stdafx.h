// stdafx.h: archivo de inclusión para archivos de inclusión estándar del sistema,
// o archivos de inclusión específicos del proyecto utilizados frecuentemente,
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


// VGI: Include llibreria GLFW
#include <GLFW/glfw3.h>

// Include all GLM core / GLSL features
#include <glm/glm.hpp>		// perspective, translate, rotate
// Include all GLM extensions
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

// VGI: Llibreries OpenGL
#include "glut_geometry.h"			// Fonts dels Objectes de la llibreria freeglut

// VGI: Llibreria SOIL2 (actualització de SOIL) per llegir imatges de diferents formats 
//     (BMP,JPG,TIF,GIF,etc.) en la funció loadIMA (visualitzacio.cpp)
#include <SOIL2/SOIL2.h>

// VGI: Includes per lectura fitxers, funcions trigonomètriques i nombres aleatoris.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <string>

// VGI: Constants de l'aplicació EntornVGI
#include "constants.h"

// Includes extra del nou entorn
#include "../Mesh.h"
//#include "../Shader.h"
#include "../Window.h"
#include "../Camera.h"

// LLibreria de Open AL per a sounds 3D
#include <AL/al.h>
#include <AL/alc.h>

// Desactivar en llistat compilació warning C4244: 'argumento': conversión de 'double' a 'GLfloat'; posible pérdida de datos
#  pragma warning (disable:4244)  // Disable bogus VC++ 4.2 conversion warnings.
#  pragma warning (disable:4305)  // VC++ 5.0 version of above warning.
#  pragma warning (disable:4473)  // Disable strpcpy deprecated command in objLoader.cpp
#  pragma warning (disable:4099)  // Warning 'no se encontro vc120.pdb
#  pragma warning (disable:4996)  // Disable strpcpy deprecated command in objLoader.cpp