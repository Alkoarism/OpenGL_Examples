#include "OpenGL\renderer.h"
#include "OpenGL\camera.h"

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
	unsigned int textureID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};

std::map<char, Character> characters;
unsigned int VAO, VBO;

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

	for (unsigned char c = 32; c < 128; ++c) {
		//load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYPE::FAILED_TO_LOAD_GLYPH: " 
				<< c << std::endl;
			continue;
		}

		//generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width, 
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer);

		//set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
	glm::mat4 projection = glm::ortho(
		0.0f, static_cast<float>(screenHeight), 
		0.0f, static_cast<float>(screenWidth));

	Shader& shader = Renderer::LoadShader(
		"freetype2D", 
		"res\\shaders\\freetype2D.vert", 
		"res\\shaders\\freetype2D.frag");
	shader.SetUniform("projection", projection);

	std::string shaderName = "main2D";
	Shader& main2D = Renderer::LoadShader(
		shaderName,
		"res\\shaders\\main2D.vert",
		"res\\shaders\\freetype2D.frag"
	);
	main2D.SetUniform("projection", projection);
	main2D.SetUniform("model", model);

	std::string bitmapShader = "bitmap2D";
	Shader& bitmap2D = Renderer::LoadShader(
		bitmapShader,
		"res\\shaders\\bitmap_vert2D.shader",
		"res\\shaders\\bitmap_frag2D.shader"
	);
	bitmap2D.SetUniform("model", model);
	bitmap2D.SetUniform("projection", projection);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//BitmapFont font(bitmapShader);
	//font.Load("res\\bitmap\\timesNewRoman.bff");

	// render loop (happens every frame) -----------------------------------------
	while (!glfwWindowShouldClose(window)) {
		// -> frame time tracker
		Renderer::FrameTimeTracker();

		// -> rendering commands and configuration
		Renderer::RenderConfig(0.2f, 0.4f, 0.2f, 1.0f);

		// ---> space configurations and rendering
		//font.Print("BitmapFont Text", 50.0f, 50.0f);

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(glm::abs(sin(glfwGetTime())), 1.0f, 1.0f));
		main2D.SetUniform("model", model);
		RenderText(main2D, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		RenderText(main2D, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

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
	glBindVertexArray(VAO);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); ++c) {
		Character ch = characters.at(*c);

		float posX = x + (ch.bearing.x * scale);
		float posY = y - ((ch.size.y - ch.bearing.y) * scale);

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;
		
		//update VBO for each character
		float vertices[6][4] = {
				   { posX,     posY + h,   0.0f, 0.0f },
				   { posX,     posY,       0.0f, 1.0f },
				   { posX + w, posY,       1.0f, 1.0f },

				   { posX,     posY + h,   0.0f, 0.0f },
				   { posX + w, posY,       1.0f, 1.0f },
				   { posX + w, posY + h,   1.0f, 0.0f }
		};

		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		//update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.advance >> 6) * scale; //bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}