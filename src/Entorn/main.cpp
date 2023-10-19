//******** PRACTICA VISUALITZACIÓ GRÀFICA INTERACTIVA (Escola Enginyeria - UAB)
//******** Entorn bàsic VS2022 MONOFINESTRA amb OpenGL 4.3+, interfície GLFW, ImGui i llibreries GLM
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (Setembre 2023)
// main.cpp : Definició de main
//    Versió 0.5:	- Interficie ImGui
//					- Per a dialeg de cerca de fitxers, s'utilitza la llibreria NativeFileDialog


// Entorn VGI.ImGui: Includes llibreria ImGui
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_glfw.h"
#include "ImGui\imgui_impl_opengl3.h"
#include "ImGui\nfd.h" // Native File Dialog

#include "stdafx.h"
#include "shader.h"
#include "visualitzacio.h"
#include "escena.h"
#include "main.h"


void InitGL()
{
// TODO: agregar aquí el código de construcción

//------ Entorn VGI: Inicialització de les variables globals de CEntornVGIView
	int i;

// Entorn VGI: Variable de control per a Status Bar (consola) 
	statusB = false;

// Entorn VGI: Variables de control per Menú Càmera: Esfèrica, Navega, Mòbil, Zoom, Satelit, Polars... 
	camera = CAM_ESFERICA;
	mobil = true;	zzoom = true;		zzoomO = false;		satelit = false;

// Entorn VGI: Variables de control de l'opció Càmera->Navega?
	n[0] = 0.0;		n[1] = 0.0;		n[2] = 0.0;
	opvN.x = 10.0;	opvN.y = 0.0;		opvN.z = 0.0;
	angleZ = 0.0;
	ViewMatrix = glm::mat4(1.0);		// Inicialitzar a identitat

// Entorn VGI: Variables de control de l'opció Càmera->Geode?
	OPV_G.R = 15.0;		OPV_G.alfa = 0.0;	OPV_G.beta = 0.0;	// Origen PV en esfèriques per a Vista_Geode

// Entorn VGI: Variables de control per Menú Vista: Pantalla Completa, Pan, dibuixar eixos i grids 
	fullscreen = false;
	pan = false;
	eixos = true;	eixos_programID = 0;  eixos_Id = 0;
	sw_grid = false;
	grid.x = false;	grid.y = false;		grid.z = false;		grid.w = false;
	hgrid.x = 0.0;	hgrid.y = 0.0;		hgrid.z = 0.0;		hgrid.w = 0.0;

// Entorn VGI: Variables opció Vista->Pan
	fact_pan = 1;
	tr_cpv.x = 0;	tr_cpv.y = 0;	tr_cpv.z = 0;		tr_cpvF.x = 0;	tr_cpvF.y = 0;	tr_cpvF.z = 0;

// Entorn VGI: Variables de control per les opcions de menú Projecció, Objecte
	projeccio = CAP;	// projeccio = PERSPECT;
	ProjectionMatrix = glm::mat4(1.0);	// Inicialitzar a identitat
	objecte = CAP;		// objecte = TETERA;

// Entorn VGI: Variables de control Skybox Cube
	SkyBoxCube = false;		skC_programID = 0;
	skC_VAOID.vaoId = 0;	skC_VAOID.vboId = 0;	skC_VAOID.nVertexs = 0;
	cubemapTexture = 0;

// Entorn VGI: Variables de control del menú Transforma
	transf = false;		trasl = false;		rota = false;		escal = false;
	fact_Tras = 1;		fact_Rota = 90;
	TG.VTras.x = 0.0;	TG.VTras.y = 0.0;	TG.VTras.z = 0;	TGF.VTras.x = 0.0;	TGF.VTras.y = 0.0;	TGF.VTras.z = 0;
	TG.VRota.x = 0;		TG.VRota.y = 0;		TG.VRota.z = 0;	TGF.VRota.x = 0;	TGF.VRota.y = 0;	TGF.VRota.z = 0;
	TG.VScal.x = 1;		TG.VScal.y = 1;		TG.VScal.z = 1;	TGF.VScal.x = 1;	TGF.VScal.y = 1;	TGF.VScal.z = 1;

	transX = false;		transY = false;		transZ = false;
	GTMatrix= glm::mat4(1.0);		// Inicialitzar a identitat

// Entorn VGI: Variables de control per les opcions de menú Ocultacions
	front_faces = true;	test_vis = false;	oculta = false;		back_line = false;

// Entorn VGI: Variables de control del menú Iluminació		
	ilumina = FILFERROS;			ifixe = false;					ilum2sides = false;
// Reflexions actives: Ambient [1], Difusa [2] i Especular [3]. No actives: Emission [0]. 
	sw_material[0] = false;			sw_material[1] = true;			sw_material[2] = true;			sw_material[3] = true;	sw_material[4] = true;
	sw_material_old[0] = false;		sw_material_old[1] = true;		sw_material_old[2] = true;		sw_material_old[3] = true;	sw_material_old[4] = true;
	textura = false;				t_textura = CAP;				textura_map = true;
	for (i = 0; i < NUM_MAX_TEXTURES; i++) texturesID[i] = -1;
	tFlag_invert_Y = false;

// Entorn VGI: Variables de control del menú Llums
// Entorn VGI: Inicialització variables Llums
	llum_ambient = true;
	for (i = 1; i < NUM_MAX_LLUMS; i++) llumGL[i].encesa = false;
	for (i = 0; i < NUM_MAX_LLUMS; i++) {
		llumGL[i].encesa = false;
		llumGL[i].difusa[0] = 1.0f;	llumGL[i].difusa[1] = 1.0f;	llumGL[i].difusa[2] = 1.0f;	llumGL[i].difusa[3] = 1.0f;
		llumGL[i].especular[0] = 1.0f; llumGL[i].especular[1] = 1.0f; llumGL[i].especular[2] = 1.0f; llumGL[i].especular[3] = 1.0f;
		}

// LLum 0: Atenuació constant (c=1), sobre l'eix Z, no restringida.
	llumGL[0].encesa = true;
	llumGL[0].difusa[0] = 1.0f;			llumGL[0].difusa[1] = 1.0f;			llumGL[0].difusa[2] = 1.0f;		llumGL[0].difusa[3] = 1.0f;
	llumGL[0].especular[0] = 1.0f;		llumGL[0].especular[1] = 1.0f;		llumGL[0].especular[2] = 1.0f;	llumGL[0].especular[3] = 1.0f;

	llumGL[0].posicio.R = 200.0;		llumGL[0].posicio.alfa = 90.0;		llumGL[0].posicio.beta = 0.0;		// Posició llum (x,y,z)=(0,0,200)
	llumGL[0].atenuacio.a = 0.0;		llumGL[0].atenuacio.b = 0.0;		llumGL[0].atenuacio.c = 1.0;		// Llum sense atenuació per distància (a,b,c)=(0,0,1)
	llumGL[0].restringida = false;
	llumGL[0].spotdirection[0] = 0.0;	llumGL[0].spotdirection[1] = 0.0;	llumGL[0].spotdirection[2] = 0.0;
	llumGL[0].spotcoscutoff = 0.0;		llumGL[0].spotexponent = 0.0;

// LLum 1: Atenuació constant (c=1), sobre l'eix X, no restringida.
	llumGL[1].encesa = false;
	llumGL[1].difusa[0] = 1.0f;			llumGL[1].difusa[1] = 1.0f;			llumGL[1].difusa[2] = 1.0f;		llumGL[1].difusa[3] = 1.0f;
	llumGL[1].especular[0] = 1.0f;		llumGL[1].especular[1] = 1.0f;		llumGL[1].especular[2] = 1.0f;	llumGL[1].especular[3] = 1;

	llumGL[1].posicio.R = 75.0;			llumGL[1].posicio.alfa = 0.0;		llumGL[1].posicio.beta = 0.0;// (x,y,z)=(75,0,0)
	llumGL[1].atenuacio.a = 0.0;		llumGL[1].atenuacio.b = 0.0;		llumGL[1].atenuacio.c = 1.0;
	llumGL[1].restringida = false;
	llumGL[1].spotdirection[0] = 0.0;	llumGL[1].spotdirection[1] = 0.0;	llumGL[1].spotdirection[2] = 0.0;
	llumGL[1].spotcoscutoff = 0.0;		llumGL[1].spotexponent = 0.0;

// LLum 2: Atenuació constant (c=1), sobre l'eix Y, no restringida.
	llumGL[2].encesa = false;
	llumGL[2].difusa[1] = 1.0f;			llumGL[2].difusa[1] = 1.0f;			llumGL[2].difusa[2] = 1.0f;		llumGL[2].difusa[3] = 1.0f;
	llumGL[2].especular[1] = 1.0f;		llumGL[2].especular[1] = 1.0f;		llumGL[2].especular[2] = 1.0f;	llumGL[2].especular[3] = 1;

	llumGL[2].posicio.R = 75.0;			llumGL[2].posicio.alfa = 0.0;		llumGL[2].posicio.beta = 90.0;	// (x,y,z)=(0,75,0)
	llumGL[2].atenuacio.a = 0.0;		llumGL[2].atenuacio.b = 0.0;		llumGL[2].atenuacio.c = 1.0;
	llumGL[2].restringida = false;
	llumGL[2].spotdirection[0] = 0.0;	llumGL[2].spotdirection[1] = 0.0;	llumGL[2].spotdirection[2] = 0.0;
	llumGL[2].spotcoscutoff = 0.0;		llumGL[2].spotexponent = 0.0;

// LLum 3: Atenuació constant (c=1), sobre l'eix Y=X, no restringida.
	llumGL[3].encesa = false;
	llumGL[3].difusa[0] = 1.0f;			llumGL[2].difusa[1] = 1.0f;			llumGL[3].difusa[2] = 1.0f;		llumGL[3].difusa[3] = 1.0f;
	llumGL[3].especular[0] = 1.0f;		llumGL[2].especular[1] = 1.0f;		llumGL[3].especular[2] = 1.0f;	llumGL[3].especular[3] = 1;

	llumGL[3].posicio.R = 75.0;			llumGL[3].posicio.alfa = 45.0;		llumGL[3].posicio.beta = 45.0;// (x,y,z)=(75,75,75)
	llumGL[3].atenuacio.a = 0.0;		llumGL[3].atenuacio.b = 0.0;		llumGL[3].atenuacio.c = 1.0;
	llumGL[3].restringida = false;
	llumGL[3].spotdirection[0] = 0.0;	llumGL[3].spotdirection[1] = 0.0;	llumGL[3].spotdirection[2] = 0.0;
	llumGL[3].spotcoscutoff = 0.0;		llumGL[3].spotexponent = 0.0;

// LLum 4: Atenuació constant (c=1), sobre l'eix -Z, no restringida.
	llumGL[4].encesa = false;
	llumGL[4].difusa[0] = 1.0f;			llumGL[4].difusa[1] = 1.0f;			llumGL[4].difusa[2] = 1.0f;		llumGL[4].difusa[3] = 1.0f;
	llumGL[4].especular[0] = 1.0f;		llumGL[4].especular[1] = 1.0f;		llumGL[4].especular[2] = 1.0f;	llumGL[4].especular[3] = 1;

	llumGL[4].posicio.R = 75.0;			llumGL[4].posicio.alfa = -90.0;		llumGL[4].posicio.beta = 0.0;// (x,y,z)=(0,0,-75)
	llumGL[4].atenuacio.a = 0.0;		llumGL[4].atenuacio.b = 0.0;		llumGL[4].atenuacio.c = 1.0;
	llumGL[4].restringida = false;
	llumGL[4].spotdirection[0] = 0.0;	llumGL[4].spotdirection[1] = 0.0;	llumGL[4].spotdirection[2] = 0.0;
	llumGL[4].spotcoscutoff = 0.0;		llumGL[4].spotexponent = 0.0;

// LLum #5:
	llumGL[5].encesa = false;
	llumGL[5].difusa[0] = 1.0f;			llumGL[5].difusa[1] = 1.0f;			llumGL[5].difusa[2] = 1.0f;		llumGL[5].difusa[3] = 1.0f;
	llumGL[5].especular[0] = 1.0f;		llumGL[5].especular[1] = 1.0f;		llumGL[5].especular[2] = 1.0f;	llumGL[5].especular[3] = 1;

	llumGL[5].posicio.R = 75.0;			llumGL[5].posicio.alfa = 0.0;		llumGL[5].posicio.beta = -90.0; // (x,y,z)=(0,-75,0)
	llumGL[5].atenuacio.a = 0.0;		llumGL[5].atenuacio.b = 0.0;		llumGL[5].atenuacio.c = 1.0;
	llumGL[5].restringida = false;
	llumGL[5].spotdirection[0] = 0.0;	llumGL[5].spotdirection[1] = 0.0;	llumGL[5].spotdirection[2] = 0.0;
	llumGL[5].spotcoscutoff = 0.0;		llumGL[5].spotexponent = 0.0;

// LLum #6: Llum Vaixell, configurada a la funció vaixell() en escena.cpp.
	llumGL[6].encesa = false;
	llumGL[6].difusa[0] = 1.0f;			llumGL[6].difusa[1] = 1.0f;			llumGL[6].difusa[2] = 1.0f;		llumGL[6].difusa[3] = 1.0f;
	llumGL[6].especular[0] = 1.0f;		llumGL[6].especular[1] = 1.0f;		llumGL[6].especular[2] = 1.0f;	llumGL[6].especular[3] = 1;

	llumGL[6].posicio.R = 0.0;			llumGL[6].posicio.alfa = 0.0;		llumGL[6].posicio.beta = 0.0; // Cap posició definida, definida en funció vaixell() en escena.cpp
	llumGL[6].atenuacio.a = 0.0;		llumGL[6].atenuacio.b = 0.0;		llumGL[6].atenuacio.c = 1.0;
	llumGL[6].restringida = false;
	llumGL[6].spotdirection[0] = 0.0;	llumGL[6].spotdirection[1] = 0.0;	llumGL[6].spotdirection[2] = 0.0;
	llumGL[6].spotcoscutoff = 0.0;		llumGL[6].spotexponent = 0.0;

// LLum #7: Llum Far, configurada a la funció faro() en escena.cpp.
	llumGL[7].encesa = false;
	llumGL[7].difusa[0] = 1.0f;			llumGL[7].difusa[1] = 1.0f;			llumGL[7].difusa[2] = 1.0f;		llumGL[7].difusa[3] = 1.0f;
	llumGL[7].especular[0] = 1.0f;		llumGL[7].especular[1] = 1.0f;		llumGL[7].especular[2] = 1.0f;	llumGL[7].especular[3] = 1;

	llumGL[7].posicio.R = 0.0;			llumGL[7].posicio.alfa = 0.0;		llumGL[7].posicio.beta = 0.0; // Cap posició definida, definida en funció faro() en escena.cpp
	llumGL[7].atenuacio.a = 0.0;		llumGL[7].atenuacio.b = 0.0;		llumGL[7].atenuacio.c = 1.0;
	llumGL[7].restringida = false;
	llumGL[7].spotdirection[0] = 0.0;	llumGL[7].spotdirection[1] = 0.0;	llumGL[7].spotdirection[2] = 0.0;
	llumGL[7].spotcoscutoff = 0.0;		llumGL[7].spotexponent = 0.0;

// Entorn VGI: Variables de control del menú Shaders
	shader = CAP_SHADER;  shader_programID = 0;	
	shaderLighting.releaseAllShaders();
	fprintf(stderr, "Gouraud_shdrML: \n");
	if (!shader_programID) shader_programID = shaderLighting.loadFileShaders(".\\shaders\\gouraud_shdrML.vert", ".\\shaders\\gouraud_shdrML.frag");
	shader = GOURAUD_SHADER;


// Càrrega SHADERS
// Càrrega Shader Eixos
	fprintf(stderr, "Eixos: \n");
	if (!eixos_programID) eixos_programID = shaderEixos.loadFileShaders(".\\shaders\\eixos.VERT", ".\\shaders\\eixos.FRAG");

// Càrrega Shader Skybox
	fprintf(stderr, "SkyBox: \n");
	if (!skC_programID) skC_programID = shader_SkyBoxC.loadFileShaders(".\\shaders\\skybox.VERT", ".\\shaders\\skybox.FRAG");

// Càrrega VAO Skybox Cube
	if (skC_VAOID.vaoId == 0) skC_VAOID = loadCubeSkybox_VAO();
	Set_VAOList(CUBE_SKYBOX, skC_VAOID);

	if (!cubemapTexture)
	{	// load Skybox textures
		// -------------
		std::vector<std::string> faces =
		{ ".\\textures\\skybox\\right.jpg",
			".\\textures\\skybox\\left.jpg",
			".\\textures\\skybox\\top.jpg",
			".\\textures\\skybox\\bottom.jpg",
			".\\textures\\skybox\\front.jpg",
			".\\textures\\skybox\\back.jpg"
		};
		cubemapTexture = loadCubemap(faces);
	}

// Entorn VGI: Variables de control dels botons de mouse
	m_PosEAvall.x = 0;			m_PosEAvall.y = 0;			m_PosDAvall.x = 0;			m_PosDAvall.y = 0;
	m_ButoEAvall = false;		m_ButoDAvall = false;
	m_EsfeEAvall.R = 0.0;		m_EsfeEAvall.alfa = 0.0;	m_EsfeEAvall.beta = 0.0;
	m_EsfeIncEAvall.R = 0.0;	m_EsfeIncEAvall.alfa = 0.0;	m_EsfeIncEAvall.beta = 0.0;

// Entorn VGI: Variables que controlen paràmetres visualització: Mides finestra Windows i PV
	w = 640;			h = 480;			// Mides de la finestra Windows (w-amplada,h-alçada)
	width_old = 640;	height_old = 480;	// Mides de la resolució actual de la pantalla (finestra Windows)
	w_old = 640;		h_old = 480;		// Mides de la finestra Windows (w-amplada,h-alçada) per restaurar Finestra des de fullscreen
	OPV.R = cam_Esferica[0];	OPV.alfa = cam_Esferica[1];		OPV.beta = cam_Esferica[2];		// Origen PV en esfèriques
	//OPV.R = 15.0;		OPV.alfa = 0.0;		OPV.beta = 0.0;										// Origen PV en esfèriques
	Vis_Polar = POLARZ;

// Entorn VGI: Color de fons i de l'objecte
	fonsR = true;		fonsG = true;		fonsB = true;
	c_fons.r = clear_colorB.x;		c_fons.g = clear_colorB.y;		c_fons.b = clear_colorB.z;			c_fons.b = clear_colorB.w;
	//c_fons.r = 0.0;		c_fons.g = 0.0;		c_fons.b = 0.0;			c_fons.b = 1.0;
	sw_color = false;
	col_obj.r = clear_colorO.x;	col_obj.g = clear_colorO.y;	col_obj.b = clear_colorO.z;		col_obj.a = clear_colorO.w;
	//col_obj.r = 1.0;	col_obj.g = 1.0;	col_obj.b = 1.0;		col_obj.a = 1.0;

// Entorn VGI: Objecte OBJ
	ObOBJ = NULL;		vao_OBJ.vaoId = 0;		vao_OBJ.vboId = 0;		vao_OBJ.nVertexs = 0;

// Entorn VGI: OBJECTE --> Corba B-Spline i Bezier
	npts_T = 0;
	for (i = 0; i < MAX_PATCH_CORBA; i = i++)
	{	PC_t[i].x = 0.0;
		PC_t[i].y = 0.0;
		PC_t[i].z = 0.0;
	}

	pas_CS = PAS_BSPLINE;
	sw_Punts_Control = false;

// TRIEDRE DE FRENET / DARBOUX: VT: vector Tangent, VNP: Vector Normal Principal, VBN: vector BiNormal
	dibuixa_TriedreFrenet = false;		dibuixa_TriedreDarboux = false;
	VT = { 0.0, 0.0, 1.0 };		VNP = { 1.0, 0.0, 0.0 };	VBN = { 0.0, 1.0, 0.0 };

// Entorn VGI: Variables del Timer
	t = 0;			anima = false;

// Entorn VGI: Variables de l'objecte FRACTAL
	t_fractal = CAP;	soroll = 'C';
	pas = 64;			pas_ini = 64;
	sw_il = true;		palcolFractal = false;

// Entorn VGI: Altres variables
	mida = 1.0;			nom = "";		buffer = "";
	initVAOList();	// Inicialtzar llista de VAO'S.
}


void InitAPI()
{
// Vendor, Renderer, Version, Shading Laguage Version i Extensions suportades per la placa gràfica gravades en fitxer extensions.txt
	std::string nomf = "extensions.txt";
	char const* nomExt = "";
	const char* nomfitxer;
	nomfitxer = nomf.c_str();	// Conversió tipus string --> char *
	int num_Ext;

	char* str = (char*)glGetString(GL_VENDOR);
	FILE* f = fopen(nomfitxer, "w");
	if(f)	{	fprintf(f,"VENDOR: %s\n",str);
				fprintf(stderr, "VENDOR: %s\n", str);
				str = (char*)glGetString(GL_RENDERER);
				fprintf(f, "RENDERER: %s\n", str);
				fprintf(stderr, "RENDERER: %s\n", str);
				str = (char*)glGetString(GL_VERSION);
				fprintf(f, "VERSION: %s\n", str);
				fprintf(stderr, "VERSION: %s\n", str);
				str = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
				fprintf(f, "SHADING_LANGUAGE_VERSION: %s\n", str);
				fprintf(stderr, "SHADING_LANGUAGE_VERSION: %s\n", str);
				glGetIntegerv(GL_NUM_EXTENSIONS, &num_Ext);
				fprintf(f, "EXTENSIONS: \n");
				fprintf(stderr, "EXTENSIONS: \n");
				for (int i = 0; i < num_Ext; i++)
				{	nomExt = (char const*)glGetStringi(GL_EXTENSIONS, i);
					fprintf(f, "%s \n", nomExt);
					//fprintf(stderr, "%s", nomExt);	// Displaiar extensions per pantalla
				}
				//fprintf(stderr, "\n");				// Displaiar <cr> per pantalla després extensions
//				str = (char*)glGetString(GL_EXTENSIONS);
//				fprintf(f, "EXTENSIONS: %s\n", str);
				//fprintf(stderr, "EXTENSIONS: %s\n", str);
				fclose(f);
			}

// Program
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
	glUniform1iv = (PFNGLUNIFORM1IVPROC)wglGetProcAddress("glUniform1iv");
	glUniform2iv = (PFNGLUNIFORM2IVPROC)wglGetProcAddress("glUniform2iv");
	glUniform3iv = (PFNGLUNIFORM3IVPROC)wglGetProcAddress("glUniform3iv");
	glUniform4iv = (PFNGLUNIFORM4IVPROC)wglGetProcAddress("glUniform4iv");
	glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
	glUniform1fv = (PFNGLUNIFORM1FVPROC)wglGetProcAddress("glUniform1fv");
	glUniform2fv = (PFNGLUNIFORM2FVPROC)wglGetProcAddress("glUniform2fv");
	glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
	glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
	glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)wglGetProcAddress("glVertexAttrib1f");
	glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC)wglGetProcAddress("glVertexAttrib1fv");
	glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC)wglGetProcAddress("glVertexAttrib2fv");
	glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC)wglGetProcAddress("glVertexAttrib3fv");
	glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)wglGetProcAddress("glVertexAttrib4fv");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
	glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)wglGetProcAddress("glGetActiveUniform");

// Shader
	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");

// VBO
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	glBufferSubData = (PFNGLBUFFERSUBDATAPROC)wglGetProcAddress("glBufferSubData");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
}


void GetGLVersion(int* major, int* minor)
{
	// for all versions
	char* ver = (char*)glGetString(GL_VERSION); // ver = "3.2.0"

	*major = ver[0] - '0';
	if (*major >= 3)
	{
		// for GL 3.x
		glGetIntegerv(GL_MAJOR_VERSION, major);		// major = 3
		glGetIntegerv(GL_MINOR_VERSION, minor);		// minor = 2
	}
	else
	{
		*minor = ver[2] - '0';
	}

	// GLSL
	ver = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);	// 1.50 NVIDIA via Cg compiler
}

void OnSize(GLFWwindow* window, int width, int height)
{
// TODO: Agregue aquí su código de controlador de mensajes

// A resize event occured; cx and cy are the window's new width and height.
// Find the OpenGL change size function given in the Lab 1 notes and call it here

// Entorn VGI: MODIFICACIÓ ->Establim les mides de la finestra actual
	w = width;	h = height;

// Crida a OnPaint per a redibuixar la pantalla
//	OnPaint();
}


