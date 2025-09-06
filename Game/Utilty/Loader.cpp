#include "Loader.h"
#include "json.hpp"
#include <fstream>
#include <cassert>
#include "ModelManager.h"
#include "LoaderConfig.h"

float Loader::Lerp(const float& a, const float& b, float t) {
	return a + (b - a) * t;
}

/// <summary>
/// JSONファイルを読み込み、オブジェクトとカメラを初期化する関数
/// </summary>
/// <param name="kDefaultBaseDirectory">デフォルトのベースディレクトリ</param>
/// <param name="fileName">読み込むJSONファイル名（拡張子なし）</param>
/// <param name="objects">生成したオブジェクトを格納するベクター</param>
/// <param name="camera">カメラオブジェクトへのポインタ</param>
void Loader::LoadJsonFile(const std::string kDefaultBaseDirectory, const std::string fileName, std::vector<Object3d*>& objects, std::vector<Collider*>& colliders, Camera* camera)
{
	// フルパスを生成
	const std::string fullpath = kDefaultBaseDirectory + "/" + fileName + ".json";

	// ファイルストリームを作成
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// JSONデータを格納する変数
	nlohmann::json deserialized;

	// ファイルからJSONデータを読み込む
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"フィールドを取得
	std::string name = deserialized["name"].get<std::string>();
	// "scene"であることを確認
	assert(name.compare("scene") == 0);

	// レベルデータを格納するインスタンスを生成
	LevelData* levelData = new LevelData();

	// "objects"配列内の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		// オブジェクトの種別を取得
		std::string type = object["type"].get<std::string>();

		// MESHオブジェクトの場合
		if (type.compare("MESH") == 0) {
			// オブジェクトデータを追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			// 追加したオブジェクトデータへの参照を取得
			LevelData::ObjectData& objectData = levelData->objects.back();

			// ファイル名を取得
			if (object.contains("file_name")) {
				objectData.filename = object["file_name"].get<std::string>();
			}

			// トランスフォームのパラメータを読み込む
			nlohmann::json& transform = object["transform"];
			// 平行移動
			objectData.transform.translate.x = (float)transform["translation"][0];
			objectData.transform.translate.y = (float)transform["translation"][2];
			objectData.transform.translate.z = (float)transform["translation"][1];
			// 回転角度
			objectData.transform.rotate.x = -(float)transform["rotation"][0];
			objectData.transform.rotate.y = -(float)transform["rotation"][2];
			objectData.transform.rotate.z = -(float)transform["rotation"][1];
			// スケーリング
			objectData.transform.scale.x = (float)transform["scaling"][0];
			objectData.transform.scale.y = (float)transform["scaling"][2];
			objectData.transform.scale.z = (float)transform["scaling"][1];

			// 当たり判定
			if (object.contains("collider")) {
				auto& colliderData = object["collider"];

				// 中心座標
				Vector3 pos = {
					(float)colliderData["center"][0],
					(float)colliderData["center"][2],
					(float)colliderData["center"][1]
				};
				objectData.colliderCenterPos = pos;
				// 大きさ
				Vector3 size = {
					(float)colliderData["size"][0],
					(float)colliderData["size"][2],
					(float)colliderData["size"][1]
				};
				objectData.colliderSize = size;

				// 当たり判定のマスク
				objectData.maskType = colliderData["type"];

				objectData.isCollision = true;
			}
			objectData.isCollision = false;

			// モデルをロード
			ModelManager::GetInstance()->LoadModel("Resources/game/", objectData.filename + ".obj");
		}

		// 子オブジェクトが存在する場合（未実装）
		if (object.contains("children")) {
			// TODO: 再帰的に子オブジェクトを処理する
		}

		// CAMERAオブジェクトの場合
		if (type.compare("CAMERA") == 0) {
			// オブジェクトデータを追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			// 追加したオブジェクトデータへの参照を取得
			LevelData::ObjectData& objectData = levelData->objects.back();

			// トランスフォームのパラメータを読み込む
			nlohmann::json& transform = object["transform"];
			// 平行移動
			objectData.transform.translate.x = (float)transform["translation"][0];
			objectData.transform.translate.y = (float)transform["translation"][2];
			objectData.transform.translate.z = (float)transform["translation"][1];
			// 回転角度
			objectData.transform.rotate.x = -((float)transform["rotation"][0] - 3.141592f / 2.0f);
			objectData.transform.rotate.y = -(float)transform["rotation"][2];
			objectData.transform.rotate.z = -(float)transform["rotation"][1];
			// スケーリング
			objectData.transform.scale.x = (float)transform["scaling"][0];
			objectData.transform.scale.y = (float)transform["scaling"][1];
			objectData.transform.scale.z = (float)transform["scaling"][2];

			// カメラの位置と回転を設定
			camera->SetTranslate(objectData.transform.translate);
			camera->SetRotate(objectData.transform.rotate);
		}
	}

	// オブジェクトを生成し、配置する
	for (auto& objectData : levelData->objects) {
		// 3Dオブジェクトを生成
		Object3d* newObject3d = new Object3d();
		newObject3d->Init();
		newObject3d->SetModel(objectData.filename + ".obj");
		newObject3d->SetTransform(objectData.transform);
		// 配列に追加
		objects.push_back(newObject3d);

		// 当たり判定をペアレント
		if (objectData.isCollision) {
			Collider* collider = new Collider();
			collider->worldTransform.parent_ = &newObject3d->worldTransform_;
			collider->SetOBBCenterPos(objectData.colliderCenterPos);
			collider->SetOBBLength(objectData.colliderSize);
			// 壁
			if (objectData.maskType == LoaderConfig::Collider::Mask::wall) {
				collider->SetCollisionPrimitive(kCollisionOBB);
				collider->SetCollisionAttribute(kCollisionAttributeObstacles);
				collider->SetCollisionMask(~kCollisionAttributeObstacles);
			}
			// 攻撃壁
			if (objectData.maskType == LoaderConfig::Collider::Mask::attack) {
				collider->SetCollisionPrimitive(kCollisionOBB);
				collider->SetCollisionAttribute(kCollisionAttributeEnemy);
				collider->SetCollisionMask(~kCollisionAttributeEnemy);
			}

			collider->SetIsActive(true);

			colliders.push_back(collider);
		}
	}
}

