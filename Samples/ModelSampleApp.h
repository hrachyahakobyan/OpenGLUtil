#pragma once

class ModelSampleApp : public glutil::ApplicationBase
{
public:
	explicit ModelSampleApp(const std::string& windowName);
	explicit ModelSampleApp(std::unique_ptr<glutil::WindowHandler> windowHandler);
	ModelSampleApp() = delete;
	ModelSampleApp(const ModelSampleApp&) = delete;
	ModelSampleApp& operator=(const ModelSampleApp& other) = delete;
	~ModelSampleApp() = default;
	void initialize();
protected:
	void updateData();
	void render();
	void updateMovement();
	glm::mat4 view;
	glm::mat4 proj;
	std::shared_ptr<glutil::Shader> shader;
	std::unique_ptr<glutil::Model> model;
	std::shared_ptr<glutil::Skybox> skybox;
};