// OnPaint: Funció de dibuix i visualització en frame buffer del frame
void OnPaint(GLFWwindow* window)
{
// TODO: Agregue aquí su código de controlador de mensajes
	GLdouble vpv[3] = { 0.0, 0.0, 1.0 };

// Entorn VGI.ImGui: Menú ImGui condicionat al color de fons
	if ((c_fons.r < 0.5) || (c_fons.g < 0.5) || (c_fons.b < 0.5))
		ImGui::StyleColorsLight();
	else ImGui::StyleColorsDark();

// Entorn VGI: Cridem a les funcions de l'escena i la projecció segons s'hagi 
// seleccionat una projecció o un altra
	switch (projeccio)
	{
	case AXONOM:
// Entorn VGI: PROJECCIÓ AXONOMÈTRICA
// Entorn VGI: Activació del retall de pantalla
		glEnable(GL_SCISSOR_TEST);

// Entorn VGI: Retall
		glScissor(0, 0, w, h);
		glViewport(0, 0, w, h);

// Aquí farem les crides per a definir Viewport, Projecció i Càmara: INICI -------------------------

// Aquí farem les cridesper a definir Viewport, Projecció i Càmara:: FI -------------------------
// Entorn VGI: Dibuixar Model (escena)
		configura_Escena();     // Aplicar Transformacions Geometriques segons persiana Transformacio i configurar objectes
		dibuixa_Escena();		// Dibuix geometria de l'escena amb comandes GL.

// Entorn VGI: Transferència del buffer OpenGL a buffer de pantalla
		//glfwSwapBuffers(window);
		break;

	case ORTO:
// Entorn VGI: PROJECCIÓ ORTOGRÀFICA
// Entorn VGI: Activació del retall de pantalla
		glEnable(GL_SCISSOR_TEST);

// Entorn VGI: Retall
		glScissor(0, 0, w, h);
		glViewport(0, 0, w, h);

		// Fons condicionat al color de fons
		if ((c_fons.r < 0.5) || (c_fons.g < 0.5) || (c_fons.b < 0.5))
			FonsB();
		else
			FonsN();

// Entorn VGI: TO DO -> Aquí farem les quatre crides a ProjeccioOrto i Ortografica per obtenir 
//						les quatre vistes ortogràfiques. De moment n'activem només una de prova
//						IMPORTANT: DESCOMENTAR LA RESTA QUAN FUNCIONI LA PRIMERA
// PLANTA (Inferior Esquerra)
		// Definició de Viewport, Projecció i Càmara
		ProjectionMatrix = Projeccio_Orto();
		ViewMatrix = Vista_Ortografica(shader_programID, 0, OPV.R, c_fons, col_obj, objecte, mida, pas, front_faces, oculta,
			test_vis, back_line, ilumina, llum_ambient, llumGL, ifixe, ilum2sides,
			eixos, grid, hgrid);
		// Dibuix de l'Objecte o l'Escena
		configura_Escena();     // Aplicar Transformacions Geometriques segons persiana Transformacio i configurar objectes
		dibuixa_Escena();		// Dibuix geometria de l'escena amb comandes GL.
/*
// ISOMÈTRICA (Inferior Dreta)
		// Definició de Viewport, Projecció i Càmara
		ProjectionMatrix = Projeccio_Orto();
		ViewMatrix = Vista_Ortografica(shader_programID, 3, OPV.R, c_fons, col_obj, objecte, mida, pas, front_faces, oculta,
			test_vis, back_line, ilumina, llum_ambient, llumGL, ifixe, ilum2sides,
			eixos, grid, hgrid);
		// Dibuix de l'Objecte o l'Escena
		configura_Escena();     // Aplicar Transformacions Geometriques segons persiana Transformacio i configurar objectes
		dibuixa_Escena();		// Dibuix geometria de l'escena amb comandes GL.

// ALÇAT (Superior Esquerra)
		// Definició de Viewport, Projecció i Càmara
		ProjectionMatrix = Projeccio_Orto();
		ViewMatrix = Vista_Ortografica(shader_programID, 1, OPV.R, c_fons, col_obj, objecte, mida, pas, front_faces, oculta,
			test_vis, back_line, ilumina, llum_ambient, llumGL, ifixe, ilum2sides,
			eixos, grid, hgrid);
		// Dibuix de l'Objecte o l'Escena
		  configura_Escena();     // Aplicar Transformacions Geom?triques segons persiana Transformacio i configurar objectes
	 	  dibuixa_Escena();		// Dibuix geometria de l'escena amb comandes GL.

// PERFIL (Superior Dreta)
		// Definició de Viewport, Projecció i Càmara
		ProjectionMatrix = Projeccio_Orto();
		ViewMatrix = Vista_Ortografica(shader_programID, 2, OPV.R, c_fons, col_obj, objecte, mida, pas, front_faces, oculta,
			test_vis, back_line, ilumina, llum_ambient, llumGL, ifixe, ilum2sides,
			eixos, grid, hgrid);
		// Dibuix de l'Objecte o l'Escena
		configura_Escena();     // Aplicar Transformacions Geom?triques segons persiana Transformacio i configurar objectes
		  // glScalef();			// Escalat d'objectes, per adequar-los a les vistes ortogràfiques (Pràctica 2)
		dibuixa_Escena();		// Dibuix geometria de l'escena amb comandes GL.
*/
// Entorn VGI: Transferència del buffer OpenGL a buffer de pantalla
		//glfwSwapBuffers(window);

		break;

	case PERSPECT:
// Entorn VGI: PROJECCIÓ PERSPECTIVA
		//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Set Perspective Calculations To Most Accurate
		glDisable(GL_SCISSOR_TEST);		// Desactivació del retall de pantalla

// Entorn VGI: Activar shader Visualització Escena
		glUseProgram(shader_programID);

// Entorn VGI: Definició de Viewport, Projecció i Càmara
		ProjectionMatrix = Projeccio_Perspectiva(shader_programID, 0, 0, w, h, OPV.R);

// Entorn VGI: Definició de la càmera.
		if (camera == CAM_ESFERICA) {
				n[0] = 0;		n[1] = 0;		n[2] = 0;
				ViewMatrix = Vista_Esferica(shader_programID, OPV, Vis_Polar, pan, tr_cpv, tr_cpvF, c_fons, col_obj, objecte, mida, pas,
					front_faces, oculta, test_vis, back_line,
					ilumina, llum_ambient, llumGL, ifixe, ilum2sides,
					eixos, grid, hgrid);
				}
		else if (camera == CAM_NAVEGA) {
				if (Vis_Polar == POLARZ) {	vpv[0] = 0.0;	vpv[1] = 0.0;	vpv[2] = 1.0;
										}
				else if (Vis_Polar == POLARY) {	vpv[0] = 0.0;	vpv[1] = 1.0;	vpv[2] = 0.0;
										}
				else if (Vis_Polar == POLARX) {	vpv[0] = 1.0;	vpv[1] = 0.0;	vpv[2] = 0.0;
										}
				ViewMatrix = Vista_Navega(shader_programID, opvN, //false, 
					n, vpv, pan, tr_cpv, tr_cpvF, c_fons, col_obj, objecte, true, pas,
					front_faces, oculta, test_vis, back_line,
					ilumina, llum_ambient, llumGL, ifixe, ilum2sides,
					eixos, grid, hgrid);
				}
		else if (camera == CAM_GEODE) {
				ViewMatrix = Vista_Geode(shader_programID, OPV_G, Vis_Polar, pan, tr_cpv, tr_cpvF, c_fons, col_obj, objecte, mida, pas,
					front_faces, oculta, test_vis, back_line,
					ilumina, llum_ambient, llumGL, ifixe, ilum2sides,
					eixos, grid, hgrid);
				}

// Entorn VGI: Dibuix de l'Objecte o l'Escena
		configura_Escena();     // Aplicar Transformacions Geometriques segons persiana Transformacio i configurar objectes.
		dibuixa_Escena();		// Dibuix geometria de l'escena amb comandes GL.

// Entorn VGI: Transferència del buffer OpenGL a buffer de pantalla
		//glfwSwapBuffers(window);
		break;

	default:
// Entorn VGI: Càrrega SHADERS
// Entorn VGI: Càrrega Shader Eixos
		if (!eixos_programID) eixos_programID = shaderEixos.loadFileShaders(".\\shaders\\eixos.VERT", ".\\shaders\\eixos.FRAG");

// Entorn VGI: Càrrega Shader de Gouraud
		if (!shader_programID) shader_programID = shaderLighting.loadFileShaders(".\\shaders\\gouraud_shdrML.vert", ".\\shaders\\gouraud_shdrML.frag");

// Entorn VGI: Creació de la llista que dibuixarà els eixos Coordenades Món. Funció on està codi per dibuixar eixos	
		if (!eixos_Id) eixos_Id = deixos();						// Funció que defineix els Eixos Coordenades Món com un VAO.

// Entorn VGI: Crida a la funció Fons Blanc
		FonsB();

// Entorn VGI: Transferència del buffer OpenGL a buffer de pantalla
		//glfwSwapBuffers(window);
		break;
}

//  Actualitzar la barra d'estat de l'aplicació amb els valors R,A,B,PVx,PVy,PVz
	if (statusB) Barra_Estat();
}

// configura_Escena: Funcio que configura els parametres de Model i dibuixa les
//                   primitives OpenGL dins classe Model
void configura_Escena() {

// Aplicar Transformacions Geometriques segons persiana Transformacio i Quaternions
	GTMatrix = instancia(transf, TG, TGF);
}

// dibuixa_Escena: Funcio que crida al dibuix dels diferents elements de l'escana
void dibuixa_Escena() {

	//glUseProgram(shader_programID);

//	Dibuix SkyBox Cúbic.
	if (SkyBoxCube) dibuixa_Skybox(skC_programID, cubemapTexture, Vis_Polar, ProjectionMatrix, ViewMatrix);

//	Dibuix Coordenades Món i Reixes.
	dibuixa_Eixos(eixos_programID, eixos, eixos_Id, grid, hgrid, ProjectionMatrix, ViewMatrix);

// Escalat d'objectes, per adequar-los a les vistes ortogràfiques (Pràctica 2)
//	GTMatrix = glm::scale();

//	Dibuix geometria de l'escena amb comandes GL.
	dibuixa_EscenaGL(shader_programID, eixos, eixos_Id, grid, hgrid, objecte, col_obj, sw_material,
		textura, texturesID, textura_map, tFlag_invert_Y,
		npts_T, PC_t, pas_CS, sw_Punts_Control, dibuixa_TriedreFrenet,
		ObOBJ,				// Classe de l'objecte OBJ que conté els VAO's
		ViewMatrix, GTMatrix);
}

// Barra_Estat: Actualitza la barra d'estat (Status Bar) de l'aplicació amb els
//      valors R,A,B,PVx,PVy,PVz en Visualització Interactiva.
void Barra_Estat()
{
	std::string buffer, sss;
	CEsfe3D OPVAux = {0.0, 0.0, 0.0};
	double PVx, PVy, PVz;

// Status Bar fitxer fractal
	if (nom != "") fprintf(stderr, "Fitxer: %s \n",nom.c_str());

// Càlcul dels valors per l'opció Vista->Navega
	if (projeccio != CAP && projeccio != ORTO) {
		if (camera == CAM_ESFERICA)
		{	// Càmera Esfèrica
			OPVAux.R = OPV.R; OPVAux.alfa = OPV.alfa; OPVAux.beta = OPV.beta;
		}
		else if (camera == CAM_NAVEGA)
		{	// Càmera Navega
			OPVAux.R = sqrt(opvN.x * opvN.x + opvN.y * opvN.y + opvN.z * opvN.z);
			OPVAux.alfa = (asin(opvN.z / OPVAux.R) * 180) / PI;
			OPVAux.beta = (atan(opvN.y / opvN.x)) * 180 / PI;
		}
		else {	// Càmera Geode
			OPVAux.R = OPV_G.R; OPVAux.alfa = OPV_G.alfa; OPVAux.beta = OPV_G.beta;
		}
	}
	else {
		OPVAux.R = OPV.R; OPVAux.alfa = OPV.alfa; OPVAux.beta = OPV.beta;
	}

// Status Bar R Origen Punt de Vista
	if (projeccio == CAP) buffer = "       ";
		else if (projeccio == ORTO) buffer = " ORTO   ";
			else if (camera == CAM_NAVEGA) buffer = " NAV   ";
				else buffer= std::to_string(OPVAux.R);
	// Refrescar posició R Status Bar
	fprintf(stderr, "R=: %s", buffer.c_str());

// Status Bar angle alfa Origen Punt de Vista
	if (projeccio == CAP) buffer = "       ";
		else if (projeccio == ORTO) buffer = "ORTO   ";
			else if (camera == CAM_NAVEGA) buffer = " NAV   ";
				else buffer = std::to_string(OPVAux.alfa);
	// Refrescar posició angleh Status Bar
	fprintf(stderr, " a=: %s", buffer.c_str());

	// Status Bar angle beta Origen Punt de Vista
	if (projeccio == CAP) buffer = "       ";
		else if (projeccio == ORTO) buffer = "ORTO   ";
			else if (camera == CAM_NAVEGA) buffer = " NAV   ";
				else buffer = std::to_string(OPVAux.beta);
	// Refrescar posició anglev Status Bar
	fprintf(stderr, " ß=: %s  ", buffer.c_str());

// Transformació PV de Coord. esfèriques (R,anglev,angleh) --> Coord. cartesianes (PVx,PVy,PVz)
	if (camera == CAM_NAVEGA) { PVx = opvN.x; PVy = opvN.y; PVz = opvN.z; }
	else {	if (Vis_Polar == POLARZ) 
			{	PVx = OPVAux.R * cos(OPVAux.beta * PI / 180) * cos(OPVAux.alfa * PI / 180);
				PVy = OPVAux.R * sin(OPVAux.beta * PI / 180) * cos(OPVAux.alfa * PI / 180);
				PVz = OPVAux.R * sin(OPVAux.alfa * PI / 180);
			}
		else if (Vis_Polar == POLARY) 
				{	PVx = OPVAux.R * sin(OPVAux.beta * PI / 180) * cos(OPVAux.alfa * PI / 180);
					PVy = OPVAux.R * sin(OPVAux.alfa * PI / 180);
					PVz = OPVAux.R * cos(OPVAux.beta * PI / 180) * cos(OPVAux.alfa * PI / 180);
				}
				else {	PVx = OPVAux.R * sin(OPVAux.alfa * PI / 180);
						PVy = OPVAux.R * cos(OPVAux.beta * PI / 180) * cos(OPVAux.alfa * PI / 180);
						PVz = OPVAux.R * sin(OPVAux.beta * PI / 180) * cos(OPVAux.alfa * PI / 180);
					}
		}

// Status Bar PVx
	if (projeccio == CAP) buffer = "       ";
	else if (pan) buffer = std::to_string(tr_cpv.x);
	else buffer = std::to_string(PVx);
	//sss = _T("PVx=") + buffer;
// Refrescar posició PVx Status Bar
	fprintf(stderr, "PVx= %s", buffer.c_str());

// Status Bar PVy
	if (projeccio == CAP) buffer = "       ";
	else if (pan) buffer = std::to_string(tr_cpv.y);
	else buffer = std::to_string(PVy);
	//sss = "PVy=" + buffer;
// Refrescar posició PVy Status Bar
	fprintf(stderr, " PVy= %s", buffer.c_str());

// Status Bar PVz
	if (projeccio == CAP) buffer = "       ";
	else if (pan) buffer = std::to_string(tr_cpv.z);
	else buffer = std::to_string(PVz);
	//sss = "PVz=" + buffer;
// Refrescar posició PVz Status Bar
	fprintf(stderr, " PVz= %s \n", buffer.c_str());

// Status Bar per indicar el modus de canvi de color (FONS o OBJECTE)
	sss = " ";
	if (sw_grid) sss = "GRID ";
		else if (pan) sss = "PAN ";
			else if (camera == CAM_NAVEGA) sss = "NAV ";
				else if (sw_color) sss = "OBJ ";
					else sss = "FONS ";
// Refrescar posició Transformacions en Status Bar
	fprintf(stderr, "%s ", sss.c_str());

// Status Bar per indicar tipus de Transformació (TRAS, ROT, ESC)
	sss = " ";
	if (transf) {	if (rota) sss = "ROT";
					else if (trasl) sss = "TRA";
					else if (escal) sss = "ESC";
				}
	else if ((!sw_grid) && (!pan) && (camera != CAM_NAVEGA))
	{	// Components d'intensitat de fons que varien per teclat
		if ((fonsR) && (fonsG) && (fonsB)) sss = " RGB";
		else if ((fonsR) && (fonsG)) sss = " RG ";
		else if ((fonsR) && (fonsB)) sss = " R   B";
		else if ((fonsG) && (fonsB)) sss = "   GB";
		else if (fonsR) sss = " R  ";
		else if (fonsG) sss = "   G ";
		else if (fonsB) sss = "      B";
	}
// Refrescar posició Transformacions en Status Bar
	fprintf(stderr, "%s ", sss.c_str());

// Status Bar dels paràmetres de Transformació, Color i posicions de Robot i Cama
	sss = " ";
	if (transf)
	{	if (rota)
		{	buffer = std::to_string(TG.VRota.x);
			sss = " " + buffer + " ";

			buffer = std::to_string(TG.VRota.y);
			sss = sss + buffer + " ";

			buffer = std::to_string(TG.VRota.z);
			sss = sss + buffer;
		}
		else if (trasl)
		{	buffer = std::to_string(TG.VTras.x);
			sss = " " + buffer + " ";

			buffer = std::to_string(TG.VTras.y);
			sss = sss + buffer + " ";

			buffer = std::to_string(TG.VTras.z);
			sss = sss + buffer;
		}
		else if (escal)
		{	buffer = std::to_string(TG.VScal.x);
			sss = " " + buffer + " ";

			buffer = std::to_string(TG.VScal.y);
			sss = sss + buffer + " ";

			buffer = std::to_string(TG.VScal.x);
			sss = sss + buffer;
		}
	}
	else if ((!sw_grid) && (!pan) && (camera != CAM_NAVEGA))
	{	if (!sw_color)
		{	// Color fons
			buffer = std::to_string(c_fons.r);
			sss = " " + buffer + " ";

			buffer = std::to_string(c_fons.g);
			sss = sss + buffer + " ";

			buffer = std::to_string(c_fons.b);
			sss = sss + buffer;
		}
		else
		{	// Color objecte
			buffer = std::to_string(col_obj.r);
			sss = " " + buffer + " ";

			buffer = std::to_string(col_obj.g);
			sss = sss + buffer + " ";

			buffer = std::to_string(col_obj.b);
			sss = sss + buffer;
		}
	}

// Refrescar posició PVz Status Bar
	fprintf(stderr, "%s \n", sss.c_str());

// Status Bar per indicar el pas del Fractal
	if (objecte == O_FRACTAL)
	{	buffer = std::to_string(pas);
		//sss = "Pas=" + buffer;
		fprintf(stderr, "Pas= %s \n", buffer.c_str());
	}
	else {	sss = "          ";
			fprintf(stderr, "%s \n", sss.c_str());
		}
}