/// <summary>
/// JSONファイルを読み込み、オブジェクトを初期化する関数（カメラ設定なし）
/// </summary>
/// <param name="kDefaultBaseDirectory">デフォルトのベースディレクトリ</param>
/// <param name="fileName">読み込むJSONファイル名（拡張子なし）</param>
/// <param name="objects">生成したオブジェクトを格納するベクター</param>
void Loader::LoadJsonFile2(const std::string kDefaultBaseDirectory, const std::string fileName, std::vector<Object3d*>& objects, std::vector<Collider*>& colliders)
{
	// フルパスを生成
	const std::string fullpath = kDefaultBaseDirectory + "/" + fileName + ".json";

	// ファイルストリームを作成
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// JSONデータを格納する変数
	nlohmann::json deserialized;

	// ファイルからJSONデータを読み込む
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"フィールドを取得
	std::string name = deserialized["name"].get<std::string>();
	// "scene"であることを確認
	assert(name.compare("scene") == 0);

	// レベルデータを格納するインスタンスを生成
	LevelData* levelData = new LevelData();

	// "objects"配列内の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		// オブジェクトの種別を取得
		std::string type = object["type"].get<std::string>();

		// MESHオブジェクトの場合
		if (type.compare("MESH") == 0) {
			// オブジェクトデータを追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			// 追加したオブジェクトデータへの参照を取得
			LevelData::ObjectData& objectData = levelData->objects.back();

			// ファイル名を取得
			if (object.contains("file_name")) {
				objectData.filename = object["file_name"].get<std::string>();
			}

			// トランスフォームのパラメータを読み込む
			nlohmann::json& transform = object["transform"];
			// 平行移動
			objectData.transform.translate.x = (float)transform["translation"][0];
			objectData.transform.translate.y = (float)transform["translation"][2];
			objectData.transform.translate.z = (float)transform["translation"][1];
			// 回転角度
			objectData.transform.rotate.x = -(float)transform["rotation"][0];
			objectData.transform.rotate.y = -(float)transform["rotation"][2];
			objectData.transform.rotate.z = -(float)transform["rotation"][1];
			// スケーリング
			objectData.transform.scale.x = (float)transform["scaling"][0];
			objectData.transform.scale.y = (float)transform["scaling"][2];
			objectData.transform.scale.z = (float)transform["scaling"][1];

			// 当たり判定
			objectData.isCollision = false;
			if (object.contains("collider")) {
				auto& colliderData = object["collider"];

				// 中心座標
				Vector3 pos = {
					(float)colliderData["center"][0],
					(float)colliderData["center"][2],
					(float)colliderData["center"][1]
				};
				objectData.colliderCenterPos = pos;
				// 大きさ
				Vector3 size = {
					(float)colliderData["size"][0],
					(float)colliderData["size"][2],
					(float)colliderData["size"][1]
				};
				objectData.colliderSize = size;

				// いったん上書き
				objectData.colliderSize.x = (float)transform["scaling"][0];
				objectData.colliderSize.y = (float)transform["scaling"][2];
				objectData.colliderSize.z = (float)transform["scaling"][1];


				// 当たり判定のマスク
				objectData.maskType = colliderData["type"];

				objectData.isCollision = true;
			}

			// モデルをロード
			ModelManager::GetInstance()->LoadModel("Resources/game/", objectData.filename + ".obj");
		}

		// 子オブジェクトが存在する場合（未実装）
		if (object.contains("children")) {
			// TODO: 再帰的に子オブジェクトを処理する
		}

		// CAMERAオブジェクトの場合（カメラ設定なし）
		if (type.compare("CAMERA") == 0) {
			// オブジェクトデータを追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			// 追加したオブジェクトデータへの参照を取得
			LevelData::ObjectData& objectData = levelData->objects.back();

			// トランスフォームのパラメータを読み込む
			nlohmann::json& transform = object["transform"];
			// 平行移動
			objectData.transform.translate.x = (float)transform["translation"][0];
			objectData.transform.translate.y = (float)transform["translation"][2];
			objectData.transform.translate.z = (float)transform["translation"][1];
			// 回転角度
			objectData.transform.rotate.x = -((float)transform["rotation"][0] - 3.141592f / 2.0f);
			objectData.transform.rotate.y = -(float)transform["rotation"][2];
			objectData.transform.rotate.z = -(float)transform["rotation"][1];
			// スケーリング
			objectData.transform.scale.x = (float)transform["scaling"][0];
			objectData.transform.scale.y = (float)transform["scaling"][2];
			objectData.transform.scale.z = (float)transform["scaling"][1];

			// カメラ設定は行わない
		}
	}

	// オブジェクトを生成し、配置する
	for (auto& objectData : levelData->objects) {
		// 3Dオブジェクトを生成
		Object3d* newObject3d = new Object3d();
		newObject3d->Init();
		newObject3d->SetModel(objectData.filename + ".obj");
		newObject3d->SetTransform(objectData.transform);
		// 配列に追加
		objects.push_back(newObject3d);

		// 当たり判定をペアレント
		if (objectData.isCollision) {
			Collider* collider = new Collider();
			collider->worldTransform.parent_ = &newObject3d->worldTransform_;
			collider->SetOBBCenterPos(objectData.colliderCenterPos);
			collider->SetOBBLength(objectData.colliderSize);
			// 壁
			if (objectData.maskType == LoaderConfig::Collider::Mask::wall) {
				collider->SetCollisionPrimitive(kCollisionOBB);
				collider->SetCollisionAttribute(kCollisionAttributeObstacles);
				collider->SetCollisionMask(~kCollisionAttributeObstacles);
			}
			// 攻撃壁
			if (objectData.maskType == LoaderConfig::Collider::Mask::attack) {
				collider->SetCollisionPrimitive(kCollisionOBB);
				collider->SetCollisionAttribute(kCollisionAttributeEnemy);
				collider->SetCollisionMask(~kCollisionAttributeEnemy);
			}

			collider->SetIsActive(true);

			colliders.push_back(collider);
		}
	}
}

