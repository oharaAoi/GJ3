#include "Loader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Engine/Module/Geometry/Structs/Vertices.h"
#include "Engine/System/Manager/MeshManager.h"
#include "Engine/Utilities/Logger.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　MeshのLoad
//////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<std::shared_ptr<Mesh>> LoadMesh(const std::string& directoryPath, const std::string& fileName, ID3D12Device* device) {
	Assimp::Importer importer;
	std::string filePath = directoryPath + fileName;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs | aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	assert(scene->HasMeshes()); // meshがないのは対応しない

	std::vector<std::vector<VertexData>> meshVertices;
	std::vector<std::vector<uint32_t>> meshIndices;
	std::vector<std::string> useMaterial;
	std::vector<std::string> meshNames;

	// mtlファイルを読み込んでおく
	Vector3 uvScale = Vector3(1, 1, 1);
	if (std::strcmp(GetFileExtension(fileName.c_str()), "obj") == 0) {
		LoadMtl(directoryPath, RemoveExtension(fileName) + ".mtl", uvScale);
	}

	// -------------------------------------------------
	// ↓ meshの解析
	// -------------------------------------------------
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		std::vector<VertexData> triangle;
		aiMesh* mesh = scene->mMeshes[meshIndex];
		//assert(mesh->HasNormals()); // 法線がないなら非対応
		//assert(mesh->HasTextureCoords(0)); // texcoordがないmeshは非対応

		meshIndices.resize(scene->mNumMeshes);
		meshNames.push_back((mesh->mName.C_Str()));

		// -------------------------------------------------
		// ↓ faceの解析をする
		// -------------------------------------------------
		std::vector<VertexData> vertices;
		vertices.resize(mesh->mNumVertices);
		// vertexの解析を行う
		for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
			aiVector3D& position = mesh->mVertices[vertexIndex];
			vertices[vertexIndex].pos = { position.x, position.y, position.z, 1.0f };

			// normal
			if (mesh->mNormals) {
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				vertices[vertexIndex].normal = { normal.x, normal.y, normal.z };
			} else {
				vertices[vertexIndex].normal = { 0.0f, 0.0f, 1.0f };
			}

			// texcoord
			if (mesh->mTextureCoords[0]) {
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
				vertices[vertexIndex].texcoord = { texcoord.x * uvScale.x, texcoord.y * uvScale.y };
			} else {
				vertices[vertexIndex].texcoord = { 0.0f, 0.0f };
			}

			// tangent
			if (mesh->mTangents) {
				aiVector3D& tangent = mesh->mTangents[vertexIndex];
				vertices[vertexIndex].tangent = { tangent.x, tangent.y, tangent.z };
			} else {
				vertices[vertexIndex].tangent = { 0.0f, 0.0f, 0.0f };
			}

			// 読み込み後の処理
			vertices[vertexIndex].pos.x *= -1.0f;
			vertices[vertexIndex].normal.x *= -1.0f;
		}

		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);

			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				meshIndices[meshIndex].push_back(vertexIndex);
			}
		}

		// -------------------------------------------------
		// ↓ メッシュのマテリアルインデックスを取得
		// -------------------------------------------------
		uint32_t materialIndex = mesh->mMaterialIndex;
		if (materialIndex < scene->mNumMaterials) {
			aiMaterial* material = scene->mMaterials[materialIndex];
			aiString materialName;
			if (AI_SUCCESS == material->Get(AI_MATKEY_NAME, materialName)) {
				std::string nameStr = materialName.C_Str();
				// DefaultMaterialを除く処理
				if (nameStr == "DefaultMaterial") {
					meshVertices.push_back(vertices);
					continue;
				}
				useMaterial.push_back(nameStr);
			} else {
				std::string nameStr = "not set MaterialName" + std::to_string(meshIndex);
				// DefaultMaterialを除く処理
				if (nameStr == "DefaultMaterial") {
					meshVertices.push_back(vertices);
					continue;
				}
				useMaterial.push_back(nameStr);
			}
		}
		// nodeの解析
		meshVertices.push_back(vertices);
	}

	for (uint32_t oi = 0; oi < meshVertices.size(); oi++) {
		MeshManager::GetInstance()->AddMesh(device, fileName, meshNames[oi], meshVertices[oi], meshIndices[oi]);
	}

	std::vector<std::shared_ptr<Mesh>> result;

	result = MeshManager::GetInstance()->GetMeshes(fileName);
	uint32_t index = 0;
	if (!useMaterial.empty()) {
		for (auto& it : result) {
			it->SetUseMaterial(useMaterial[index]);
			index++;
		}
	}

	return result;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　MaterialのLoad
