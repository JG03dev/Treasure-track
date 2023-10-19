//******** PRACTICA VISUALITZACI� GR�FICA INTERACTIVA (EE-UAB)
//******** Entorn b�sic VS2022 amb interf�cie MFC/GLFW i Status Bar
//******** Enric Marti (Setembre 2023)
// phong_shdrML.frag: Fragment Program en GLSL en versi� OpenGL 3.3 o 4.30 per a implementar:
//     a) Iluminaci� de Gouraud
//     b) Fonts de llum puntuals o direccionals
//     c) Fonts de llum restringides
//     d) Atenuaci� de fonts de llum
//     e) Modus d'assignaci� de textura MODULATE o DECAL

//#version 330 core
#version 430 core

#define MaxLights 8

// ---- L16- Estructures Font de LLum i Material (coeficients reflectivitat).
struct Light
{	bool sw_light;		// Booleana que indica si llum encesa (TRUE) o apagada (FALSE).
	vec4 position;		// Posici� de la font de llum, en Coord. M�n o C�mera (Punt de Vista).
	vec4 ambient;		// Intensitat (r,g,b,a) de llum ambient de la font.
	vec4 diffuse;		// Intensitat (r,g,b,a) de llum difusa de la font.
	vec4 specular;		// Intensitat (r,g,b,a) de la llum especular de la font
	vec3 attenuation;	// Atenuaci� de la font per dist�ncia: .x (quadr�tica), .y (lineal), .z (constant).
	bool restricted;	// Booleana si font de llum restringida (TRUE) o no (FALSE).
	vec3 spotDirection;	// Vector de direcci� de la font restringida (Coord. M�n).
	float spotCosCutoff;	// Angle d'obertura de la font restringids (en graus). 
	float spotExponent;	// Exponent per al c�lcul intenstitat font restringida segons model Warn.
};

struct Material
{	vec4 emission;		// Coeficient d'emissi� (r,g,b,a) del material.
	vec4 ambient;		// Coeficient de reflectivitat ambient (r,g,b,a) del material.
        vec4 diffuse;		// Coeficient de reflectivitat difusa (r,g,b,a) del material.
	vec4 specular;		// Coeficient de reflectivitat especular (r,g,b,a) del material.
	float shininess;	// Exponent per al coeficient de reflectivitat especular del material (1,500).
};

// ---- L38- Variables in
in vec3 vertexPV;		// V�rtex en coordenades c�mera (Punt de Vista).
in vec3 vertexNormalPV;		// Normal en coordenades c�mera (Punt de Vista).
in vec2 vertexTexCoord;		// Coordenades textura.
in vec4 vertexColor;		// Color (r,g,b,a) del v�rtex.

// ---- L44- Variables uniform
uniform mat4 normalMatrix;	// �the transpose of the inverse of the gl_ModelViewMatrix.� 
uniform mat4 projectionMatrix;	// Projection Matrix
uniform mat4 viewMatrix; 	// View Matrix
uniform mat4 modelMatrix;	// Model Matrix

uniform sampler2D texture0;	// Imatge textura
uniform bool textur;		// Booleana d�activaci� (TRUE) de textures o no (FALSE).
uniform bool flag_invert_y;	// Booleana que activa la inversi� coordenada textura t (o Y) a 1.0-cty segons llibreria SOIL (TRUE) o no (FALSE).
uniform bool modulate;		// Booleana d'activaci� de barreja color textura- color intensitat llum (TRUE) o nom�s color textura (FALSE)
uniform bool fixedLight;	// Booleana que defineix la font de llum fixe en Coordenades M�n (TRUE) o no (FALSE).
uniform bool sw_material;	// Booleana que indica si el color del v�rtex ve del Material emission, ambient, diffuse, specular (TRUE) o del vector de color del v�rtex in_Color (FALSE)
uniform bvec4 sw_intensity;	// Filtre per a cada tipus de reflexi�: Emissiva (sw_intensity[0]), Ambient (sw_intensity[1]), Difusa (sw_intensity[2]) o Especular (sw_intensity[2]).
uniform vec4 LightModelAmbient;	// Intensitat de llum ambient (r,g,b,a)-
uniform Light LightSource[MaxLights];	// Vector de fonts de llum.
uniform Material material;	// Vector de coeficients reflectivitat de materials.

// ---- L61- Variables out
out vec4 FragColor; 		// Color fragment (r,g,b,a)