void Loader::LoadAllConeJsonFile(const std::string kDefaultBaseDirectory, const std::string fileName, const std::string sceneType, std::vector<Object3d*>& objects, std::vector<Collider*>& colliders, Camera* camera)
{
	// フルパスを生成
	const std::string fullpath = kDefaultBaseDirectory + "/" + fileName + ".json";

	// ファイルストリームを作成
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// JSONデータを格納する変数
	nlohmann::json deserialized;

	// ファイルからJSONデータを読み込む
	file >> deserialized;

	// JSONデータの形式をチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("scenes"));

	// シーンを探索して目的のシーンを見つける
	nlohmann::json selectedScene;
	bool sceneFound = false;

	for (auto& scene : deserialized["scenes"]) {
		if (scene.contains("type") && scene["type"] == sceneType) {
			selectedScene = scene;
			sceneFound = true;
			break;
		}
	}

	// 指定されたシーンが見つからない場合はエラー
	assert(sceneFound);

	// レベルデータを格納するインスタンスを生成
	LevelData* levelData = new LevelData();

	// シーン内のオブジェクトを処理
	for (auto& object : selectedScene["objects"]) {
		assert(object.contains("type"));

		// オブジェクトの種別を取得
		std::string type = object["type"].get<std::string>();

		// MESHオブジェクトの場合
		if (type == "MESH") {
			levelData->objects.emplace_back(LevelData::ObjectData{});
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				objectData.filename = object["file_name"].get<std::string>();
			}

			// トランスフォームのパラメータを読み込む
			auto& transform = object["transform"];
			objectData.transform.translate.x = (float)transform["translation"][0];
			objectData.transform.translate.y = (float)transform["translation"][2];
			objectData.transform.translate.z = (float)transform["translation"][1];
			objectData.transform.rotate.x = -(float)transform["rotation"][0];
			objectData.transform.rotate.y = -(float)transform["rotation"][2];
			objectData.transform.rotate.z = -(float)transform["rotation"][1];
			// スケーリング
			objectData.transform.scale.x = (float)transform["scaling"][0];
			objectData.transform.scale.y = (float)transform["scaling"][2];
			objectData.transform.scale.z = (float)transform["scaling"][1];

			// 当たり判定
			if (object.contains("collider")) {
				auto& colliderData = object["collider"];

				// 中心座標
				Vector3 pos = {
					(float)colliderData["center"][0],
					(float)colliderData["center"][2],
					(float)colliderData["center"][1]
				};
				objectData.colliderCenterPos = pos;
				// 大きさ
				Vector3 size = {
					(float)colliderData["size"][0],
					(float)colliderData["size"][2],
					(float)colliderData["size"][1]
				};
				objectData.colliderSize = size;

				// 当たり判定のマスク
				objectData.maskType = colliderData["type"];

				objectData.isCollision = true;
			}
			objectData.isCollision = false;

			// モデルをロード
			ModelManager::GetInstance()->LoadModel("Resources/game/", objectData.filename + ".obj");
		}

		// CAMERAオブジェクトの場合
		if (type.compare("CAMERA") == 0) {
			// トランスフォームのパラメータを読み込む
			nlohmann::json& transform = object["transform"];

			// 平行移動
			Vector3 translate;
			translate.x = (float)transform["translation"][0];
			translate.y = (float)transform["translation"][2];
			translate.z = (float)transform["translation"][1];

			// 回転角度
			Vector3 rotate;
			rotate.x = -((float)transform["rotation"][0] - 3.141592f / 2.0f);
			rotate.y = -(float)transform["rotation"][2];
			rotate.z = -(float)transform["rotation"][1];

			// スケーリング (必要であれば)
			Vector3 scale;
			scale.x = (float)transform["scaling"][0];
			scale.y = (float)transform["scaling"][2];
			scale.z = (float)transform["scaling"][1];

			// カメラの位置と回転を設定
			camera->SetTranslate(translate);
			camera->SetRotate(rotate);

			// スケーリングを使う場合の処理 (必要に応じて追加)
			// camera->SetScale(scale); // SetScale が必要なら実装してください
		}


		// LIGHTオブジェクトなど他の種別に対応する場合、ここに追加
	}

	// オブジェクトを生成し、配置する
	for (auto& objectData : levelData->objects) {
		Object3d* newObject3d = new Object3d();
		newObject3d->Init();
		newObject3d->SetModel(objectData.filename + ".obj");
		newObject3d->SetTransform(objectData.transform);
		objects.push_back(newObject3d);

		// 当たり判定をペアレント
		if (objectData.isCollision) {
			Collider* collider = new Collider();
			collider->worldTransform.parent_ = &newObject3d->worldTransform_;
			collider->SetOBBCenterPos(objectData.colliderCenterPos);
			collider->SetOBBLength(objectData.colliderSize);
			// 壁
			if (objectData.maskType == LoaderConfig::Collider::Mask::wall) {
				collider->SetCollisionPrimitive(kCollisionOBB);
				collider->SetCollisionAttribute(kCollisionAttributeObstacles);
				collider->SetCollisionMask(~kCollisionAttributeObstacles);
			}
			// 攻撃壁
			if (objectData.maskType == LoaderConfig::Collider::Mask::attack) {
				collider->SetCollisionPrimitive(kCollisionOBB);
				collider->SetCollisionAttribute(kCollisionAttributeEnemy);
				collider->SetCollisionMask(~kCollisionAttributeEnemy);
			}

			collider->SetIsActive(true);

			colliders.push_back(collider);
		}
	}
}
void Loader::LoadAllStarJsonFile(const std::string kDefaultBaseDirectory, const std::string fileName, const std::string sceneType, std::vector<Object3d*>& objects, std::vector<Collider*>& colliders)
{
	// フルパスを生成
	const std::string fullpath = kDefaultBaseDirectory + "/" + fileName + ".json";

	// ファイルストリームを作成
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// JSONデータを格納する変数
	nlohmann::json deserialized;

	// ファイルからJSONデータを読み込む
	file >> deserialized;

	// JSONデータの形式をチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("scenes"));

	// シーンを探索して目的のシーンを見つける
	nlohmann::json selectedScene;
	bool sceneFound = false;

	for (auto& scene : deserialized["scenes"]) {
		if (scene.contains("type") && scene["type"] == sceneType) {
			selectedScene = scene;
			sceneFound = true;
			break;
		}
	}

	// 指定されたシーンが見つからない場合はエラー
	assert(sceneFound);

	// レベルデータを格納するインスタンスを生成
	LevelData* levelData = new LevelData();

	// シーン内のオブジェクトを処理
	for (auto& object : selectedScene["objects"]) {
		assert(object.contains("type"));

		// オブジェクトの種別を取得
		std::string type = object["type"].get<std::string>();

		// MESHオブジェクトの場合
		if (type == "MESH") {
			levelData->objects.emplace_back(LevelData::ObjectData{});
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				objectData.filename = object["file_name"].get<std::string>();
			}

			// トランスフォームのパラメータを読み込む
			auto& transform = object["transform"];
			objectData.transform.translate.x = (float)transform["translation"][0];
			objectData.transform.translate.y = (float)transform["translation"][2];
			objectData.transform.translate.z = (float)transform["translation"][1];
			objectData.transform.rotate.x = -(float)transform["rotation"][0];
			objectData.transform.rotate.y = -(float)transform["rotation"][2];
			objectData.transform.rotate.z = -(float)transform["rotation"][1];
			// スケーリング
			objectData.transform.scale.x = (float)transform["scaling"][0];
			objectData.transform.scale.y = (float)transform["scaling"][2];
			objectData.transform.scale.z = (float)transform["scaling"][1];

			// 当たり判定
			if (object.contains("collider")) {
				auto& colliderData = object["collider"];

				// 中心座標
				Vector3 pos = {
					(float)colliderData["center"][0],
					(float)colliderData["center"][2],
					(float)colliderData["center"][1]
				};
				objectData.colliderCenterPos = pos;
				// 大きさ
				Vector3 size = {
					(float)colliderData["size"][0],
					(float)colliderData["size"][2],
					(float)colliderData["size"][1]
				};
				objectData.colliderSize = size;

				// 当たり判定のマスク
				objectData.maskType = colliderData["type"];

				objectData.isCollision = true;
			}
			objectData.isCollision = false;

			// モデルをロード
			ModelManager::GetInstance()->LoadModel("Resources/game/", objectData.filename + ".obj");
		}

		// LIGHTオブジェクトなど他の種別に対応する場合、ここに追加
	}

	// オブジェクトを生成し、配置する
	for (auto& objectData : levelData->objects) {
		Object3d* newObject3d = new Object3d();
		newObject3d->Init();
		newObject3d->SetModel(objectData.filename + ".obj");
		newObject3d->SetTransform(objectData.transform);
		objects.push_back(newObject3d);

		// 当たり判定をペアレント
		if (objectData.isCollision) {
			Collider* collider = new Collider();
			collider->worldTransform.parent_ = &newObject3d->worldTransform_;
			collider->SetOBBCenterPos(objectData.colliderCenterPos);
			collider->SetOBBLength(objectData.colliderSize);
			// 壁
			if (objectData.maskType == LoaderConfig::Collider::Mask::wall) {
				collider->SetCollisionPrimitive(kCollisionOBB);
				collider->SetCollisionAttribute(kCollisionAttributeObstacles);
				collider->SetCollisionMask(~kCollisionAttributeObstacles);
			}
			// 攻撃壁
			if (objectData.maskType == LoaderConfig::Collider::Mask::attack) {
				collider->SetCollisionPrimitive(kCollisionOBB);
				collider->SetCollisionAttribute(kCollisionAttributeEnemy);
				collider->SetCollisionMask(~kCollisionAttributeEnemy);
			}

			collider->SetIsActive(true);

			colliders.push_back(collider);
		}
	}
}
void Loader::LoadAllItemJsonFile(const std::string kDefaultBaseDirectory, const std::string fileName, const std::string sceneType, std::vector<Object3d*>& objects, std::vector<Collider*>& colliders) {
	// フルパスを生成
	const std::string fullpath = kDefaultBaseDirectory + "/" + fileName + ".json";

	// ファイルストリームを作成
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// JSONデータを格納する変数
	nlohmann::json deserialized;

	// ファイルからJSONデータを読み込む
	file >> deserialized;

	// JSONデータの形式をチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("scenes"));

	// シーンを探索して目的のシーンを見つける
	nlohmann::json selectedScene;
	bool sceneFound = false;

	for (auto& scene : deserialized["scenes"]) {
		if (scene.contains("type") && scene["type"] == sceneType) {
			selectedScene = scene;
			sceneFound = true;
			break;
		}
	}

	// 指定されたシーンが見つからない場合はエラー
	assert(sceneFound);

	// レベルデータを格納するインスタンスを生成
	LevelData* levelData = new LevelData();

	// シーン内のオブジェクトを処理
	for (auto& object : selectedScene["objects"]) {
		assert(object.contains("type"));

		// オブジェクトの種別を取得
		std::string type = object["type"].get<std::string>();

		// MESHオブジェクトの場合
		if (type == "MESH") {
			levelData->objects.emplace_back(LevelData::ObjectData{});
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				objectData.filename = object["file_name"].get<std::string>();
			}

			// トランスフォームのパラメータを読み込む
			auto& transform = object["transform"];
			objectData.transform.translate.x = (float)transform["translation"][0];
			objectData.transform.translate.y = (float)transform["translation"][2];
			objectData.transform.translate.z = (float)transform["translation"][1];
			objectData.transform.rotate.x = -(float)transform["rotation"][0];
			objectData.transform.rotate.y = -(float)transform["rotation"][2];
			objectData.transform.rotate.z = -(float)transform["rotation"][1];
			// スケーリング
			objectData.transform.scale.x = (float)transform["scaling"][0];
			objectData.transform.scale.y = (float)transform["scaling"][2];
			objectData.transform.scale.z = (float)transform["scaling"][1];

			if (object.contains("collider")) {
				auto& colliderData = object["collider"];

				// 中心座標
				Vector3 pos = {
					(float)colliderData["center"][0],
					(float)colliderData["center"][2],
					(float)colliderData["center"][1]
				};
				objectData.colliderCenterPos = pos;
				// 大きさ
				Vector3 size = {
					(float)colliderData["size"][0],
					(float)colliderData["size"][2],
					(float)colliderData["size"][1]
				};
				objectData.colliderSize = size;

				// 当たり判定のマスク
				objectData.maskType = colliderData["type"];

				objectData.isCollision = true;
			}
			objectData.isCollision = false;

			// モデルをロード
			ModelManager::GetInstance()->LoadModel("Resources/game/", objectData.filename + ".obj");
		}

		// LIGHTオブジェクトなど他の種別に対応する場合、ここに追加
	}

	// オブジェクトを生成し、配置する
	for (auto& objectData : levelData->objects) {
		// オブジェクト生成
		Object3d* newObject3d = new Object3d();
		newObject3d->Init();
		newObject3d->SetModel(objectData.filename + ".obj");
		newObject3d->SetTransform(objectData.transform);
		objects.push_back(newObject3d);

		// 当たり判定をペアレント
		if (objectData.isCollision) {
			Collider* collider = new Collider();
			collider->worldTransform.parent_ = &newObject3d->worldTransform_;
			collider->SetOBBCenterPos(objectData.colliderCenterPos);
			collider->SetOBBLength(objectData.colliderSize);
			// 壁
			if (objectData.maskType == LoaderConfig::Collider::Mask::wall) {
				collider->SetCollisionPrimitive(kCollisionOBB);
				collider->SetCollisionAttribute(kCollisionAttributeObstacles);
				collider->SetCollisionMask(~kCollisionAttributeObstacles);
			}
			// 攻撃壁
			if (objectData.maskType == LoaderConfig::Collider::Mask::attack) {
				collider->SetCollisionPrimitive(kCollisionOBB);
				collider->SetCollisionAttribute(kCollisionAttributeEnemy);
				collider->SetCollisionMask(~kCollisionAttributeEnemy);
			}

			collider->SetIsActive(true);

			colliders.push_back(collider);
		}
	}
}

