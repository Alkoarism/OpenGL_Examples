#include "OpenGL/renderer.h"
#include "OpenGL/camera.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "bitmap_font.h"

using std::vector;

// function declarations ------------------------------------------------------
void framebuffer_size_callback(GLFWwindow*, int, int);
void key_callback(GLFWwindow*, int, int, int, int);
void RenderText(Shader&, std::string, float, float, float, glm::vec3);

// global variables -----------------------------------------------------------
const int screenWidth = 600, screenHeight = 800;

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));

struct Character {
	Texture& texture;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};

std::map<char, Character> characters;
std::unique_ptr<VertexArray> VAO;
std::unique_ptr<VertexBuffer> VBO;
std::unique_ptr<IndexBuffer> IBO;

int main() {
	// glfw: initialize and configure --------------------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw: window creation and setup -------------------------------------------
	GLFWwindow* window = glfwCreateWindow
		(screenWidth,  screenHeight, "OpenGL Space_Invaders_Clone", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// glfw: setup
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// freetype: font loading and setup ------------------------------------------
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cout << "ERROR::FREETYPE::FAILED_TO_INITIALIZE" << std::endl;
		return -1;
	}

	FT_Face face;
	if (FT_New_Face(ft, "res\\fonts\\Lora-Regular.ttf", 0, &face)) {
		std::cout << "ERROR::FREETYPE::FAILED_TO_LOAD_FACE" << std::endl;
		return -1;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
		std::cout << "ERROR::FREETYPE::FAILED_TO_LOAD_GLYPH" << std::endl;
		return -1;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //disable byte-alignment restriction

	for (unsigned char c = 32; c < 127; ++c) {
		//load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYPE::FAILED_TO_LOAD_GLYPH: " 
				<< c << std::endl;
			continue;
		}

		//generate texture
		Texture& texture = Renderer::LoadTexture("Freetype_" + std::to_string(c));
		texture.type = GL_TEXTURE_2D;
		texture.format = GL_RED;
		texture.Load(
			face->glyph->bitmap.buffer, 
			face->glyph->bitmap.width, 
			face->glyph->bitmap.rows);

		//set texture options
		texture.SetPar(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		texture.SetPar(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		texture.SetPar(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		texture.SetPar(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		characters.insert(std::pair<char, Character>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	// initialization before rendering -------------------------------------------
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::ortho(
		0.0f, static_cast<float>(screenHeight), 
		0.0f, static_cast<float>(screenWidth));

	Shader& freetype2D = Renderer::LoadShader(
		"freetype2D",
		"res\\shaders\\main2D.vert",
		"res\\shaders\\freetype2D.frag"
	);
	freetype2D.SetUniform("projection", projection);
	freetype2D.SetUniform("model", model);

	Shader& bitmap2D = Renderer::LoadShader(
		"bitmap2D",
		"res\\shaders\\main2D.vert",
		"res\\shaders\\bitmap2D.frag"
	);
	bitmap2D.SetUniform("projection", projection);
	bitmap2D.SetUniform("model", model);

	VAO.reset(new VertexArray());
	VBO.reset(new VertexBuffer(nullptr, sizeof(float) * 4 * 4, GL_DYNAMIC_DRAW));
	VertexBufferLayout VBL;
	VBL.Push<float>(4);
	VAO->AddBuffer(*VBO, VBL);

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	IBO.reset(new IndexBuffer(indices, 6));

	BitmapFont font(bitmap2D, Renderer::LoadTexture("BitmapTexture"));
	font.Load("res\\bitmap\\timesNewRoman.bff");

	// render loop (happens every frame) -----------------------------------------
	while (!glfwWindowShouldClose(window)) {
		// -> frame time tracker
		Renderer::FrameTimeTracker();

		// -> rendering commands and configuration
		Renderer::RenderConfig(0.2f, 0.4f, 0.2f, 1.0f);

		// ---> space configurations and rendering
		font.SetColor(glm::vec4(
			sin(glfwGetTime() + (2 * 3.14 / 3)),
			sin(glfwGetTime()),
			sin(glfwGetTime() - (2 * 3.14 / 3)),
			1.0f));
		font.Print("BitmapFont sample text.", 25.0f, 75.0f, 2.0f);
		font.Print("(C) LearnOpenGL.com", 540.0f, 495.0f, 1.5f);

		RenderText(freetype2D, "Freetype sample text.", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		RenderText(freetype2D, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

		// -> check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void RenderText(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color) {
	s.SetUniform("textColor", color);
	glActiveTexture(GL_TEXTURE0);
	VAO->Bind();

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); ++c) {
		Character ch = characters.at(*c);

		float posX = x + (ch.bearing.x * scale);
		float posY = y - ((ch.size.y - ch.bearing.y) * scale);

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;
		
		//update VBO for each character
		float vertices[4][4] = {
				   { posX,     posY + h,   0.0f, 0.0f },
				   { posX,     posY,       0.0f, 1.0f },
				   { posX + w, posY,       1.0f, 1.0f },
				   { posX + w, posY + h,   1.0f, 0.0f }
		};

		// render glyph texture over quad
		ch.texture.Bind();
		VBO->Update(vertices, sizeof(vertices), 0);
		Renderer::Render(*VAO, *IBO, s);

		//now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.advance >> 6) * scale; //bitshift by 6 to get value in pixels (2^6 = 64)
	}
}