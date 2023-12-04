#version 420 core
 
uniform vec3 font_colour;
uniform sampler2D alphabet_texture;
 
in vec2 texture_coordinates;
in vec3 texcoords_anim;
 
out vec4 fragment_colour;
 
void main(void)
{		
	float texture_value = texture(alphabet_texture, texture_coordinates).r;
 
	// Enable this if-statement for 2D window-positioned text
	// -------------------------------------------------------------------------
	 if (texture_value == 1) // Fully opaque character pixels.
	 {
		 fragment_colour = vec4(font_colour / 255, texture_value);
			 // fragment_colour = vec4(255 / 255, 255 / 255, 255 / 255, 1.0);
	 }
	 else if (texture_value == 0) // Fully transparent, i.e. background pixels (Note: you cannot use the "discard" method as used for 3D text further down, if colouring in the font's background)
	 {
		 fragment_colour = vec4(font_colour / 255, texture_value);
			 // fragment_colour = vec4(85.0 / 255, 160.0 / 255, 155.0 / 255, 1.0);
	 }
	 else // Anti-aliased character pixels.
	 {
		 fragment_colour = vec4(font_colour / 255, texture_value);
			 // fragment_colour = vec4(255 / 255, 255 / 255, 255 / 255, 1.0);
	 }
 
	// Enable these three lines instead for 3D animated text
	// ----------------------------------------------------------------------
	// fragment_colour = vec4((texcoords_anim * 2) + vec3(0.25, 0.25, 0.25), texture_value);
 
	// if (texture_value == 0)
		//  discard;
}