/// <summary>
/// JSONファイルを読み込み、番号付きのオブジェクトを初期化する関数
/// </summary>
/// <param name="kDefaultBaseDirectory">デフォルトのベースディレクトリ</param>
/// <param name="fileName">読み込むJSONファイル名（拡張子なし）</param>
/// <param name="objects">生成したオブジェクトを格納するベクター</param>
void Loader::LoadJsonFileNumber(const std::string kDefaultBaseDirectory, const std::string fileName, std::vector<Object3d*>& objects, std::vector<Collider*>& colliders) {
	// フルパスを生成
	const std::string fullpath = kDefaultBaseDirectory + "/" + fileName + ".json";

	// ファイルストリームを作成
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// JSONデータを格納する変数
	nlohmann::json deserialized;

	// ファイルからJSONデータを読み込む
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"フィールドを取得
	std::string name = deserialized["name"].get<std::string>();
	// "scene"であることを確認
	assert(name.compare("scene") == 0);

	// レベルデータを格納するインスタンスを生成
	LevelData* levelData = new LevelData();

	// "objects"配列内の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		// オブジェクトの種別を取得
		std::string type = object["type"].get<std::string>();

		// MESHオブジェクトの場合
		if (type.compare("MESH") == 0) {
			// オブジェクトデータを追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			// 追加したオブジェクトデータへの参照を取得
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				// ファイル名を取得
				objectData.filename = object["file_name"].get<std::string>();
			}

			// トランスフォームのパラメータを読み込む
			nlohmann::json& transform = object["transform"];
			// 平行移動
			objectData.transform.translate.x = (float)transform["translation"][0];
			objectData.transform.translate.y = (float)transform["translation"][2];
			objectData.transform.translate.z = (float)transform["translation"][1];
			// 回転角度
			objectData.transform.rotate.x = -(float)transform["rotation"][0];
			objectData.transform.rotate.y = -(float)transform["rotation"][2];
			objectData.transform.rotate.z = -(float)transform["rotation"][1];
			// スケーリング
			objectData.transform.scale.x = (float)transform["scaling"][0];
			objectData.transform.scale.y = (float)transform["scaling"][2];
			objectData.transform.scale.z = (float)transform["scaling"][1];

			if (object.contains("collider")) {
				auto& colliderData = object["collider"];

				// 中心座標
				Vector3 pos = {
					(float)colliderData["center"][0],
					(float)colliderData["center"][2],
					(float)colliderData["center"][1]
				};
				objectData.colliderCenterPos = pos;
				// 大きさ
				Vector3 size = {
					(float)colliderData["size"][0],
					(float)colliderData["size"][2],
					(float)colliderData["size"][1]
				};
				objectData.colliderSize = size;

				// 当たり判定のマスク
				objectData.maskType = colliderData["type"];

				objectData.isCollision = true;
			}
			objectData.isCollision = false;
		}

		// 子オブジェクトが存在する場合（未実装）
		if (object.contains("children")) {
			// TODO: 再帰的に子オブジェクトを処理する
		}

		// CAMERAオブジェクトの場合
		if (type.compare("CAMERA") == 0) {
			// オブジェクトデータを追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			// 追加したオブジェクトデータへの参照を取得
			LevelData::ObjectData& objectData = levelData->objects.back();

			// トランスフォームのパラメータを読み込む
			nlohmann::json& transform = object["transform"];
			// 平行移動
			objectData.transform.translate.x = (float)transform["translation"][0];
			objectData.transform.translate.y = (float)transform["translation"][2];
			objectData.transform.translate.z = (float)transform["translation"][1];
			// 回転角度
			objectData.transform.rotate.x = -((float)transform["rotation"][0] - 3.141592f / 2.0f);
			objectData.transform.rotate.y = -(float)transform["rotation"][2];
			objectData.transform.rotate.z = -(float)transform["rotation"][1];
			// スケーリング
			objectData.transform.scale.x = (float)transform["scaling"][0];
			objectData.transform.scale.y = (float)transform["scaling"][2];
			objectData.transform.scale.z = (float)transform["scaling"][1];
		}
	}

	// オブジェクトを生成し、配置する
	for (auto& objectData : levelData->objects) {
		// 3Dオブジェクトを生成
		Object3d* newObject3d = new Object3d();
		newObject3d->Init();
		newObject3d->SetModel(objectData.filename + ".obj");
		newObject3d->SetTransform(objectData.transform);
		// 配列に追加
		objects.push_back(newObject3d);

		// 当たり判定をペアレント
		if (objectData.isCollision) {
			Collider* collider = new Collider();
			collider->worldTransform.parent_ = &newObject3d->worldTransform_;
			collider->SetOBBCenterPos(objectData.colliderCenterPos);
			collider->SetOBBLength(objectData.colliderSize);
			// 壁
			if (objectData.maskType == LoaderConfig::Collider::Mask::wall) {
				collider->SetCollisionPrimitive(kCollisionOBB);
				collider->SetCollisionAttribute(kCollisionAttributeObstacles);
				collider->SetCollisionMask(~kCollisionAttributeObstacles);
			}
			// 攻撃壁
			if (objectData.maskType == LoaderConfig::Collider::Mask::attack) {
				collider->SetCollisionPrimitive(kCollisionOBB);
				collider->SetCollisionAttribute(kCollisionAttributeEnemy);
				collider->SetCollisionMask(~kCollisionAttributeEnemy);
			}

			collider->SetIsActive(true);

			colliders.push_back(collider);
		}
	}
}