//////////////////////////////////////////////////////////////////////////////////////////////////

std::unordered_map<std::string, std::unique_ptr<Material>> LoadMaterial(const std::string& directoryPath, const std::string& fileName) {
	Assimp::Importer importer;
	std::string filePath = directoryPath + fileName;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs | aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	assert(scene->HasMeshes()); // meshがないのは対応しない

	std::unordered_map<std::string, ModelMaterialData> materialData;
	std::vector<std::string> materials;
	// -------------------------------------------------
	// ↓ materialの解析
	// -------------------------------------------------
	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];

		aiString materialName;
		if (AI_SUCCESS == material->Get(AI_MATKEY_NAME, materialName)) {
			std::string nameStr = materialName.C_Str();
			if (nameStr == "DefaultMaterial") {
				continue;
			}
		}

		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			materials.push_back(materialName.C_Str());
			materialData[materialName.C_Str()] = ModelMaterialData();
			std::string objTexture = textureFilePath.C_Str();
			materialData[materialName.C_Str()].textureFilePath = objTexture;
			TextureManager::GetInstance()->StackTexture(directoryPath, textureFilePath.C_Str());
		}
	}

	std::unordered_map<std::string, std::unique_ptr<Material>> materialResult;// 結果
	for (uint32_t oi = 0; oi < materials.size(); oi++) {
		materialResult[materials[oi]] = std::make_unique<Material>();
		materialResult[materials[oi]]->Init();
		materialResult[materials[oi]]->SetMaterialData(materialData[materials[oi]]);
	}

	return materialResult;
}

std::unordered_map<std::string, ModelMaterialData> LoadMaterialData(const std::string& directoryPath, const std::string& fileName) {
	Assimp::Importer importer;
	std::string filePath = directoryPath + fileName;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs | aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	assert(scene->HasMeshes()); // meshがないのは対応しない

	std::unordered_map<std::string, ModelMaterialData> materialData;

	// -------------------------------------------------
	// ↓ materialの解析
	// -------------------------------------------------
	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];

		aiString materialName;
		if (AI_SUCCESS == material->Get(AI_MATKEY_NAME, materialName)) {
			std::string nameStr = materialName.C_Str();
			if (nameStr == "DefaultMaterial") {
				continue;
			}
		} else {
			materialName = "not set MaterialName" + std::to_string(materialIndex);
		}

		materialData[materialName.C_Str()] = ModelMaterialData();
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			std::string objTexture = textureFilePath.C_Str();
			materialData[materialName.C_Str()].textureFilePath = objTexture;
			TextureManager::GetInstance()->StackTexture(directoryPath, objTexture);
		}

		aiColor3D color;
		if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, color)) {
			materialData[materialName.C_Str()].color = { color.r, color.g, color.b, 1.0f };
		}
	}

	return materialData;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　MaterialのLoad
//////////////////////////////////////////////////////////////////////////////////////////////////

