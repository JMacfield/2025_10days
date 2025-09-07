#include "Object3d.h"
#include "Modelmanager.h"
#include "Object3dCommon.h"
#include <random>
#include <Lerp.h>

float Object3d::GetEasedT(float t) const
{
	switch (easingType_) {
	case EasingType::EaseInSine:      return Easings::EaseInSine(t);
	case EasingType::EaseOutSine:     return Easings::EaseOutSine(t);
	case EasingType::EaseInOutSine:   return Easings::EaseInOutSine(t);
	case EasingType::EaseInCubic:     return Easings::EaseInCubic(t);
	case EasingType::EaseOutCubic:    return Easings::EaseOutCubic(t);
	case EasingType::EaseInOutCubic:  return Easings::EaseInOutCubic(t);
	case EasingType::EaseInQuint:     return Easings::EaseInQuint(t);
	case EasingType::EaseOutQuint:    return Easings::EaseOutQuint(t);
	case EasingType::EaseInOutQuint:  return Easings::EaseInOutQuint(t);
	case EasingType::EaseInCirc:      return Easings::EaseInCirc(t);
	case EasingType::EaseOutCirc:     return Easings::EaseOutCirc(t);
	case EasingType::EaseInOutCirc:   return Easings::EaseInOutCirc(t);
	case EasingType::EaseInBack:      return Easings::EaseInBack(t);
	case EasingType::EaseOutBack:     return Easings::EaseOutBack(t);
	case EasingType::EaseInOutBack:   return Easings::EaseInOutBack(t);
	case EasingType::EaseOutBounce:   return Easings::EaseOutBounce(t);
	case EasingType::EaseInBounce:    return Easings::EaseInBounce(t);
	case EasingType::EaseInOutBounce: return Easings::EaseInOutBounce(t);
	case EasingType::EaseInElastic:   return Easings::EaseInElastic(t);
	case EasingType::EaseOutElastic:  return Easings::EaseOutElastic(t);
	case EasingType::EaseInOutElastic:return Easings::EaseInOutElastic(t);
	case EasingType::EaseInQuart:     return Easings::EaseInQuart(t);
	case EasingType::EaseOutQuart:    return Easings::EaseOutQuart(t);
	case EasingType::EaseInOutQuart:  return Easings::EaseInOutQuart(t);
	case EasingType::EaseInExpo:      return Easings::EaseInExpo(t);
	case EasingType::EaseOutExpo:     return Easings::EaseOutExpo(t);
	case EasingType::EaseInOutExpo:   return Easings::EaseInOutExpo(t);
	default: return t;
	}
}

void Object3d::StartLerpToOriginalVertices()
{
	if (!model_ || originalVertices_.empty() || glitchedVertices_.empty()) return;
	isLerping_ = true;
	lerpT_ = 0.0f;
	lerpToGlitch_ = false; // ランダム→元
}

void Object3d::Init()
{

	WinAPI* sWinAPI = WinAPI::GetInstance();
	DirectXCommon* directXCommon = DirectXCommon::GetInstance();
	worldTransform_.Initialize();

	//バッファリソース
	// データを書き込む
	wvpData = nullptr;
	// WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	wvpResource = Mesh::CreateBufferResource(directXCommon->GetDevice(), sizeof(TransformationMatrix));
	// 書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	//単位行列を書き込んでいく
	wvpData->WVP = MakeIdentity4x4();
	wvpData->World = MakeIdentity4x4();
	// カメラ用
	cameraForGPUData_ = nullptr;
	cameraForGPUResource_ = Mesh::CreateBufferResource(directXCommon->GetDevice(), sizeof(CameraForGPU));
	//// 書き込むためのアドレスを取得
	cameraForGPUResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraForGPUData_));

	cameraForGPUData_->worldPosition = { 1.0f,1.0f,-5.0f };

}

void Object3d::Update()
{
	worldTransform_.UpdateMatrix();
	if (animationModel_) {
		animationModel_->Update();
	}
	else if (model_) {
		model_->Update();
	}
	if (isLerping_) {
		lerpT_ += lerpSpeed_;
		if (lerpT_ >= 1.0f) {
			lerpT_ = 1.0f;
			isLerping_ = false;
		}
		if (lerpToGlitch_) {
			LerpToGlitchedVertices(lerpT_);
		}
		else {
			LerpToOriginalVertices(lerpT_);
		}
	}
}