void draw_Menu_ImGui()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	// 1. Show the EntornVGI window. Finestra amb totes les opcions de l'aplicació.
	if (show_EntornVGI_window)
		ShowEntornVGIWindow(&show_EntornVGI_window); //ShowEntornVGIWindow(&show_EntornVGI_window);

	// 3. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;
		static float PV[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

		ImGui::Begin("Status Menu");                          // Create a window called "Status Menu" and append into it.

		ImGui::Text("FInestres EntornVGI:");               // Display some text (you can use a format strings too)
		ImGui::SameLine();
		ImGui::Checkbox("EntornVGI Window", &show_EntornVGI_window);
		ImGui::Separator();
		ImGui::Spacing();

		// Transformació PV de Coord. esfèriques (R,anglev,angleh) --> Coord. cartesianes (PVx,PVy,PVz)
		if (camera == CAM_NAVEGA) { PV[0] = opvN.x; PV[1] = opvN.y; PV[2] = opvN.z; }
		else {
			cam_Esferica[0] = OPV.R;	cam_Esferica[1] = OPV.alfa; cam_Esferica[2] = OPV.beta;
			if (Vis_Polar == POLARZ)
			{
				PV[0] = OPV.R * cos(OPV.beta * PI / 180) * cos(OPV.alfa * PI / 180);
				PV[1] = OPV.R * sin(OPV.beta * PI / 180) * cos(OPV.alfa * PI / 180);
				PV[2] = OPV.R * sin(OPV.alfa * PI / 180);
			}
			else if (Vis_Polar == POLARY)
			{
				PV[0] = OPV.R * sin(OPV.beta * PI / 180) * cos(OPV.alfa * PI / 180);
				PV[1] = OPV.R * sin(OPV.alfa * PI / 180);
				PV[2] = OPV.R * cos(OPV.beta * PI / 180) * cos(OPV.alfa * PI / 180);
			}
			else {
				PV[0] = OPV.R * sin(OPV.alfa * PI / 180);
				PV[1] = OPV.R * cos(OPV.beta * PI / 180) * cos(OPV.alfa * PI / 180);
				PV[2] = OPV.R * sin(OPV.beta * PI / 180) * cos(OPV.alfa * PI / 180);
			}
		}

		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::SeparatorText("CAMERA:");
		ImGui::PopStyleColor();
		
		ImGui::InputFloat3("Esferiques (R,alfa,beta)", cam_Esferica);
		ImGui::InputFloat3("Cartesianes (PVx,PVy,PVz)", PV);
		//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::SeparatorText("COLORS:");
		ImGui::PopStyleColor();
		ImGui::ColorEdit3("Color de Fons", (float*)&clear_colorB); // Edit 3 floats representing a background color
		ImGui::ColorEdit3("Color d'Objecte", (float*)&clear_colorO); // Edit 3 floats representing a object color
		c_fons.r = clear_colorB.x;	c_fons.g = clear_colorB.y;	c_fons.b = clear_colorB.z;	c_fons.a = clear_colorB.w;
		col_obj.r = clear_colorO.x;	col_obj.g = clear_colorO.y;	col_obj.b = clear_colorO.z;		col_obj.a = clear_colorO.w;
		ImGui::Separator();


		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::SeparatorText("TRANSFORMA:");
		ImGui::PopStyleColor();
		float tras_ImGui[3] = { (float) TG.VTras.x,(float) TG.VTras.y,(float) TG.VTras.z };
		ImGui::InputFloat3("Traslacio (Tx,Ty,Tz)", tras_ImGui);
		float rota_ImGui[3] = { (float) TG.VRota.x,(float) TG.VRota.y,(float) TG.VRota.z };
		ImGui::InputFloat3("Rotacio (Rx,Ry,Rz)", rota_ImGui);
		float scal_ImGui[3] = { (float) TG.VScal.x,(float) TG.VScal.y,(float) TG.VScal.z };
		ImGui::InputFloat3("Escala (Sx, Sy, Sz)", scal_ImGui);

		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::SeparatorText("ImGui:");               // Display some text (you can use a format strings too)
		ImGui::PopStyleColor();
		ImGui::Checkbox("Demo ImGui Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::SameLine();
		ImGui::Checkbox("Another ImGui Window", &show_another_window);
		ImGui::Spacing();

		ImGui::Text("imgui versions: (%s) (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);
		ImGui::Spacing();

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// Rendering ImGui Menus
	ImGui::Render();
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void MostraEntornVGIWindow(bool* p_open)
{
// Exceptionally add an extra assert here for people confused about initial Dear ImGui setup
// Most functions would normally just crash if the context is missing.
	IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!");

	// Examples Apps (accessible from the "Examples" menu)
	//static bool show_window_about = false;

	if (show_window_about)       ShowAboutWindow(&show_window_about);

// We specify a default position/size in case there's no data in the .ini file.
// We only do it to make the demo applications a little more welcoming, but typically this isn't required.
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

	ImGuiWindowFlags window_flags = 0;
	// Main body of the Demo window starts here.
	if (!ImGui::Begin("EntornVGI Menu", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	// Menu Bar
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Arxius"))
		{
			//IMGUI_DEMO_MARKER("Menu/File");
			ShowArxiusOptions();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Sobre EntornVGI"))
		{
			//IMGUI_DEMO_MARKER("Menu/Examples");
			//ShowAboutOptions(&show_window_about);
			ImGui::EndMenu();
		}
		
		ImGui::EndMenuBar();
	}

// End of ShowEntornVGIWindow()
	ImGui::PopItemWidth();
	ImGui::End();
}

void ShowArxiusOptions()
{
	//IMGUI_DEMO_MARKER("Examples/Menu");
	ImGui::MenuItem("(Arxius menu)", NULL, false, false);
	if (ImGui::MenuItem("New")) {}

	if (ImGui::MenuItem("Obrir Fitxer OBJ", "Ctrl+O")) {
		nfdchar_t* nomFitxer = NULL;
		nfdresult_t result = NFD_OpenDialog(NULL, NULL, &nomFitxer);

		if (result == NFD_OKAY) {
			puts("Success!");
			puts(nomFitxer);

			objecte = OBJOBJ;	textura = true;		tFlag_invert_Y = false;
			//char* nomfitx = nomfitxer;
			if (ObOBJ == NULL) ObOBJ = ::new COBJModel;
			else { // Si instància ja s'ha utilitzat en un objecte OBJ
				ObOBJ->netejaVAOList_OBJ();		// Netejar VAO, EBO i VBO
				ObOBJ->netejaTextures_OBJ();	// Netejar buffers de textures
				}

			//int error = ObOBJ->LoadModel(nomfitx);			// Carregar objecte OBJ amb textura com a varis VAO's
			int error = ObOBJ->LoadModel(nomFitxer);			// Carregar objecte OBJ amb textura com a varis VAO's

			//	Pas de paràmetres textura al shader
			if (!shader_programID) glUniform1i(glGetUniformLocation(shader_programID, "textur"), textura);
			if (!shader_programID) glUniform1i(glGetUniformLocation(shader_programID, "flag_invert_y"), tFlag_invert_Y);

			free(nomFitxer);
		}
	}

	if (ImGui::MenuItem("Obrir Fractal", "Ctrl+F")) {
		nfdchar_t* nomFitxer = NULL;
		nfdresult_t result = NFD_OpenDialog(NULL, NULL, &nomFitxer);

		if (result == NFD_OKAY) {	puts("Success!");
									puts(nomFitxer);
									free(nomFitxer);
								}
		else if (result == NFD_CANCEL) puts("User pressed cancel.");
			else printf("Error: %s\n", NFD_GetError());
		}

	if (ImGui::BeginMenu("Open Recent"))
	{
		ImGui::MenuItem("fish_hat.c");
		ImGui::MenuItem("fish_hat.inl");
		ImGui::MenuItem("fish_hat.h");
		if (ImGui::BeginMenu("More.."))
		{
			ImGui::MenuItem("Hello");
			ImGui::MenuItem("Sailor");
			if (ImGui::BeginMenu("Recurse.."))
			{
				ShowArxiusOptions();
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Save", "Ctrl+S")) {}
	if (ImGui::MenuItem("Save As..")) {}

	if (ImGui::BeginMenu("Disabled", false)) // Disabled
	{
		IM_ASSERT(0);
	}
	if (ImGui::MenuItem("Checked", NULL, true)) {}
	ImGui::Separator();
	if (ImGui::MenuItem("Quit", "Alt+F4")) {}
}


void ShowAboutWindow(bool* p_open)
{
	// For the demo: add a debug button _BEFORE_ the normal log window contents
// We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to the _same_ window.
// Most of the contents of the window will be added by the log.Draw() call.
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
	ImGui::Begin("About", p_open);
	//IMGUI_DEMO_MARKER("Window/About");
	ImGui::Text("VISUALITZACIO GRAFICA INTERACTIVA (Escola d'Enginyeria - UAB");
	ImGui::Text("Entorn Grafic VS2022 amb interficie GLFW, ImGui i OpenGL 3.3+ per a practiques i ABP");
	ImGui::Text("AUTOR: Enric Marti Godia");
	ImGui::Text("Copyright (C) 2023");
	if (ImGui::Button("Acceptar"))
		show_window_about = false;
	ImGui::End();
}

// Entorn VGI: Funció que retorna opció de menú TIPUS CAMERA segons variable camera (si modificada per teclat)
int shortCut_Camera()
{
	int auxCamera;

	// Entorn VGI. Gestió opcions desplegable TIPUS CAMERA segons el valor de la variable selected
	switch (camera)
	{
	case CAM_ESFERICA:	// Càmera ESFÈRICA
		auxCamera = 0;
		break;
	case CAM_NAVEGA:	// Càmera NAVEGA
		auxCamera = 1;
		break;
	case CAM_GEODE:		// Càmera GEODE
		auxCamera = 2;
		break;
	default:			// Opció CÀMERA <Altres Càmeres>
		auxCamera = 0;
		break;
	}
	return auxCamera;
}


// Entorn VGI: Funció que retorna opció de menú TIPUS PROJECCIO segons variable projecte (si modificada per teclat)
int shortCut_Projeccio()
{
	int auxProjeccio;

	// Entorn VGI. Gestió opcions desplegable TIPUS PROJECCIO segons el valor de la variable selected
	switch (projeccio)
	{
	case CAP:	// Projeccio CAP
		auxProjeccio = 0;
		break;
	case AXONOM:		// Projeccio AXONOMÈTRICA
		auxProjeccio = 1;
		break;
	case ORTO:			// Projeccio ORTOGRÀFICA
		auxProjeccio = 2;
		break;
	case PERSPECT:		// Projeccio PERSPECTIVA
		auxProjeccio = 3;
		break;
	default:			// Opció PROJECCIÓ <Altres Projeccions>
		auxProjeccio = 0;
		break;
	}
	return auxProjeccio;
}

// Entorn VGI: Funció que retorna opció de menú Objecte segons variable objecte (si modificada per teclat)
int shortCut_Objecte()
{
	int auxObjecte;

// Entorn VGI. Gestió opcions desplegable OBJECTES segons el valor de la variable selected
	switch (objecte)
	{
	case CAP:			// Objecte CAP
		auxObjecte = 0;
		break;
	case CUB:			// Objecte CUB
		auxObjecte = 1;
		break;
	case CUB_RGB:		// Objecte CUB_RGB
		auxObjecte = 2;
		break;
	case ESFERA:		// Objecte ESFERA
		auxObjecte = 3;
		break;
	case TETERA:		// Objecte TETERA
		auxObjecte = 4;
		break;
	case ARC:			// Objecte ARC
		auxObjecte = 5;
		break;
	case MATRIUP:		// Objecte MATRIU PRIMITIVES
		auxObjecte = 6;
		break;
	case MATRIUP_VAO:	// Objecte MATRIU PRIMITIVES VAO
		auxObjecte = 7;
		break;
	case TIE:			// Objecte TIE
		auxObjecte = 8;
		break;
	case C_BEZIER:		// Objecte CORBA BEZIER
		auxObjecte = 10;
		break;
	case C_BSPLINE:		// Objecte CORBA B-SPLINE
		auxObjecte = 11;
		break;
	case C_LEMNISCATA:	// Objecte CORBA LEMNISCATA
		auxObjecte = 12;
		break;
	case OBJOBJ:	// Objecte Arxiu OBJ
		auxObjecte = 9;
		break;
	default:			// Opció OBJECTE <Altres Objectes>
		auxObjecte = 0;
		break;
	}
	return auxObjecte;
}

// Entorn VGI: Funció que retorna opció de menú TIPUS ILUMINACIO segons variable ilumina (si modificada per teclat)
int shortCut_Iluminacio()
{
	int auxIlumina;

// Entorn VGI. Gestió opcions desplegable OBJECTES segons el valor de la variable selected
	switch (ilumina)
	{
	case PUNTS:		// Ilumninacio PUNTS
		auxIlumina = 0;
		break;
	case FILFERROS:	// Iluminacio FILFERROS
		auxIlumina = 1;
		break;
	case PLANA:		// Iluminacio PLANA
		auxIlumina = 2;
		break;
	case SUAU:	// Iluminacio SUAU
		auxIlumina = 3;
		break;
/*	case PHONG:		// Iluminacio PHONG
		auxIlumina = 4;
		break;
*/
	default:		 // Opció Iluminacio <Altres Iluminacio>
		auxIlumina = 0;
		break;
	}
	return auxIlumina;
}


// Demonstrate most Dear ImGui features (this is big function!)
// You may execute this function to experiment with the UI and understand what it does.
// You may then search for keywords in the code when you are interested by a specific feature.
void ShowEntornVGIWindow(bool* p_open)
{
	int i = 0; // Variable per a menús de selecció.
	static int selected = -1;

// Exceptionally add an extra assert here for people confused about initial Dear ImGui setup
// Most functions would normally just crash if the context is missing.
	IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!");

// Examples Apps(accessible from the "Examples" menu)
	static bool show_app_main_menu_bar = false;
	static bool show_app_documents = false;
	static bool show_app_console = false;
	static bool show_app_log = false;
	static bool show_app_layout = false;
	static bool show_app_property_editor = false;
	static bool show_app_long_text = false;
	static bool show_app_auto_resize = false;
	static bool show_app_constrained_resize = false;
	static bool show_app_simple_overlay = false;
	static bool show_app_fullscreen = false;
	static bool show_app_window_titles = false;
	static bool show_app_custom_rendering = false;

	// Examples Apps (accessible from the "Examples" menu)
	//static bool show_window_about = false;

	if (show_window_about)       ShowAboutWindow(&show_window_about);

/*
	if (show_app_main_menu_bar)       ShowExampleAppMainMenuBar();
	if (show_app_documents)           ShowExampleAppDocuments(&show_app_documents);
	if (show_app_console)             ShowExampleAppConsole(&show_app_console);
	if (show_app_log)                 ShowExampleAppLog(&show_app_log);
	if (show_app_layout)              ShowExampleAppLayout(&show_app_layout);
	if (show_app_property_editor)     ShowExampleAppPropertyEditor(&show_app_property_editor);
	if (show_app_long_text)           ShowExampleAppLongText(&show_app_long_text);
	if (show_app_auto_resize)         ShowExampleAppAutoResize(&show_app_auto_resize);
	if (show_app_constrained_resize)  ShowExampleAppConstrainedResize(&show_app_constrained_resize);
	if (show_app_simple_overlay)      ShowExampleAppSimpleOverlay(&show_app_simple_overlay);
	if (show_app_fullscreen)          ShowExampleAppFullscreen(&show_app_fullscreen);
	if (show_app_window_titles)       ShowExampleAppWindowTitles(&show_app_window_titles);
	if (show_app_custom_rendering)    ShowExampleAppCustomRendering(&show_app_custom_rendering);
*/

	// Dear ImGui Tools/Apps (accessible from the "Tools" menu)
	static bool show_app_metrics = false;
	static bool show_app_debug_log = false;
	static bool show_app_stack_tool = false;
	static bool show_app_about = false;
	static bool show_app_style_editor = false;

	if (show_app_metrics)
		ImGui::ShowMetricsWindow(&show_app_metrics);
	if (show_app_debug_log)
		ImGui::ShowDebugLogWindow(&show_app_debug_log);
	if (show_app_stack_tool)
		ImGui::ShowStackToolWindow(&show_app_stack_tool);
	if (show_app_about)
		ImGui::ShowAboutWindow(&show_app_about);
	if (show_app_style_editor)
	{
		ImGui::Begin("Dear ImGui Style Editor", &show_app_style_editor);
		ImGui::ShowStyleEditor();
		ImGui::End();
	}

// Demonstrate the various window flags. Typically you would just use the default!
	static bool no_titlebar = false;
	static bool no_scrollbar = false;
	static bool no_menu = false;
	static bool no_move = false;
	static bool no_resize = false;
	static bool no_collapse = false;
	static bool no_close = false;
	static bool no_nav = false;
	static bool no_background = false;
	static bool no_bring_to_front = false;
	static bool unsaved_document = false;

	ImGuiWindowFlags window_flags = 0;
	if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
	if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
	if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
	if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
	if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
	if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
	if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;
	if (no_close)           p_open = NULL; // Don't pass our bool* to Begin

// We specify a default position/size in case there's no data in the .ini file.
// We only do it to make the demo applications a little more welcoming, but typically this isn't required.
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

	// Main body of the Demo window starts here.
	if (!ImGui::Begin("EntornVGI Menu", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

// Entorn VGI: Menu Bar (Pop Ups desplagables (Arxius, About)
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Arxius"))
		{
			//IMGUI_DEMO_MARKER("Menu/File");
			//ShowExampleMenuFile();
			//ImGui::Text("Desplegable Arxius");
			ShowArxiusOptions();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("About"))
		{
			//IMGUI_DEMO_MARKER("Menu/Examples");
			//ShowAboutOptions(&show_window_about);
			ImGui::MenuItem("Sobre EntornVGI", NULL, &show_window_about);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}


	ImGui::Text("Finestra de menus d'EntornVGI");
	ImGui::Spacing();

// Entorn VGI: DESPLEGABLES
// DESPLEGABLE CAMERA
	//IMGUI_DEMO_MARKER("Help");
	if (ImGui::CollapsingHeader("CAMERA"))
	{
		// Entorn VGI. Mostrar Opcions desplegable TIPUS CAMERA
 
		//IMGUI_DEMO_MARKER("Widgets/Basic/RadioButton");
		oCamera = shortCut_Camera();	//static int oCamera = 0;
		ImGui::RadioButton("Esferica (<Shift>+L)", &oCamera, 0); ImGui::SameLine();
		static int clickCE = 0;
		if (ImGui::Button("Origen Esferica")) clickCE++;
		if (ImGui::BeginTable("split", 3))
		{
			ImGui::TableNextColumn(); ImGui::Checkbox("Mobil", &mobil);
			ImGui::TableNextColumn(); ImGui::Checkbox("Zoom?", &zzoom);
			ImGui::TableNextColumn(); ImGui::Checkbox("Zoom Orto?", &zzoomO);
			ImGui::Separator();
			ImGui::TableNextColumn(); ImGui::Checkbox("Satelit?", &satelit);
			ImGui::EndTable();
		}
		ImGui::Separator();
		ImGui::Spacing();

		// EntornVGI: Si s'ha apretat el botó "Origen Esfèrica"
		if (clickCE)
		{
			clickCE = 0;
			if (camera == CAM_ESFERICA) {
				OPV.R = 15.0;	OPV.alfa = 0.0; OPV.beta = 0.0;
				}
		}

		static int clickCN = 0;
		ImGui::RadioButton("Navega (<Shift>+N)", &oCamera, 1); ImGui::SameLine();
		if (ImGui::Button("Origen Navega")) clickCN++;
		ImGui::Separator();
		ImGui::Spacing();

		// EntornVGI: Si s'ha apretat el botó "Origen Navega"
		if (clickCN)
		{
			clickCN = 0;
			if (camera == CAM_NAVEGA) {
				n[0] = 0.0;		n[1] = 0.0;		n[2] = 0.0;
				opvN.x = 10.0;	opvN.y = 0.0;		opvN.z = 0.0;
				angleZ = 0.0;
			}
		}

		static int clickCG = 0;
		ImGui::RadioButton("Geode (<Shift>+J)", &oCamera, 2); ImGui::SameLine();
		if (ImGui::Button("Origen Geode")) clickCG++;

		// EntornVGI: Si s'ha apretat el botó "Origen Geode"
		if (clickCG)
		{	clickCG = 0;
			if (camera == CAM_GEODE) {
				OPV.R = 0.0;		OPV.alfa = 0.0;		OPV.beta = 0.0;				// Origen PV en esfèriques
				mobil = true;		zzoom = true;	zzoomO = false;	 satelit = false;	pan = false;
				Vis_Polar = POLARZ;	llumGL[5].encesa = true;
				glFrontFace(GL_CW);
			}
		}

		// Entorn VGI. Gestió opcions desplegable OBJECTES segons el valor de la variable selected
		switch (oCamera)
		{
		case 0: // Opció CAMERA Esfèrica
			if ((projeccio != ORTO) && (projeccio != CAP)) camera = CAM_ESFERICA;
			// Activació de zoom, mobil
			mobil = true;	zzoom = true;
			break;
		case 1: // Opció CAMERA Navega
			if ((projeccio != ORTO) && (projeccio != CAP))
				{	camera = CAM_NAVEGA;
					// Desactivació de zoom, mobil, Transformacions Geomètriques via mouse i pan 
					//		si navega activat
					mobil = true;	zzoom = true;
					transX = false;	transY = false;	transZ = false;
					//pan = false;
					tr_cpv.x = 0.0;		tr_cpv.y = 0.0;		tr_cpv.z = 0.0;		// Inicialitzar a 0 desplaçament de pantalla
					tr_cpvF.x = 0.0;	tr_cpvF.y = 0.0;	tr_cpvF.x = 0.0;	// Inicialitzar a 0 desplaçament de pantalla

					// Incialitzar variables Navega segons configuració eixos en Polars
					if (Vis_Polar == POLARZ) {
						opvN.x = 10.0;	opvN.y = 0.0;	opvN.z = 0.0; // opvN = (10,0,0)
						n[0] = 0.0;		n[1] = 0.0;		n[2] = 0.0;
						angleZ = 0.0;
					}
						else if (Vis_Polar == POLARY) {
								opvN.x = 0.0;	opvN.y = 0.0;	opvN.z = 10.0; // opvN = (10,0,0)
								n[0] = 0.0;		n[1] = 0.0;		n[2] = 0.0;
								angleZ = 0.0;
								}
									else if (Vis_Polar == POLARX) {
											opvN.x = 0.0;	opvN.y = 10.0;	opvN.z = 0.0;	 // opvN = (0,10,0)
											n[0] = 0.0;		n[1] = 0.0;		n[2] = 0.0;
											angleZ = 0.0;
											}
				}
			break;
		case 2:	// Opció CAMERA Geode
			if ((projeccio != ORTO) && (projeccio != CAP))
				{	// Inicialitzar paràmetres Càmera Geode
					OPV.R = 0.0;		OPV.alfa = 0.0;		OPV.beta = 0.0;				// Origen PV en esfèriques
					mobil = true;	zzoom = true;	zzoomO = false;	 satelit = false;	pan = false;
					Vis_Polar = POLARZ;
					llumGL[5].encesa = true;
				}
			break;
		default: // Opció PROJECCIÓ <Altres Projeccions>
			break;
		}

		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::SeparatorText("POLARS");
		ImGui::PopStyleColor();
		// Entorn VGI. Mostrar Opcions desplegable POLARS
		
		//IMGUI_DEMO_MARKER("Widgets/Basic/RadioButton");
		static int oPolars = 2;
		ImGui::RadioButton("Polars X (<Shift>+X)", &oPolars, 0); ImGui::SameLine();
		ImGui::RadioButton("Polars Y (<Shift>+Y)", &oPolars, 1); ImGui::SameLine();
		ImGui::RadioButton("Polars Z (<Shift>+Z)", &oPolars, 2);

		// Entorn VGI. Gestió opcions desplegable OBJECTES segons el valor de la variable selected
		switch (oPolars)
		{
		case 0: // Opció POLARS X
			if ((camera == CAM_ESFERICA) || (camera == CAM_NAVEGA)) Vis_Polar = POLARX;
			// EntornVGI: Inicialitzar la càmera en l'opció NAVEGA (posició i orientació eixos)
			if (camera == CAM_NAVEGA) {
				opvN.x = 0.0;	opvN.y = 10.0;	opvN.z = 0.0;	 // opvN = (0,10,0)
				n[0] = 0.0;		n[1] = 0.0;		n[2] = 0.0;
				angleZ = 0.0;
				}
			break;
		case 1: // Opció POLARS Y
			if ((camera == CAM_ESFERICA) || (camera == CAM_NAVEGA)) Vis_Polar = POLARY;
			// EntornVGI: Inicialitzar la càmera en l'opció NAVEGA (posició i orientació eixos)
			if (camera == CAM_NAVEGA) {
				opvN.x = 0.0;	opvN.y = 0.0;	opvN.z = 10.0; // opvN = (0,0,10)
				n[0] = 0.0;		n[1] = 0.0;		n[2] = 0.0;
				angleZ = 0.0;
				}
			break;
		case 2:	// Opció POLARS Z
			if ((camera == CAM_ESFERICA) || (camera == CAM_NAVEGA)) Vis_Polar = POLARZ;
			// EntornVGI: Inicialitzar la càmera en l'opció NAVEGA (posició i orientació eixos)
			if (camera == CAM_NAVEGA) {
				opvN.x = 10.0;	opvN.y = 0.0;	opvN.z = 0.0; // opvN = (10,0,0)
				n[0] = 0.0;		n[1] = 0.0;		n[2] = 0.0;
				angleZ = 0.0;
			}
			break;
		}
	}

// DESPLEGABLE VISTA
	//IMGUI_DEMO_MARKER("Help");
	if (ImGui::CollapsingHeader("VISTA"))
	{
		if (ImGui::BeginTable("split", 2))
		{	ImGui::TableNextColumn(); ImGui::Checkbox("Full Screen? ", &fullscreen);
			ImGui::TableNextColumn(); ImGui::Checkbox("Eixos?", &eixos);
			ImGui::TableNextColumn(); ImGui::Checkbox("SkyBox?", &SkyBoxCube);
			ImGui::EndTable();
			}
		ImGui::Spacing();
		ImGui::Separator();

		if (SkyBoxCube) {
			// Càrrega Shader Skybox
			if (!skC_programID) skC_programID = shader_SkyBoxC.loadFileShaders(".\\shaders\\skybox.VERT", ".\\shaders\\skybox.FRAG");

			// Càrrega VAO Skybox Cube
			if (skC_VAOID.vaoId == 0) skC_VAOID = loadCubeSkybox_VAO();
			Set_VAOList(CUBE_SKYBOX, skC_VAOID);

			if (!cubemapTexture)
			{	// load Skybox textures
				// -------------
				std::vector<std::string> faces =
				{ ".\\textures\\skybox\\right.jpg",
					".\\textures\\skybox\\left.jpg",
					".\\textures\\skybox\\top.jpg",
					".\\textures\\skybox\\bottom.jpg",
					".\\textures\\skybox\\front.jpg",
					".\\textures\\skybox\\back.jpg"
				};
				cubemapTexture = loadCubemap(faces);
			}
		}
		static int clicked = 0;
		if (ImGui::BeginTable("split", 2))
		{
			ImGui::TableNextColumn(); ImGui::Checkbox("Pan? ", &pan);
			ImGui::TableNextColumn(); //ImGui::Checkbox("Origen Pan", &pan);
			//IMGUI_DEMO_MARKER("Widgets/Basic/Button");
			if (ImGui::Button("Origen Pan")) clicked++;
			ImGui::EndTable();
		}
		ImGui::Spacing();
		ImGui::Separator();

		if (clicked)
		{
			if (pan) {	clicked = 0;
						fact_pan = 1;
						tr_cpv.x = 0;	tr_cpv.y = 0;	tr_cpv.z = 0;
					}
		}

		if (pan) {	mobil = true;		zzoom = true;
					transX = false;		transY = false;		transZ = false;
					//navega = false;
				}
	}

// DESPLEGABLE PROJECCIO
		//IMGUI_DEMO_MARKER("Help");
	if (ImGui::CollapsingHeader("PROJECCIO"))
	{
		//IMGUI_DEMO_MARKER("Widgets/Selectables/Single Selection PROJECCIO");
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::SeparatorText("TIPUS PROJECCIO");
		ImGui::PopStyleColor();

		// Entorn VGI. Mostrar Opcions desplegable POLARS

		//IMGUI_DEMO_MARKER("Widgets/Basic/RadioButton");
		oProjeccio = shortCut_Projeccio();	//static int oProjeccio = 3;
		ImGui::RadioButton("Axonometrica (<Shift>+A)", &oProjeccio, 1); ImGui::SameLine();
		ImGui::RadioButton("Ortografica (<Shift>+O)", &oProjeccio, 2); ImGui::SameLine();
		ImGui::RadioButton("Perspectiva (<Shift>+P)", &oProjeccio, 3);

		// Entorn VGI. Gestió opcions desplegable OBJECTES segons el valor de la variable selected
		switch (oProjeccio)
		{
		case 1: // Opció PROJECCIÓ Axonomètrica
			if (projeccio != AXONOM) {
				projeccio = AXONOM;
				mobil = true;			zzoom = true;
			}
			break;

		case 2: // Opció PROJECCIÓ Ortogràfica
			if (projeccio != ORTO) {
				projeccio = ORTO;
				mobil = false;			zzoom = false;	zzoom = true;
			}
			break;

		case 3:	// Opció PROJECCIÓ Perspectiva
			if (projeccio != PERSPECT) {
				projeccio = PERSPECT;
				mobil = true;			zzoom = true;
			}break;

		default: // Opció PROJECCIÓ <Altres Projeccions>
			break;
		}
	}

// DESPLEGABLE VISTA
	//IMGUI_DEMO_MARKER("Help");
	if (ImGui::CollapsingHeader("OBJECTE"))
	{
		// Entorn VGI. Mostrar Opcions desplegable OBJECTES
		//IMGUI_DEMO_MARKER("Widgets/Basic/RadioButton");
		oObjecte = shortCut_Objecte(); //static int oObjecte = 0;
/*
		ImGui::RadioButton("Cap(<Shift>+B)", &oObjecte, 0);
		if (ImGui::BeginTable("split", 3))
		{
			ImGui::TableNextColumn(); ImGui::RadioButton("Cub (<Shift>+C)", &oObjecte, 1);
			ImGui::TableNextColumn(); ImGui::RadioButton("Cub RGB (<Shift>+D)", &oObjecte, 2);
			ImGui::TableNextColumn(); ImGui::RadioButton("Esfera (<Shift>+E)", &oObjecte, 3);
			ImGui::TableNextColumn(); ImGui::RadioButton("Tetera (<Shift>+T)", &oObjecte, 4);
			ImGui::TableNextColumn(); ImGui::RadioButton("Arc (<Shift>+R)", &oObjecte, 5);
			ImGui::TableNextColumn(); ImGui::RadioButton("Matriu Primitives (<Shift>+H)", &oObjecte, 6);
			ImGui::TableNextColumn(); ImGui::RadioButton("Matriu Primitives VAO (<Shift>+V)", &oObjecte, 7);
			ImGui::TableNextColumn(); ImGui::RadioButton("Tie (<Shift>+I)", &oObjecte, 8);
			ImGui::TableNextColumn(); ImGui::RadioButton("Arxiu OBJ", &oObjecte, 9);
			ImGui::EndTable();
		}
		
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::SeparatorText("CORBES:");
		ImGui::PopStyleColor();
		if (ImGui::BeginTable("split", 3))
		{	ImGui::TableNextColumn(); ImGui::RadioButton("Bezier (<Shift>+F9)", &oObjecte, 10);
			ImGui::TableNextColumn(); ImGui::RadioButton("B-spline (<Shift>+F10)", &oObjecte, 11);
			ImGui::TableNextColumn(); ImGui::RadioButton("Lemniscata (<Shift>+F11)", &oObjecte, 12);
			ImGui::EndTable();
		}
		//ImGui::Separator();
		ImGui::Spacing();
*/

		// Using the _simplified_ one-liner Combo() api here
		// See "Combo" section for examples of how to use the more flexible BeginCombo()/EndCombo() api.
		//IMGUI_DEMO_MARKER("Widgets/Basic/Combo");
		const char* items[] = { "Cap(<Shift>+B)", "Cub (<Shift>+C)", "Cub RGB (<Shift>+D)", "Esfera (<Shift>+E)", "Tetera (<Shift>+T)", 
			"Arc (<Shift>+R)", "Matriu Primitives (<Shift>+H)", "Matriu Primitives VAO (<Shift>+V)", "Tie (<Shift>+I)", "Arxiu OBJ", 
			"Bezier (<Shift>+F9)", "B-spline (<Shift>+F10)", "Lemniscata (<Shift>+F11)" };
		//static int item_current = 0;
		ImGui::Combo("Tipus d'Objecte", &oObjecte, items, IM_ARRAYSIZE(items));
		ImGui::Spacing();

		// Mostrar fram PARAMETRES CORBES si Objectes Corbes seleccionats
		if ((oObjecte > 9) && (oObjecte < 13)) {
			//IMGUI_DEMO_MARKER("Widgets/Selectables/Single Selection CORBES");
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
			ImGui::SeparatorText("PARAMETRES CORBES:");
			ImGui::PopStyleColor();

			if (ImGui::BeginTable("split", 2))
			{
				ImGui::TableNextColumn(); ImGui::Checkbox("Triedre de Frenet?", &dibuixa_TriedreFrenet);
				ImGui::TableNextColumn(); ImGui::Checkbox("Triedre de Darboux?", &dibuixa_TriedreDarboux);
				ImGui::TableNextColumn(); ImGui::Checkbox("Punts de Control? (<Shift>+F12)", &sw_Punts_Control);
				ImGui::EndTable();
			}
		}

// EntornVGI: Variables associades a Pop Up OBJECTE
		bool testA = false;
		int error = 0;
		CColor color_Mar;

		// Lectura fitxers Punts de Control Corbes
		nfdchar_t* nomFitxer = NULL;
		nfdresult_t result; // = NFD_OpenDialog(NULL, NULL, &nomFitxer);

// Entorn VGI. Gestió opcions desplegable OBJECTES segons el valor de la variable selected
		switch (oObjecte)
		{
		case 0: // Opció OBJECTE Cap
			if (objecte != CAP) {
				objecte = CAP;
				netejaVAOList();											// Neteja Llista VAO.
			}
			break;
		case 1: // Opció OBJECTE Cub
			if (objecte != CUB) {
				objecte = CUB;
				//	---- Entorn VGI: ATENCIÓ!!. Canviar l'escala per a centrar la vista (Ortogràfica)

				//  ---- Entorn VGI: ATENCIÓ!!. Modificar R per centrar la Vista a la mida de l'objecte (Perspectiva)
				netejaVAOList();											// Neteja Llista VAO.
				// Posar color objecte (col_obj) al vector de colors del VAO.
				SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);
				Set_VAOList(GLUT_CUBE, loadglutSolidCube_EBO(1.0));		// Genera EBO de cub mida 1 i el guarda a la posició GLUT_CUBE.
			}
			break;
		case 2:	// Opció OBJECTE Cub RGB
			if (objecte != CUB_RGB) {
				objecte = CUB_RGB;
				//	---- Entorn VGI: ATENCIÓ!!. Canviar l'escala per a centrar la vista (Ortogràfica)
					//  ---- Entorn VGI: ATENCIÓ!!. Modificar R per centrar la Vista a la mida de l'objecte (Perspectiva)
				netejaVAOList();						// Neteja Llista VAO.
				Set_VAOList(GLUT_CUBE_RGB, loadglutSolidCubeRGB_EBO(1.0));	// Genera EBO de cub mida 1 i el guarda a la posició GLUT_CUBE_RGB.
			}
			break;
		case 3: // Opció OBJECTE Esfera
			if (objecte != ESFERA) {
				objecte = ESFERA;
				//	---- Entorn VGI: ATENCIÓ!!. Canviar l'escala per a centrar la vista (Ortogràfica)

				//  ---- Entorn VGI: ATENCIÓ!!. Modificar R per centrar la Vista a la mida de l'objecte (Perspectiva)
				netejaVAOList();						// Neteja Llista VAO.
				// Posar color objecte (col_obj) al vector de colors del VAO.
				SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);
				Set_VAOList(GLU_SPHERE, loadgluSphere_EBO(1.0, 30, 30));
			}
			break;
		case 4: // Opció OBJECTE Tetera
			if (objecte != TETERA) {
				objecte = TETERA;
				//	---- Entorn VGI: ATENCIÓ!!. Canviar l'escala per a centrar la vista (Ortogràfica)

				//  ---- Entorn VGI: ATENCIÓ!!. Modificar R per centrar la Vista a la mida de l'objecte (Perspectiva)
				netejaVAOList();						// Neteja Llista VAO.
				// Posar color objecte (col_obj) al vector de colors del VAO.
				SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);
				//if (Get_VAOId(GLUT_TEAPOT) != 0) deleteVAOList(GLUT_TEAPOT);
				Set_VAOList(GLUT_TEAPOT, loadglutSolidTeapot_VAO()); //Genera VAO tetera mida 1 i el guarda a la posició GLUT_TEAPOT.
			}
			break;
		case 5: // Opció OBJECTE Arc
			if (objecte != ARC) {
				objecte = ARC;
				//  Modificar R per centrar la Vista a la mida de l'objecte (Perspectiva)
				//	Canviar l'escala per a centrar la vista (Ortogràfica)
				color_Mar.r = 0.5;	color_Mar.g = 0.4; color_Mar.b = 0.9; color_Mar.a = 1.0;
				// TODO: Agregue aquí su código de controlador de comandos
				objecte = ARC;
				//	---- Entorn VGI: ATENCIÓ!!. Canviar l'escala per a centrar la vista (Ortogràfica)
				//  ---- Entorn VGI: ATENCIÓ!!. Modificar R per centrar la Vista a la mida de l'objecte (Perspectiva)

				// Càrrega dels VAO's per a construir objecte ARC
				netejaVAOList();						// Neteja Llista VAO.

				// Posar color objecte (col_obj) al vector de colors del VAO.
				SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);

				//if (Get_VAOId(GLUT_CUBE) != 0) deleteVAOList(GLUT_CUBE);
				Set_VAOList(GLUT_CUBE, loadglutSolidCube_EBO(1.0));		// Càrrega Cub de costat 1 com a EBO a la posició GLUT_CUBE.

				//if (Get_VAOId(GLU_SPHERE) != 0) deleteVAOList(GLU_SPHERE);
				Set_VAOList(GLU_SPHERE, loadgluSphere_EBO(0.5, 20, 20));	// Càrrega Esfera a la posició GLU_SPHERE.

				//if (Get_VAOId(GLUT_TEAPOT) != 0) deleteVAOList(GLUT_TEAPOT);
				Set_VAOList(GLUT_TEAPOT, loadglutSolidTeapot_VAO());		// Carrega Tetera a la posició GLUT_TEAPOT.

				//if (Get_VAOId(MAR_FRACTAL_VAO) != 0) deleteVAOList(MAR_FRACTAL_VAO);
				Set_VAOList(MAR_FRACTAL_VAO, loadSea_VAO(color_Mar));		// Carrega Mar a la posició MAR_FRACTAL_VAO.
			}
			break;
		case 6: // Opció OBJECTE MatrVAOiu Primitives
			objecte = MATRIUP;
			break;
		case 7: // Opció OBJECTE Matriu Primitives VAO
			if (objecte != MATRIUP_VAO) {
				objecte = MATRIUP_VAO;

				// Càrrega dels VAO's per a construir objecte ARC
				netejaVAOList();						// Neteja Llista VAO.

				// Posar color objecte (col_obj) al vector de colors del VAO.
				SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);

				//if (Get_VAOId(GLUT_CUBE) != 0) deleteVAOList(GLUT_CUBE);
				Set_VAOList(GLUT_CUBE, loadglutSolidCube_EBO(1.0f));

				//if (Get_VAOId(GLUT_TORUS) != 0)deleteVAOList(GLUT_TORUS);
				Set_VAOList(GLUT_TORUS, loadglutSolidTorus_EBO(2.0, 3.0, 20, 20));

				//if (Get_VAOId(GLUT_SPHERE) != 0) deleteVAOList(GLU_SPHERE);
				Set_VAOList(GLU_SPHERE, loadgluSphere_EBO(1.0, 20, 20));
			}
			break;
		case 8: // Opció OBJECTE Tie
			if (objecte != TIE) {
				objecte = TIE;		textura = true;
				//	---- Entorn VGI: ATENCIÓ!!. Canviar l'escala per a centrar la vista (Ortogràfica)

				//  ---- Entorn VGI: ATENCIÓ!!. Modificar R per centrar la Vista a la mida de l'objecte (Perspectiva)

				// Càrrega dels VAO's per a construir objecte TIE
				netejaVAOList();						// Neteja Llista VAO.

				// Posar color objecte (col_obj) al vector de colors del VAO.
				SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);

				//if (Get_VAOId(GLU_CYLINDER) != 0) deleteVAOList(GLU_CYLINDER);
				Set_VAOList(GLUT_CYLINDER, loadgluCylinder_EBO(5.0f, 5.0f, 0.5f, 6, 1));// Càrrega cilindre com a VAO.

				//if (Get_VAOId(GLU_DISK) != 0)deleteVAOList(GLU_DISK);
				Set_VAOList(GLU_DISK, loadgluDisk_EBO(0.0f, 5.0f, 6, 1));	// Càrrega disc com a VAO

				//if (Get_VAOId(GLU_SPHERE) != 0)deleteVAOList(GLU_SPHERE);
				Set_VAOList(GLU_SPHERE, loadgluSphere_EBO(10.0f, 80, 80));	// Càrrega disc com a VAO

				//if (Get_VAOId(GLUT_USER1) != 0)deleteVAOList(GLUT_USER1);
				Set_VAOList(GLUT_USER1, loadgluCylinder_EBO(5.0f, 5.0f, 2.0f, 6, 1)); // Càrrega cilindre com a VAO

				//if (Get_VAOId(GLUT_CUBE) != 0)deleteVAOList(GLUT_CUBE);
				Set_VAOList(GLUT_CUBE, loadglutSolidCube_EBO(1.0));			// Càrrega cub com a EBO

				//if (Get_VAOId(GLUT_TORUS) != 0)deleteVAOList(GLUT_TORUS);
				Set_VAOList(GLUT_TORUS, loadglutSolidTorus_EBO(1.0, 5.0, 20, 20));

				//if (Get_VAOId(GLUT_USER2) != 0)deleteVAOList(GLUT_USER2);	
				Set_VAOList(GLUT_USER2, loadgluCylinder_EBO(1.0f, 0.5f, 5.0f, 60, 1)); // Càrrega cilindre com a VAO

				//if (Get_VAOId(GLUT_USER3) != 0)deleteVAOList(GLUT_USER3);
				Set_VAOList(GLUT_USER3, loadgluCylinder_EBO(0.35f, 0.35f, 5.0f, 80, 1)); // Càrrega cilindre com a VAO

				//if (Get_VAOId(GLUT_USER4) != 0)deleteVAOList(GLUT_USER4);
				Set_VAOList(GLUT_USER4, loadgluCylinder_EBO(4.0f, 2.0f, 10.25f, 40, 1)); // Càrrega cilindre com a VAO

				//if (Get_VAOId(GLUT_USER5) != 0) deleteVAOList(GLUT_USER5);
				Set_VAOList(GLUT_USER5, loadgluCylinder_EBO(1.5f, 4.5f, 2.0f, 8, 1)); // Càrrega cilindre com a VAO

				//if (Get_VAOId(GLUT_USER6) != 0) deleteVAOList(GLUT_USER6);
				Set_VAOList(GLUT_USER6, loadgluDisk_EBO(0.0f, 1.5f, 8, 1)); // Càrrega disk com a VAO
			}
			break;
		case 9: // Opció OBJECTE Arxiu OBJ
			objecte = OBJOBJ;
			break;
		case 10: // Opció OBJECTE CORBA BEZIER
			if (objecte != C_BEZIER) {
				nomFitxer = NULL;
				// Entorn VGI: Obrir diàleg de lectura de fitxer (fitxers (*.MNT)
				result = NFD_OpenDialog(NULL, NULL, &nomFitxer);

				if (result == NFD_OKAY) {
					puts("Bezier File Success!");
					puts(nomFitxer);

					objecte = C_BEZIER;		sw_material[4] = true;
					npts_T = llegir_ptsC(nomFitxer);
					free(nomFitxer);

					// Càrrega dels VAO's per a construir la corba Bezier
					netejaVAOList();						// Neteja Llista VAO.

					// Posar color objecte (col_obj) al vector de colors del VAO.
					SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);

					// Definir Esfera EBO per a indicar punts de control de la corba
					Set_VAOList(GLU_SPHERE, loadgluSphere_EBO(5.0, 20, 20));	// Genera esfera i la guarda a la posició GLUT_CUBE.

					// Definir Corba Bezier com a VAO
						//Set_VAOList(CRV_BEZIER, load_Bezier_Curve_VAO(npts_T, PC_t, pas_CS, false)); // Genera corba i la guarda a la posició CRV_BEZIER.
					Set_VAOList(CRV_BEZIER, load_Bezier_Curve_EBO(npts_T, PC_t, pas_CS, false)); // Genera corba i la guarda a la posició CRV_BEZIER.
				}
			}
			break;
		case 11: // Opció OBJECTE CORBA B-SPLINE
			if (objecte != C_BSPLINE) {
				nomFitxer = NULL;
				// Entorn VGI: Obrir diàleg de lectura de fitxer (fitxers (*.MNT)
				result = NFD_OpenDialog(NULL, NULL, &nomFitxer);

				if (result == NFD_OKAY) {
					puts("Bezier File Success!");
					puts(nomFitxer);

					objecte = C_BSPLINE;		sw_material[4] = true;
					npts_T = llegir_ptsC(nomFitxer);
					free(nomFitxer);

					// Càrrega dels VAO's per a construir la corba BSpline
					netejaVAOList();						// Neteja Llista VAO.

					// Posar color objecte (col_obj) al vector de colors del VAO.
					SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);

					// Definir Esfera EBO per a indicar punts de control de la corba
					Set_VAOList(GLU_SPHERE, loadgluSphere_EBO(5.0, 20, 20));	// Guarda (vaoId, vboId, nVertexs) a la posició GLUT_CUBE.

					// Definr Corba BSpline com a VAO
						//Set_VAOList(CRV_BSPLINE, load_BSpline_Curve_VAO(npts_T, PC_t, pas_CS)); // Genera corba i la guarda a la posició CRV_BSPLINE.
					Set_VAOList(CRV_BSPLINE, load_BSpline_Curve_EBO(npts_T, PC_t, pas_CS)); // Genera corba i la guarda a la posició CRV_BSPLINE.
				}
			}
			break;
		case 12: // Opció OBJECTE CORBA LEMNISCATA
			if (objecte != C_LEMNISCATA) {
				objecte = C_LEMNISCATA;		sw_material[4] = true;

				// Càrrega dels VAO's per a construir la corba Bezier
				netejaVAOList();						// Neteja Llista VAO.

				// Posar color objecte (col_obj) al vector de colors del VAO.
				SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);

				// Definr Corba Lemniscata 3D com a VAO
					//Set_VAOList(CRV_LEMNISCATA3D, load_Lemniscata3D_VAO(800, pas_CS * 20.0)); // Genera corba i la guarda a la posició CRV_LEMNISCATA3D.
				Set_VAOList(CRV_LEMNISCATA3D, load_Lemniscata3D_EBO(800, pas_CS * 20.0)); // Genera corba i la guarda a la posició CRV_LEMNISCATA3D.
			}

			break;
		}
	}

	// DESPLEGABLE VISTA
	//IMGUI_DEMO_MARKER("Help");
	if (ImGui::CollapsingHeader("TRANSFORMA"))
	{
		//IMGUI_DEMO_MARKER("Widgets/Basic/RadioButton");
		ImGui::Checkbox("Traslacio (<Shift>+T)", &trasl); ImGui::SameLine();
		static int clickTR = 0;
		if (ImGui::Button("Origen Traslacio")) clickTR++;
		ImGui::Spacing();

		if (trasl) { rota = false; escal = false; transf = true; }
		else transf = rota || escal;

		static int clickTT = 0;
		// EntornVGI: Si s'ha apretat el botó "Origen Esfèrica"
		if (clickTT)
		{
			clickTT = 0;
			OPV.R = 15.0;	OPV.alfa = 0.0; OPV.beta = 0.0;
		}

		static int clickTRota = 0;
		ImGui::Checkbox("Rotacio (<Shift>+R)", &rota); ImGui::SameLine();
		if (ImGui::Button("Origen Rotacio")) clickTRota++;
		ImGui::Spacing();

		if (rota) { trasl = false; escal = false; transf = true; }
		else transf = trasl || escal;

		// EntornVGI: Si s'ha apretat el botó "Origen Navega"
		if (clickTRota)
		{
			clickTR = 0;
			n[0] = 0.0;		n[1] = 0.0;		n[2] = 0.0;
			opvN.x = 10.0;	opvN.y = 0.0;		opvN.z = 0.0;
			angleZ = 0.0;
		}

		static int clickTE = 0;
		ImGui::Checkbox("Escalat (<Shift>+E)", &rota); ImGui::SameLine();
		if (ImGui::Button("Origen Escalat")) clickTE++;

		if (escal) { trasl = false; rota = false; transf = true; }
		else transf = trasl || rota;

		// EntornVGI: Si s'ha apretat el botó "Origen Geode"
		if (clickTE)
		{
			clickTE = 0;
			OPV.R = 0.0;		OPV.alfa = 0.0;		OPV.beta = 0.0;				// Origen PV en esfèriques
			mobil = true;		zzoom = true;	zzoomO = false;	 satelit = false;	pan = false;
			Vis_Polar = POLARZ;	llumGL[5].encesa = true;
			glFrontFace(GL_CW);
		}

		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::SeparatorText("MOBIL TRANSFORMA:");
		ImGui::PopStyleColor();

		ImGui::Checkbox("Mobil Eix X? (<Shift>+E)", &transX); ImGui::SameLine();
		ImGui::Checkbox("Mobil Eix y? (<Shift>+E)", &transY); ImGui::SameLine();
		ImGui::Checkbox("Mobil Eix Z? (<Shift>+E)", &transZ);

		if (transX) {
			mobil = false;	zzoom = false;
			pan = false;	//navega = false;
		}
		else if ((!transY) && (!transZ)) {
			mobil = true;
			zzoom = true;
		}

		if (transY) {
			mobil = false;	zzoom = false;
			pan = false;	//navega = false;
		}
		else if ((!transX) && (!transZ)) {
			mobil = true;
			zzoom = true;
		}

		if (transZ) {
			mobil = false;	zzoom = false;
			pan = false;	//navega = false;
		}
		else if ((!transX) && (!transY)) {
			mobil = true;
			zzoom = true;
		}
	}

	// DESPLEGABLE VISTA
	//IMGUI_DEMO_MARKER("Help");
	if (ImGui::CollapsingHeader("OCULTACIONS"))
	{
		//IMGUI_DEMO_MARKER("Widgets/Selectables/Single Selection OCULTACIONS");
		if (ImGui::BeginTable("split", 2))
		{	ImGui::TableNextColumn(); ImGui::Checkbox("Test Visibilitat? (<Ctrl>+C)", &test_vis);
			ImGui::TableNextColumn(); ImGui::Checkbox("Z-Buffer? (<Ctrl>+O)", &oculta);
			ImGui::EndTable();
		}
			ImGui::Separator();
		if (ImGui::BeginTable("split", 2))
		{	ImGui::TableNextColumn(); ImGui::Checkbox("Front Faces? (<Ctrl>+D)", &front_faces);
			ImGui::TableNextColumn(); ImGui::Checkbox("Back Line? (<Ctrl>+B)", &back_line);
			ImGui::EndTable();
		}
	}

// DESPLEGABLE ILUMINACIO
//IMGUI_DEMO_MARKER("Help");
	if (ImGui::CollapsingHeader("ILUMINACIO"))
	{
		ImGui::Checkbox("Llum Fixe (T) / Llum lligada a camera (F)", &ifixe);
		ImGui::Spacing();

		// Entorn VGI. Mostrar Opcions desplegable TIPUS ILUMINACIO
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::SeparatorText("TIPUS ILUMINACIO:");
		ImGui::PopStyleColor();

		oIlumina = shortCut_Iluminacio();	//static int oIlumina = 1;

/*
		//IMGUI_DEMO_MARKER("Widgets/Basic/RadioButton");
		if (ImGui::BeginTable("split", 3))
		{
			ImGui::TableNextColumn(); ImGui::RadioButton("Punts (<Ctrl>+F1)", &oIlumina, 0);
			ImGui::TableNextColumn(); ImGui::RadioButton("Filferros (<Ctrl>+F2)", &oIlumina, 1);
			ImGui::TableNextColumn(); ImGui::RadioButton("Plana (<Ctrl>+F3)", &oIlumina, 2);
			ImGui::TableNextColumn(); ImGui::RadioButton("Gouraud (<Ctrl>+F4)", &oIlumina, 3);
			ImGui::TableNextColumn(); ImGui::RadioButton("Phong (<Ctrl>+F5)", &oIlumina, 4);
			ImGui::EndTable();
		}
		ImGui::Spacing();
*/

/*
		// Using the _simplified_ one-liner Combo() api here
		// See "Combo" section for examples of how to use the more flexible BeginCombo()/EndCombo() api.
		//IMGUI_DEMO_MARKER("Widgets/Basic/Combo");
		const char* itemsI[] = { "Punts (<Ctrl>+F1)", "Filferros (<Ctrl>+F2)", "Plana (<Ctrl>+F3)", 
			"Gouraud (<Ctrl>+F4)", "Phong (<Ctrl>+F5)"};
		ImGui::Combo(" ", &oIlumina, itemsI, IM_ARRAYSIZE(itemsI));
		ImGui::Spacing();
*/

		// Combo Boxes are also called "Dropdown" in other systems
		// Expose flags as checkbox for the demo
		static ImGuiComboFlags flags = ( 0 && ImGuiComboFlags_PopupAlignLeft && ImGuiComboFlags_NoPreview && ImGuiComboFlags_NoArrowButton);

		// Using the generic BeginCombo() API, you have full control over how to display the combo contents.
		// (your selection data could be an index, a pointer to the object, an id for the object, a flag intrusively
		// stored in the object itself, etc.)
		const char* items[] = { "Punts (<Ctrl>+F1)", "Filferros (<Ctrl>+F2)", "Plana (<Ctrl>+F3)", "Suau (<Ctrl>+F4)"};
		const char* combo_preview_value = items[oIlumina];  // Pass in the preview value visible before opening the combo (it could be anything)
		if (ImGui::BeginCombo("Tipus d'Iluminacio", combo_preview_value, flags))
		{	for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{	const bool is_selected = (oIlumina == n);
				if (ImGui::Selectable(items[n], is_selected))
					oIlumina = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		// Entorn VGI. Gestió opcions desplegable TIPUS ILUMINACIO segons el valor de la variable selected
		switch (oIlumina)
		{
		case 0: // Opció ILUMINACIO Punts
			if (ilumina != PUNTS) {
				ilumina = PUNTS;
				test_vis = false;		oculta = false;
			}
			break;
		case 1: // Opció ILUMINACIO Filferros
			if (ilumina != FILFERROS) {
				ilumina = FILFERROS;
				test_vis = false;		oculta = false;
			}
			break;

		case 2:	// Opció ILUMINACIO Plana
			if (ilumina != PLANA) {
				ilumina = PLANA;
				test_vis = false;		oculta = true;
			}
			break;

		case 3:	// Opció ILUMINACIO Suau
			if (ilumina != SUAU) {
				ilumina = SUAU;
				test_vis = false;		oculta = true;
			}
			break;
		}

		//IMGUI_DEMO_MARKER("Widgets/Selectables/Single Selection REFLEXIO MATERIAL");
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::SeparatorText("REFLEXIO MATERIAL:");
		ImGui::PopStyleColor();
		ImGui::Checkbox("Material (T) / Color (F)", &sw_material[4]);
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::BeginTable("split", 2))
		{	ImGui::TableNextColumn(); ImGui::Checkbox("Emissio?", &sw_material[0]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Ambient?", &sw_material[1]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Difusa?", &sw_material[2]);
			ImGui::TableNextColumn(); ImGui::Checkbox("Especular?", &sw_material[3]);
			ImGui::EndTable();
		}

		//IMGUI_DEMO_MARKER("Widgets/Selectables/Single Selection TEXTURES");
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::SeparatorText("TEXTURES:");
		ImGui::PopStyleColor();
		ImGui::Checkbox("Textura?", &textura);
		ImGui::SameLine();


		static int clickITS = 0;
		if (ImGui::Button("Imatge Textura SOIL")) clickITS++;
		// EntornVGI: Si s'ha apretat el botó "Image Textura SOIL"
		if (clickITS)	{
			clickITS = 0;
			t_textura = FITXERIMA;		tFlag_invert_Y = true;

			// Entorn VGI: Diàleg de la cerca de fitxer textura.
			nfdchar_t* nomFitxer = NULL;
			nfdresult_t result = NFD_OpenDialog(NULL, NULL, &nomFitxer);

			if (result == NFD_OKAY) {
				puts("Success!");
				puts(nomFitxer);

				textura = true;
				// Entorn VGI: Eliminar buffers de textures previs del vector texturesID[].
				for (int i = 0; i < NUM_MAX_TEXTURES; i++) {
					if (texturesID[i]) {
						//err = glIsTexture(texturesID[i]);
						glDeleteTextures(1, &texturesID[i]);
						//err = glIsTexture(texturesID[i]);
						texturesID[i] = 0;
						}
					}
				// EntornVGI: Carregar fitxer textura i definir buffer de textura.Identificador guardat a texturesID[0].
				texturesID[0] = loadIMA_SOIL(nomFitxer);

				//	Pas de textura al shader
				if (!shader_programID) glUniform1i(glGetUniformLocation(shader_programID, "texture0"), GLint(0));
				free(nomFitxer);
				}
			}

		ImGui::Spacing();
		ImGui::Checkbox("Flag_Invert_Y?", &tFlag_invert_Y);
		if ((tFlag_invert_Y) && (!shader_programID)) glUniform1i(glGetUniformLocation(shader_programID, "flag_invert_y"), tFlag_invert_Y);
	}

// DESPLEGABLE LLUMS
//IMGUI_DEMO_MARKER("Help");
	if (ImGui::CollapsingHeader("LLUMS"))
	{
		ImGui::Checkbox("Llum Ambient? (<Ctrl>+A)", &llum_ambient);
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::BeginTable("split", 2))
			{	ImGui::TableNextColumn(); ImGui::Checkbox("Llum #0 (+Z)? (<Ctrl>+1)", &llumGL[0].encesa);
				ImGui::TableNextColumn(); ImGui::Checkbox("Llum #1 (+X)? (<Ctrl>+2)", &llumGL[1].encesa);
				ImGui::TableNextColumn(); ImGui::Checkbox("Llum #2 (+Y)? (<Ctrl>+2)", &llumGL[2].encesa);
				ImGui::TableNextColumn(); ImGui::Checkbox("Llum #3 (Z=Y=X) (<Ctrl>+3)?", &llumGL[3].encesa);
				ImGui::TableNextColumn(); ImGui::Checkbox("Llum #4 (-Z)?(<Ctrl>+4)", &llumGL[4].encesa);
				ImGui::EndTable();
			}

		if (llum_ambient) 
			{	sw_il = true;
				if (!shader_programID) glUniform1i(glGetUniformLocation(shader_programID, "sw_intensity[1]"), (llum_ambient && sw_material[1])); // Pas màscara llums al shade
			}
			else sw_il = true;

		if (llumGL[0].encesa)
			{	sw_il = true;
				glUniform1i(glGetUniformLocation(shader_programID, "sw_lights[0]"), llumGL[0].encesa); // Pas màscara Llum #0 al shader
			}
			else sw_il = true;

		if (llumGL[1].encesa)
			{	sw_il = true;
				glUniform1i(glGetUniformLocation(shader_programID, "sw_lights[0]"), llumGL[1].encesa); // Pas màscara Llum #1 al shader
			}
			else sw_il = true;

		if (llumGL[2].encesa)
			{	sw_il = true;
			glUniform1i(glGetUniformLocation(shader_programID, "sw_lights[0]"), llumGL[2].encesa); // Pas màscara Llum #2 al shader
			}
			else sw_il = true;

		if (llumGL[3].encesa)
			{	sw_il = true;
				glUniform1i(glGetUniformLocation(shader_programID, "sw_lights[0]"), llumGL[3].encesa); // Pas màscara Llum #3 al shader
			}
			else sw_il = true;
	}

// DESPLEGABLE SHADERS
//IMGUI_DEMO_MARKER("Help");
	if (ImGui::CollapsingHeader("SHADERS"))
	{
		// Combo Boxes are also called "Dropdown" in other systems
		// Expose flags as checkbox for the demo
		static ImGuiComboFlags flagsS = (0 && ImGuiComboFlags_PopupAlignLeft && ImGuiComboFlags_NoPreview && ImGuiComboFlags_NoArrowButton);

		// Using the generic BeginCombo() API, you have full control over how to display the combo contents.
		// (your selection data could be an index, a pointer to the object, an id for the object, a flag intrusively
		// stored in the object itself, etc.)
		const char* itemsS[] = { "Flat (<Ctrl>+F)", "Gouraud (<Ctrl>+G)", "Phong (<Ctrl>+P)",
			"Carregar fitxers Shader (.vert, .frag)", "Carregar Binary Program (fitxer .bin)",
			"Gravar Binary Program (fitxer .bin)" };
		const char* combo_preview_value = itemsS[oShader];  // Pass in the preview value visible before opening the combo (it could be anything)
		if (ImGui::BeginCombo("Tipus de Shader", combo_preview_value, flagsS))
		{
			for (int n = 0; n < IM_ARRAYSIZE(itemsS); n++)
			{
				const bool is_selected = (oShader == n);
				if (ImGui::Selectable(itemsS[n], is_selected))
					oShader = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		// Entorn VGI. Definició de nou Program que substituirà l'actual
		GLuint newShaderID = 0;
		// Entorn VGI. Gestió opcions desplegable TIPUS ILUMINACIO segons el valor de la variable selected
		switch (oShader)
		{
		case 0: // Opció SHADER Flat
			if (shader != FLAT_SHADER) {
				shader = FLAT_SHADER;
				test_vis = false;		oculta = true;
				// Càrrega Shader de Gouraud
				fprintf(stderr, "Flat Shader: \n");
				// Elimina shader anterior
				shaderLighting.DeleteProgram();	shader_programID = 0;
				// Càrrega Gouraud shader
				shader_programID = shaderLighting.loadFileShaders(".\\shaders\\flat_shdrML.vert", ".\\shaders\\flat_shdrML.frag");
				glUseProgram(shader_programID); // shaderLighting.use();
			}
			break;

		case 1:	// Opció SHADER Gouraud
			if (shader != GOURAUD_SHADER) {
				shader = GOURAUD_SHADER;
				test_vis = false;		oculta = true;
				// Càrrega Shader de Gouraud
				fprintf(stderr, "Gouraud_shdrML Shader: \n");
				// Elimina shader anterior
				shaderLighting.DeleteProgram();	shader_programID = 0;
				// Càrrega Gouraud shader
				shader_programID = shaderLighting.loadFileShaders(".\\shaders\\gouraud_shdrML.vert", ".\\shaders\\gouraud_shdrML.frag");
				glUseProgram(shader_programID); // shaderLighting.use();
			}
			break;

		case 2:	// Opció SHADER Phong
			if (shader != PHONG_SHADER) {
				shader = PHONG_SHADER;
				test_vis = false;		oculta = true;
				// Càrrega Shader de Phong
				fprintf(stderr, "Phong_shdrML: \n");
				// Elimina shader anterior
				shaderLighting.DeleteProgram(); shader_programID = 0;
				// Càrrega Phong Shader
				shader_programID = shaderLighting.loadFileShaders(".\\shaders\\phong_shdrML.vert", ".\\shaders\\phong_shdrML.frag");
				glUseProgram(shader_programID); // shaderLighting.use();
			}
			break;

		case 3:	// Opció SHADER Carregar fitxers shader (.vert, .frag)
			if (shader != FILE_SHADER) {
				shader = FILE_SHADER;	ilumina = SUAU;
				test_vis = false;		oculta = true;
				Menu_Shaders_Opcio_CarregarVSFS();
				}
			break;

		case 4:	// Opció SHADER Carregar binary program, (.bin)
			if (shader != PROG_BINARY_SHADER) {
				shader = PROG_BINARY_SHADER;
				test_vis = false;		oculta = true;

				Menu_Shaders_Opcio_CarregarProgram();
				}
			break;

		case 5:	// Opció SHADER Gravar binary program (.bin)
			if (shader != PROG_BINARY_SHADERW) {
				shader = PROG_BINARY_SHADERW;
				test_vis = false;		oculta = true;

				Menu_Shaders_Opcio_GravarProgram();
				}
			break;
		}
	}

	// DESPLEGABLE SHADERS
//IMGUI_DEMO_MARKER("Help");
	if (ImGui::CollapsingHeader("ABOUT"))
	{
		//IMGUI_DEMO_MARKER("Window/About");
		ImGui::Text("VISUALITZACIO GRAFICA INTERACTIVA (Escola d'Enginyeria - UAB");
		ImGui::Text("Entorn Grafic VS2022 amb interficie GLFW, ImGui i OpenGL 4.3+ per a practiques i ABP");
		ImGui::Text("AUTOR: Enric Marti Godia");
		ImGui::Text("Copyright (C) 2023");
		ImGui::Separator();
	}

// End of ShowDemoWindow()
	ImGui::PopItemWidth();
	ImGui::End();
}

void Menu_Shaders_Opcio_CarregarVSFS()
{
		GLuint newShaderID = 0;

// Càrrega fitxer VERT
// Entorn VGI: Obrir diàleg de lectura de fitxer (fitxers (*.VERT)
	nfdchar_t* nomVert = NULL;
	nfdresult_t dialeg_Fitxer = NFD_OpenDialog("vert,vrt,vs", NULL, &nomVert);
	//				nfdresult_t result = NFD_OpenDialog(NULL, NULL, &nomVert);

	if (dialeg_Fitxer == NFD_OKAY) {
		puts("Fitxer .vert llegit!");
	}
	else if (dialeg_Fitxer == NFD_CANCEL) {
		puts("User pressed cancel.");
		return;
		}
	else {	printf("Error: %s\n", NFD_GetError());
			return;
			}

// Càrrega fitxer FRAG
// Entorn VGI: Obrir diàleg de lectura de fitxer (fitxers (*.FRAG)
	nfdchar_t* nomFrag = NULL;
	dialeg_Fitxer = NFD_OpenDialog("frag,frg,fs", NULL, &nomFrag);
	//				nfdresult_t resultFS = NFD_OpenDialog(NULL, NULL, &nomFrag);

	if (dialeg_Fitxer == NFD_OKAY) {
		puts("Fitxer .frag llegit!");
	}
	else if (dialeg_Fitxer == NFD_CANCEL) {
		puts("User pressed cancel.");
		return;
		}
	else {	printf("Error: %s\n", NFD_GetError());
			return;
			}
	// Càrrega Fitxers de shader .vert, .frag
	fprintf(stderr, "Càrregar fitxers .vert, .frag. \n");
	//				GLuint newShaderID = 0;
					// Elimina shader anterior
	shaderLighting.DeleteProgram();
	newShaderID = shaderLighting.loadFileShadersTest(nomVert, nomFrag);
	// Càrrega shaders dels fitxers
	if (!newShaderID) fprintf(stderr, "GLSL_Error. Fitxers .vert o .frag amb errors");
	else shader_programID = newShaderID;
	glUseProgram(shader_programID); // shaderLighting.use();
}

void Menu_Shaders_Opcio_CarregarProgram()
{
	FILE* fd;

// Càrrega fitxer PROGRAM;
// Entorn VGI: Obrir diàleg de lectura de fitxer (fitxers (*.bin)
	nfdchar_t* nomProgram = NULL;
	nfdresult_t dialeg_Fitxer = NFD_OpenDialog("bin,prog,txt", NULL, &nomProgram);

	if (dialeg_Fitxer == NFD_OKAY) {
		puts("Fitxer .bin llegit!");
	}
	else if (dialeg_Fitxer == NFD_CANCEL) {
		puts("User pressed cancel.");
		return;
		}
	else {
		fprintf(stderr, "Error: %s\n", NFD_GetError());  //printf("Error: %s\n", NFD_GetError());
		return;
		}
// Entorn VGI: To read de Shader Program from a file and install it
	GLint filelength = 0;
	GLenum format = 0;

// Entorn VGI: Read from a binary file
	FILE* sb;
	sb = fopen(nomProgram, "rb");
	if (!sb) {
		//printf("Error:GLSL_Error. Unable to open file");
		fprintf(stderr, "%s \n", "Error:GLSL_Error. Unable to open file");
		return;
	}

	// Get file length
	fseek(sb, 0, SEEK_END);
	filelength = ftell(sb);
	fseek(sb, 0, SEEK_SET);

	std::vector<GLubyte> buffer(filelength + 1); // Allocatem buffer amb mida de Binary Program
	fclose(sb);

	sb = fopen(nomProgram, "rb");
	fread(buffer.data(), filelength, 1, sb);
	fclose(sb);

	// Install shader binary
	GLint formats = 0;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);

	GLuint shader_BinProgramID = glCreateProgram();
	glProgramBinary(shader_BinProgramID, formats, buffer.data(), filelength);

	//glLinkProgram(shader_BinProgramID); // Linkedició del program.
// Check for success/failure
	GLint status;
	glGetProgramiv(shader_BinProgramID, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		// Llista error de linkedició del Shader Program
		GLint maxLength = 0;

		glGetProgramiv(shader_BinProgramID, GL_INFO_LOG_LENGTH, &maxLength);
		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(shader_BinProgramID, maxLength, &maxLength, &errorLog[0]);

		//printf("Error Linkedicio Shader Binary Program");
		fprintf(stderr, "%s \n", "Error Linkedicio Shader Program");

		// Volcar missatges error a fitxer GLSL_Error.LINK
		if ((fd = fopen("GLSL_Error.LINK", "w")) == NULL)
			{	//printf("GLSL_Error.LINK was not opened");
				fprintf(stderr, "%s \n", "GLSL_Error.LINK was not opened");
			}
		for (int i = 0; i <= maxLength; i = i++) fprintf(fd, "%c", errorLog[i]);
		fclose(fd);
		glDeleteProgram(shader_BinProgramID);		// Don't leak the program.
	}
	else {
		//shaderLighting.DeleteProgram();	// Eliminar shader anterior
		shader_programID = shader_BinProgramID; // Assignar nou Binary Program com l'actual.
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glUseProgram(shader_programID);			// Activa shader llegit.
	}
	glUseProgram(shader_programID); // shaderLighting.use();
}

void Menu_Shaders_Opcio_GravarProgram()
{
	// Obrir diàleg per a definir fitxer on gravar BINARY PROGRAM;
// Entorn VGI: Obrir diàleg de lectura de fitxer (fitxers (*.VERT)
	nfdchar_t* nomProgram = NULL;
	nfdresult_t dialeg_Fitxer = NFD_SaveDialog("bin,prog,txt", NULL, &nomProgram);
	//	nfdresult_t result = NFD_OpenDialog(NULL, NULL, &nomVert);

	if (dialeg_Fitxer == NFD_OKAY) {
		puts("Fitxer .bin llegit!");
	}
	else if (dialeg_Fitxer == NFD_CANCEL) {
		puts("User pressed cancel.");
		return;
		}
	else {	printf("Error: %s\n", NFD_GetError());
			return;
			}

// Entorn VGI: To retrieve the compiled Binary Program shader code and write it to a file
	GLint formats = 0;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);
	GLint* binaryFormats = new GLint[formats];
	glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, binaryFormats);

	GLint length = 0;
	glGetProgramiv(shader_programID, GL_PROGRAM_BINARY_LENGTH, &length);

	// Retrieve the binary code
	std::vector<GLubyte> buffer(length);
	GLenum* Formats = 0;
	glGetProgramBinary(shader_programID, length, NULL, (GLenum*)Formats, buffer.data());

	// Write the binary to a binary file
	FILE* sb;
	sb = fopen(nomProgram, "wb");
	fwrite(buffer.data(), length, 1, sb);
	fclose(sb);
	glUseProgram(shader_programID); // shaderLighting.use();
}