void LoadMtl(const std::string& directoryPath, const std::string& fileName, Vector3& scale) {
	std::unordered_map<std::string, ModelMaterialData> materialDatas;// 後で一気に結果の変数に代入するための物

	std::string line;// ファイルから読み込んだ1行を格納する物
	std::string materialName; // newmtlの名前

	// マテリアルの名前を格納しておく
	std::vector<std::string> materials;

	// mtlファイルを開く
	std::ifstream file2(directoryPath + "/" + fileName);
	assert(file2.is_open());

	// ファイルを読む
	while (std::getline(file2, line)) {
		std::string materialIdentifier;
		std::istringstream s(line);
		s >> materialIdentifier; // 先頭の識別子を読む

		if (materialIdentifier == "newmtl") {
			s >> materialName;
			materials.push_back(materialName);

		} else if (materialIdentifier == "map_Kd") {

			// テクスチャのスケーリングオプションを処理
			std::string scalingOption;
			if (s >> scalingOption && scalingOption == "-s") {
				// スケーリング値を読み取る
				float scaleX, scaleY, scaleZ;
				s >> scaleX >> scaleY >> scaleZ;
				// スケーリング情報を保存
				scale = Vector3(scaleX, scaleY, scaleZ);
			}

		} else if (materialIdentifier == "Ka") {
			// アルベド色を読み取る(環境反射率)
			Vector4 color;
			s >> color.x >> color.y >> color.z;

		} else if (materialIdentifier == "Kd") {
			// ディフューズ色を読み取る(拡散反射率)
			Vector4 color;
			s >> color.x >> color.y >> color.z;

		} else if (materialIdentifier == "Ks") {
			// スペキュラ色(鏡面反射率)
			Vector4 color;
			s >> color.x >> color.y >> color.z;


		} else if (materialIdentifier == "Ke") {
			// 自己発光
			Vector4 color;
			s >> color.x >> color.y >> color.z;


		} else if (materialIdentifier == "Ni") {
			// 屈折率
			float refraction;
			s >> refraction;


		} else if (materialIdentifier == "Ns") {
			// shininess(鏡面反射の鋭さ)
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Nodeを返す
//////////////////////////////////////////////////////////////////////////////////////////////////

Model::Node LoadNode(const std::string& directoryPath, const std::string& fileName) {
	Assimp::Importer importer;
	std::string filePath = directoryPath + fileName;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs | aiProcess_Triangulate | aiProcess_CalcTangentSpace);

	Model::Node result;
	result = ReadNode(scene->mRootNode, scene);

	return result;
}

Model::Node ReadNode(aiNode* node, const aiScene* scene) {
	Model::Node result;
	// ----------------------------------
	// LocalMatrixを取得する
	// ----------------------------------
	aiVector3D scale, translate;
	aiQuaternion rotate;
	node->mTransformation.Decompose(scale, rotate, translate);

	//result.transform.scale = { 1, 1, 1 };
	result.transform.scale = { scale.x, scale.y, scale.z };
	result.transform.rotate = { rotate.x, -rotate.y, -rotate.z, rotate.w };
	result.transform.translate = { -translate.x, translate.y, translate.z };
	result.localMatrix = Matrix4x4::MakeAffine(result.transform.scale, result.transform.rotate.Normalize(), result.transform.translate);
	result.name = node->mName.C_Str(); // Nodeの名前を格納

	// ----------------------------------
	// Nodeを格納する
	// ----------------------------------
	result.children.resize(node->mNumChildren); // 子供の数だけ確保
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		// 再帰的に読んで階層構造を作っていく
		result.children[childIndex] = ReadNode(node->mChildren[childIndex], scene);
	}

	return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Animationを読み込む
//////////////////////////////////////////////////////////////////////////////////////////////////

std::unordered_map<std::string, Animation> LoadAnimation(const std::string directoryPath, const std::string& animationFile) {
	Assimp::Importer importer;
	std::string filePath = directoryPath + animationFile;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);
	if (!scene || scene->mNumAnimations == 0) {
		throw std::runtime_error("Failed to load animations or no animations present");
	}

	Logger::Log("[Load Animation] :" + animationFile + "]\n");

	std::unordered_map<std::string, Animation> animationMap{};

	for (uint32_t animationIndex = 0; animationIndex < scene->mNumAnimations; ++animationIndex) {
		// sceneからanimationの情報を取得する
		aiAnimation* animationAssimp = scene->mAnimations[animationIndex];

		std::string animationName = animationAssimp->mName.C_Str();										// animationの名前
		Animation animationData{};																		// animationのデータ
		animationData.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond);	// 時間の単位を秒に変換
		animationData.animationName = animationName;													// animatonの名前を取得


		// -------------------------------------------------
		// ↓ アニメーションの解析
		// -------------------------------------------------
		// assimpでは個々のNodeのAnimationをchannelと呼ぶ
		for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex) {
			aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
			NodeAnimation& nodeAnimation = animationData.nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];

			// -------------------------------------------------
			// ↓ Vector3の読み込み
			// -------------------------------------------------

			for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex) {
				aiVectorKey& keyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
				KeyframeVector3 keyframe{};
				keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);	// 秒に変換
				keyframe.value = { -keyAssimp.mValue.x,keyAssimp.mValue.y, keyAssimp.mValue.z };
				nodeAnimation.translate.keyframes.push_back(keyframe);
				/*Log("---------------------------------\n");
				std::string timeLog = "keyFrame.time : " + std::to_string(keyframe.time) + "\n";
				Log(timeLog);

				std::string valueXLog = "keyFrame.value X : " + std::to_string(keyframe.value.x) + "\n";
				std::string valueYLog = "keyFrame.value Y : " + std::to_string(keyframe.value.y) + "\n";
				std::string valueZLog = "keyFrame.value Z : " + std::to_string(keyframe.value.z) + "\n";

				Log(valueXLog);
				Log(valueYLog);
				Log(valueZLog);*/
			}

			// -------------------------------------------------
			// ↓ Quaternionの読み込み
			// -------------------------------------------------

			for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumRotationKeys; ++keyIndex) {

				aiQuatKey& keyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
				KeyframeQuaternion keyframe{};
				keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);	// 秒に変換
				keyframe.value = { keyAssimp.mValue.x, -keyAssimp.mValue.y, -keyAssimp.mValue.z, keyAssimp.mValue.w };
				nodeAnimation.rotate.keyframes.push_back(keyframe);
				/*std::string timeLog = "keyFrame.time : " + std::to_string(keyframe.time) + "\n";
				Log(timeLog);*/
				/*Log("--------------------------------------------------\n");
				std::string timeLog = "TicksPerSecond : " + std::to_string(animationAssimp->mTicksPerSecond) + "\n";
				Log(timeLog);

				Log("--------------------------------------------------\n");*/

			}

			// -------------------------------------------------
			// ↓ Scaleの読み込み
			// -------------------------------------------------

			for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumScalingKeys; ++keyIndex) {
				aiVectorKey& keyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
				KeyframeVector3 keyframe{};
				keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);	// 秒に変換
				keyframe.value = { keyAssimp.mValue.x,keyAssimp.mValue.y, keyAssimp.mValue.z };
				nodeAnimation.scale.keyframes.push_back(keyframe);

				/*Log("---------------------------------\n");
				std::string timeLog = "keyFrame.time : " + std::to_string(keyframe.time) + "\n";
				Log(timeLog);

				std::string valueXLog = "keyFrame.value X : " + std::to_string(keyframe.value.x) + "\n";
				std::string valueYLog = "keyFrame.value Y : " + std::to_string(keyframe.value.y) + "\n";
				std::string valueZLog = "keyFrame.value Z : " + std::to_string(keyframe.value.z) + "\n";

				Log(valueXLog);
				Log(valueYLog);
				Log(valueZLog);*/
			}
		}

		animationMap.try_emplace(animationName, animationData);
	}

	Logger::Log("success\n");

	return animationMap;
}

