#include "ModelManager.h"
#include "Engine/Utilities/Logger.h"

std::unordered_map<std::string, std::unique_ptr<Model>> ModelManager::modelMap_;
std::unordered_map<std::string, std::string> ModelManager::modelPathMap_;
std::vector<std::string> ModelManager::modelNameList_;

ModelManager::~ModelManager() {}

ModelManager* ModelManager::GetInstance() {
	static ModelManager instance;
	return &instance;
}

void ModelManager::Init() {
	
}

void ModelManager::Finalize() {
	modelMap_.clear();
}

void ModelManager::LoadModel(const std::string& directoryPath, const std::string& modelName) {
	if (auto it = modelMap_.find(modelName); it != modelMap_.end()) {
		return;
	}
	modelPathMap_.try_emplace(modelName, directoryPath);
	modelNameList_.push_back(modelName);
	modelMap_.try_emplace(modelName, Engine::CreateModel(directoryPath, modelName));
}

Model* ModelManager::GetModel(const std::string& modelName) {
	if (auto it = modelMap_.find(modelName); it == modelMap_.end()) {
		Logger::Log(std::string("Not Found : " + modelName + "\n"));
		assert(false && "Model not found!");
	}

	return modelMap_[modelName].get();
}

std::string ModelManager::GetModelPath(const std::string& modelName) {
	if (auto it = modelPathMap_.find(modelName); it == modelPathMap_.end()) {
		Logger::Log(std::string("Not Found Path: " + modelName + "\n"));
		assert(false && "Model not found Path!");
	}

	return modelPathMap_[modelName];
}
