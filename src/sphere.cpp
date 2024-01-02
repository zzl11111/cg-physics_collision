#include "sphere.h"
#define STB_IMAGE_IMPLEMENTATION // should be used by stb_image.h
#include "stb_image.h"

unsigned int Sphere::load_texture(char const * path) {
	unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void Sphere::setup_Mesh() {
	float sector_Angle, stack_Angle;
	glm::vec3 v;
	glm::vec3 n;
	glm::vec2 texcoords;
	// initialize the vertices
	for (int i = 0; i <= stack_count; i++) {
		stack_Angle = PI / 2 - i * stackstep;
		float xy_plane = radius * cos(stack_Angle);
		v[2] = radius * sin(stack_Angle);
		for (int j = 0; j <= sector_count; ++j) {
			sector_Angle = j * sectorstep;
			v[0] = xy_plane * cos(sector_Angle);
			v[1] = xy_plane * sin(sector_Angle);
			n[0] = v[0] / radius;
			n[1] = v[1] / radius;
			n[2] = v[2] / radius;
			texcoords[0] = (float)j / (float)sector_count;
			texcoords[1] = (float)i / (float)stack_count;
			Vertex vertex{v, n, texcoords};
			mesh.vertices.push_back(vertex);
		}
	}
	int k1, k2;
	// get ebo;
	for (int i = 0; i < stack_count; ++i) {
		k1 = i * (sector_count + 1);
		k2 = k1 + sector_count + 1;
		for (int j = 0; j < sector_count; ++j) {
			if (i != 0) {
				mesh.indices.push_back(k1);
				mesh.indices.push_back(k2);
				mesh.indices.push_back(k1 + 1);
			}
			if (i != (stack_count) - 1) {
				mesh.indices.push_back(k1 + 1);
				mesh.indices.push_back(k2);
				mesh.indices.push_back(k2 + 1);
			}
			k1++;
			k2++;
		}
	}

	// set texture
	mesh.texture_id = load_texture("..\\resources\\container2.png");
}

glm::mat3 Sphere::get_Ineritial_mat3(float mass) const {
	// set the ineria in local world
	glm::mat3 mat(1.0f);
	float ineria = 2.0 / 5.0 * mass * radius * radius;
	for (int i = 0; i < 3; i++) {
		mat[i][i] = ineria;
	}
	return mat;
}