std::vector<std::unique_ptr<SkinCluster>> LoadSkinCluster(const std::string& directoryPath, const std::string& fileName) {
	Assimp::Importer importer;
	std::string filePath = directoryPath + fileName;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs | aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	assert(scene->HasMeshes()); // meshがないのは対応しない

	std::vector<std::unique_ptr<SkinCluster>> result;

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];

		// -------------------------------------------------
		// ↓ skinningを取得する用の処理
		// -------------------------------------------------
		std::map<std::string, JointWeightData> newMap;

		for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
			// jointごとの格納領域を作る
			aiBone* bone = mesh->mBones[boneIndex];
			std::string jointName = bone->mName.C_Str();
			JointWeightData& jointWeightData = newMap[jointName];

			// InverseBindPoseMatrixの抽出
			aiMatrix4x4 bindPoseMatrixAssimp = bone->mOffsetMatrix.Inverse();
			aiVector3D scale, translate;
			aiQuaternion rotate;
			bindPoseMatrixAssimp.Decompose(scale, rotate, translate);
			Matrix4x4 bindPoseMatrix = Matrix4x4::MakeAffine({ scale.x, scale.y, scale.z },
															 Quaternion{ rotate.x, -rotate.y, -rotate.z, rotate.w },
															 { -translate.x, translate.y, translate.z }
			);
			jointWeightData.inverseBindPoseMatrix = bindPoseMatrix.Inverse();

			// Weight情報を取り出す
			for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex) {
				jointWeightData.vertexWeight.push_back({ bone->mWeights[weightIndex].mWeight, bone->mWeights[weightIndex].mVertexId });
			}
		}

		auto& newData = result.emplace_back(std::make_unique<SkinCluster>());
		newData->AddData(newMap);
	}

	return result;
}

//================================================================================================//
//
// ファイル拡張子の探索
//
//================================================================================================//

const char* GetFileExtension(const char* filename) {
	const char* ext = std::strrchr(filename, '.'); // 最後のピリオドを探す
	if (ext == nullptr) {
		return ""; // 拡張子がない場合は空文字を返す
	}
	return ext + 1; // ピリオドの次の文字から拡張子を返す
}

std::string RemoveExtension(const std::string& filename) {
	size_t dotPos = filename.find_last_of('.');
	if (dotPos != std::string::npos) {
		return filename.substr(0, dotPos); // ドットより前の部分を返す
	}
	return filename; // ドットがない場合はそのまま返す
}

std::string GetBaseName(const std::string& filename) {
	size_t dotPos = filename.find_last_of('.');
	if (dotPos == std::string::npos) {
		return filename; // '.' がなければそのまま返す
	}
	return filename.substr(0, dotPos); // 先頭から '.' の直前まで
}