/* ------------------------------------------------------------------------- */
/*                           CONTROL DEL TECLAT                              */
/* ------------------------------------------------------------------------- */

// OnKeyDown: Funció de tractament de teclat (funció que es crida quan es prem una tecla)
//   PARÀMETRES:
//    - key: Codi del caracter seleccionat
//    - scancode: Nombre de vegades que s'ha apretat la tecla (acceleració)
//    - action: Acció de la tecla: GLFW_PRESS (si s'ha apretat), GLFW_REPEAT, si s'ha repetit pressió i GL_RELEASE, si es deixa d'apretar.
//    - mods: Variable que identifica si la tecla s'ha pulsat directa (mods=0), juntament amb la tecla Shift (mods=1) o la tecla Ctrl (mods=2).
void OnKeyDown(GLFWwindow* window, int key, int scancode, int action, int mods)
{
// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
	const double incr = 0.025f;
	double modul = 0;
	GLdouble vdir[3] = { 0, 0, 0 };

	// EntornVGI.ImGui: Test si events de mouse han sigut filtrats per ImGui (io.WantCaptureMouse)
// (1) ALWAYS forward mouse data to ImGui! This is automatic with default backends. With your own backend:
	ImGuiIO& io = ImGui::GetIO();
	//io.AddMouseButtonEvent(button, true);

	// (2) ONLY forward mouse data to your underlying app/game.
	if (!io.WantCaptureKeyboard) { //<Tractament mouse de l'aplicació>}
		// EntornVGI: Si tecla pulsada és ESCAPE, tancar finestres i aplicació.
		if (mods == 0 && key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
		else if (mods == 0 && key == GLFW_KEY_PRINT_SCREEN && action == GLFW_PRESS) statusB = !statusB;
		else if ((mods == 1) && (action == GLFW_PRESS)) Teclat_Shift(key, window);	// Shorcuts Shift Key
		else if ((mods == 2) && (action == GLFW_PRESS)) Teclat_Ctrl(key);	// Shortcuts Ctrl Key
		else if ((sw_grid) && ((grid.x) || (grid.y) || (grid.z))) Teclat_Grid(key, action);
		else if (((key == GLFW_KEY_G) && (action == GLFW_PRESS)) && ((grid.x) || (grid.y) || (grid.z))) sw_grid = !sw_grid;
		else if ((key == GLFW_KEY_O) && (action == GLFW_PRESS)) sw_color = true; // Activació color objecte
		else if ((key == GLFW_KEY_F) && (action == GLFW_PRESS)) sw_color = false; // Activació color objecte
		else if (pan) Teclat_Pan(key, action);
		else if (transf)
				{	if (rota) Teclat_TransRota(key, action);
						else if (trasl) Teclat_TransTraslada(key, action);
							else if (escal) Teclat_TransEscala(key, action);
				}
		else if (camera == CAM_NAVEGA) Teclat_Navega(key, action);
		else if (!sw_color) Teclat_ColorFons(key, action);
		else Teclat_ColorObjecte(key, action);
	}
// Crida a OnPaint() per redibuixar l'escena
	//OnPaint(window);

/*	if (key == GLFW_KEY_E && action == GLFW_PRESS)
		activate_airship();

	int state = glfwGetKey(window, GLFW_KEY_E);
	if (state == GLFW_PRESS) activate_airship();
*/
}

void OnKeyUp(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}

void OnTextDown(GLFWwindow* window, unsigned int codepoint)
{
}

// Teclat_Shift: Shortcuts per Pop Ups Fitxer, Finestra, Vista, Projecció i Objecte
void Teclat_Shift(int key, GLFWwindow* window)
{
	//const char* nomfitxer;
	bool testA = false;
	int error = 0;

	nfdchar_t* nomFitxer = NULL;
	nfdresult_t result; // = NFD_OpenDialog(NULL, NULL, &nomFitxer);

	CColor color_Mar = { 0.0,0.0,0.0,1.0 };

	switch (key)
	{	
// ----------- POP UP Fitxers
		// Tecla Obrir Fractal
		case GLFW_KEY_F1:
			nomFitxer = NULL;
			// Entorn VGI: Obrir diàleg de lectura de fitxer (fitxers (*.MNT)
			result = NFD_OpenDialog(NULL, NULL, &nomFitxer);

			if (result == NFD_OKAY) {
				puts("Fitxer Fractal Success!");
				puts(nomFitxer);

				objecte = O_FRACTAL;
				// Entorn VGI: TO DO -> Llegir fitxer fractal (nomFitxer) i inicialitzar alçades

				free(nomFitxer);
				}
			break;

		// Tecla Obrir Fitxer OBJ
		case GLFW_KEY_F2:
			// Entorn VGI: Obrir diàleg de lectura de fitxer (fitxers (*.OBJ)
			nomFitxer = NULL;
			result = NFD_OpenDialog(NULL, NULL, &nomFitxer);

			if (result == NFD_OKAY) {
				puts("Success!");
				puts(nomFitxer);

				objecte = OBJOBJ;	textura = true;		tFlag_invert_Y = false;
				//char* nomfitx = nomFitxer;
				if (ObOBJ == NULL) ObOBJ = ::new COBJModel;
				else { // Si instància ja s'ha utilitzat en un objecte OBJ
					ObOBJ->netejaVAOList_OBJ();		// Netejar VAO, EBO i VBO
					ObOBJ->netejaTextures_OBJ();	// Netejar buffers de textures
					}

				//int error = ObOBJ->LoadModel(nomfitx);			// Carregar objecte OBJ amb textura com a varis VAO's
				int error = ObOBJ->LoadModel(nomFitxer);			// Carregar objecte OBJ amb textura com a varis VAO's

				//	Pas de paràmetres textura al shader
				if (!shader_programID) glUniform1i(glGetUniformLocation(shader_programID, "textur"), textura);
				if (!shader_programID) glUniform1i(glGetUniformLocation(shader_programID, "flag_invert_y"), tFlag_invert_Y);
				free(nomFitxer);
				}
			break;

// ----------- POP UP Finestra
		// Tecla Obrir nova finestra
		case GLFW_KEY_W:

			break;

// ----------- POP UP Camera

// 		// Tecla Camera Esferica
		case GLFW_KEY_L:
			if ((projeccio != ORTO) && (projeccio != CAP)) camera = CAM_ESFERICA;
			// Activació de zoom, mobil
			mobil = true;	zzoom = true;
			break;


		// Tecla Mobil?
		case GLFW_KEY_M:
			if ((projeccio != ORTO) && (projeccio != CAP)) mobil = !mobil;
			// Desactivació de Transformacions Geomètriques via mouse 
			//		si Visualització Interactiva activada.	
			if (mobil) {	transX = false;	transY = false; transZ = false;
						}
			break;

		// Tecla Zoom
		case GLFW_KEY_Q:
			if ((projeccio != ORTO) && (projeccio != CAP)) zzoom = !zzoom;
			// Desactivació de Transformacions Geomètriques via mouse 
			//		si Zoom activat.
			if (zzoom) {
				zzoomO = false;  transX = false;	transY = false;	transZ = false;
				}
			break;

		// Tecla Zoom Orto
		case GLFW_KEY_F3:
			if (projeccio == ORTO || projeccio==AXONOM) zzoomO = !zzoomO;
			// Desactivació de Transformacions Geomètriques via mouse 
			//		si Zoom Orto activat.
			if (zzoomO) {
				zzoom = false;  transX = false;		transY = false;		transZ = false;
				}
			break;

		// Tecla Satèl.lit?
		case GLFW_KEY_S:
			if ((projeccio != CAP && projeccio != ORTO)) satelit = !satelit;
			if (satelit) mobil = true;
			testA = anima;				// Testejar si hi ha alguna animació activa apart de Satèlit.
			break;

		// Tecla Camera Navega
		case GLFW_KEY_N:
			if ((projeccio != ORTO) && (projeccio != CAP)) camera = CAM_NAVEGA;
			// Desactivació de zoom, mobil, Transformacions Geomètriques via mouse i pan 
			//		si navega activat
			if (camera == CAM_NAVEGA)
			{	mobil = false;	zzoom = false;
				transX = false;	transY = false;	transZ = false;
				pan = false;
				tr_cpv.x = 0.0;		tr_cpv.y = 0.0;		tr_cpv.z = 0.0;	// Inicialitzar a 0 desplaçament de pantalla
				tr_cpvF.x = 0.0;	tr_cpvF.y = 0.0;	tr_cpvF.x = 0.0; // Inicialitzar a 0 desplaçament de pantalla
			}
			else {	mobil = true;
					zzoom = true;
				}
			break;

		// Tecla Camera Geode
		case GLFW_KEY_J:
			if ((projeccio != ORTO) && (projeccio != CAP)) camera = CAM_GEODE;
			// Desactivació de zoom, mobil, Transformacions Geomètriques via mouse i pan 
			//		si navega activat
			if (camera == CAM_GEODE)
			{
				OPV_G.R = 0.0;		OPV_G.alfa = 0.0;		OPV_G.beta = 0.0;
				OPV.R = 0.0;		OPV.alfa = 0.0;			OPV.beta = 0.0;				// Origen PV en esfèriques
				mobil = true;		zzoom = true;	zzoomO = false;	 satelit = false;	pan = false;
				Vis_Polar = POLARZ;	llumGL[5].encesa = true;
				glFrontFace(GL_CW);
			}
			break;

		// Tecla Escape (per a Pan i Navega)
		case GLFW_KEY_ESCAPE:
			if (pan) {	fact_pan = 1;
						tr_cpv.x = 0;	tr_cpv.y = 0;	tr_cpv.z = 0;
					}
			else 	if (camera == CAM_NAVEGA) {	n[0] = 0.0;		n[1] = 0.0;		n[2] = 0.0;
									opvN.x = 10.0;	opvN.y = 0.0;		opvN.z = 0.0;
									angleZ = 0.0;
								}
			break;

		// Tecla Polars Eix X?
		case GLFW_KEY_X:
			if ((projeccio != CAP) && (camera != CAM_NAVEGA)) Vis_Polar = POLARX;
			break;

		// Tecla Polars Eix Y?
		case GLFW_KEY_Y:
			if ((projeccio != CAP) && (camera != CAM_NAVEGA)) Vis_Polar = POLARY;
			break;

		// Tecla Polars Eix Z?
		case GLFW_KEY_Z:
			if ((projeccio != CAP) && (camera != CAM_NAVEGA)) Vis_Polar = POLARZ;
			break;

// ----------- POP UP Vista
// 
		// Tecla Full Screen?
		case GLFW_KEY_F:
			OnFull_Screen(primary, window);
			break;

		// Tecla Eixos?
		case GLFW_KEY_F4:
			eixos = !eixos;
			break;

		// Tecla Skybox Cube
		case GLFW_KEY_K:
			SkyBoxCube = !SkyBoxCube;
			if (SkyBoxCube)
			{	Vis_Polar = POLARY;
				// Càrrega VAO Skybox Cube
				if (skC_VAOID.vaoId == 0) skC_VAOID = loadCubeSkybox_VAO();
				
				if (!cubemapTexture)
					{	// load Skybox textures
						// -------------
						std::vector<std::string> faces =
							{	".\\textures\\skybox\\right.jpg",
								".\\textures\\skybox\\right.jpg",
								".\\textures\\skybox\\left.jpg",
								".\\textures\\skybox\\top.jpg",
								".\\textures\\skybox\\bottom.jpg",
								".\\textures\\skybox\\front.jpg",
								".\\textures\\skybox\\back.jpg"
							};
						cubemapTexture = loadCubemap(faces);	
					}
			}
			break;

		// Tecla Pan?
		case GLFW_KEY_G:
			if ((projeccio != ORTO) && (projeccio != CAP)) pan = !pan;
			// Desactivació de Transformacions Geomètriques via mouse i navega si pan activat
			if (pan) {	mobil = true;		zzoom = true;
						transX = false;		transY = false;	transZ = false;
					}
			break;

		// Tecla Grid XY?
		case GLFW_KEY_F5:
			grid.x = !grid.x;	hgrid.x = 0.0;
			if (grid.x) grid.w = false;
			break;

		// Tecla Grid XZ?
		case GLFW_KEY_F6:
			grid.y = !grid.y;	hgrid.y = 0.0;
			if (grid.y) grid.w = false;
			break;

		// Tecla Grid YZ?
		case GLFW_KEY_F7:
			grid.z = !grid.z;	hgrid.z = 0.0;
			if (grid.z) grid.w = false;
			break;

		// Tecla Grid XYZ?
		case GLFW_KEY_F8:
			grid.w = !grid.w;	hgrid.w = 0.0;
			if (grid.w)
				{	grid.x = false;	grid.y = false;		grid.z = false;
				}
			break;

// ----------- POP UP Projecció
		// Tecla Axonomètrica
		case GLFW_KEY_A:
			if (projeccio != AXONOM) {
				projeccio = AXONOM;
				mobil = true;			zzoom = true;
			}
			break;

		// Tecla Ortogràfica
		case GLFW_KEY_O:
			if (projeccio != ORTO) {
				projeccio = ORTO;
				mobil = false;			zzoom = false;
				}
			break;

		// Tecla Perspectiva
		case GLFW_KEY_P:
			if (projeccio != PERSPECT) {
				projeccio = PERSPECT;
				mobil = true;			zzoom = true;
				}
			break;

// ----------- POP UP Objecte
// 		// Tecla CAP
		case GLFW_KEY_B:
			if (objecte != CAP) {
				objecte = CAP;
				netejaVAOList();							// Neteja Llista VAO.
				}
			break;

		// Tecla Cub
		case GLFW_KEY_C:
			objecte = CUB;
			//	---- Entorn VGI: ATENCIÓ!!. Canviar l'escala per a centrar la vista (Ortogràfica)

			//  ---- Entorn VGI: ATENCIÓ!!. Modificar R per centrar la Vista a la mida de l'objecte (Perspectiva)
			netejaVAOList();											// Neteja Llista VAO.

			// Posar color objecte (col_obj) al vector de colors del VAO.
			SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);
			Set_VAOList(GLUT_CUBE, loadglutSolidCube_EBO(1.0));		// Genera EBO de cub mida 1 i el guarda a la posició GLUT_CUBE.
			break;

		// Tecla Cub RGB
		case GLFW_KEY_D:
			objecte = CUB_RGB;
			//	---- Entorn VGI: ATENCIÓ!!. Canviar l'escala per a centrar la vista (Ortogràfica)

			//  ---- Entorn VGI: ATENCIÓ!!. Modificar R per centrar la Vista a la mida de l'objecte (Perspectiva)
			netejaVAOList();						// Neteja Llista VAO.

			Set_VAOList(GLUT_CUBE_RGB, loadglutSolidCubeRGB_EBO(1.0));	// Genera EBO de cub mida 1 i el guarda a la posició GLUT_CUBE_RGB.
			break;

		// Tecla Esfera
		case GLFW_KEY_E:
			objecte = ESFERA;
			//	---- Entorn VGI: ATENCIÓ!!. Canviar l'escala per a centrar la vista (Ortogràfica)

			//  ---- Entorn VGI: ATENCIÓ!!. Modificar R per centrar la Vista a la mida de l'objecte (Perspectiva)
			netejaVAOList();						// Neteja Llista VAO.

			// Posar color objecte (col_obj) al vector de colors del VAO.
			SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);

			Set_VAOList(GLU_SPHERE, loadgluSphere_EBO(1.0, 30, 30));
			break;

		// Tecla Tetera
		case GLFW_KEY_T:
			objecte = TETERA;
			//	---- Entorn VGI: ATENCIÓ!!. Canviar l'escala per a centrar la vista (Ortogràfica)

			//  ---- Entorn VGI: ATENCIÓ!!. Modificar R per centrar la Vista a la mida de l'objecte (Perspectiva)
			netejaVAOList();						// Neteja Llista VAO.

			// Posar color objecte (col_obj) al vector de colors del VAO.
			SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);

			//if (Get_VAOId(GLUT_TEAPOT) != 0) deleteVAOList(GLUT_TEAPOT);
			Set_VAOList(GLUT_TEAPOT, loadglutSolidTeapot_VAO()); //Genera VAO tetera mida 1 i el guarda a la posició GLUT_TEAPOT.
			break;

		// Tecla Arc
		case GLFW_KEY_R:
			objecte = ARC;
			//  Modificar R per centrar la Vista a la mida de l'objecte (Perspectiva)
			//	Canviar l'escala per a centrar la vista (Ortogràfica)

			color_Mar.r = 0.5;	color_Mar.g = 0.4; color_Mar.b = 0.9; color_Mar.a = 1.0;
			//	---- Entorn VGI: ATENCIÓ!!. Canviar l'escala per a centrar la vista (Ortogràfica)

			//  ---- Entorn VGI: ATENCIÓ!!. Modificar R per centrar la Vista a la mida de l'objecte (Perspectiva)

			// Càrrega dels VAO's per a construir objecte ARC
			netejaVAOList();						// Neteja Llista VAO.

			// Posar color objecte (col_obj) al vector de colors del VAO.
			SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);

			//if (Get_VAOId(GLUT_CUBE) != 0) deleteVAOList(GLUT_CUBE);
			Set_VAOList(GLUT_CUBE, loadglutSolidCube_EBO(1.0));		// Càrrega Cub de costat 1 com a EBO a la posició GLUT_CUBE.

			//if (Get_VAOId(GLU_SPHERE) != 0) deleteVAOList(GLU_SPHERE);
			Set_VAOList(GLU_SPHERE, loadgluSphere_EBO(0.5, 20, 20));	// Càrrega Esfera a la posició GLU_SPHERE.

			//if (Get_VAOId(GLUT_TEAPOT) != 0) deleteVAOList(GLUT_TEAPOT);
			Set_VAOList(GLUT_TEAPOT, loadglutSolidTeapot_VAO());		// Carrega Tetera a la posició GLUT_TEAPOT.

			//if (Get_VAOId(MAR_FRACTAL_VAO) != 0) deleteVAOList(MAR_FRACTAL_VAO);
			Set_VAOList(MAR_FRACTAL_VAO, loadSea_VAO(color_Mar));		// Carrega Mar a la posició MAR_FRACTAL_VAO.
			break;

		// Tecla Tie (Star Wars)
		case GLFW_KEY_I:
			objecte = TIE;		textura = true;
				//	---- Entorn VGI: ATENCIÓ!!. Canviar l'escala per a centrar la vista (Ortogràfica)

				//  ---- Entorn VGI: ATENCIÓ!!. Modificar R per centrar la Vista a la mida de l'objecte (Perspectiva)

			// Càrrega dels VAO's per a construir objecte TIE
			netejaVAOList();						// Neteja Llista VAO.

			// Posar color objecte (col_obj) al vector de colors del VAO.
			SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);

			//if (Get_VAOId(GLU_CYLINDER) != 0) deleteVAOList(GLU_CYLINDER);
			Set_VAOList(GLUT_CYLINDER, loadgluCylinder_EBO(5.0f, 5.0f, 0.5f, 6, 1));// Càrrega cilindre com a VAO.

			//if (Get_VAOId(GLU_DISK) != 0)deleteVAOList(GLU_DISK);
			Set_VAOList(GLU_DISK, loadgluDisk_EBO(0.0f, 5.0f, 6, 1));	// Càrrega disc com a VAO

			//if (Get_VAOId(GLU_SPHERE) != 0)deleteVAOList(GLU_SPHERE);
			Set_VAOList(GLU_SPHERE, loadgluSphere_EBO(10.0f, 80, 80));	// Càrrega disc com a VAO

			//if (Get_VAOId(GLUT_USER1) != 0)deleteVAOList(GLUT_USER1);
			Set_VAOList(GLUT_USER1, loadgluCylinder_EBO(5.0f, 5.0f, 2.0f, 6, 1)); // Càrrega cilindre com a VAO

			//if (Get_VAOId(GLUT_CUBE) != 0)deleteVAOList(GLUT_CUBE);
			Set_VAOList(GLUT_CUBE, loadglutSolidCube_EBO(1.0));			// Càrrega cub com a EBO

			//if (Get_VAOId(GLUT_TORUS) != 0)deleteVAOList(GLUT_TORUS);
			Set_VAOList(GLUT_TORUS, loadglutSolidTorus_EBO(1.0, 5.0, 20, 20));

			//if (Get_VAOId(GLUT_USER2) != 0)deleteVAOList(GLUT_USER2);	
			Set_VAOList(GLUT_USER2, loadgluCylinder_EBO(1.0f, 0.5f, 5.0f, 60, 1)); // Càrrega cilindre com a VAO

			//if (Get_VAOId(GLUT_USER3) != 0)deleteVAOList(GLUT_USER3);
			Set_VAOList(GLUT_USER3, loadgluCylinder_EBO(0.35f, 0.35f, 5.0f, 80, 1)); // Càrrega cilindre com a VAO

			//if (Get_VAOId(GLUT_USER4) != 0)deleteVAOList(GLUT_USER4);
			Set_VAOList(GLUT_USER4, loadgluCylinder_EBO(4.0f, 2.0f, 10.25f, 40, 1)); // Càrrega cilindre com a VAO

			//if (Get_VAOId(GLUT_USER5) != 0) deleteVAOList(GLUT_USER5);
			Set_VAOList(GLUT_USER5, loadgluCylinder_EBO(1.5f, 4.5f, 2.0f, 8, 1)); // Càrrega cilindre com a VAO

			//if (Get_VAOId(GLUT_USER6) != 0) deleteVAOList(GLUT_USER6);
			Set_VAOList(GLUT_USER6, loadgluDisk_EBO(0.0f, 1.5f, 8, 1)); // Càrrega disk com a VAO
			break;

		// Tecla Corbes Bezier
		case GLFW_KEY_F9:
			nomFitxer = NULL;
			// Entorn VGI: Obrir diàleg de lectura de fitxer (fitxers (*.MNT)
			result = NFD_OpenDialog(NULL, NULL, &nomFitxer);

			if (result == NFD_OKAY) {
				puts("Bezier File Success!");
				puts(nomFitxer);

				nom = "";			objecte = C_BEZIER;
				npts_T = llegir_ptsC(nomFitxer);
				free(nomFitxer);

				// Càrrega dels VAO's per a construir la corba Bezier
				netejaVAOList();						// Neteja Llista VAO.

				// Posar color objecte (col_obj) al vector de colors del VAO.
				SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);

				// Definir Esfera EBO per a indicar punts de control de la corba
				Set_VAOList(GLU_SPHERE, loadgluSphere_EBO(5.0, 20, 20));	// Genera esfera i la guarda a la posició GLUT_CUBE.

				// Definir Corba Bezier com a VAO
					//Set_VAOList(CRV_BEZIER, load_Bezier_Curve_VAO(npts_T, PC_t, pas_CS, false)); // Genera corba i la guarda a la posició CRV_BEZIER.
				Set_VAOList(CRV_BEZIER, load_Bezier_Curve_EBO(npts_T, PC_t, pas_CS, false)); // Genera corba i la guarda a la posició CRV_BEZIER.
				}
			break;

		// Tecla Corbes B-Spline
		case GLFW_KEY_F10:
			nomFitxer = NULL;
			// Entorn VGI: Obrir diàleg de lectura de fitxer (fitxers (*.MNT)
			result = NFD_OpenDialog(NULL, NULL, &nomFitxer);

			if (result == NFD_OKAY) {
				puts("B-Spline File Success!");
				puts(nomFitxer);

				nom = "";			objecte = C_BSPLINE;
				npts_T = llegir_ptsC(nomFitxer);
				free(nomFitxer);

				// Càrrega dels VAO's per a construir la corba BSpline
				netejaVAOList();						// Neteja Llista VAO.

				// Posar color objecte (col_obj) al vector de colors del VAO.
				SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);

				// Definir Esfera EBO per a indicar punts de control de la corba
				Set_VAOList(GLU_SPHERE, loadgluSphere_EBO(5.0, 20, 20));	// Guarda (vaoId, vboId, nVertexs) a la posició GLUT_CUBE.

				// Definr Corba BSpline com a VAO
					//Set_VAOList(CRV_BSPLINE, load_BSpline_Curve_VAO(npts_T, PC_t, pas_CS)); // Genera corba i la guarda a la posició CRV_BSPLINE.
				Set_VAOList(CRV_BSPLINE, load_BSpline_Curve_EBO(npts_T, PC_t, pas_CS)); // Genera corba i la guarda a la posició CRV_BSPLINE.
				}
			break;

		// Tecla Corbes Lemniscata
		case GLFW_KEY_F11:
			if (objecte != C_LEMNISCATA) {
				objecte = C_LEMNISCATA;
				// Càrrega dels VAO's per a construir la corba Bezier
				netejaVAOList();						// Neteja Llista VAO.

				// Posar color objecte (col_obj) al vector de colors del VAO.
				SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);

				// Definr Corba Lemniscata 3D com a VAO
					//Set_VAOList(CRV_LEMNISCATA3D, load_Lemniscata3D_VAO(800, pas_CS * 20.0)); // Genera corba i la guarda a la posició CRV_LEMNISCATA3D.
				Set_VAOList(CRV_LEMNISCATA3D, load_Lemniscata3D_EBO(800, pas_CS * 20.0)); // Genera corba i la guarda a la posició CRV_LEMNISCATA3D.
				}
			break;

		// Tecla Punts de Control?
		case GLFW_KEY_F12:
			sw_Punts_Control = !sw_Punts_Control;
			break;

		// Tecla Matriu Primitives
		case GLFW_KEY_H:
			objecte = MATRIUP;
			break;

		// Tecla Matriu Primitives VAO
		case GLFW_KEY_V:
			if (objecte != MATRIUP_VAO) {
				objecte = MATRIUP_VAO;

				// Càrrega dels VAO's per a construir objecte ARC
				netejaVAOList();						// Neteja Llista VAO.

				// Posar color objecte (col_obj) al vector de colors del VAO.
				SetColor4d(col_obj.r, col_obj.g, col_obj.b, col_obj.a);

				//if (Get_VAOId(GLUT_CUBE) != 0) deleteVAOList(GLUT_CUBE);
				Set_VAOList(GLUT_CUBE, loadglutSolidCube_EBO(1.0f));

				//if (Get_VAOId(GLUT_TORUS) != 0)deleteVAOList(GLUT_TORUS);
				Set_VAOList(GLUT_TORUS, loadglutSolidTorus_EBO(2.0, 3.0, 20, 20));

				//if (Get_VAOId(GLUT_SPHERE) != 0) deleteVAOList(GLU_SPHERE);
				Set_VAOList(GLU_SPHERE, loadgluSphere_EBO(1.0, 20, 20));
			}
			break;

		default:
			break;
		}
}


