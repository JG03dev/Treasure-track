#include "Skybox.h"

std::vector<float> skyboxVertices = {
	// front
	-1.0, -1.0,  1.0, // 0
	 1.0, -1.0,  1.0, // 1
	 1.0,  1.0,  1.0, // 2
	-1.0,  1.0,  1.0, // 3
	// back
	-1.0, -1.0, -1.0, // 4
	 1.0, -1.0, -1.0, // 5
	 1.0,  1.0, -1.0, // 6
	-1.0,  1.0, -1.0, // 7
};

std::vector<unsigned int> skyboxElements = {
	// right
	1, 2, 6,
	6, 5, 1,
	// left
	4, 7, 3,
	3, 0, 4,
	// top
	3, 7, 6,
	6, 2, 3,
	// bottom
	4, 0, 1,
	1, 5, 4,
	// front
	0, 3, 2,
	2, 1, 0,
	// back
	7, 4, 5,
	5, 6, 7,
};

Skybox::Skybox()
	: active_texture_unit(15)
{
	setup();
}

void Skybox::setup() {

	glGenBuffers(1, &vbo); // Generate an empty vertex buffer on the GPU
	glGenBuffers(1, &ebo);
	glGenVertexArrays(1, &vao); // Tell OpenGL to create new Vertex Array Object

	glBindVertexArray(vao); // Make the vertices buffer the current one

	glBindBuffer(GL_ARRAY_BUFFER, vbo); // "bind" or set as the current buffer we are working with
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * skyboxVertices.size(),
		skyboxVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * skyboxElements.size(),
		skyboxElements.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);

	glBindVertexArray(0); // Release control of vao
}

void Skybox::Draw(Shader& shader, glm::mat4& view, glm::mat4& projection) {

	shader.use();
	shader.setUniform("view", view);
	shader.setUniform("projection", projection);
	shader.setUniform("skybox", 0); // no other texture units

	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL); // change depth func so depth test passes when val == depth buffer

	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0 + active_texture_unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tid);
	glDrawElements(GL_TRIANGLES, skyboxElements.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0); // Unbind
	glActiveTexture(GL_TEXTURE0);
	glDepthFunc(GL_LESS); // restore default depth func
	glDepthMask(GL_TRUE);
}

void Skybox::LoadTexture(std::vector<std::string>& faces) {

	/**
	* loads a cubemap texture from 6 individual texture faces
	* order:
	* +X (right)
	* -X (left)
	* +Y (top)
	* -Y (bottom)
	* +Z (front)
	* -Z (back)
	*/

	unsigned int textureID{};
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrComponents;

	for (unsigned int i = 0; i < faces.size(); i++) {

		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);

		if (!data)
			std::cerr << "LoadCubemap: Texture failed to load at path: " << faces[i] << "\n";

		else {
			GLenum imageFormat;
			if (nrComponents == 1) imageFormat = GL_RED;
			else if (nrComponents == 3) imageFormat = GL_RGB;
			else if (nrComponents == 4) imageFormat = GL_RGBA;

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, imageFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);
		}

		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	tid = textureID;
}