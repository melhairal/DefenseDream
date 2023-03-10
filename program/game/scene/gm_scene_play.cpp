#include "../gm_manager.h"
#include "../gm_camera.h"
#include "../model/gm_anim_sprite3d.h"
#include "gm_scene_play.h"
#include "gm_scene_result.h"
#include "../gm_object.h"
#include "../gm_ui.h"
#include "../gm_stage.h"
#include "../object/gm_object_player.h"
#include "../object/gm_object_enemy.h"
#include "../object/gm_object_combo.h"
#include "../object/gm_object_other.h"

tnl::Quaternion	fix_rot;

ScenePlay::~ScenePlay() {
	delete camera_;
	delete field_;
	delete ui_;
	delete stage_;

	for (auto object : objects_) delete object;
}

void ScenePlay::initialzie() {
	//カメラ生成
	camera_ = new GmCamera();
	camera_->pos_ = { 0, 150, -300 };
	camera_->far_ = 2000.0f;
	//プレイヤー生成
	player_ = objects_.emplace_back(new Player(this));
	//拠点を生成
	home_ = objects_.emplace_back(new Home(this));
	//ステージを生成
	field_ = dxe::Mesh::CreateDisk(FIELD_R_ * 1.1f);
	field_->setTexture(dxe::Texture::CreateFromFile("graphics/ground.jpg"));
	field_->rot_q_ *= tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	field_->pos_ = { 0, FIELD_H_, 0 };
	//雲を生成(フィールド外壁)
	for (int i = 0; i < CLOUD_DENSITY_; ++i) {
		double th = ((double)i / (double)CLOUD_DENSITY_) * 3.14f * 2;
		float x = FIELD_R_ * cos(th);
		float y = FIELD_R_ * sin(th);
		objects_.emplace_back(new Cloud(this))->mesh_->pos_ = {x, 0, y};
	}
	//背景を生成
	backGround_ = LoadGraph("graphics/sky.jpg");
	//UIを生成
	ui_ = new Ui(this);
	//ステージを生成
	stage_ = new Stage1(this);
}

void ScenePlay::update(float delta_time)
{
	GameManager* mgr = GameManager::GetInstance();

	//------------------------------------------------------------------
	//
	// ステージ制御
	//

	stage_->update(delta_time);
	int home_hp_ = std::clamp(home_hp_, 0, home_hp_max_);

	//------------------------------------------------------------------
	//
	// オブジェクト制御
	//

	updateObject(delta_time);

	//------------------------------------------------------------------
	//
	// カメラ制御
	//
	
	updateCamera(delta_time);

	//------------------------------------------------------------------
	//
	// その他(デバッグ等)
	//

	//敵生成テスト用
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_Z)) {
		objects_.emplace_back(new EnemySprite(this, { 0,0,0 }));
	}

	//シーン切り替え
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		mgr->chengeScene(new SceneResult());
	}
}

void ScenePlay::render()
{
	camera_->update();

	//ステージの描画
	DrawGridGround(camera_, 50, 20); //グリッドの作成
	DrawRotaGraph(DXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT / 2, 2.5f, 0, backGround_, true); //背景の描画
	field_->render(camera_); //床の描画

	//全オブジェクトの描画
	for (auto object : objects_) {
		if (object->mesh_ != nullptr) object->mesh_->render(camera_);
		if (object->sprite_ != nullptr) object->sprite_->render(camera_);
	}

	//UIの描画
	ui_->render();


	//------------------------------------------------------------------
	//
	// その他(デバッグ等)
	//

	DrawStringEx(100, 100, 0, "%d", stage_->enemyStock_);
}

void ScenePlay::updateObject(float delta_time) {
	//全てのオブジェクトのアップデート
	for (auto object : objects_) {
		if (object->moving_)	object->update(delta_time);
	}

	//オブジェクトの生存フラグがfalseになったらデリート
	auto it_object = objects_.begin();
	while (it_object != objects_.end()) {
		if (!(*it_object)->alive_) {
			delete (*it_object);
			it_object = objects_.erase(it_object);
			continue;
		}
		it_object++;
	}

	// カメラに近い順にソート(近いオブジェクトから描画するため)
	objects_.sort([&](const GameObj* l, const GameObj* r) {
		float ld = 0;
		float rd = 0;
		if (l->mesh_ != nullptr) ld = (camera_->pos_ - l->mesh_->pos_).length();
		if (l->sprite_ != nullptr) ld = (camera_->pos_ - l->sprite_->pos_).length();
		if (r->mesh_ != nullptr) rd = (camera_->pos_ - r->mesh_->pos_).length();
		if (r->sprite_ != nullptr) rd = (camera_->pos_ - r->sprite_->pos_).length();
		return ld > rd;
		});
}

void ScenePlay::updateCamera(float delta_time) {
	//カメラ回転
	tnl::Vector3 rot[2] = {
		{ 0, tnl::ToRadian(CAM_ROT_SPEED_), 0 },
		{ 0, -tnl::ToRadian(CAM_ROT_SPEED_), 0 }
	};
	tnl::Input::RunIndexKeyDown([&](uint32_t idx) {
		camera_->free_look_angle_xy_ += rot[idx];
		}, eKeys::KB_E, eKeys::KB_Q);

	//カメラのズーム
	int wheel = GetMouseWheelRotVol();
	if (wheel < 0) {
		camera_->target_distance_ += CAM_ZOOM_SPEED_;
	}
	if (wheel > 0) {
		camera_->target_distance_ -= CAM_ZOOM_SPEED_;
	}

	//カメラズームの上限・下限
	camera_->target_distance_ = std::clamp(camera_->target_distance_, CAM_ZOOM_MIN_, CAM_ZOOM_MAX_);

	camera_->target_ = player_->sprite_->pos_;
}