/// <summary>
/// JSONファイルを読み込み、テキストオブジェクトを初期化する関数
/// </summary>
/// <param name="kDefaultBaseDirectory">デフォルトのベースディレクトリ</param>
/// <param name="fileName">読み込むJSONファイル名（拡張子なし）</param>
/// <param name="objects">生成したオブジェクトを格納するベクター</param>
void Loader::LoadJsonFileText(const std::string kDefaultBaseDirectory, const std::string fileName, std::vector<Object3d*>& objects, std::vector<Collider*>& colliders) {
	// フルパスを生成
	const std::string fullpath = kDefaultBaseDirectory + "/" + fileName + ".json";

	// ファイルストリームを作成
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// JSONデータを格納する変数
	nlohmann::json deserialized;

	// ファイルからJSONデータを読み込む
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"フィールドを取得
	std::string name = deserialized["name"].get<std::string>();
	// "scene"であることを確認
	assert(name.compare("scene") == 0);

	// レベルデータを格納するインスタンスを生成
	LevelData* levelData = new LevelData();

	// "objects"配列内の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		// オブジェクトの種別を取得
		std::string type = object["type"].get<std::string>();

		// MESHオブジェクトの場合
		if (type.compare("MESH") == 0) {
			// オブジェクトデータを追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			// 追加したオブジェクトデータへの参照を取得
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				// ファイル名を取得
				objectData.filename = object["file_name"].get<std::string>();
			}

			// トランスフォームのパラメータを読み込む
			nlohmann::json& transform = object["transform"];
			// 平行移動
			objectData.transform.translate.x = (float)transform["translation"][0];
			objectData.transform.translate.y = (float)transform["translation"][2];
			objectData.transform.translate.z = (float)transform["translation"][1];
			// 回転角度
			objectData.transform.rotate.x = -(float)transform["rotation"][0];
			objectData.transform.rotate.y = -(float)transform["rotation"][2];
			objectData.transform.rotate.z = -(float)transform["rotation"][1];
			// スケーリング
			objectData.transform.scale.x = (float)transform["scaling"][0];
			objectData.transform.scale.y = (float)transform["scaling"][2];
			objectData.transform.scale.z = (float)transform["scaling"][1];

			// 当たり判定
			if (object.contains("collider")) {
				auto& colliderData = object["collider"];

				// 中心座標
				Vector3 pos = {
					(float)colliderData["center"][0],
					(float)colliderData["center"][2],
					(float)colliderData["center"][1]
				};
				objectData.colliderCenterPos = pos;
				// 大きさ
				Vector3 size = {
					(float)colliderData["size"][0],
					(float)colliderData["size"][2],
					(float)colliderData["size"][1]
				};
				objectData.colliderSize = size;

				// 当たり判定のマスク
				objectData.maskType = colliderData["type"];

				objectData.isCollision = true;
			}
			objectData.isCollision = false;

			// テキスト用モデルをロード
			ModelManager::GetInstance()->LoadModel("Resources/game/Text/", objectData.filename + ".obj");
		}

		// 子オブジェクトが存在する場合（未実装）
		if (object.contains("children")) {
			// TODO: 再帰的に子オブジェクトを処理する
		}

		// CAMERAオブジェクトの場合
		if (type.compare("CAMERA") == 0) {
			// オブジェクトデータを追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			// 追加したオブジェクトデータへの参照を取得
			LevelData::ObjectData& objectData = levelData->objects.back();

			// トランスフォームのパラメータを読み込む
			nlohmann::json& transform = object["transform"];
			// 平行移動
			objectData.transform.translate.x = (float)transform["translation"][0];
			objectData.transform.translate.y = (float)transform["translation"][2];
			objectData.transform.translate.z = (float)transform["translation"][1];
			// 回転角度
			objectData.transform.rotate.x = -((float)transform["rotation"][0] - 3.141592f / 2.0f);
			objectData.transform.rotate.y = -(float)transform["rotation"][2];
			objectData.transform.rotate.z = -(float)transform["rotation"][1];
			// スケーリング
			objectData.transform.scale.x = (float)transform["scaling"][0];
			objectData.transform.scale.y = (float)transform["scaling"][2];
			objectData.transform.scale.z = (float)transform["scaling"][1];
		}
	}

	// オブジェクトを生成し、配置する
	for (auto& objectData : levelData->objects) {
		// 3Dオブジェクトを生成
		Object3d* newObject3d = new Object3d();
		newObject3d->Init();
		newObject3d->SetModel(objectData.filename + ".obj");
		newObject3d->SetTransform(objectData.transform);
		// 配列に追加
		objects.push_back(newObject3d);

		// 当たり判定をペアレント
		if (objectData.isCollision) {
			Collider* collider = new Collider();
			collider->worldTransform.parent_ = &newObject3d->worldTransform_;
			collider->SetOBBCenterPos(objectData.colliderCenterPos);
			collider->SetOBBLength(objectData.colliderSize);
			// 壁
			if (objectData.maskType == LoaderConfig::Collider::Mask::wall) {
				collider->SetCollisionPrimitive(kCollisionOBB);
				collider->SetCollisionAttribute(kCollisionAttributeObstacles);
				collider->SetCollisionMask(~kCollisionAttributeObstacles);
			}
			// 攻撃壁
			if (objectData.maskType == LoaderConfig::Collider::Mask::attack) {
				collider->SetCollisionPrimitive(kCollisionOBB);
				collider->SetCollisionAttribute(kCollisionAttributeEnemy);
				collider->SetCollisionMask(~kCollisionAttributeEnemy);
			}

			collider->SetIsActive(true);

			colliders.push_back(collider);
		}
	}
}
