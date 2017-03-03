#pragma once
#include "ApplicationBase.h"
#include "Shader.h"
#include "Model.h"
#include "ModelArray.h"
#include "Camera.h"

class InstanceSampleApp : public glutil::ApplicationBase
{
public:
	explicit InstanceSampleApp(const std::string& windowName);
	explicit InstanceSampleApp(std::unique_ptr<glutil::WindowHandler> windowHandler);
	InstanceSampleApp() = delete;
	InstanceSampleApp(const InstanceSampleApp&) = delete;
	InstanceSampleApp& operator=(const InstanceSampleApp& other) = delete;
	~InstanceSampleApp() = default;
	void initialize();
protected:
	void updateData();
	void render();
	void updateMovement();
	std::unique_ptr<glutil::Shader> shader;
	std::unique_ptr<glutil::Model> model;
	std::unique_ptr<glutil::ModelArray<glutil::Model, glm::mat4>> modelArray;
	const std::size_t rockCount = 100;
};

