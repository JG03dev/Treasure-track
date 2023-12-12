// stdafx.h: archivo de inclusi�n para archivos de inclusi�n est�ndar del sistema,
// o archivos de inclusi�n espec�ficos del proyecto utilizados frecuentemente,
// pero cambiados rara vez

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Excluir material rara vez utilizado de encabezados de Windows
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
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
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

/// Llibreries standard de C++
//Streaming
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <cstddef>
//System&Utilities
#include <math.h>
#include <time.h>
#include <memory>
#include <malloc.h>
//Datastructures
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <inttypes.h>

// LLibreria de Open AL per a sounds 3D
#include <AL/al.h>
#include <AL/alc.h>
#include <AL\alext.h>
#include <sndfile.h>

//Llibreria d'importacio de imatges stb_image
#include "stb_image.h"

//Llibreria d'importacio de models
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//Llibreria per la UI
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

// Desactivar en llistat compilaci� warning C4244: 'argumento': conversi�n de 'double' a 'GLfloat'; posible p�rdida de datos
#  pragma warning (disable:4244)  // Disable bogus VC++ 4.2 conversion warnings.
#  pragma warning (disable:4305)  // VC++ 5.0 version of above warning.
#  pragma warning (disable:4473)  // Disable strpcpy deprecated command in objLoader.cpp
#  pragma warning (disable:4099)  // Warning 'no se encontro vc120.pdb
#  pragma warning (disable:4996)  // Disable strpcpy deprecated command in objLoader.cpp
#  pragma warning (disable:4477)  // Disable snprintf model for printing indexes for shader uniform assign
#  pragma warning (disable:4313)  // Warning for printf when printing GLenum as a string