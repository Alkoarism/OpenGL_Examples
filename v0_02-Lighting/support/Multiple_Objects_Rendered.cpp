		// ---> texture configurations
		if (text_cng) {
			textures[0].Bind();
		}
		else {
			textures[1].Bind();
		}

		if (elapsedTime > 0.5) {
			text_cng = !text_cng;
			elapsedTime = 0;
		}

		// ---> space configurations and rendering
		va.Bind();

		glm::mat4 projection = glm::perspective
			(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		myShader.setMat4("projection", projection);

		glm::mat4 view = camera.GetViewMatrix();
		myShader.setMat4("view", view);

		for (size_t i = 0; i < aliensTransformations.size(); i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, aliensTransformations[i]);
			myShader.setMat4("model", model);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