// Teclat_Ctrl: Shortcuts per Pop Ups Transforma, Iluminació, llums, Shaders
void Teclat_Ctrl(int key)
{
	//const char* nomfitxer;
	std::string nomVert, nomFrag;	// Nom de fitxer.

	nfdchar_t* nomFitxer = NULL;
	nfdresult_t result; // = NFD_OpenDialog(NULL, NULL, &nomFitxer);

	switch (key)
	{
		// ----------- POP UP TRANSFORMA
			// Tecla Transforma --> Traslació?
	case GLFW_KEY_T:
		trasl = !trasl;
		rota = false;
		if (trasl) escal = true;
		transf = trasl || rota || escal;
		break;

		// Tecla Transforma --> Rotació?
	case GLFW_KEY_R:
		rota = !rota;
		trasl = false;
		if (rota) escal = true;
		transf = trasl || rota || escal;
		break;

		// Tecla Transforma --> Escalat?
	case GLFW_KEY_S:
		if ((!rota) && (!trasl)) escal = !escal;
		transf = trasl || rota || escal;
		break;

		// Tecla Escape (per a Transforma --> Origen Traslació, Transforma --> Origen Rotació i Transforma --> Origen Escalat)
	case GLFW_KEY_ESCAPE:
		if (trasl)
		{
			fact_Tras = 1;
			TG.VTras.x = 0.0;	TG.VTras.y = 0.0;	TG.VTras.z = 0;
		}
		else if (rota) {
			fact_Rota = 90;
			TG.VRota.x = 0;		TG.VRota.y = 0;		TG.VRota.z = 0;
		}
		else if (escal) { TG.VScal.x = 1;	TG.VScal.y = 1;	TG.VScal.z = 1; }
		break;

		// Tecla Transforma --> Mobil Eix X? (opció booleana).
	case GLFW_KEY_X:
		if (transf)
		{
			transX = !transX;
			if (transX) {
				mobil = false;	zzoom = false;
				pan = false;
			}
			else if ((!transY) && (!transZ)) {
				mobil = true;
				zzoom = true;
			}
		}
		break;

		// Tecla Transforma --> Mobil Eix Y? (opció booleana).
	case GLFW_KEY_Y:
		if (transf)
		{
			transY = !transY;
			if (transY) {
				mobil = false;	zzoom = false;
				pan = false;
			}
			else if ((!transX) && (!transZ)) {
				mobil = true;
				zzoom = true;
			}
		}
		break;

		// Tecla Transforma --> Mobil Eix Z? (opció booleana).
	case GLFW_KEY_Z:
		if (transf)
		{
			transZ = !transZ;
			if (transZ) {
				mobil = false;	zzoom = false;
				pan = false;
			}
			else if ((!transX) && (!transY)) {
				mobil = true;
				zzoom = true;
			}
		}
		break;

		// ----------- POP UP OCULTACIONS
			// Tecla Ocultacions --> Front faces? (opció booleana).
	case GLFW_KEY_D:
		front_faces = !front_faces;
		break;

		// Tecla Ocultacions --> Test Visibilitat? (back face culling)
	case GLFW_KEY_C:
		test_vis = !test_vis;
		break;

		// Tecla Ocultacions --> Z-Buffer? (opció booleana).
	case GLFW_KEY_O:
		oculta = !oculta;
		break;

		// Tecla Ocultacions --> Back-lines? (opció booleana).
	case GLFW_KEY_B:
		back_line = !back_line;
		break;

		// ----------- POP UP ILUMINACIÓ
			// Tecla Llum Fixe? (opció booleana).
	case GLFW_KEY_F:
		ifixe = !ifixe;
		break;

		// Tecla Iluminació --> Punts
	case GLFW_KEY_F1:
		ilumina = PUNTS;
		test_vis = false;		oculta = false;
		break;

		// Tecla Iluminació --> Filferros
	case GLFW_KEY_F2:
		if (ilumina != FILFERROS) {
			ilumina = FILFERROS;
			test_vis = false;		oculta = false;
		}
		break;

		// Tecla Iluminació --> Plana
	case GLFW_KEY_F3:
		if (ilumina != PLANA) {
			ilumina = PLANA;
			test_vis = false;		oculta = true;
/*			// Elimina shader anterior
			shaderLighting.DeleteProgram();
			// Càrrega Flat shader
			shader_programID = shaderLighting.loadFileShaders(".\\shaders\\flat_shdrML.vert", ".\\shaders\\flat_shdrML.frag");
*/
		}
		break;

		// Tecla Iluminació --> Suau
	case GLFW_KEY_F4:
		if (ilumina != SUAU) {
			ilumina = SUAU;
			test_vis = false;		oculta = true;
		}
		break;

	// Tecla Iluminació --> Reflexió Material --> Emissió?
	case GLFW_KEY_F6:
		sw_material[0] = !sw_material[0];
		if (!shader_programID) glUniform1i(glGetUniformLocation(shader_programID, "sw_intensity[0]"), sw_material[0]); // Pas màscara llums al shader
		break;

	// Tecla Iluminació --> Reflexió Material -> Ambient?
	case GLFW_KEY_F7:
		sw_material[1] = !sw_material[1];
		if (!shader_programID) glUniform1i(glGetUniformLocation(shader_programID, "sw_intensity[1]"), sw_material[1]); // Pas màscara llums al shader
		break;

	// Tecla Iluminació --> Reflexió Material -> Difusa?
	case GLFW_KEY_F8:
		sw_material[2] = !sw_material[2];
		if (!shader_programID) glUniform1i(glGetUniformLocation(shader_programID, "sw_intensity[2]"), sw_material[2]); // Pas màscara llums al shader
		break;

	// Tecla Iluminació --> Reflexió Material -> Especular?
	case GLFW_KEY_F9:
		sw_material[3] = !sw_material[3];
		if (!shader_programID) glUniform1i(glGetUniformLocation(shader_programID, "sw_intensity[3]"), sw_material[3]); // Pas màscara llums al shader
		break;

// Tecla Iluminació --> Reflexió Material -> Especular?
	case GLFW_KEY_F10:
		sw_material[4] = !sw_material[4];
		if (!shader_programID) glUniform1i(glGetUniformLocation(shader_programID, "sw_intensity[4]"), sw_material[4]); // Pas màscara llums al shader
		break;

	// Tecla Iluminació --> Textura?.
	case GLFW_KEY_I:
		textura = !textura;
		if (!shader_programID) glUniform1i(glGetUniformLocation(shader_programID, "texture"), textura); //	Pas de textura al shader
		break;

	// Tecla Iluminació --> Fitxer Textura?
	case GLFW_KEY_J:
		// Entorn VGI: Diàleg de la cerca de fitxer textura.
		//nfdchar_t* nomFitxer = NULL;
		//nfdresult_t result = NFD_OpenDialog(NULL, NULL, &nomFitxer);
		result = NFD_OpenDialog(NULL, NULL, &nomFitxer);

		if (result == NFD_OKAY) {
			t_textura = FITXERIMA;		tFlag_invert_Y = true;
			textura = true;
			// Entorn VGI: Eliminar buffers de textures previs del vector texturesID[].
			for (int i = 0; i < NUM_MAX_TEXTURES; i++) {
				if (texturesID[i]) {
					//err = glIsTexture(texturesID[i]);
					glDeleteTextures(1, &texturesID[i]);
					//err = glIsTexture(texturesID[i]);
					texturesID[i] = 0;
				}
			}
			// EntornVGI: Carregar fitxer textura i definir buffer de textura.Identificador guardat a texturesID[0].
			texturesID[0] = loadIMA_SOIL(nomFitxer);

			//	Pas de textura al shader
			if (!shader_programID) glUniform1i(glGetUniformLocation(shader_programID, "texture0"), GLint(0));
			}
		break;


// ----------- POP UP Llums
	// Tecla Llums --> Llum Ambient?.
	case GLFW_KEY_A:
		llum_ambient = !llum_ambient;
		sw_il = true;
		if (!shader_programID) glUniform1i(glGetUniformLocation(shader_programID, "sw_intensity[1]"), (llum_ambient && sw_material[1])); // Pas màscara llums al shader
		break;

	// Tecla Llums --> Llum #0? (+Z)
	case GLFW_KEY_0:
		llumGL[0].encesa = !llumGL[0].encesa;
		sw_il = true;
		if (!shader_programID) glUniform1i(glGetUniformLocation(shader_programID, "sw_lights[0]"), llumGL[0].encesa); // Pas màscara Llum #0 al shader
		break;

	// Tecla Llums --> Llum #1? (+X)
	case GLFW_KEY_1:
		llumGL[1].encesa = !llumGL[1].encesa;
		sw_il = true;
		if (!shader_programID) glUniform1i(glGetUniformLocation(shader_programID, "sw_lights[1]"), llumGL[1].encesa);
		break;

	// Tecla Llums --> Llum #2? (+Y)
	case GLFW_KEY_2:
		llumGL[2].encesa = !llumGL[2].encesa;
		sw_il = true;
		if (!shader_programID) glUniform1i(glGetUniformLocation(shader_programID, "sw_lights[2]"), llumGL[2].encesa);
		break;

	// Tecla Llums --> Llum #3? (Z=Y=X)
	case GLFW_KEY_3:
		llumGL[3].encesa = !llumGL[3].encesa;
		sw_il = true;

		if (!shader_programID) glUniform1i(glGetUniformLocation(shader_programID, "sw_lights[3]"), llumGL[3].encesa);
		break;

	// Tecla Llums --> Llum #4? (-Z)
	case GLFW_KEY_4:
		llumGL[4].encesa = !llumGL[4].encesa;
		sw_il = true;
		if (!shader_programID) glUniform1i(glGetUniformLocation(shader_programID, "sw_lights[4]"), llumGL[4].encesa);
		break;

	// Tecla Shaders --> Fitxers Shaders
	case GLFW_KEY_F12:

		break;

	default:
		break;
	}
}

