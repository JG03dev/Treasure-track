#include "Primitives.h"

/*************************************************
*
* 2D Objects
*
*************************************************/

Base2D::Base2D() {}

Base2D :: ~Base2D() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void Base2D::setup() {

	glGenBuffers(1, &vbo); // Generate an empty vertex buffer on the GPU
	glGenBuffers(1, &ebo);
	glGenVertexArrays(1, &vao); // Tell OpenGL to create new Pixel Array Object

	glBindVertexArray(vao); // Make the vertices buffer the current one

	glBindBuffer(GL_ARRAY_BUFFER, vbo); // "bind" or set as the current buffer we are working with
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Pixel), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // vertex positions
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Pixel), NULL);
	glEnableVertexAttribArray(1); // vertex texture coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Pixel), (void*)offsetof(Pixel, texCoords));

	glBindVertexArray(0); // Release control of vao
}

void Base2D::Draw(Shader& shader) {

	shader.use();

	// Bind textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	unsigned int emissionNr = 1;
	unsigned int ambientNr = 1;

	for (unsigned int i = 0; i < textures.size(); i++) {

		glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding

		std::string number;
		TextureType type = textures[i].type;

		if (type == TEX_DIFFUSE)
			number = std::to_string(diffuseNr++);
		else if (type == TEX_SPECULAR)
			number = std::to_string(specularNr++);
		else if (type == TEX_NORMAL)
			number = std::to_string(normalNr++);
		else if (type == TEX_HEIGHT)
			number = std::to_string(heightNr++);
		else if (type == TEX_EMISSION)
			number = std::to_string(emissionNr++);
		else if (type == TEX_AMBIENT)
			number = std::to_string(ambientNr++);

		shader.setUniform("uMaterial." + TextureTypeName[type] + number, (int)i);
		// Bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

	// Draw mesh
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void Base2D::AddTexture(unsigned int tid) { // for frame buffer
	Texture texture;
	texture.id = tid;
	if (texture.id != 0) textures.push_back(texture);
}

void Base2D::AddTexture(const std::string path, TextureType type, bool gamma) {

	Texture texture;
	texture.id = LoadTexture(path, gamma);
	texture.path = path;
	texture.type = type;
	if (texture.id != 0) {
		textures.push_back(texture);
		std::cout << "Base2D::loadTextures: " << texture.id << "\t"
			<< TextureTypeName[texture.type] << "\tfrom: " << texture.path << "\n";
	}
}


/*************************************************
*
* 3D Objects
*
*************************************************/

Base3D::Base3D() {}

Base3D :: ~Base3D() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void Base3D::setup() {

	glGenBuffers(1, &vbo); // Generate an empty vertex buffer on the GPU
	glGenBuffers(1, &ebo);
	glGenVertexArrays(1, &vao); // Tell OpenGL to create new Vertex Array Object

	glBindVertexArray(vao); // Make the vertices buffer the current one

	glBindBuffer(GL_ARRAY_BUFFER, vbo); // "bind" or set as the current buffer we are working with
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
	glEnableVertexAttribArray(1); // vertex normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2); // vertex texture coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(3); // vertex tangent coords
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	glEnableVertexAttribArray(4); // vertex bitangent coords
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

	glBindVertexArray(0); // Release control of vao
}

void Base3D::Draw(Shader& shader) {

	shader.use();

	// Bind textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	unsigned int emissionNr = 1;
	unsigned int ambientNr = 1;

	for (unsigned int i = 0; i < textures.size(); i++) {

		glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding

		std::string number;
		TextureType type = textures[i].type;

		if (type == TEX_DIFFUSE)
			number = std::to_string(diffuseNr++);
		else if (type == TEX_SPECULAR)
			number = std::to_string(specularNr++);
		else if (type == TEX_NORMAL)
			number = std::to_string(normalNr++);
		else if (type == TEX_HEIGHT)
			number = std::to_string(heightNr++);
		else if (type == TEX_EMISSION)
			number = std::to_string(emissionNr++);
		else if (type == TEX_AMBIENT)
			number = std::to_string(ambientNr++);

		shader.setUniform("uMaterial." + TextureTypeName[type] + number, (int)i);
		// Bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

	// Draw mesh
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void Base3D::AddTexture(unsigned int tid) {
	Texture texture;
	texture.id = tid;
	if (texture.id != 0) textures.push_back(texture);
}

void Base3D::AddTexture(const std::string path, TextureType type, bool gamma) {

	Texture texture;
	texture.id = LoadTexture(path, gamma);
	texture.type = type;
	texture.path = path;
	if (texture.id != 0) {
		textures.push_back(texture);
		std::cout << "Base3D::loadTextures: " << texture.id << "\t"
			<< TextureTypeName[texture.type] << "\tfrom: " << texture.path << "\n";
	}
}