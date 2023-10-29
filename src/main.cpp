
// Llibreries del entorn VGI
#include "Entorn/objLoader.h"
#include "Entorn/escena.h"
#include "Entorn/visualitzacio.h"
#include "Entorn/quatern.h"
#include "Entorn/quatern.h"

#include "Shader.h"
#include "skybox.h"

const float toRadians = 3.14159265f / 180.0f;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "../../../Shaders/shader.vert";
// Fragment shader
static const char* fShader = "../../../Shaders/shader.frag";

//void InitGL()
//{
//	// TODO: agregar aquí el código de construcción
//
//	//------ Entorn VGI: Inicialització de les variables globals de CEntornVGIView
//	int i;
//
//	// Entorn VGI: Variable de control per a Status Bar (consola) 
//	statusB = false;
//
//	// Entorn VGI: Variables de control per Menú Càmera: Esfèrica, Navega, Mòbil, Zoom, Satelit, Polars... 
//	camera = CAM_ESFERICA;
//	mobil = true;	zzoom = true;		zzoomO = false;		satelit = false;
//
//	// Entorn VGI: Variables de control de l'opció Càmera->Navega?
//	n[0] = 0.0;		n[1] = 0.0;		n[2] = 0.0;
//	opvN.x = 10.0;	opvN.y = 0.0;		opvN.z = 0.0;
//	angleZ = 0.0;
//	ViewMatrix = glm::mat4(1.0);		// Inicialitzar a identitat
//
//	// Entorn VGI: Variables de control de l'opció Càmera->Geode?
//	OPV_G.R = 15.0;		OPV_G.alfa = 0.0;	OPV_G.beta = 0.0;	// Origen PV en esfèriques per a Vista_Geode
//
//	// Entorn VGI: Variables de control per Menú Vista: Pantalla Completa, Pan, dibuixar eixos i grids 
//	fullscreen = false;
//	pan = false;
//	eixos = true;	eixos_programID = 0;  eixos_Id = 0;
//	sw_grid = false;
//	grid.x = false;	grid.y = false;		grid.z = false;		grid.w = false;
//	hgrid.x = 0.0;	hgrid.y = 0.0;		hgrid.z = 0.0;		hgrid.w = 0.0;
//
//	// Entorn VGI: Variables opció Vista->Pan
//	fact_pan = 1;
//	tr_cpv.x = 0;	tr_cpv.y = 0;	tr_cpv.z = 0;		tr_cpvF.x = 0;	tr_cpvF.y = 0;	tr_cpvF.z = 0;
//
//	// Entorn VGI: Variables de control per les opcions de menú Projecció, Objecte
//	projeccio = CAP;	// projeccio = PERSPECT;
//	ProjectionMatrix = glm::mat4(1.0);	// Inicialitzar a identitat
//	objecte = CAP;		// objecte = TETERA;
//
//	// Entorn VGI: Variables de control Skybox Cube
//	SkyBoxCube = false;		skC_programID = 0;
//	skC_VAOID.vaoId = 0;	skC_VAOID.vboId = 0;	skC_VAOID.nVertexs = 0;
//	cubemapTexture = 0;
//
//	// Entorn VGI: Variables de control del menú Transforma
//	transf = false;		trasl = false;		rota = false;		escal = false;
//	fact_Tras = 1;		fact_Rota = 90;
//	TG.VTras.x = 0.0;	TG.VTras.y = 0.0;	TG.VTras.z = 0;	TGF.VTras.x = 0.0;	TGF.VTras.y = 0.0;	TGF.VTras.z = 0;
//	TG.VRota.x = 0;		TG.VRota.y = 0;		TG.VRota.z = 0;	TGF.VRota.x = 0;	TGF.VRota.y = 0;	TGF.VRota.z = 0;
//	TG.VScal.x = 1;		TG.VScal.y = 1;		TG.VScal.z = 1;	TGF.VScal.x = 1;	TGF.VScal.y = 1;	TGF.VScal.z = 1;
//
//	transX = false;		transY = false;		transZ = false;
//	GTMatrix = glm::mat4(1.0);		// Inicialitzar a identitat
//
//	// Entorn VGI: Variables de control per les opcions de menú Ocultacions
//	front_faces = true;	test_vis = false;	oculta = false;		back_line = false;
//
//	// Entorn VGI: Variables de control del menú Iluminació		
//	ilumina = FILFERROS;			ifixe = false;					ilum2sides = false;
//	// Reflexions actives: Ambient [1], Difusa [2] i Especular [3]. No actives: Emission [0]. 
//	sw_material[0] = false;			sw_material[1] = true;			sw_material[2] = true;			sw_material[3] = true;	sw_material[4] = true;
//	sw_material_old[0] = false;		sw_material_old[1] = true;		sw_material_old[2] = true;		sw_material_old[3] = true;	sw_material_old[4] = true;
//	textura = false;				t_textura = CAP;				textura_map = true;
//	for (i = 0; i < NUM_MAX_TEXTURES; i++) texturesID[i] = -1;
//	tFlag_invert_Y = false;
//
//	// Entorn VGI: Variables de control del menú Llums
//	// Entorn VGI: Inicialització variables Llums
//	llum_ambient = true;
//	for (i = 1; i < NUM_MAX_LLUMS; i++) llumGL[i].encesa = false;
//	for (i = 0; i < NUM_MAX_LLUMS; i++) {
//		llumGL[i].encesa = false;
//		llumGL[i].difusa[0] = 1.0f;	llumGL[i].difusa[1] = 1.0f;	llumGL[i].difusa[2] = 1.0f;	llumGL[i].difusa[3] = 1.0f;
//		llumGL[i].especular[0] = 1.0f; llumGL[i].especular[1] = 1.0f; llumGL[i].especular[2] = 1.0f; llumGL[i].especular[3] = 1.0f;
//	}
//
//	// LLum 0: Atenuació constant (c=1), sobre l'eix Z, no restringida.
//	llumGL[0].encesa = true;
//	llumGL[0].difusa[0] = 1.0f;			llumGL[0].difusa[1] = 1.0f;			llumGL[0].difusa[2] = 1.0f;		llumGL[0].difusa[3] = 1.0f;
//	llumGL[0].especular[0] = 1.0f;		llumGL[0].especular[1] = 1.0f;		llumGL[0].especular[2] = 1.0f;	llumGL[0].especular[3] = 1.0f;
//
//	llumGL[0].posicio.R = 200.0;		llumGL[0].posicio.alfa = 90.0;		llumGL[0].posicio.beta = 0.0;		// Posició llum (x,y,z)=(0,0,200)
//	llumGL[0].atenuacio.a = 0.0;		llumGL[0].atenuacio.b = 0.0;		llumGL[0].atenuacio.c = 1.0;		// Llum sense atenuació per distància (a,b,c)=(0,0,1)
//	llumGL[0].restringida = false;
//	llumGL[0].spotdirection[0] = 0.0;	llumGL[0].spotdirection[1] = 0.0;	llumGL[0].spotdirection[2] = 0.0;
//	llumGL[0].spotcoscutoff = 0.0;		llumGL[0].spotexponent = 0.0;
//
//	// LLum 1: Atenuació constant (c=1), sobre l'eix X, no restringida.
//	llumGL[1].encesa = false;
//	llumGL[1].difusa[0] = 1.0f;			llumGL[1].difusa[1] = 1.0f;			llumGL[1].difusa[2] = 1.0f;		llumGL[1].difusa[3] = 1.0f;
//	llumGL[1].especular[0] = 1.0f;		llumGL[1].especular[1] = 1.0f;		llumGL[1].especular[2] = 1.0f;	llumGL[1].especular[3] = 1;
//
//	llumGL[1].posicio.R = 75.0;			llumGL[1].posicio.alfa = 0.0;		llumGL[1].posicio.beta = 0.0;// (x,y,z)=(75,0,0)
//	llumGL[1].atenuacio.a = 0.0;		llumGL[1].atenuacio.b = 0.0;		llumGL[1].atenuacio.c = 1.0;
//	llumGL[1].restringida = false;
//	llumGL[1].spotdirection[0] = 0.0;	llumGL[1].spotdirection[1] = 0.0;	llumGL[1].spotdirection[2] = 0.0;
//	llumGL[1].spotcoscutoff = 0.0;		llumGL[1].spotexponent = 0.0;
//
//	// LLum 2: Atenuació constant (c=1), sobre l'eix Y, no restringida.
//	llumGL[2].encesa = false;
//	llumGL[2].difusa[1] = 1.0f;			llumGL[2].difusa[1] = 1.0f;			llumGL[2].difusa[2] = 1.0f;		llumGL[2].difusa[3] = 1.0f;
//	llumGL[2].especular[1] = 1.0f;		llumGL[2].especular[1] = 1.0f;		llumGL[2].especular[2] = 1.0f;	llumGL[2].especular[3] = 1;
//
//	llumGL[2].posicio.R = 75.0;			llumGL[2].posicio.alfa = 0.0;		llumGL[2].posicio.beta = 90.0;	// (x,y,z)=(0,75,0)
//	llumGL[2].atenuacio.a = 0.0;		llumGL[2].atenuacio.b = 0.0;		llumGL[2].atenuacio.c = 1.0;
//	llumGL[2].restringida = false;
//	llumGL[2].spotdirection[0] = 0.0;	llumGL[2].spotdirection[1] = 0.0;	llumGL[2].spotdirection[2] = 0.0;
//	llumGL[2].spotcoscutoff = 0.0;		llumGL[2].spotexponent = 0.0;
//
//	// LLum 3: Atenuació constant (c=1), sobre l'eix Y=X, no restringida.
//	llumGL[3].encesa = false;
//	llumGL[3].difusa[0] = 1.0f;			llumGL[2].difusa[1] = 1.0f;			llumGL[3].difusa[2] = 1.0f;		llumGL[3].difusa[3] = 1.0f;
//	llumGL[3].especular[0] = 1.0f;		llumGL[2].especular[1] = 1.0f;		llumGL[3].especular[2] = 1.0f;	llumGL[3].especular[3] = 1;
//
//	llumGL[3].posicio.R = 75.0;			llumGL[3].posicio.alfa = 45.0;		llumGL[3].posicio.beta = 45.0;// (x,y,z)=(75,75,75)
//	llumGL[3].atenuacio.a = 0.0;		llumGL[3].atenuacio.b = 0.0;		llumGL[3].atenuacio.c = 1.0;
//	llumGL[3].restringida = false;
//	llumGL[3].spotdirection[0] = 0.0;	llumGL[3].spotdirection[1] = 0.0;	llumGL[3].spotdirection[2] = 0.0;
//	llumGL[3].spotcoscutoff = 0.0;		llumGL[3].spotexponent = 0.0;
//
//	// LLum 4: Atenuació constant (c=1), sobre l'eix -Z, no restringida.
//	llumGL[4].encesa = false;
//	llumGL[4].difusa[0] = 1.0f;			llumGL[4].difusa[1] = 1.0f;			llumGL[4].difusa[2] = 1.0f;		llumGL[4].difusa[3] = 1.0f;
//	llumGL[4].especular[0] = 1.0f;		llumGL[4].especular[1] = 1.0f;		llumGL[4].especular[2] = 1.0f;	llumGL[4].especular[3] = 1;
//
//	llumGL[4].posicio.R = 75.0;			llumGL[4].posicio.alfa = -90.0;		llumGL[4].posicio.beta = 0.0;// (x,y,z)=(0,0,-75)
//	llumGL[4].atenuacio.a = 0.0;		llumGL[4].atenuacio.b = 0.0;		llumGL[4].atenuacio.c = 1.0;
//	llumGL[4].restringida = false;
//	llumGL[4].spotdirection[0] = 0.0;	llumGL[4].spotdirection[1] = 0.0;	llumGL[4].spotdirection[2] = 0.0;
//	llumGL[4].spotcoscutoff = 0.0;		llumGL[4].spotexponent = 0.0;
//
//	// LLum #5:
//	llumGL[5].encesa = false;
//	llumGL[5].difusa[0] = 1.0f;			llumGL[5].difusa[1] = 1.0f;			llumGL[5].difusa[2] = 1.0f;		llumGL[5].difusa[3] = 1.0f;
//	llumGL[5].especular[0] = 1.0f;		llumGL[5].especular[1] = 1.0f;		llumGL[5].especular[2] = 1.0f;	llumGL[5].especular[3] = 1;
//
//	llumGL[5].posicio.R = 75.0;			llumGL[5].posicio.alfa = 0.0;		llumGL[5].posicio.beta = -90.0; // (x,y,z)=(0,-75,0)
//	llumGL[5].atenuacio.a = 0.0;		llumGL[5].atenuacio.b = 0.0;		llumGL[5].atenuacio.c = 1.0;
//	llumGL[5].restringida = false;
//	llumGL[5].spotdirection[0] = 0.0;	llumGL[5].spotdirection[1] = 0.0;	llumGL[5].spotdirection[2] = 0.0;
//	llumGL[5].spotcoscutoff = 0.0;		llumGL[5].spotexponent = 0.0;
//
//	// LLum #6: Llum Vaixell, configurada a la funció vaixell() en escena.cpp.
//	llumGL[6].encesa = false;
//	llumGL[6].difusa[0] = 1.0f;			llumGL[6].difusa[1] = 1.0f;			llumGL[6].difusa[2] = 1.0f;		llumGL[6].difusa[3] = 1.0f;
//	llumGL[6].especular[0] = 1.0f;		llumGL[6].especular[1] = 1.0f;		llumGL[6].especular[2] = 1.0f;	llumGL[6].especular[3] = 1;
//
//	llumGL[6].posicio.R = 0.0;			llumGL[6].posicio.alfa = 0.0;		llumGL[6].posicio.beta = 0.0; // Cap posició definida, definida en funció vaixell() en escena.cpp
//	llumGL[6].atenuacio.a = 0.0;		llumGL[6].atenuacio.b = 0.0;		llumGL[6].atenuacio.c = 1.0;
//	llumGL[6].restringida = false;
//	llumGL[6].spotdirection[0] = 0.0;	llumGL[6].spotdirection[1] = 0.0;	llumGL[6].spotdirection[2] = 0.0;
//	llumGL[6].spotcoscutoff = 0.0;		llumGL[6].spotexponent = 0.0;
//
//	// LLum #7: Llum Far, configurada a la funció faro() en escena.cpp.
//	llumGL[7].encesa = false;
//	llumGL[7].difusa[0] = 1.0f;			llumGL[7].difusa[1] = 1.0f;			llumGL[7].difusa[2] = 1.0f;		llumGL[7].difusa[3] = 1.0f;
//	llumGL[7].especular[0] = 1.0f;		llumGL[7].especular[1] = 1.0f;		llumGL[7].especular[2] = 1.0f;	llumGL[7].especular[3] = 1;
//
//	llumGL[7].posicio.R = 0.0;			llumGL[7].posicio.alfa = 0.0;		llumGL[7].posicio.beta = 0.0; // Cap posició definida, definida en funció faro() en escena.cpp
//	llumGL[7].atenuacio.a = 0.0;		llumGL[7].atenuacio.b = 0.0;		llumGL[7].atenuacio.c = 1.0;
//	llumGL[7].restringida = false;
//	llumGL[7].spotdirection[0] = 0.0;	llumGL[7].spotdirection[1] = 0.0;	llumGL[7].spotdirection[2] = 0.0;
//	llumGL[7].spotcoscutoff = 0.0;		llumGL[7].spotexponent = 0.0;
//
//	// Entorn VGI: Variables de control del menú Shaders
//	shader = CAP_SHADER;  shader_programID = 0;
//	shaderLighting.releaseAllShaders();
//	fprintf(stderr, "Gouraud_shdrML: \n");
//	if (!shader_programID) shader_programID = shaderLighting.loadFileShaders(".\\shaders\\gouraud_shdrML.vert", ".\\shaders\\gouraud_shdrML.frag");
//	shader = GOURAUD_SHADER;
//
//
//	// Càrrega SHADERS
//	// Càrrega Shader Eixos
//	fprintf(stderr, "Eixos: \n");
//	if (!eixos_programID) eixos_programID = shaderEixos.loadFileShaders(".\\shaders\\eixos.VERT", ".\\shaders\\eixos.FRAG");
//
//	// Càrrega Shader Skybox
//	fprintf(stderr, "SkyBox: \n");
//	if (!skC_programID) skC_programID = shader_SkyBoxC.loadFileShaders(".\\shaders\\skybox.VERT", ".\\shaders\\skybox.FRAG");
//
//	// Càrrega VAO Skybox Cube
//	if (skC_VAOID.vaoId == 0) skC_VAOID = loadCubeSkybox_VAO();
//	Set_VAOList(CUBE_SKYBOX, skC_VAOID);
//
//	if (!cubemapTexture)
//	{	// load Skybox textures
//		// -------------
//		std::vector<std::string> faces =
//		{ ".\\textures\\skybox\\right.jpg",
//			".\\textures\\skybox\\left.jpg",
//			".\\textures\\skybox\\top.jpg",
//			".\\textures\\skybox\\bottom.jpg",
//			".\\textures\\skybox\\front.jpg",
//			".\\textures\\skybox\\back.jpg"
//		};
//		cubemapTexture = loadCubemap(faces);
//	}
//
//	// Entorn VGI: Variables de control dels botons de mouse
//	m_PosEAvall.x = 0;			m_PosEAvall.y = 0;			m_PosDAvall.x = 0;			m_PosDAvall.y = 0;
//	m_ButoEAvall = false;		m_ButoDAvall = false;
//	m_EsfeEAvall.R = 0.0;		m_EsfeEAvall.alfa = 0.0;	m_EsfeEAvall.beta = 0.0;
//	m_EsfeIncEAvall.R = 0.0;	m_EsfeIncEAvall.alfa = 0.0;	m_EsfeIncEAvall.beta = 0.0;
//
//	// Entorn VGI: Variables que controlen paràmetres visualització: Mides finestra Windows i PV
//	w = 640;			h = 480;			// Mides de la finestra Windows (w-amplada,h-alçada)
//	width_old = 640;	height_old = 480;	// Mides de la resolució actual de la pantalla (finestra Windows)
//	w_old = 640;		h_old = 480;		// Mides de la finestra Windows (w-amplada,h-alçada) per restaurar Finestra des de fullscreen
//	OPV.R = cam_Esferica[0];	OPV.alfa = cam_Esferica[1];		OPV.beta = cam_Esferica[2];		// Origen PV en esfèriques
//	//OPV.R = 15.0;		OPV.alfa = 0.0;		OPV.beta = 0.0;										// Origen PV en esfèriques
//	Vis_Polar = POLARZ;
//
//	// Entorn VGI: Color de fons i de l'objecte
//	fonsR = true;		fonsG = true;		fonsB = true;
//	c_fons.r = clear_colorB.x;		c_fons.g = clear_colorB.y;		c_fons.b = clear_colorB.z;			c_fons.b = clear_colorB.w;
//	//c_fons.r = 0.0;		c_fons.g = 0.0;		c_fons.b = 0.0;			c_fons.b = 1.0;
//	sw_color = false;
//	col_obj.r = clear_colorO.x;	col_obj.g = clear_colorO.y;	col_obj.b = clear_colorO.z;		col_obj.a = clear_colorO.w;
//	//col_obj.r = 1.0;	col_obj.g = 1.0;	col_obj.b = 1.0;		col_obj.a = 1.0;
//
//// Entorn VGI: Objecte OBJ
//	ObOBJ = NULL;		vao_OBJ.vaoId = 0;		vao_OBJ.vboId = 0;		vao_OBJ.nVertexs = 0;
//
//	// Entorn VGI: OBJECTE --> Corba B-Spline i Bezier
//	npts_T = 0;
//	for (i = 0; i < MAX_PATCH_CORBA; i = i++)
//	{
//		PC_t[i].x = 0.0;
//		PC_t[i].y = 0.0;
//		PC_t[i].z = 0.0;
//	}
//
//	pas_CS = PAS_BSPLINE;
//	sw_Punts_Control = false;
//
//	// TRIEDRE DE FRENET / DARBOUX: VT: vector Tangent, VNP: Vector Normal Principal, VBN: vector BiNormal
//	dibuixa_TriedreFrenet = false;		dibuixa_TriedreDarboux = false;
//	VT = { 0.0, 0.0, 1.0 };		VNP = { 1.0, 0.0, 0.0 };	VBN = { 0.0, 1.0, 0.0 };
//
//	// Entorn VGI: Variables del Timer
//	t = 0;			anima = false;
//
//	// Entorn VGI: Variables de l'objecte FRACTAL
//	t_fractal = CAP;	soroll = 'C';
//	pas = 64;			pas_ini = 64;
//	sw_il = true;		palcolFractal = false;
//
//	// Entorn VGI: Altres variables
//	mida = 1.0;			nom = "";		buffer = "";
//	initVAOList();	// Inicialtzar llista de VAO'S.
//}