// Teclat_ColorObjecte: Teclat pels canvis de color de l'objecte per teclat.
void Teclat_ColorObjecte(int key, int action)
{
	const double incr = 0.025f;

	if (action == GLFW_PRESS)
	{
		// FRACTAL: Canvi resolució del fractal pe tecles '+' i'-'
		if (objecte == O_FRACTAL)
		{
			if (key == GLFW_KEY_KP_SUBTRACT) // Caràcter '-' (ASCII 109)
			{
				pas = pas * 2;
				if (pas > 64) pas = 64;
				sw_il = true;
			}
			else if (key == GLFW_KEY_KP_ADD) // Caràcter '+' (ASCII 107)
			{
				pas = pas / 2;
				if (pas < 1) pas = 1;
				sw_il = true;
			}
		}
		//	else 
		if (key == GLFW_KEY_DOWN) // Caràcter VK_DOWN
		{
			if (fonsR) {
				col_obj.r -= incr;
				if (col_obj.r < 0.0) col_obj.r = 0.0;
			}
			if (fonsG) {
				col_obj.g -= incr;
				if (col_obj.g < 0.0) col_obj.g = 0.0;
			}
			if (fonsB) {
				col_obj.b -= incr;
				if (col_obj.b < 0.0) col_obj.b = 0.0;
			}
		}
		else if (key == GLFW_KEY_UP)
		{
			if (fonsR) {
				col_obj.r += incr;
				if (col_obj.r > 1.0) col_obj.r = 1.0;
			}
			if (fonsG) {
				col_obj.g += incr;
				if (col_obj.g > 1.0) col_obj.g = 1.0;
			}
			if (fonsB) {
				col_obj.b += incr;
				if (col_obj.b > 1.0) col_obj.b = 1.0;
			}
		}
		else if (key == GLFW_KEY_SPACE)
		{
			if ((fonsR) && (fonsG) && (fonsB)) {
				fonsG = false;
				fonsB = false;
			}
			else if ((fonsR) && (fonsG)) {
				fonsG = false;
				fonsB = true;
			}
			else if ((fonsR) && (fonsB)) {
				fonsR = false;
				fonsG = true;
			}
			else if ((fonsG) && (fonsB)) fonsR = true;
			else if (fonsR) {
				fonsR = false;
				fonsG = true;
			}
			else if (fonsG) {
				fonsG = false;
				fonsB = true;
			}
			else if (fonsB) {
				fonsR = true;
				fonsG = true;
				fonsB = false;
			}
		}
		else if (key == GLFW_KEY_O) sw_color = true;
		else if (key == GLFW_KEY_F) sw_color = false;
	}
}


// Teclat_ColorFons: Teclat pels canvis del color de fons.
	void Teclat_ColorFons(int key, int action)
{		const double incr = 0.025f;

		if (action == GLFW_PRESS)
		{
			// FRACTAL: Canvi resolució del fractal pe tecles '+' i'-'
			if (objecte == O_FRACTAL)
			{
				if (key == GLFW_KEY_KP_SUBTRACT) // Caràcter '-' - (ASCII:109)
				{
					pas = pas * 2;
					if (pas > 64) pas = 64;
					sw_il = true;
				}
				else if (key == GLFW_KEY_KP_ADD) // Caràcter '+' - (ASCII:107)
				{
					pas = pas / 2;
					if (pas < 1) pas = 1;
					sw_il = true;
				}
			}
			//	else 
			if (key == GLFW_KEY_DOWN) {
				if (fonsR) {
					c_fons.r -= incr;
					if (c_fons.r < 0.0) c_fons.r = 0.0;
				}
				if (fonsG) {
					c_fons.g -= incr;
					if (c_fons.g < 0.0) c_fons.g = 0.0;
				}
				if (fonsB) {
					c_fons.b -= incr;
					if (c_fons.b < 0.0) c_fons.b = 0.0;
				}
			}
			else if (key == GLFW_KEY_UP) {
				if (fonsR) {
					c_fons.r +=  incr;
					if (c_fons.r > 1.0) c_fons.r = 1.0;
				}
				if (fonsG) {
					c_fons.g += incr;
					if (c_fons.g > 1.0) c_fons.g = 1.0;
				}
				if (fonsB) {
					c_fons.b += incr;
					if (c_fons.b > 1.0) c_fons.b = 1.0;
				}
			}
			else if (key == GLFW_KEY_SPACE) {
				if ((fonsR) && (fonsG) && (fonsB)) {
					fonsG = false;
					fonsB = false;
				}
				else if ((fonsR) && (fonsG)) {
					fonsG = false;
					fonsB = true;
				}
				else if ((fonsR) && (fonsB)) {
					fonsR = false;
					fonsG = true;
				}
				else if ((fonsG) && (fonsB)) fonsR = true;
				else if (fonsR) {
					fonsR = false;
					fonsG = true;
				}
				else if (fonsG) {
					fonsG = false;
					fonsB = true;
				}
				else if (fonsB) {
					fonsR = true;
					fonsG = true;
					fonsB = false;
				}
			}
			else if (key == GLFW_KEY_O) sw_color = true;
			else if (key == GLFW_KEY_F) sw_color = false;
		}
}

// Teclat_Navega: Teclat pels moviments de navegació.
void Teclat_Navega(int key, int action)
{
	GLdouble vdir[3] = { 0, 0, 0 };
	double modul = 0;

	// Entorn VGI: Controls de moviment de navegació
	vdir[0] = n[0] - opvN.x;
	vdir[1] = n[1] - opvN.y;
	vdir[2] = n[2] - opvN.z;
	modul = sqrt(vdir[0] * vdir[0] + vdir[1] * vdir[1] + vdir[2] * vdir[2]);
	vdir[0] = vdir[0] / modul;
	vdir[1] = vdir[1] / modul;
	vdir[2] = vdir[2] / modul;
	
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		// Tecla cursor amunt
		case GLFW_KEY_UP:
			if (Vis_Polar == POLARZ) {  // (X,Y,Z)
				opvN.x += fact_pan * vdir[0];
				opvN.y += fact_pan * vdir[1];
				n[0] += fact_pan * vdir[0];
				n[1] += fact_pan * vdir[1];
			}
			else if (Vis_Polar == POLARY) { //(X,Y,Z) --> (Z,X,Y)
				opvN.x += fact_pan * vdir[0];
				opvN.z += fact_pan * vdir[2];
				n[0] += fact_pan * vdir[0];
				n[2] += fact_pan * vdir[2];
			}
			else if (Vis_Polar == POLARX) {	//(X,Y,Z) --> (Y,Z,X)
				opvN.y += fact_pan * vdir[1];
				opvN.z += fact_pan * vdir[2];
				n[1] += fact_pan * vdir[1];
				n[2] += fact_pan * vdir[2];
			}
			break;

		// Tecla cursor avall
		case GLFW_KEY_DOWN:
			if (Vis_Polar == POLARZ) { // (X,Y,Z)
				opvN.x -= fact_pan * vdir[0];
				opvN.y -= fact_pan * vdir[1];
				n[0] -= fact_pan * vdir[0];
				n[1] -= fact_pan * vdir[1];
			}
			else if (Vis_Polar == POLARY) { //(X,Y,Z) --> (Z,X,Y)
				opvN.x -= fact_pan * vdir[0];
				opvN.z -= fact_pan * vdir[2];
				n[0] -= fact_pan * vdir[0];
				n[2] -= fact_pan * vdir[2];
			}
			else if (Vis_Polar == POLARX) { //(X,Y,Z) --> (Y,Z,X)
				opvN.y -= fact_pan * vdir[1];
				opvN.z -= fact_pan * vdir[2];
				n[1] -= fact_pan * vdir[1];
				n[2] -= fact_pan * vdir[2];
			}
			break;

		// Tecla cursor esquerra
		case GLFW_KEY_LEFT:
			angleZ += fact_pan;
			if (Vis_Polar == POLARZ) { // (X,Y,Z)
				n[0] = vdir[0]; // n[0] - opvN.x;
				n[1] = vdir[1]; // n[1] - opvN.y;
				n[0] = n[0] * cos(angleZ * PI / 180) - n[1] * sin(angleZ * PI / 180);
				n[1] = n[0] * sin(angleZ * PI / 180) + n[1] * cos(angleZ * PI / 180);
				n[0] = n[0] + opvN.x;
				n[1] = n[1] + opvN.y;
			}
			else if (Vis_Polar == POLARY) { //(X,Y,Z) --> (Z,X,Y)
				n[2] = vdir[2]; // n[2] - opvN.z;
				n[0] = vdir[0]; // n[0] - opvN.x;
				n[2] = n[2] * cos(angleZ * PI / 180) - n[0] * sin(angleZ * PI / 180);
				n[0] = n[2] * sin(angleZ * PI / 180) + n[0] * cos(angleZ * PI / 180);
				n[2] = n[2] + opvN.z;
				n[0] = n[0] + opvN.x;
			}
			else if (Vis_Polar == POLARX) { //(X,Y,Z) --> (Y,Z,X)
				n[1] = vdir[1]; // n[1] - opvN.y;
				n[2] = vdir[2]; // n[2] - opvN.z;
				n[1] = n[1] * cos(angleZ * PI / 180) - n[2] * sin(angleZ * PI / 180);
				n[2] = n[1] * sin(angleZ * PI / 180) + n[2] * cos(angleZ * PI / 180);
				n[1] = n[1] + opvN.y;
				n[2] = n[2] + opvN.z;
			}
			break;

		// Tecla cursor dret
		case GLFW_KEY_RIGHT:
			angleZ = 360 - fact_pan;
			if (Vis_Polar == POLARZ) { // (X,Y,Z)
				n[0] = vdir[0]; // n[0] - opvN.x;
				n[1] = vdir[1]; // n[1] - opvN.y;
				n[0] = n[0] * cos(angleZ * PI / 180) - n[1] * sin(angleZ * PI / 180);
				n[1] = n[0] * sin(angleZ * PI / 180) + n[1] * cos(angleZ * PI / 180);
				n[0] = n[0] + opvN.x;
				n[1] = n[1] + opvN.y;
			}
			else if (Vis_Polar == POLARY) { //(X,Y,Z) --> (Z,X,Y)
				n[2] = vdir[2]; // n[2] - opvN.z;
				n[0] = vdir[0]; // n[0] - opvN.x;
				n[2] = n[2] * cos(angleZ * PI / 180) - n[0] * sin(angleZ * PI / 180);
				n[0] = n[2] * sin(angleZ * PI / 180) + n[0] * cos(angleZ * PI / 180);
				n[2] = n[2] + opvN.z;
				n[0] = n[0] + opvN.x;
			}
			else if (Vis_Polar == POLARX) { //(X,Y,Z) --> (Y,Z,X)
				n[1] = vdir[1]; // n[1] - opvN.y;
				n[2] = vdir[2]; // n[2] - opvN.z;
				n[1] = n[1] * cos(angleZ * PI / 180) - n[2] * sin(angleZ * PI / 180);
				n[2] = n[1] * sin(angleZ * PI / 180) + n[2] * cos(angleZ * PI / 180);
				n[1] = n[1] + opvN.y;
				n[2] = n[2] + opvN.z;
			}
			break;

		// Tecla Inicio
		case GLFW_KEY_HOME:
			if (Vis_Polar == POLARZ) {
				opvN.z += fact_pan;
				n[2] += fact_pan;
			}
			else if (Vis_Polar == POLARY) {
				opvN.y += fact_pan;
				n[1] += fact_pan;
			}
			else if (Vis_Polar == POLARX) {
				opvN.x += fact_pan;
				n[0] += fact_pan;
			}
			break;

		// Tecla Fin
		case GLFW_KEY_END:
			if (Vis_Polar == POLARZ) {
				opvN.z -= fact_pan;
				n[2] -= fact_pan;
			}
			else if (Vis_Polar == POLARY) {
				opvN.y -= fact_pan;
				n[1] -= fact_pan;
			}
			else if (Vis_Polar == POLARX) {
				opvN.x -= fact_pan;
				n[0] -= fact_pan;
			}
			break;

		// Tecla PgUp
		case GLFW_KEY_PAGE_UP:
			fact_pan /= 2;
			if (fact_pan < 0.125) fact_pan = 0.125;
			break;

		// Tecla PgDown
		case GLFW_KEY_PAGE_DOWN:
			fact_pan *= 2;
			if (fact_pan > 2048) fact_pan = 2048;
			break;

		default:
			break;
		}
	}
}


