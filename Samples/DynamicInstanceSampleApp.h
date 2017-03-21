#pragma once

class DynamicInstanceSampleApp : public glutil::ApplicationBase
{
public:
	explicit DynamicInstanceSampleApp(const std::string& windowName);
	explicit DynamicInstanceSampleApp(std::unique_ptr<glutil::WindowHandler> windowHandler);
	DynamicInstanceSampleApp() = delete;
	DynamicInstanceSampleApp(const DynamicInstanceSampleApp&) = delete;
	DynamicInstanceSampleApp& operator=(const DynamicInstanceSampleApp& other) = delete;
	~DynamicInstanceSampleApp() = default;
	void initialize();
protected:
	void updateData();
	void render();
	void updateMovement();
	std::unique_ptr<glutil::Shader> shader;
	glutil::Model model;
	std::size_t currentModel;
	std::unique_ptr<glutil::ModelArray<glutil::Model, glm::vec3, glm::mat4>> modelArray;
	void initializeModel();
};

