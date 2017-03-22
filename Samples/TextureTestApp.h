#pragma once

class TextureTestApp : public glutil::ApplicationBase
{
public:
	explicit TextureTestApp(const std::string& windowName);
	explicit TextureTestApp(std::unique_ptr<glutil::WindowHandler> windowHandler);
	TextureTestApp() = delete;
	TextureTestApp(const TextureTestApp&) = delete;
	TextureTestApp& operator=(const TextureTestApp& other) = delete;
	~TextureTestApp() = default;
	void initialize();
protected:
	void updateData();
	void render();
	void updateMovement();
	void initializeModel();
	glm::mat4 modelMat;
	std::unique_ptr<glutil::Shader> shader;
	std::unique_ptr<glutil::Model> model;
};