void Object3d::Draw(uint32_t texture, Camera* camera)
{
	DirectXCommon* directXCommon = DirectXCommon::GetInstance();
	if (animationModel_) {
		PSOAnimationModel* pso = PSOAnimationModel::GetInstance();
		directXCommon->GetCommandList()->SetGraphicsRootSignature(pso->GetProperty().rootSignature.Get());
		directXCommon->GetCommandList()->SetPipelineState(pso->GetProperty().graphicsPipelineState.Get());    //PSOを設定
	}
	else if (model_) {
		PSO* pso = PSO::GetInstance();

		directXCommon->GetCommandList()->SetGraphicsRootSignature(pso->GetProperty().rootSignature.Get());
		directXCommon->GetCommandList()->SetPipelineState(pso->GetProperty().graphicsPipelineState.Get());    //PSOを設定

	}
	else if (skybox_) {
		PSOSkybox* pso = PSOSkybox::GetInstance();
		directXCommon->GetCommandList()->SetGraphicsRootSignature(pso->GetProperty().rootSignature.Get());
		directXCommon->GetCommandList()->SetPipelineState(pso->GetProperty().graphicsPipelineState.Get());    //PSOを設定
	}

	cameraForGPUData_->worldPosition = camera->GetTransform().translate;
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldTransform_.matWorld_, camera->GetViewprojectionMatrix());


	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	directXCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	directXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
	directXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(4, cameraForGPUResource_->GetGPUVirtualAddress());
	// 3Dモデルが割り当てられていれば描画する
	if (animationModel_) {
		wvpData->WVP = worldViewProjectionMatrix;
		wvpData->World = worldTransform_.matWorld_;
		animationModel_->Draw(texture, { { 1.0f,1.0f,1.0f,1.0f },false
			}, { { 1.0f,1.0,1.0,1.0f } ,{ 0.0f,-1.0f,0.0f },0.5f }, mapTexture_);
	}
	else if (model_) {
		wvpData->WVP = worldViewProjectionMatrix;
		wvpData->World = worldTransform_.matWorld_;
		model_->Draw(texture, { {Materialquaternion_.x,Materialquaternion_.y,Materialquaternion_.z,Materialquaternion_.w},isLight },
			{ { DirectionalLightquaternion_.x,DirectionalLightquaternion_.y,DirectionalLightquaternion_.z,DirectionalLightquaternion_.w },
			{ lightDirection_.x,lightDirection_.y,lightDirection_.z},ambientLightIntensity_ }, mapTexture_);
	}
	else if (skybox_) {
		wvpData->WVP = worldViewProjectionMatrix;
		wvpData->World = worldTransform_.matWorld_;
		skybox_->Draw(texture, { { 1.0f,1.0f,1.0f,1.0f },false
			}, { { 1.0f,1.0,1.0,1.0f } ,{ 0.0f,-1.0f,0.0f },0.5f });
	}
}

void Object3d::Release()
{

}

void Object3d::SetPosition(const Vector3& pos) {
	position_ = pos;
	worldTransform_.UpdateMatrix();
}

Vector3 Object3d::GetPosition() const {
	return position_;
}

void Object3d::SetRotation(const Vector3& rot) {
	rotation_ = rot;
	worldTransform_.UpdateMatrix();

}

Vector3 Object3d::GetRotation() const {
	return rotation_;
}

void Object3d::SetScale(const Vector3& scale) {
	scale_ = scale;
	worldTransform_.UpdateMatrix();

}

Vector3 Object3d::GetScale() const {
	return scale_;
}

void Object3d::SetModel(const std::string& filePath)
{
	model_ = ModelManager::GetInstance()->FindModel(filePath);
	// 初期頂点を保存
	if (model_) {
		originalVertices_ = model_->GetModelData().vertices;
	}
}

void Object3d::SetAnimationModel(const std::string& filePath)
{
	animationModel_ = ModelManager::GetInstance()->FindAnimationModel(filePath);
}

void Object3d::SetTransform(Transform transform)
{
	worldTransform_.translation_ = transform.translate;
	worldTransform_.rotation_ = transform.rotate;
	worldTransform_.scale_ = transform.scale;
}

