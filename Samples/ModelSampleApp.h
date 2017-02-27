#pragma once

class ModelSampleApp : public shared::ApplicationBase
{
public:
	explicit ModelSampleApp(const std::string& windowName);
	explicit ModelSampleApp(std::unique_ptr<shared::WindowHandler> windowHandler);
	ModelSampleApp() = delete;
	ModelSampleApp(const ModelSampleApp&) = delete;
	ModelSampleApp& operator=(const ModelSampleApp& other) = delete;
	~ModelSampleApp() = default;
	void initialize();
protected:
	void updateData();
	void render();
	void updateMovement();
	std::unique_ptr<shared::Shader> shader;
	std::unique_ptr<shared::Model> model;
};