void CreateObjects()
{
    unsigned int indices[] = {
      0, 3, 1,
      1, 3, 2,
      2, 3, 0,
      0, 1, 2
    };

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

    Mesh *obj1 = new Mesh();
    // TODO: clear magic numbers
    obj1->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj1);

    Mesh *obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj2);


}

void CreateShader()
{
    auto* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

/*
* This should return the Imported, the Device and the context from 
* the libraries, but for now is to test that it compiles and links 
* them correctly.
*/

int initialize_libraries()
{
    // Initialize OpenAL
    ALCdevice* device = alcOpenDevice(NULL);
    if (!device) {
        std::cerr << "Failed to initialize OpenAL device" << std::endl;
        return -1;
    }

    ALCcontext* context = alcCreateContext(device, NULL);
    if (!context) {
        std::cerr << "Failed to initialize OpenAL context" << std::endl;
        return -1;
    }

    alcMakeContextCurrent(context);

    // Since this is just for testing we destroy everything after initialization

    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
    return 0;
}

Skybox loadSkybox()
{
    //Load Faces
    std::vector<std::string> faces =
    { "../../../Assets/skybox/right.jpg",
        "../../../Assets/skybox/left.jpg",
        "../../../Assets/skybox/top.jpg",
        "../../../Assets/skybox/bottom.jpg",
        "../../../Assets/skybox/front.jpg",
        "../../../Assets/skybox/back.jpg"
    };
    //Load Skybox with shaders
    return Skybox(faces, "../../../Shaders/skybox.VERT", "../../../Shaders/skybox.FRAG");
}

int main(int argc, char* argv[])
{

    COBJModel* obj = new COBJModel;
    obj->LoadModel("../../../Assets/Skull/12140_Skull_v3_L2.obj");

    Skybox Sky = loadSkybox();

    Window mainWindow = Window(800, 600);
    mainWindow.Initialise();

	CreateObjects();
    CreateShader();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 1.0f);

    // Test other libraries
    if (initialize_libraries() < 0)
        return -1;

    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat) mainWindow.getBufferWidth() / (GLfloat) mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
        GLfloat now = (GLfloat) glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

		// Get + Handle user input events
		glfwPollEvents();

        camera.keyControl(mainWindow.getKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        uniformView = shaderList[0].GetViewLocation();


		glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv((int) uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv((int) uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

        meshList[0]->RenderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        glUniformMatrix4fv((int) uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        //TODO: import shaders
        //obj->draw_TriVAO_OBJ();	// Dibuixar VAO a pantalla

        meshList[1]->RenderMesh();

        //Skybox



        glUseProgram(0);

		mainWindow.swapBuffers();

	}

	return 0;
}