ModelData Object3d::LoadObjFile(const std::string& directoryPath, const std::string& filename)
{
	ModelData modelData; // 構築するMataData
	std::vector<Vector4> positions; // 位置
	std::vector<Vector3> normals; // 法線
	std::vector<Vector2> texcoords; // テクスチャ座標
	std::string line; // ファイルから読んだ1行を格納するもの

	std::ifstream file(directoryPath + "/" + filename); // ファイルを開く
	assert(file.is_open()); // とりあえず開けなかったら止める

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier; // 先頭の識別子を読む

		// identifierに応じた処理
		if (identifier == "v") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.w = 1.0f;
			position.z *= -1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt") {
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;

			texcoord.y *= -1.0f;// -texcoord.y; //- texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normal.z *= -1.0f;
			normals.push_back(normal);
		}
		else if (identifier == "f") {
			VertexData triangle[3];
			// 面は三角形限定。その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;
				// 頂点の要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');// 区切りでインデックスを読んでいく
					elementIndices[element] = std::stoi(index);

				}
				// 要素へのIndexから、実際の要素の値を取得して、頂点を構築する
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				//position.x *= -1.0f;
				//texcoord.y = 1.0f - texcoord.y;
				//normal.x *= -1.0f;

				VertexData vertex = { position, texcoord, normal };
				modelData.vertices.push_back(vertex);

				triangle[faceVertex] = { position,texcoord,normal };

			}
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);

		}
		else if (identifier == "mtllib") {
			// materialtemplateLibraryファイルの名前を取得する
			std::string materialFilename;
			s >> materialFilename;
			// 基本的にobjファイルと同一階層にmtlは存在させるので、ディレクトリ名とファイル名を渡す
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}

	}
	return modelData;
}

MaterialData Object3d::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
{
	MaterialData materialData;// 構築するMaterialData
	std::string line; // ファイルから読んだ1行をかくのうするもの
	std::ifstream file(directoryPath + "/" + filename); // ファイルを開く
	assert(file.is_open()); // とりあえず開けなかったら止める

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		// identiferに応じた処理
		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			// 連結してファイルパスにする
			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}

	}

	return materialData;
}

void Object3d::GlitchVertices(float intensity)
{
	if (!model_ || originalVertices_.empty()) return;

	// 乱数生成器
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(-intensity, intensity);

	glitchedVertices_ = originalVertices_; // 初期位置から生成
	for (auto& vertex : glitchedVertices_) {
		vertex.position.x += dist(gen);
		vertex.position.y += dist(gen);
		vertex.position.z += dist(gen);
	}

	// ここで一度ランダム位置を表示したい場合は
	auto& vertices = model_->GetModelData().vertices;
	vertices = glitchedVertices_;
	model_->UpdateVertexBuffer();
}

void Object3d::GlitchVerticesLerp(float intensity)
{
	if (!model_ || originalVertices_.empty()) return;

	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(-intensity, intensity);

	glitchedVertices_ = originalVertices_;
	for (auto& vertex : glitchedVertices_) {
		vertex.position.x += dist(gen);
		vertex.position.y += dist(gen);
		vertex.position.z += dist(gen);
	}

	isLerping_ = true;
	lerpT_ = 0.0f;
	lerpToGlitch_ = true; // 元→ランダム
}



void Object3d::LerpToOriginalVertices(float lerpT)
{
	if (!model_ || originalVertices_.empty() || glitchedVertices_.empty()) return;

	float easedT = GetEasedT(lerpT);

	auto& vertices = model_->GetModelData().vertices;
	vertices.resize(originalVertices_.size());
	for (size_t i = 0; i < vertices.size(); ++i) {
		vertices[i].position.x = glitchedVertices_[i].position.x * (1.0f - easedT) + originalVertices_[i].position.x * easedT;
		vertices[i].position.y = glitchedVertices_[i].position.y * (1.0f - easedT) + originalVertices_[i].position.y * easedT;
		vertices[i].position.z = glitchedVertices_[i].position.z * (1.0f - easedT) + originalVertices_[i].position.z * easedT;
	}
	model_->UpdateVertexBuffer();
}

void Object3d::LerpToGlitchedVertices(float lerpT)
{
	if (!model_ || originalVertices_.empty() || glitchedVertices_.empty()) return;

	float easedT = GetEasedT(lerpT);

	auto& vertices = model_->GetModelData().vertices;
	vertices.resize(originalVertices_.size());
	for (size_t i = 0; i < vertices.size(); ++i) {
		vertices[i].position.x = originalVertices_[i].position.x * (1.0f - easedT) + glitchedVertices_[i].position.x * easedT;
		vertices[i].position.y = originalVertices_[i].position.y * (1.0f - easedT) + glitchedVertices_[i].position.y * easedT;
		vertices[i].position.z = originalVertices_[i].position.z * (1.0f - easedT) + glitchedVertices_[i].position.z * easedT;
	}
	model_->UpdateVertexBuffer();
}