// Teclat_Pan: Teclat pels moviments de Pan.
void Teclat_Pan(int key, int action)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
			// Tecla cursor amunt
		case VK_UP:
			tr_cpv.y -=  fact_pan;
			if (tr_cpv.y < -100000) tr_cpv.y = 100000;
			break;

		// Tecla cursor avall
		case GLFW_KEY_DOWN:
			tr_cpv.y += fact_pan;
			if (tr_cpv.y > 100000) tr_cpv.y = 100000;
			break;

		// Tecla cursor esquerra
		case GLFW_KEY_LEFT:
			tr_cpv.x +=  fact_pan;
			if (tr_cpv.x > 100000) tr_cpv.x = 100000;
			break;

		// Tecla cursor dret
		case GLFW_KEY_RIGHT:
			tr_cpv.x -=  fact_pan;
			if (tr_cpv.x < -100000) tr_cpv.x = 100000;
			break;

		// Tecla PgUp
		case GLFW_KEY_PAGE_UP:
			fact_pan /= 2;
			if (fact_pan < 0.125) fact_pan = 0.125;
			break;

		// Tecla PgDown
		case GLFW_KEY_PAGE_DOWN:
			fact_pan *= 2;
			if (fact_pan > 2048) fact_pan = 2048;
			break;

		// Tecla Insert: Fixar el desplaçament de pantalla (pan)
		case GLFW_KEY_INSERT:
			// Acumular desplaçaments de pan (tr_cpv) en variables fixes (tr_cpvF).
			tr_cpvF.x += tr_cpv.x;		tr_cpv.x = 0.0;
			if (tr_cpvF.x > 100000) tr_cpvF.y = 100000;
			tr_cpvF.y += tr_cpv.y;		tr_cpv.y = 0.0;
			if (tr_cpvF.y > 100000) tr_cpvF.y = 100000;
			tr_cpvF.z += tr_cpv.z;		tr_cpv.z = 0.0;
			if (tr_cpvF.z > 100000) tr_cpvF.z = 100000;
			break;

		// Tecla Delete: Inicialitzar el desplaçament de pantalla (pan)
		case GLFW_KEY_DELETE:
			// Inicialitzar els valors de pan tant de la variable tr_cpv com de la tr_cpvF.
			tr_cpv.x = 0.0;			tr_cpv.y = 0.0;			tr_cpv.z = 0.0;
			tr_cpvF.x = 0.0;		tr_cpvF.y = 0.0;		tr_cpvF.z = 0.0;
			break;

		default:
			break;
		}
	}
}

// Teclat_TransEscala: Teclat pels canvis del valor d'escala per X,Y,Z.
void Teclat_TransEscala(int key, int action)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
// Modificar vector d'Escalatge per teclat (actiu amb Escalat únicament)
		// Tecla '+' (augmentar tot l'escalat)
		case GLFW_KEY_KP_ADD:
			TG.VScal.x = TG.VScal.x * 2;
			if (TG.VScal.x > 8192) TG.VScal.x = 8192;
			TG.VScal.y = TG.VScal.y * 2;
			if (TG.VScal.y > 8192) TG.VScal.y = 8192;
			TG.VScal.z = TG.VScal.z * 2;
			if (TG.VScal.z > 8192) TG.VScal.z = 8192;
			break;

		// Tecla '-' (disminuir tot l'escalat)
		case GLFW_KEY_KP_SUBTRACT:
			TG.VScal.x = TG.VScal.x / 2;
			if (TG.VScal.x < 0.25) TG.VScal.x = 0.25;
			TG.VScal.y = TG.VScal.y / 2;
			if (TG.VScal.y < 0.25) TG.VScal.y = 0.25;
			TG.VScal.z = TG.VScal.z / 2;
			if (TG.VScal.z < 0.25) TG.VScal.z = 0.25;
			break;

		// Tecla cursor amunt ('8')
		case GLFW_KEY_UP:
			TG.VScal.x = TG.VScal.x * 2;
			if (TG.VScal.x > 8192) TG.VScal.x = 8192;
			break;

		// Tecla cursor avall ('2')
		case GLFW_KEY_DOWN:
			TG.VScal.x = TG.VScal.x / 2;
			if (TG.VScal.x < 0.25) TG.VScal.x = 0.25;
			break;

		// Tecla cursor esquerra ('4')
		case GLFW_KEY_LEFT:
			TG.VScal.y = TG.VScal.y / 2;
			if (TG.VScal.y < 0.25) TG.VScal.y = 0.25;
			break;

		// Tecla cursor dret ('6')
		case GLFW_KEY_RIGHT:
			TG.VScal.y = TG.VScal.y * 2;
			if (TG.VScal.y > 8192) TG.VScal.y = 8192;
			break;

		// Tecla HOME ('7')
		case GLFW_KEY_HOME:
			TG.VScal.z = TG.VScal.z * 2;
			if (TG.VScal.z > 8192) TG.VScal.z = 8192;
			break;

		// Tecla END ('1')
		case GLFW_KEY_END:
			TG.VScal.z = TG.VScal.z / 2;
			if (TG.VScal.z < 0.25) TG.VScal.z = 0.25;
			break;

		// Tecla INSERT
		case GLFW_KEY_INSERT:
			// Acumular transformacions Geomètriques (variable TG) i de pan en variables fixes (variable TGF)
			TGF.VScal.x *= TG.VScal.x;	TGF.VScal.y *= TG.VScal.y; TGF.VScal.z *= TG.VScal.z;
			if (TGF.VScal.x > 8192)		TGF.VScal.x = 8192;
			if (TGF.VScal.y > 8192)		TGF.VScal.y = 8192;
			if (TGF.VScal.z > 8192)		TGF.VScal.z = 8192;
			TG.VScal.x = 1.0;				TG.VScal.y = 1.0;			TG.VScal.z = 1.0;
			TGF.VRota.x += TG.VRota.x;	TGF.VRota.y += TG.VRota.y; TGF.VRota.z += TG.VRota.z;
			if (TGF.VRota.x >= 360)		TGF.VRota.x -= 360; 		if (TGF.VRota.x < 0) TGF.VRota.x += 360;
			if (TGF.VRota.y >= 360)		TGF.VRota.y -= 360;		if (TGF.VRota.y < 0) TGF.VRota.y += 360;
			if (TGF.VRota.z >= 360)		TGF.VRota.z -= 360;		if (TGF.VRota.z < 0) TGF.VRota.z += 360;
			TG.VRota.x = 0.0;				TG.VRota.y = 0.0;					TG.VRota.z = 0.0;
			TGF.VTras.x += TG.VTras.x;	TGF.VTras.y += TG.VTras.y; TGF.VTras.z += TG.VTras.z;
			if (TGF.VTras.x < -100000)		TGF.VTras.x = 100000;		if (TGF.VTras.x > 10000) TGF.VTras.x = 100000;
			if (TGF.VTras.y < -100000)		TGF.VTras.y = 100000;		if (TGF.VTras.y > 10000) TGF.VTras.y = 100000;
			if (TGF.VTras.z < -100000)		TGF.VTras.z = 100000;		if (TGF.VTras.z > 10000) TGF.VTras.z = 100000;
			TG.VTras.x = 0.0;		TG.VTras.y = 0.0;		TG.VTras.z = 0.0;
			break;

		// Tecla Delete: Esborrar les Transformacions Geomètriques Calculades
		case GLFW_KEY_DELETE:
			// Inicialitzar els valors de transformacions Geomètriques i de pan en variables fixes.
			TGF.VScal.x = 1.0;		TGF.VScal.y = 1.0;;		TGF.VScal.z = 1.0;
			TG.VScal.x = 1.0;		TG.VScal.y = 1.0;		TG.VScal.z = 1.0;
			TGF.VRota.x = 0.0;		TGF.VRota.y = 0.0;		TGF.VRota.z = 0.0;
			TG.VRota.x = 0.0;		TG.VRota.y = 0.0;		TG.VRota.z = 0.0;
			TGF.VTras.x = 0.0;		TGF.VTras.y = 0.0;		TGF.VTras.z = 0.0;
			TG.VTras.x = 0.0;		TG.VTras.y = 0.0;		TG.VTras.z = 0.0;
			break;

		default:
			break;
		}
	}
}

// Teclat_TransRota: Teclat pels canvis del valor del vector de l'angle de rotació per X,Y,Z.
void Teclat_TransRota(int key, int action)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		// Tecla cursor amunt ('8')
		case GLFW_KEY_UP:
			TG.VRota.x += fact_Rota;
			if (TG.VRota.x >= 360) TG.VRota.x -= 360;
			break;

		// Tecla cursor avall ('2')
		case GLFW_KEY_DOWN:
			TG.VRota.x -= fact_Rota;
			if (TG.VRota.x < 1) TG.VRota.x += 360;
			break;

		// Tecla cursor esquerra ('4')
		case GLFW_KEY_LEFT:
			TG.VRota.y -= fact_Rota;
			if (TG.VRota.y < 1) TG.VRota.y += 360;
			break;

		// Tecla cursor dret ('6')
		case GLFW_KEY_RIGHT:
			TG.VRota.y += fact_Rota;
			if (TG.VRota.y >= 360) TG.VRota.y -= 360;
			break;

			// Tecla HOME ('7')
		case GLFW_KEY_HOME:
			TG.VRota.z += fact_Rota;
			if (TG.VRota.z >= 360) TG.VRota.z -= 360;
			break;

			// Tecla END ('1')
		case GLFW_KEY_END:
			TG.VRota.z -= fact_Rota;
			if (TG.VRota.z < 1) TG.VRota.z += 360;
			break;

			// Tecla PgUp ('9')
		case GLFW_KEY_PAGE_UP:
			fact_Rota /= 2;
			if (fact_Rota < 1) fact_Rota = 1;
			break;

			// Tecla PgDown ('3')
		case GLFW_KEY_PAGE_DOWN:
			fact_Rota *= 2;
			if (fact_Rota > 90) fact_Rota = 90;
			break;

			// Modificar vector d'Escalatge per teclat (actiu amb Rotació)
				// Tecla '+' (augmentar escalat)
		case GLFW_KEY_KP_ADD:
			TG.VScal.x = TG.VScal.x * 2;
			if (TG.VScal.x > 8192) TG.VScal.x = 8192;
			TG.VScal.y = TG.VScal.y * 2;
			if (TG.VScal.y > 8192) TG.VScal.y = 8192;
			TG.VScal.z = TG.VScal.z * 2;
			if (TG.VScal.z > 8192) TG.VScal.z = 8192;
			break;

			// Tecla '-' (disminuir escalat)
		case GLFW_KEY_KP_SUBTRACT:
			TG.VScal.x = TG.VScal.x / 2;
			if (TG.VScal.x < 0.25) TG.VScal.x = 0.25;
			TG.VScal.y = TG.VScal.y / 2;
			if (TG.VScal.y < 0.25) TG.VScal.y = 0.25;
			TG.VScal.z = TG.VScal.z / 2;
			if (TG.VScal.z < 0.25) TG.VScal.z = 0.25;
			break;

			// Tecla Insert: Acumular transformacions Geomètriques (variable TG) i de pan en variables fixes (variable TGF)
		case GLFW_KEY_INSERT:
			TGF.VScal.x *= TG.VScal.x;	TGF.VScal.y *= TG.VScal.y; TGF.VScal.z *= TG.VScal.z;
			if (TGF.VScal.x > 8192)		TGF.VScal.x = 8192;
			if (TGF.VScal.y > 8192)		TGF.VScal.y = 8192;
			if (TGF.VScal.z > 8192)		TGF.VScal.z = 8192;
			TG.VScal.x = 1.0;				TG.VScal.y = 1.0;			TG.VScal.z = 1.0;
			TGF.VRota.x += TG.VRota.x;	TGF.VRota.y += TG.VRota.y; TGF.VRota.z += TG.VRota.z;
			if (TGF.VRota.x >= 360)		TGF.VRota.x -= 360; 		if (TGF.VRota.x < 0) TGF.VRota.x += 360;
			if (TGF.VRota.y >= 360)		TGF.VRota.y -= 360;		if (TGF.VRota.y < 0) TGF.VRota.y += 360;
			if (TGF.VRota.z >= 360)		TGF.VRota.z -= 360;		if (TGF.VRota.z < 0) TGF.VRota.z += 360;
			TG.VRota.x = 0.0;				TG.VRota.y = 0.0;					TG.VRota.z = 0.0;
			TGF.VTras.x += TG.VTras.x;	TGF.VTras.y += TG.VTras.y; TGF.VTras.z += TG.VTras.z;
			if (TGF.VTras.x < -100000)		TGF.VTras.x = 100000;		if (TGF.VTras.x > 10000) TGF.VTras.x = 100000;
			if (TGF.VTras.y < -100000)		TGF.VTras.y = 100000;		if (TGF.VTras.y > 10000) TGF.VTras.y = 100000;
			if (TGF.VTras.z < -100000)		TGF.VTras.z = 100000;		if (TGF.VTras.z > 10000) TGF.VTras.z = 100000;
			TG.VTras.x = 0.0;		TG.VTras.y = 0.0;		TG.VTras.z = 0.0;
			break;

			// Tecla Delete: Esborrar les Transformacions Geomètriques Calculades
		case GLFW_KEY_DELETE:
			// Inicialitzar els valors de transformacions Geomètriques i de pan en variables fixes.
			TGF.VScal.x = 1.0;	TGF.VScal.y = 1.0;;	TGF.VScal.z = 1.0;
			TG.VScal.x = 1.0;		TG.VScal.y = 1.0;		TG.VScal.z = 1.0;
			TGF.VRota.x = 0.0;	TGF.VRota.y = 0.0;	TGF.VRota.z = 0.0;
			TG.VRota.x = 0.0;		TG.VRota.y = 0.0;		TG.VRota.z = 0.0;
			TGF.VTras.x = 0.0;	TGF.VTras.y = 0.0;	TGF.VTras.z = 0.0;
			TG.VTras.x = 0.0;		TG.VTras.y = 0.0;		TG.VTras.z = 0.0;
			break;

			// Tecla Espaiador
		case GLFW_KEY_SPACE:
			rota = !rota;
			trasl = !trasl;
			break;

		default:
			break;
		}
	}
}


// Teclat_TransTraslada: Teclat pels canvis del valor de traslació per X,Y,Z.
void Teclat_TransTraslada(int key, int action)
{
	GLdouble vdir[3] = { 0, 0, 0 };
	double modul = 0;

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
			// Tecla cursor amunt ('8') - (ASCII:104)
		case GLFW_KEY_UP:
			TG.VTras.x -= fact_Tras;
			if (TG.VTras.x < -100000) TG.VTras.x = 100000;
			break;

			// Tecla cursor avall ('2') - (ASCII:98)
		case GLFW_KEY_DOWN:
			TG.VTras.x += fact_Tras;
			if (TG.VTras.x > 10000) TG.VTras.x = 100000;
			break;

			// Tecla cursor esquerra ('4') - (ASCII:100)
		case GLFW_KEY_LEFT:
			TG.VTras.y -= fact_Tras;
			if (TG.VTras.y < -100000) TG.VTras.y = -100000;
			break;

			// Tecla cursor dret ('6') - (ASCII:102)
		case GLFW_KEY_RIGHT:
			TG.VTras.y += fact_Tras;
			if (TG.VTras.y > 100000) TG.VTras.y = 100000;
			break;

			// Tecla HOME ('7') - (ASCII:103)
		case GLFW_KEY_HOME:
			TG.VTras.z += fact_Tras;
			if (TG.VTras.z > 100000) TG.VTras.z = 100000;
			break;

			// Tecla END ('1') - (ASCII:10#)
		case GLFW_KEY_END:
			TG.VTras.z -= fact_Tras;
			if (TG.VTras.z < -100000) TG.VTras.z = -100000;
			break;

			// Tecla PgUp ('9') - (ASCII:105)
		case GLFW_KEY_PAGE_UP:
			fact_Tras /= 2;
			if (fact_Tras < 1) fact_Tras = 1;
			break;

			// Tecla PgDown ('3') - (ASCII:99)
		case GLFW_KEY_PAGE_DOWN:
			fact_Tras *= 2;
			if (fact_Tras > 100000) fact_Tras = 100000;
			break;

			// Modificar vector d'Escalatge per teclat (actiu amb Traslació)
				// Tecla '+' (augmentar escalat)
		case GLFW_KEY_KP_ADD:
			TG.VScal.x = TG.VScal.x * 2;
			if (TG.VScal.x > 8192) TG.VScal.x = 8192;
			TG.VScal.y = TG.VScal.y * 2;
			if (TG.VScal.y > 8192) TG.VScal.y = 8192;
			TG.VScal.z = TG.VScal.z * 2;
			if (TG.VScal.z > 8192) TG.VScal.z = 8192;
			break;

			// Tecla '-' (disminuir escalat) . (ASCII:109)
		case GLFW_KEY_KP_SUBTRACT:
			TG.VScal.x = TG.VScal.x / 2;
			if (TG.VScal.x < 0.25) TG.VScal.x = 0.25;
			TG.VScal.y = TG.VScal.y / 2;
			if (TG.VScal.y < 0.25) TG.VScal.y = 0.25;
			TG.VScal.z = TG.VScal.z / 2;
			if (TG.VScal.z < 0.25) TG.VScal.z = 0.25;
			break;

			// Tecla INSERT
		case GLFW_KEY_INSERT:
			// Acumular transformacions Geomètriques (variable TG) i de pan en variables fixes (variable TGF)
			TGF.VScal.x *= TG.VScal.x;	TGF.VScal.y *= TG.VScal.y; TGF.VScal.z *= TG.VScal.z;
			if (TGF.VScal.x > 8192)		TGF.VScal.x = 8192;
			if (TGF.VScal.y > 8192)		TGF.VScal.y = 8192;
			if (TGF.VScal.z > 8192)		TGF.VScal.z = 8192;
			TG.VScal.x = 1.0;				TG.VScal.y = 1.0;			TG.VScal.z = 1.0;
			TGF.VRota.x += TG.VRota.x;	TGF.VRota.y += TG.VRota.y; TGF.VRota.z += TG.VRota.z;
			if (TGF.VRota.x >= 360)		TGF.VRota.x -= 360; 		if (TGF.VRota.x < 0) TGF.VRota.x += 360;
			if (TGF.VRota.y >= 360)		TGF.VRota.y -= 360;		if (TGF.VRota.y < 0) TGF.VRota.y += 360;
			if (TGF.VRota.z >= 360)		TGF.VRota.z -= 360;		if (TGF.VRota.z < 0) TGF.VRota.z += 360;
			TG.VRota.x = 0.0;				TG.VRota.y = 0.0;					TG.VRota.z = 0.0;
			TGF.VTras.x += TG.VTras.x;	TGF.VTras.y += TG.VTras.y; TGF.VTras.z += TG.VTras.z;
			if (TGF.VTras.x < -100000)		TGF.VTras.x = 100000;		if (TGF.VTras.x > 10000) TGF.VTras.x = 100000;
			if (TGF.VTras.y < -100000)		TGF.VTras.y = 100000;		if (TGF.VTras.y > 10000) TGF.VTras.y = 100000;
			if (TGF.VTras.z < -100000)		TGF.VTras.z = 100000;		if (TGF.VTras.z > 10000) TGF.VTras.z = 100000;
			TG.VTras.x = 0.0;		TG.VTras.y = 0.0;		TG.VTras.z = 0.0;
			break;

			// Tecla Delete: Esborrar les Transformacions Geomètriques Calculades
		case GLFW_KEY_DELETE:
			// Inicialitzar els valors de transformacions Geomètriques i de pan en variables fixes.
			TGF.VScal.x = 1.0;		TGF.VScal.y = 1.0;;		TGF.VScal.z = 1.0;
			TG.VScal.x = 1.0;		TG.VScal.y = 1.0;		TG.VScal.z = 1.0;
			TGF.VRota.x = 0.0;		TGF.VRota.y = 0.0;		TGF.VRota.z = 0.0;
			TG.VRota.x = 0.0;		TG.VRota.y = 0.0;		TG.VRota.z = 0.0;
			TGF.VTras.x = 0.0;		TGF.VTras.y = 0.0;		TGF.VTras.z = 0.0;
			TG.VTras.x = 0.0;		TG.VTras.y = 0.0;		TG.VTras.z = 0.0;
			break;

			// Tecla Espaiador
		case GLFW_KEY_SPACE:
			rota = !rota;
			trasl = !trasl;
			break;

		default:
			break;
		}
	}
}


// Teclat_Grid: Teclat pels desplaçaments dels gridXY, gridXZ i gridYZ.
void Teclat_Grid(int key, int action)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		// Key Up cursor ('8') - (ASCII:104)
		case GLFW_KEY_UP:
			hgrid.x -= PAS_GRID;
			break;

		// Key Down cursor ('2') - (ASCII:98)
		case GLFW_KEY_DOWN:
			hgrid.x += PAS_GRID;
			break;

		// Key Left cursor ('4') - (ASCII:100)
		case GLFW_KEY_LEFT:
			hgrid.y -= PAS_GRID;
			break;

		// Key Right cursor ('6') - (ASCII:102)
		case GLFW_KEY_RIGHT:
			hgrid.y += PAS_GRID;
			break;

			// Key HOME ('7') - (ASCII:103)
		case GLFW_KEY_HOME:
			hgrid.z += PAS_GRID;
			break;

			// Key END ('1') - (ASCII:97)
		case GLFW_KEY_END:
			hgrid.z -= PAS_GRID;
			break;

			// Key grid ('G') - (ASCII:'G')
		case GLFW_KEY_G:
			sw_grid = !sw_grid;
			break;

			/*
			// Key grid ('g')
			case 'g':
			sw_grid = !sw_grid;
			break;
			*/

		default:
			break;
		}
	}
}


/* ------------------------------------------------------------------------- */
/*                           CONTROL DEL RATOLI                              */
/* ------------------------------------------------------------------------- */

// OnMouseButton: Funció que es crida quan s'apreta algun botó (esquerra o dreta) del mouse.
//      PARAMETRES: - window: Finestra activa
//					- button: Botó seleccionat (GLFW_MOUSE_BUTTON_LEFT o GLFW_MOUSE_BUTTON_RIGHT)
//					- action: Acció de la tecla: GLFW_PRESS (si s'ha apretat), GLFW_REPEAT, si s'ha repetit pressió i GL_RELEASE, si es deixa d'apretar.
void OnMouseButton(GLFWwindow* window, int button, int action, int mods)
{
// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
// Get the cursor position when the mouse key has been pressed or released.
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

// EntornVGI.ImGui: Test si events de mouse han sigut filtrats per ImGui (io.WantCaptureMouse)
// (1) ALWAYS forward mouse data to ImGui! This is automatic with default backends. With your own backend:
	ImGuiIO& io = ImGui::GetIO();
	io.AddMouseButtonEvent(button, action);

// (2) ONLY forward mouse data to your underlying app/game.
	if (!io.WantCaptureMouse) { //<Tractament mouse de l'aplicació>}
		// OnLButtonDown
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			// Entorn VGI: Detectem en quina posició s'ha apretat el botó esquerra del
			//				mouse i ho guardem a la variable m_PosEAvall i activem flag m_ButoEAvall
			m_ButoEAvall = true;
			m_PosEAvall.x = xpos;	m_PosEAvall.y = ypos;
			m_EsfeEAvall = OPV;
		}
		// OnLButtonUp: Funció que es crida quan deixem d'apretar el botó esquerra del mouse.
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		{	// Entorn VGI: Desactivem flag m_ButoEAvall quan deixem d'apretar botó esquerra del mouse.
			m_ButoEAvall = false;

			// OPCIÓ VISTA-->SATÈLIT: Càlcul increment desplaçament del Punt de Vista
			if ((satelit) && (projeccio != ORTO))
			{	//m_EsfeIncEAvall.R = m_EsfeEAvall.R - OPV.R;
				m_EsfeIncEAvall.alfa = 0.01f * (OPV.alfa - m_EsfeEAvall.alfa); //if (abs(m_EsfeIncEAvall.alfa)<0.01) { if ((m_EsfeIncEAvall.alfa)>0.0) m_EsfeIncEAvall.alfa = 0.01 else m_EsfeIncEAvall.alfa=0.01}
				m_EsfeIncEAvall.beta = 0.01f * (OPV.beta - m_EsfeEAvall.beta);
				if (abs(m_EsfeIncEAvall.beta) < 0.01)
				{
					if ((m_EsfeIncEAvall.beta) > 0.0) m_EsfeIncEAvall.beta = 0.01;
					else m_EsfeIncEAvall.beta = 0.01;
				}
				//if ((m_EsfeIncEAvall.R == 0.0) && (m_EsfeIncEAvall.alfa == 0.0) && (m_EsfeIncEAvall.beta == 0.0)) KillTimer(WM_TIMER);
				//else SetTimer(WM_TIMER, 10, NULL);
			}
		}
		// OnRButtonDown
		else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{	// Entorn VGI: Detectem en quina posició s'ha apretat el botó esquerra del
			//				mouse i ho guardem a la variable m_PosEAvall i activem flag m_ButoEAvall
			m_ButoDAvall = true;
			//m_PosDAvall = point;
			m_PosDAvall.x = xpos;	m_PosDAvall.y = ypos;
		}
		// OnLButtonUp: Funció que es crida quan deixem d'apretar el botó esquerra del mouse.
		else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		{	// Entorn VGI: Desactivem flag m_ButoEAvall quan deixem d'apretar botó esquerra del mouse.
			m_ButoDAvall = false;
		}
	}
}

