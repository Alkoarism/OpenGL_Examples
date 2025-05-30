# Sound
In this version, there are a few objectives for the project:
- [ ] Make sense of the core engine
- [ ] Sketch a preliminary documentation
- [ ] Implement sound on the core engine

This README by itself will be used as such sketch in order to facilitate future understanding of the inner workings of the engine. Thus:

## Core engine Documentation
The engine is structured mainly with an DOP desing in mind.
Here are the purpose and usage of some of the classes:

### Core Engine:
#### Camera
This subsystem implements a 3D free-view camera with pitch and yaw controls based on 3 dimensional vectors. The class can be constructed in two ways:
>Camera(glm::vec3 position, glm::vec3 direction, float yaw,	float pitch)
>Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
The first constructor provides standard default values, allowing quick setup - the camera starts at the origin and faces forward (negative Z) according to the right-hand rule.

__Camera Controls__
There are three functions available for user input processing:
>void ProcessKeyboard(direction, float deltaTime);
>void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch);
>void ProcessMouseScroll(float yOffset);
ProcessKeyBoard() expects a direction from the Camera_Movement enum: *FORWARD*, *BACKWARD*, *LEFT*, *RIGHT*. 
:26A0: Note: The camera class __does not define which key is responsible for each direction__, that must be handled by the user´s code. A valid diretion command would look as follows:
>if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
>		glfwSetWindowShouldClose(window, true);
ProcessMouseMovement() updates the camera angles based on mouse delta (how much it moved between renderings). The Pitch is constrained by defaut to ±89° to prevent gimbal lock, it can be deactivated by setting *constrainPitch* as *true*. 
ProcessMouseScroll() acts like a vertical camera shift, changing the value of y position.

__Public Variables__
The following variables are available for forced changes if needed:
- glm::vec3 Position;
- glm::vec3 Front;
- glm::vec3 Up;
- glm::vec3 Right;
- glm::vec3 WorldUp;

- float Yaw;
- float Pitch;

- float MovementSpeed;
- float MouseSensitivity;
- float Zoom;

#### VAO´s, VBO´s and IBO´s:
#### Renderer
#### Shader
#### Texture

### Modules:
#### Bitmap Font
#### Freetype Font
#### Entity
#### Sprite Renderer

## Sound Implementation Notes
- v0.04.00: README refactoring
- v0.04.01: Inclusion of Bitmap and Freetype as modules of the core engine

# References
External sources:
1. Bitmap text rendering was made using an implementation of [CBFG](https://github.com/CodeheadUK/CBFG) and his example code with some changes;
2. [Learn OpenGL](https://learnopengl.com/) book site;
3. The [Cherno`s youtube channel](https://www.youtube.com/channel/UCQ-W1KE9EYfdxhL6S4twUNw), OpenGL series;
4. [The FreeType Project](https://freetype.org/) (All rights reserved);