void main ()
{
// ---- L66- Calcul variables previes
    //mat4 NormalMatrix = transpose(inverse(viewMatrix * modelMatrix));
    vec3 N = normalize(vertexNormalPV);
    vec3 V = normalize(-vertexPV);
  
// ---- L71- Multiples llums
    float diffuseLight = 0.0;			// C�lcul del factor de reflexi� difusa depenent angles N,L.
    vec3 Idiffuse = vec3 (0.0,0.0,0.0);	        // Intensitat difusa d�una font de llum.
    vec3 R = vec3 (0.0,0.0,0.0);		// Vector R per a calcular la reflexio especular.
    float specularLight = 0.0;			// C�lcul del factor de reflexi� especular depenent angles R,V.
    vec3 Ispecular = vec3 (0.0,0.0,0.0);	// Intensitat especular d�una font de llum.
    vec3 ILlums = vec3 (0.0,0.0,0.0);	        // Intensitat acumulada per a totes les fonts de llum (r,g,b).
    vec4 lightPosition = vec4(0.0,0.0,0.0,1.0);	// Posici� de la font de llum en coordenades Punt de Vista.
    vec3 L = vec3 (0.0,0.0,0.0); 	        // Vector L per a c�lcul intensitat difusa i especular.
    vec4 fragmentColor = vec4(0.0,0.0,0.0,1.0);	// Intensitat final del fragment (pixel) segons equaci� il.luminaci�.
    float fatt=1.0;				// Factor d'atenuaci� llum per dist�ncia (fatt).
						// fatt = 1/Light.attenuation.x*d2 + Light.attenuation.y * d + Light.attenuation.z
    vec3 spotDirectionPV = vec3 (0.0,0.0,0.0);	// Vector spotDirection en coordenades c�mera i normalitzat.
    float spotDot; 				// Angle d'obertura en radians entre el v�rtex i el vector de la font restringida (spotDirection).

// ---- L86- Compute emissive term
    vec3 Iemissive = vec3 (0.0,0.0,0.0);	// Intensitat emissiva del fragment (pixel).
    if (sw_intensity[0])  {  if (sw_material) Iemissive = material.emission.rgb;
				else Iemissive = vertexColor.rgb;
		         }
    
// ---- L92- Compute ambient term
    vec3 Iambient = vec3 (0.0,0.0,0.0);		// Intensitat ambient reflexada del fragment (pixel).
    if (sw_intensity[1]) {  if (sw_material) Iambient = material.ambient.rgb * LightModelAmbient.rgb;
				else Iambient = vertexColor.rgb * LightModelAmbient.rgb;
			    Iambient = clamp(Iambient, 0.0, 1.0);
    			 }

// ---- L99- Multiples llums
    for (int i=0;i<MaxLights;i++) {
	if (LightSource[i].sw_light) { 
		fatt = 1.0; 	// Inicialitzar factor d'atenuaci�.
		// ---- L103- Compute light position (fixed in WC of attached to camera)
		if (fixedLight) lightPosition = viewMatrix * LightSource[i].position;
		  else lightPosition = LightSource[i].position; // vec4(-vertexPV,1.0);
		
		// ---- L107- Compute point light source (w=1) or direccional light (w=0)
		if (LightSource[i].position.w == 1) 
		  {  //L = normalize(lightPosition.xyz - vertex-PV);
		     L = lightPosition.xyz - vertexPV;
		     // -L111- Compute Attenuation factor
		     fatt = length(L);		// C�lcul de la dist�ncia entre la posici� de la font de llum i el v�rtex.
		     fatt = 1 / (LightSource[i].attenuation.x * fatt * fatt + LightSource[i].attenuation.y * fatt + LightSource[i].attenuation.z);
		     L = normalize(L);		// Normalitzar el vector.
		  }
                   else L = normalize(lightPosition.xyz);

		// ---- L118- Compute Spot Light Factor
		if (LightSource[i].restricted) 
                  { spotDirectionPV = vec3(normalMatrix * vec4(LightSource[i].spotDirection,1.0));
  		    spotDirectionPV = normalize(spotDirectionPV);
		    spotDot = dot(-L,spotDirectionPV);
		    if (spotDot > LightSource[i].spotCosCutoff)
		      { spotDot = pow(spotDot, LightSource[i].spotExponent); // Model de Warn
			fatt = spotDot * fatt;
                      }
		      else fatt=0.0;
                  }

		// ---- L30- Compute the diffuse term
    		Idiffuse = vec3 (0.0,0.0,0.0);	// Intensitat difusa de la font de llum i-�ssima reflexada del fragment (pixel).
     		if (sw_intensity[2]) {	diffuseLight = max(dot(L, N), 0.0);
    					if (sw_material) Idiffuse = material.diffuse.rgb * LightSource[i].diffuse.rgb * diffuseLight * fatt;
					  else Idiffuse = vertexColor.rgb * LightSource[i].diffuse.rgb * diffuseLight * fatt;
    					Idiffuse = clamp(Idiffuse, 0.0, 1.0);
    				     }

		//-138- Compute the specular term
    		Ispecular = vec3 (0.0,0.0,0.0);	// Intensitat especular de la font de llum i-�ssima reflexada del fragment (pixel).
    		if (sw_intensity[3]) {	R = normalize (-reflect(L,N));
    					specularLight = pow(max(dot(R, V), 0.0), material.shininess);
    					if (sw_material) Ispecular = material.specular.rgb * LightSource[i].specular.rgb * specularLight * fatt;
					   else Ispecular = vertexColor.rgb * LightSource[i].specular.rgb * specularLight * fatt;
    					Ispecular = clamp(Ispecular, 0.0, 1.0);
				     }
		ILlums += Idiffuse + Ispecular;
		}
   }

// ---- L150- Calcul intensitat final del pixel
   fragmentColor.rgb = Iemissive + Iambient + ILlums;
   fragmentColor.a = vertexColor.a;

   if (textur) { // Intensitat amb textura
		 vec4 colorT = texture(texture0,vertexTexCoord);
		// Textura modulada amb intensitat llum
	    	if (modulate) FragColor = colorT * fragmentColor;
       			else FragColor = colorT; // textura sense modular intensitat llum
    	    }
    else { // Intensitat sense textura
           FragColor = fragmentColor;   
         }
}