// OnMouseMove: Funció que es crida quan es mou el mouse. La utilitzem per la 
//				  Visualització Interactiva amb les tecles del mouse apretades per 
//				  modificar els paràmetres de P.V. (R,angleh,anglev) segons els 
//				  moviments del mouse.
//      PARAMETRES: - window: Finestra activa
//					- xpos: Posició X del cursor del mouse (coord. pantalla) quan el botó s'ha apretat.
//					- ypos: Posició Y del cursor del mouse(coord.pantalla) quan el botó s'ha apretat.
void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
	double modul = 0;
	GLdouble vdir[3] = { 0, 0, 0 };
	CSize gir = { 0,0 }, girn = { 0,0 }, girT = { 0,0 }, zoomincr = { 0,0 };

	// TODO: Add your message handler code here and/or call default
	if (m_ButoEAvall && mobil && projeccio != CAP)
	{
// Entorn VGI: Determinació dels angles (en graus) segons l'increment
//				horitzontal i vertical de la posició del mouse.
		gir.cx = m_PosEAvall.x - xpos;		gir.cy = m_PosEAvall.y - ypos;
		m_PosEAvall.x = xpos;				m_PosEAvall.y = ypos;
		if (camera == CAM_ESFERICA)
		{	// Càmera Esfèrica
			OPV.beta = OPV.beta - gir.cx / 2.0;
			OPV.alfa = OPV.alfa + gir.cy / 2.0;

			// Entorn VGI: Control per evitar el creixement desmesurat dels angles.
			if (OPV.alfa >= 360)	OPV.alfa = OPV.alfa - 360.0;
			if (OPV.alfa < 0)		OPV.alfa = OPV.alfa + 360.0;
			if (OPV.beta >= 360)	OPV.beta = OPV.beta - 360.0;
			if (OPV.beta < 0)		OPV.beta = OPV.beta + 360.0;
		}
		else { // Càmera Geode
			OPV_G.beta = OPV_G.beta + gir.cx / 2;
			OPV_G.alfa = OPV_G.alfa + gir.cy / 2;
			// Entorn VGI: Control per evitar el creixement desmesurat dels angles
			if (OPV_G.alfa >= 360.0f)	OPV_G.alfa = OPV_G.alfa - 360.0;
			if (OPV_G.alfa < 0.0f)		OPV_G.alfa = OPV_G.alfa + 360.0;
			if (OPV_G.beta >= 360.f)	OPV_G.beta = OPV_G.beta - 360.0;
			if (OPV_G.beta < 0.0f)		OPV_G.beta = OPV_G.beta + 360.0;
		}
		// Crida a OnPaint() per redibuixar l'escena
		//OnPaint(window);
	}
	else if (m_ButoEAvall && (camera == CAM_NAVEGA) && (projeccio != CAP && projeccio != ORTO)) // Opció Navegació
	{
		// Entorn VGI: Canviar orientació en opció de Navegació
		girn.cx = m_PosEAvall.x - xpos;		girn.cy = m_PosEAvall.y - ypos;
		angleZ = girn.cx / 2.0;
		// Entorn VGI: Control per evitar el creixement desmesurat dels angles.
		if (angleZ >= 360) angleZ = angleZ - 360;
		if (angleZ < 0)	angleZ = angleZ + 360;

		// Entorn VGI: Segons orientació dels eixos Polars (Vis_Polar)
		if (Vis_Polar == POLARZ) { // (X,Y,Z)
			n[0] = n[0] - opvN.x;
			n[1] = n[1] - opvN.y;
			n[0] = n[0] * cos(angleZ * PI / 180) - n[1] * sin(angleZ * PI / 180);
			n[1] = n[0] * sin(angleZ * PI / 180) + n[1] * cos(angleZ * PI / 180);
			n[0] = n[0] + opvN.x;
			n[1] = n[1] + opvN.y;
		}
		else if (Vis_Polar == POLARY) { //(X,Y,Z) --> (Z,X,Y)
			n[2] = n[2] - opvN.z;
			n[0] = n[0] - opvN.x;
			n[2] = n[2] * cos(angleZ * PI / 180) - n[0] * sin(angleZ * PI / 180);
			n[0] = n[2] * sin(angleZ * PI / 180) + n[0] * cos(angleZ * PI / 180);
			n[2] = n[2] + opvN.z;
			n[0] = n[0] + opvN.x;
		}
		else if (Vis_Polar == POLARX) { //(X,Y,Z) --> (Y,Z,X)
			n[1] = n[1] - opvN.y;
			n[2] = n[2] - opvN.z;
			n[1] = n[1] * cos(angleZ * PI / 180) - n[2] * sin(angleZ * PI / 180);
			n[2] = n[1] * sin(angleZ * PI / 180) + n[2] * cos(angleZ * PI / 180);
			n[1] = n[1] + opvN.y;
			n[2] = n[2] + opvN.z;
		}

		m_PosEAvall.x = xpos;		m_PosEAvall.y = ypos;
		// Crida a OnPaint() per redibuixar l'escena
		//OnPaint(window);
	}

	// Entorn VGI: Transformació Geomètrica interactiva pels eixos X,Y boto esquerra del mouse.
	else {
		bool transE = transX || transY;
		if (m_ButoEAvall && transE && transf)
		{
			// Calcular increment
			girT.cx = m_PosEAvall.x - xpos;		girT.cy = m_PosEAvall.y - ypos;
			if (transX)
			{
				long int incrT = girT.cx;
				if (trasl)
				{
					TG.VTras.x += incrT * fact_Tras;
					if (TG.VTras.x < -100000) TG.VTras.x = 100000;
					if (TG.VTras.x > 100000) TG.VTras.x = 100000;
				}
				else if (rota)
				{
					TG.VRota.x += incrT * fact_Rota;
					while (TG.VRota.x >= 360) TG.VRota.x -= 360;
					while (TG.VRota.x < 0) TG.VRota.x += 360;
				}
				else if (escal)
				{
					if (incrT < 0) incrT = -1 / incrT;
					TG.VScal.x = TG.VScal.x * incrT;
					if (TG.VScal.x < 0.25) TG.VScal.x = 0.25;
					if (TG.VScal.x > 8192) TG.VScal.x = 8192;
				}
			}
			if (transY)
			{
				long int incrT = girT.cy;
				if (trasl)
				{
					TG.VTras.y += incrT * fact_Tras;
					if (TG.VTras.y < -100000) TG.VTras.y = 100000;
					if (TG.VTras.y > 100000) TG.VTras.y = 100000;
				}
				else if (rota)
				{
					TG.VRota.y += incrT * fact_Rota;
					while (TG.VRota.y >= 360) TG.VRota.y -= 360;
					while (TG.VRota.y < 0) TG.VRota.y += 360;
				}
				else if (escal)
				{
					if (incrT <= 0) {
						if (incrT >= -2) incrT = -2;
						incrT = 1 / Log2(-incrT);
					}
					else incrT = Log2(incrT);
					TG.VScal.y = TG.VScal.y * incrT;
					if (TG.VScal.y < 0.25) TG.VScal.y = 0.25;
					if (TG.VScal.y > 8192) TG.VScal.y = 8192;
				}
			}
			m_PosEAvall.x = xpos;	m_PosEAvall.y = ypos;
			// Crida a OnPaint() per redibuixar l'escena
			//InvalidateRect(NULL, false);
			//OnPaint(windows);
		}
	}

	// Entorn VGI: Determinació del desplaçament del pan segons l'increment
	//				vertical de la posició del mouse (tecla dreta apretada).
	if (m_ButoDAvall && pan && (projeccio != CAP && projeccio != ORTO))
	{
		//CSize zoomincr = m_PosDAvall - point;
		zoomincr.cx = m_PosDAvall.x - xpos;		zoomincr.cy = m_PosDAvall.y - ypos;
		long int incrx = zoomincr.cx;
		long int incry = zoomincr.cy;

		// Desplaçament pan vertical
		tr_cpv.y -= incry * fact_pan;
		if (tr_cpv.y > 100000) tr_cpv.y = 100000;
		else if (tr_cpv.y < -100000) tr_cpv.y = -100000;

		// Desplaçament pan horitzontal
		tr_cpv.x += incrx * fact_pan;
		if (tr_cpv.x > 100000) tr_cpv.x = 100000;
		else if (tr_cpv.x < -100000) tr_cpv.x = -100000;

		//m_PosDAvall = point;
		m_PosDAvall.x = xpos;	m_PosDAvall.y = ypos;
		// Crida a OnPaint() per redibuixar l'escena
		//OnPaint(window);
	}
	// Determinació del paràmetre R segons l'increment
	//   vertical de la posició del mouse (tecla dreta apretada)
		//else if (m_ButoDAvall && zzoom && (projeccio!=CAP && projeccio!=ORTO))
	else if (m_ButoDAvall && zzoom && (projeccio != CAP))
	{
		//CSize zoomincr = m_PosDAvall - point;
		zoomincr.cx = m_PosDAvall.x - xpos;		zoomincr.cy = m_PosDAvall.y - ypos;
		long int incr = zoomincr.cy / 1.0;

		//		zoom=zoom+incr;
		OPV.R = OPV.R + incr;
		if (OPV.R < p_near) OPV.R = p_near;
		if (OPV.R > p_far) OPV.R = p_far;
		//m_PosDAvall = point;
		m_PosDAvall.x = xpos;				m_PosDAvall.y = ypos;
		// Crida a OnPaint() per redibuixar l'escena
		//OnPaint(window);
	}
	else if (m_ButoDAvall &&  (camera == CAM_NAVEGA) && (projeccio != CAP && projeccio != ORTO))
	{	// Avançar en opció de Navegació
		if ((m_PosDAvall.x != xpos) && (m_PosDAvall.y != ypos))
		{
			//CSize zoomincr = m_PosDAvall - point;
			zoomincr.cx = m_PosDAvall.x - xpos;		zoomincr.cy = m_PosDAvall.y - ypos;
			double incr = zoomincr.cy / 2.0;
			//	long int incr=zoomincr.cy/2.0;  // Causa assertion en "afx.inl" lin 169
			vdir[0] = n[0] - opvN.x;
			vdir[1] = n[1] - opvN.y;
			vdir[2] = n[2] - opvN.z;
			modul = sqrt(vdir[0] * vdir[0] + vdir[1] * vdir[1] + vdir[2] * vdir[2]);
			vdir[0] = vdir[0] / modul;
			vdir[1] = vdir[1] / modul;
			vdir[2] = vdir[2] / modul;
			opvN.x += incr * vdir[0];
			opvN.y += incr * vdir[1];
			n[0] += incr * vdir[0];
			n[1] += incr * vdir[1];
			//m_PosDAvall = point;
			m_PosDAvall.x = xpos;				m_PosDAvall.y = ypos;
			// Crida a OnPaint() per redibuixar l'escena
			//OnPaint(window);
		}
	}

// Entorn VGI: Transformació Geomètrica interactiva per l'eix Z amb boto dret del mouse.
	else if (m_ButoDAvall && transZ && transf)
	{
		// Calcular increment
		girT.cx = m_PosDAvall.x - xpos;		girT.cy = m_PosDAvall.y - ypos;
		long int incrT = girT.cy;
		if (trasl)
		{
			TG.VTras.z += incrT * fact_Tras;
			if (TG.VTras.z < -100000) TG.VTras.z = 100000;
			if (TG.VTras.z > 100000) TG.VTras.z = 100000;
		}
		else if (rota)
		{
			incrT = girT.cx;
			TG.VRota.z += incrT * fact_Rota;
			while (TG.VRota.z >= 360) TG.VRota.z -= 360;
			while (TG.VRota.z < 0) TG.VRota.z += 360;
		}
		else if (escal)
		{
			if (incrT <= 0) {
				if (incrT >= -2) incrT = -2;
				incrT = 1 / Log2(-incrT);
			}
			else incrT = Log2(incrT);
			TG.VScal.z = TG.VScal.z * incrT;
			if (TG.VScal.z < 0.25) TG.VScal.z = 0.25;
			if (TG.VScal.z > 8192) TG.VScal.z = 8192;
		}
		m_PosDAvall.x = xpos;	m_PosDAvall.y = ypos;
	}
}

// OnMouseWheel: Funció que es crida quan es mou el rodet del mouse. La utilitzem per la 
//				  Visualització Interactiva per modificar el paràmetre R de P.V. (R,angleh,anglev) 
//				  segons el moviment del rodet del mouse.
//      PARAMETRES: -  (xoffset,yoffset): Estructura (x,y) que dóna la posició del mouse 
//							 (coord. pantalla) quan el botó s'ha apretat.
void OnMouseWheel(GLFWwindow* window, double xoffset, double yoffset)
{
	// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
	double modul = 0;
	GLdouble vdir[3] = { 0, 0, 0 };

// EntornVGI.ImGui: Test si events de mouse han sigut filtrats per ImGui (io.WantCaptureMouse)
// (1) ALWAYS forward mouse data to ImGui! This is automatic with default backends. With your own backend:
	ImGuiIO& io = ImGui::GetIO();
	//io.AddMouseButtonEvent(button, true);

// (2) ONLY forward mouse data to your underlying app/game.
	if (!io.WantCaptureMouse) { // <Tractament mouse de l'aplicació>}
		// Funció de zoom quan està activada la funció pan o les T. Geomètriques
		if ((zzoom) || (transX) || (transY) || (transZ))
		{	OPV.R = OPV.R + yoffset / 4;
			if (OPV.R < 1) OPV.R = 1;
		}
		else if (camera == CAM_NAVEGA && !io.WantCaptureMouse)
		{	vdir[0] = n[0] - opvN.x;
			vdir[1] = n[1] - opvN.y;
			vdir[2] = n[2] - opvN.z;
			modul = sqrt(vdir[0] * vdir[0] + vdir[1] * vdir[1] + vdir[2] * vdir[2]);
			vdir[0] = vdir[0] / modul;
			vdir[1] = vdir[1] / modul;
			vdir[2] = vdir[2] / modul;
			opvN.x += (yoffset / 4) * vdir[0];		//opvN.x += (zDelta / 4) * vdir[0];
			opvN.y += (yoffset / 4) * vdir[1];		//opvN.y += (zDelta / 4) * vdir[1];
			n[0] += (yoffset / 4) * vdir[0];		//n[0] += (zDelta / 4) * vdir[0];
			n[1] += (yoffset / 4) * vdir[1];		//n[1] += (zDelta / 4) * vdir[1];
		}
	}
}


/* ------------------------------------------------------------------------- */
/*					     TIMER (ANIMACIÓ)									 */
/* ------------------------------------------------------------------------- */
void OnTimer()
{
	// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
	if (anima) {
		// Codi de tractament de l'animació quan transcorren els ms. del crono.

		// Crida a OnPaint() per redibuixar l'escena
		//InvalidateRect(NULL, false);
	}
	else if (satelit) {	// OPCIÓ SATÈLIT: Increment OPV segons moviments mouse.
		//OPV.R = OPV.R + m_EsfeIncEAvall.R;
		OPV.alfa = OPV.alfa + m_EsfeIncEAvall.alfa;
		while (OPV.alfa > 360) OPV.alfa = OPV.alfa - 360;	while (OPV.alfa < 0) OPV.alfa = OPV.alfa + 360;
		OPV.beta = OPV.beta + m_EsfeIncEAvall.beta;
		while (OPV.beta > 360) OPV.beta = OPV.beta - 360;	while (OPV.beta < 0) OPV.beta = OPV.beta + 360;

		// Crida a OnPaint() per redibuixar l'escena
		//OnPaint();
	}
}

// ---------------- Entorn VGI: Funcions locals a main.cpp

// Log2: Càlcul del log base 2 de num
int Log2(int num)
{
	int tlog;

	if (num >= 8192) tlog = 13;
	else if (num >= 4096) tlog = 12;
	else if (num >= 2048) tlog = 11;
	else if (num >= 1024) tlog = 10;
	else if (num >= 512) tlog = 9;
	else if (num >= 256) tlog = 8;
	else if (num >= 128) tlog = 7;
	else if (num >= 64) tlog = 6;
	else if (num >= 32) tlog = 5;
	else if (num >= 16) tlog = 4;
	else if (num >= 8) tlog = 3;
	else if (num >= 4) tlog = 2;
	else if (num >= 2) tlog = 1;
	else tlog = 0;

	return tlog;
}


// -------------------- FUNCIONS CORBES SPLINE i BEZIER

// llegir_ptsC: Llegir punts de control de corba (spline o Bezier) d'un fitxer .crv. 
//				Retorna el nombre de punts llegits en el fitxer.
//int llegir_pts(CString nomf)
int llegir_ptsC(const char* nomf)
{
	int i, j;
	FILE* fd;

	// Inicialitzar vector punts de control de la corba spline
	for (i = 0; i < MAX_PATCH_CORBA; i = i++)
	{
		PC_t[i].x = 0.0;
		PC_t[i].y = 0.0;
		PC_t[i].z = 0.0;
	}

	//	ifstream f("altinicials.dat",ios::in);
	//    f>>i; f>>j;
	if ((fd = fopen(nomf, "rt")) == NULL)
	{
		//LPCWSTR texte1 = reinterpret_cast<LPCWSTR> ("ERROR:");
		//LPCWSTR texte2 = reinterpret_cast<LPCWSTR> ("File .crv was not opened");
		//MessageBox(texte1, texte2, MB_OK);
		fprintf(stderr, "ERROR: File .crv was not opened");
		return false;
	}
	fscanf(fd, "%d \n", &i);
	if (i == 0) return false;
	else {
		for (j = 0; j < i; j = j++)
		{	//fscanf(fd, "%f", &corbaSpline[j].x);
			//fscanf(fd, "%f", &corbaSpline[j].y);
			//fscanf(fd, "%f \n", &corbaSpline[j].z);
			fscanf(fd, "%lf %lf %lf \n", &PC_t[j].x, &PC_t[j].y, &PC_t[j].z);

		}
	}
	fclose(fd);

	return i;
}


// Entorn VGI. OnFull_Screen: Funció per a pantalla completa
void OnFull_Screen(GLFWmonitor* monitor, GLFWwindow *window)
{   
	//int winPosX, winPosY;
	//winPosX = 0;	winPosY = 0;

	fullscreen = !fullscreen;

	if (fullscreen) {	// backup window position and window size
						//glfwGetWindowPos(window, &winPosX, &winPosY);
						//glfwGetWindowSize(window, &width_old, &height_old);

						// Get resolution of monitor
						const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

						w = mode->width;	h = mode->height;
						// Switch to full screen
						glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
					}
	else {	// Restore last window size and position
			glfwSetWindowMonitor(window, nullptr, 216, 239, 640, 480, mode->refreshRate);
		}
	}

// -------------------- TRACTAMENT ERRORS
// error_callback: Displaia error que es pugui produir
void error_callback(int code, const char* description)
{
    //const char* descripcio;
    //int codi = glfwGetError(&descripcio);

 //   display_error_message(code, description);
	fprintf(stderr, "Codi Error: %i", code);
	fprintf(stderr, "Descripcio: %s\n",description);
}


GLenum glCheckError_(const char* file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		fprintf(stderr, "ERROR %s | File: %s | Line ( %3i ) \n", error.c_str(), file, line);
		//fprintf(stderr, "ERROR %s | ", error.c_str());
		//fprintf(stderr, "File: %s | ", file);
		//fprintf(stderr, "Line ( %3i ) \n", line);
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
	const GLchar* message, const void* userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return; // ignore these non-significant error codes

	fprintf(stderr, "---------------\n");
	fprintf(stderr, "Debug message ( %3i %s \n", id, message);

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             fprintf(stderr, "Source: API \n"); break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   fprintf(stderr, "Source: Window System \n"); break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: fprintf(stderr, "Source: Shader Compiler \n"); break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     fprintf(stderr, "Source: Third Party \n"); break;
	case GL_DEBUG_SOURCE_APPLICATION:     fprintf(stderr, "Source: Application \n"); break;
	case GL_DEBUG_SOURCE_OTHER:           fprintf(stderr, "Source: Other \n"); break;
	} //std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               fprintf(stderr, "Type: Error\n"); break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: fprintf(stderr, "Type: Deprecated Behaviour\n"); break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  fprintf(stderr, "Type: Undefined Behaviour\n"); break;
	case GL_DEBUG_TYPE_PORTABILITY:         fprintf(stderr, "Type: Portability\n"); break;
	case GL_DEBUG_TYPE_PERFORMANCE:         fprintf(stderr, "Type: Performance\n"); break;
	case GL_DEBUG_TYPE_MARKER:              fprintf(stderr, "Type: Marker\n"); break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          fprintf(stderr, "Type: Push Group\n"); break;
	case GL_DEBUG_TYPE_POP_GROUP:           fprintf(stderr, "Type: Pop Group\n"); break;
	case GL_DEBUG_TYPE_OTHER:               fprintf(stderr, "Type: Other\n"); break;
	} //std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         fprintf(stderr, "Severity: high\n"); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       fprintf(stderr, "Severity: medium\n"); break;
	case GL_DEBUG_SEVERITY_LOW:          fprintf(stderr, "Severity: low\n"); break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: fprintf(stderr, "Severity: notification\n"); break;
	} //std::cout << std::endl;
	//std::cout << std::endl;
	fprintf(stderr, "\n");
}


int main(void)
{
//    GLFWwindow* window;
// Entorn VGI. Timer: Variables
	float time = elapsedTime;
	float now;
	float delta;

// glfw: initialize and configure
// ------------------------------
	if (!glfwInit())
	{	fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

// Retrieving main monitor
    primary = glfwGetPrimaryMonitor();

// To get current video mode of a monitor
    mode = glfwGetVideoMode(primary);

// Retrieving monito++rs
//    int countM;
//   GLFWmonitor** monitors = glfwGetMonitors(&countM);

// Retrieving video modes of the monitor
    int countVM;
    const GLFWvidmode* modes = glfwGetVideoModes(primary, &countVM);

	const unsigned char* version = (unsigned char*)glGetString(GL_VERSION);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

// Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Entorn Grafic VS2022 amb GLFW i OpenGL 4.3+ (Visualitzacio Grafica Interactiva - Grau en Enginyeria Informatica - Escola Enginyeria - UAB)", NULL, NULL);
    if (!window)
    {	fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
        return -1;
    }

// Make the window's context current
    glfwMakeContextCurrent(window);

// Llegir resolució actual de pantalla
	glfwGetWindowSize(window, &width_old, &height_old);

// Initialize GLEW
	if (GLEW_VERSION_3_3) glewExperimental = GL_TRUE; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		glGetError();	// Esborrar codi error generat per bug a llibreria GLEW
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	int major, minor;
	GetGLVersion(&major, &minor);

// ------------- Entorn VGI: Configure OpenGL context
//	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor); // GL4.3

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Si funcions deprecades són eliminades (no ARB_COMPATIBILITY)
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);  // Si funcions deprecades NO són eliminades (Si ARB_COMPATIBILITY)

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// Creació contexte CORE
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);	// Creació contexte ARB_COMPATIBILITY
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); // comment this line in a release build! 


// ------------ - Entorn VGI : Enable OpenGL debug context if context allows for DEBUG CONTEXT (GL4.3)
	if (GLEW_VERSION_4_3)
	{	GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{	glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // makes sure errors are displayed synchronously
			glDebugMessageCallback(glDebugOutput, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}
	}

// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

// Initialize Application control varibles
	InitGL();

// ------------- Entorn VGI: Callbacks
// Set GLFW event callbacks. I removed glfwSetWindowSizeCallback for conciseness
	glfwSetWindowSizeCallback(window, OnSize);										// - Windows Size in screen Coordinates
	glfwSetFramebufferSizeCallback(window, OnSize);									// - Windows Size in Pixel Coordinates
	glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)OnMouseButton);			// - Directly redirect GLFW mouse button events
	glfwSetCursorPosCallback(window, (GLFWcursorposfun)OnMouseMove);				// - Directly redirect GLFW mouse position events
	glfwSetScrollCallback(window, (GLFWscrollfun)OnMouseWheel);						// - Directly redirect GLFW mouse wheel events
	glfwSetKeyCallback(window, (GLFWkeyfun)OnKeyDown);								// - Directly redirect GLFW key events
	//glfwSetCharCallback(window, OnTextDown);										// - Directly redirect GLFW char events
	glfwSetErrorCallback(error_callback);											// Error callback
	glfwSetWindowRefreshCallback(window, (GLFWwindowrefreshfun)OnPaint);			// - Callback to refresh the screen

// Entorn VGI; Timer: Lectura temps
	float previous = glfwGetTime();

// Entorn VGI.ImGui: Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

// Entorn VGI.ImGui: Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsLight();

// Entorn VGI.ImGui: Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
// Entorn VGI.ImGui: End Setup Dear ImGui context

// Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {  
// Poll for and process events */
//        glfwPollEvents();

// Entorn VGI. Timer: common part, do this only once
		now = glfwGetTime();
		delta = now - previous;
		previous = now;

// Entorn VGI. Timer: for each timer do this
		time -= delta;
		if ((time <= 0.0) && (satelit || anima)) OnTimer();

// Poll for and process events
		glfwPollEvents();

// Entorn VGI.ImGui: Dibuixa menú ImGui
		draw_Menu_ImGui();

// Crida a OnPaint() per redibuixar l'escena
		OnPaint(window);

// Entorn VGI.ImGui: Capta dades del menú InGui
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

// Entorn VGI: Activa la finestra actual
		glfwMakeContextCurrent(window);

// Entorn VGI: Transferència del buffer OpenGL a buffer de pantalla
		glfwSwapBuffers(window);
    }

// Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

// Entorn VGI.ImGui: Cleanup ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);

// Terminating GLFW: Destroy the windows, monitors and cursor objects
    glfwTerminate();

	if (shaderLighting.getProgramID() != -1) shaderLighting.DeleteProgram();
	if (shaderSkyBox.getProgramID() != -1) shaderSkyBox.DeleteProgram();
    return 0;
}