void Object3d::ResetVerticesToOriginal() {
	if (!model_ || originalVertices_.empty()) return;
	auto& vertices = model_->GetModelData().vertices;
	vertices = originalVertices_;
	model_->UpdateVertexBuffer();
}

void Object3d::LightDebug(const char* name)
{
#ifdef _DEBUG
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.2f, 0.7f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.1f, 0.3f, 0.5f));
	ImGui::Begin("light");
	if (ImGui::TreeNode(name))
	{
		// マテリアルのクォータニオン
		float materialQ[4] = { Materialquaternion_.x, Materialquaternion_.y, Materialquaternion_.z, Materialquaternion_.w };
		if (ImGui::DragFloat4("Material Quaternion", materialQ, 0.01f)) {
			Materialquaternion_ = { materialQ[0], materialQ[1], materialQ[2], materialQ[3] };
		}

		// ディレクショナルライトのクォータニオン
		float lightQ[4] = { DirectionalLightquaternion_.x, DirectionalLightquaternion_.y, DirectionalLightquaternion_.z, DirectionalLightquaternion_.w };
		if (ImGui::DragFloat4("Directional Light Quaternion", lightQ, 0.01f)) {
			DirectionalLightquaternion_ = { lightQ[0], lightQ[1], lightQ[2], lightQ[3] };
		}

		// ライト方向
		float direction[3] = { lightDirection_.x, lightDirection_.y, lightDirection_.z };
		if (ImGui::DragFloat3("Light Direction", direction, 0.01f)) {
			lightDirection_ = { direction[0], direction[1], direction[2] };
		}

		// アンビエント強度
		ImGui::DragFloat("Ambient Intensity", &ambientLightIntensity_, 0.01f, 0.0f, 5.0f);

		// ライトのON/OFF
		ImGui::Checkbox("Enable Light", &isLight);

		ImGui::TreePop();
	}
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
#endif // _DEBUG

}



void Object3d::ModelDebug(const char* name)
{
	//#ifdef _DEBUG
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.2f, 0.7f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.1f, 0.3f, 0.5f));
	ImGui::Begin("model");
	if (ImGui::TreeNode(name))
	{
		float translate[3] = { worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z };
		ImGui::DragFloat3("transform", translate, 0.01f);
		worldTransform_.translation_ = { translate[0],translate[1],translate[2] };

		float rotate[3] = { worldTransform_.rotation_.x , worldTransform_.rotation_.y, worldTransform_.rotation_.z };
		ImGui::DragFloat3("rotate", rotate, 0.01f);
		worldTransform_.rotation_ = { rotate[0],rotate[1],rotate[2] };

		float scale[3] = { worldTransform_.scale_.x, worldTransform_.scale_.y, worldTransform_.scale_.z };
		ImGui::DragFloat3("scale", scale, 0.01f);
		worldTransform_.scale_ = { scale[0],scale[1],scale[2] };
		ImGui::TreePop();
	}
	worldTransform_.UpdateMatrix();
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	//#endif // _DEBUG

}

void Object3d::EasingDebugUI(const char* name)
{
#ifdef _DEBUG
	static const char* easingNames[] = {
		"EaseInSine", "EaseOutSine", "EaseInOutSine",
		"EaseInCubic", "EaseOutCubic", "EaseInOutCubic",
		"EaseInQuint", "EaseOutQuint", "EaseInOutQuint",
		"EaseInCirc", "EaseOutCirc", "EaseInOutCirc",
		"EaseInBack", "EaseOutBack", "EaseInOutBack",
		"EaseOutBounce", "EaseInBounce", "EaseInOutBounce",
		"EaseInElastic", "EaseOutElastic", "EaseInOutElastic",
		"EaseInQuart", "EaseOutQuart", "EaseInOutQuart",
		"EaseInExpo", "EaseOutExpo", "EaseInOutExpo"
	};
	int current = static_cast<int>(easingType_);
	ImGui::Begin("EasingDebug C : Easing , V : Re-Easing");
	ImGui::Combo(name, &current, easingNames, static_cast<int>(EasingType::Count));
	easingType_ = static_cast<EasingType>(current);
	// nameをラベルに付加
	std::string lerpLabel = std::string(name) + " Lerp Speed";
	ImGui::DragFloat(lerpLabel.c_str(), &lerpSpeed_, 0.00001f, 0.000000f, 1.0f, "%.6f");
	ImGui::End();
#endif
}
