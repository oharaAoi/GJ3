#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <assert.h>
#include "Engine.h"
#include "Engine/Module/Components/GameObject/Model.h"

/// <summary>
/// Modelを管理するクラス
/// </summary>
class ModelManager {
public:

	ModelManager() = default;
	~ModelManager();
	ModelManager(const ModelManager&) = delete;
	const ModelManager& operator=(const ModelManager&) = delete;

	static ModelManager* GetInstance();

	void Init();
	void Finalize();

	/// <summary>
	/// modelを読み込む
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="modelName"></param>
	static void LoadModel(const std::string& directoryPath, const std::string& modelName);

	/// <summary>
	/// modelを取得する
	/// </summary>
	/// <param name="modelName"></param>
	/// <returns></returns>
	static Model* GetModel(const std::string& modelName);

	static std::string GetModelPath(const std::string& modelName);

	std::vector<std::string>& GetModelNameList() { return modelNameList_; }
	static std::vector<std::string>& GetModelNames() { return GetInstance()->GetModelNameList(); }

private:

	static std::unordered_map<std::string, std::unique_ptr<Model>> modelMap_;
	static std::vector<std::string> modelNameList_;

	static std::unordered_map<std::string, std::string> modelPathMap_;
};

