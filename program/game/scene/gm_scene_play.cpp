#include "../gm_manager.h"
#include "../gm_camera.h"
#include "../model/gm_anim_sprite3d.h"
#include "gm_scene_play.h"
#include "gm_scene_result.h"

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

	//------------------------------------------------------------------
	//
	// オブジェクト制御
	//

	//全てのオブジェクトのアップデート
	for (auto object : objects_) {
		if(object->moving_)	object->update(delta_time);
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

	//------------------------------------------------------------------
	//
	// カメラ制御
	//

	//カメラ回転
	tnl::Vector3 rot[2] = {
		{ 0, tnl::ToRadian(1.0f), 0 },
		{ 0, -tnl::ToRadian(1.0f), 0 }
	};
	tnl::Input::RunIndexKeyDown([&](uint32_t idx) {
		camera_->free_look_angle_xy_ += rot[idx];
		}, eKeys::KB_E, eKeys::KB_Q);
	//カメラのズーム
	int wheel = GetMouseWheelRotVol();
	if (wheel < 0) {
		camera_->target_distance_ += 30.0f;
	}
	if (wheel > 0) {
		camera_->target_distance_ -= 30.0f;
	}
	//カメラズームの上限
	if (camera_->target_distance_ < 200) {
		camera_->target_distance_ = 200;
	}
	if (camera_->target_distance_ > 400) {
		camera_->target_distance_ = 400;
	}
	camera_->target_ = player_->sprite_->pos_;

	//------------------------------------------------------------------
	//
	// その他(デバッグ等)
	//

	//雲(壁)/拠点の衝突判定
	for (auto object : objects_) {
		if (object->tag_ != GameObj::eCloud && object->tag_ != GameObj::eHome) continue;
		if (tnl::IsIntersectAABB(player_->sprite_->pos_, { player_->size_,player_->size_,player_->size_ }, object->mesh_->pos_, { object->size_,object->size_,object->size_ })) {
			tnl::GetCorrectPositionIntersectAABB(player_->prev_pos_, { player_->size_,player_->size_,player_->size_ }, object->mesh_->pos_, { object->size_,object->size_,object->size_ }, player_->sprite_->pos_);
		}
	}

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
	DrawGridGround(camera_, 50, 20);
	DrawRotaGraph(DXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT / 2, 2.5f, 0, backGround_, true);
	field_->render(camera_);
	//全オブジェクトの描画
	for (auto object : objects_) {
		if (object->mesh_ != nullptr) object->mesh_->render(camera_);
		if (object->sprite_ != nullptr) object->sprite_->render(camera_);
	}

	ui_->render();

	DrawStringEx(100, 100, 0, "%d", stage_->enemyStock_);
}

//------------------------------------------------------------------
//
// ゲームオブジェクト
//

Player::Player(ScenePlay* scene) {
	tag_ = GameObj::ePlayer;
	scene_ = scene;
	size_ = SIZE_;
	//プレイヤー生成
	sprite_ = new AnimSprite3D(scene_->camera_);
	sprite_->regist(SPRITE_W_, SPRITE_H_, "walk_front", "graphics/c1_anim_up.png", tnl::SeekUnit::ePlayMode::REPEAT, 1.0f, 4, SPRITE_H_, 0);
	sprite_->regist(SPRITE_W_, SPRITE_H_, "walk_back", "graphics/c1_anim_down.png", tnl::SeekUnit::ePlayMode::REPEAT, 1.0f, 4, SPRITE_H_, 0);
	sprite_->regist(SPRITE_W_, SPRITE_H_, "walk_left", "graphics/c1_anim_left.png", tnl::SeekUnit::ePlayMode::REPEAT, 1.0f, 4, SPRITE_H_, 0);
	sprite_->regist(SPRITE_W_, SPRITE_H_, "walk_right", "graphics/c1_anim_right.png", tnl::SeekUnit::ePlayMode::REPEAT, 1.0f, 4, SPRITE_H_, 0);
	sprite_->setCurrentAnim("walk_front");
	sprite_->pos_ = { 50,0,0 };
	//プレイヤーサークル生成
	mesh_ = dxe::Mesh::CreatePlane({ SPRITE_W_ * 2, SPRITE_W_ * 2, 0 });
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	mesh_->pos_ = { sprite_->pos_.x, sprite_->pos_.y - SPRITE_H_ / 2, sprite_->pos_.z };
	mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/circle.png"));
}

void Player::update(float delta_time) {
	//------------------------------------------------------------------
	//
	// 移動制御
	//

	int t = tnl::GetXzRegionPointAndOBB(
		scene_->camera_->pos_
		, sprite_->pos_
		, { SPRITE_W_, SPRITE_H_, SPRITE_W_ }
	, sprite_->rot_);

	std::string anim_names[4] = {
		"walk_back", "walk_right", "walk_front", "walk_left"
	};
	sprite_->setCurrentAnim(anim_names[t]);
	tnl::Vector3 move_v = { 0,0,0 };
	tnl::Vector3 dir[4] = {
		scene_->camera_->front().xz(),
		scene_->camera_->right().xz(),
		scene_->camera_->back().xz(),
		scene_->camera_->left().xz(),
	};
	tnl::Input::RunIndexKeyDown([&](uint32_t idx) {
		move_v += dir[idx];
		}, eKeys::KB_W, eKeys::KB_D, eKeys::KB_S, eKeys::KB_A);

	int speed = SPEED_;
	if (tnl::Input::IsKeyDown(eKeys::KB_LSHIFT) && scene_->magic_ > 0) {
		scene_->magic_--;
		speed = SPRINT_;
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_W, eKeys::KB_D, eKeys::KB_S, eKeys::KB_A)) {
		move_v.normalize();
		prev_pos_ = sprite_->pos_;
		sprite_->pos_ += move_v * speed;
	}
	mesh_->pos_ = { sprite_->pos_.x, sprite_->pos_.y - SPRITE_H_ / 2, sprite_->pos_.z };
	//------------------------------------------------------------------
	//
	// マウスポインタ制御
	//

	tnl::Vector3 msv = tnl::Input::GetMousePosition();
	tnl::Vector3 ray = tnl::Vector3::CreateScreenRay(
		msv.x
		, msv.y
		, scene_->camera_->screen_w_
		, scene_->camera_->screen_h_
		, scene_->camera_->view_
		, scene_->camera_->proj_);

	tnl::Vector3 hit;
	if (tnl::IsIntersectLinePlane(scene_->camera_->pos_, scene_->camera_->pos_ + (ray * 10000.0f), { 0, scene_->FIELD_H_, 0 }, { 0, 1, 0 }, &hit)) {
		//マウスカーソルの方向を向く
		sprite_->rot_.slerp(tnl::Quaternion::LookAtAxisY(sprite_->pos_, hit), 0.3f);
		mesh_->rot_q_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90)) * sprite_->rot_;
	}
	looking_ = hit - sprite_->pos_;
	looking_.y = 0;
	looking_.normalize();
	//------------------------------------------------------------------
	//
	// プレイヤー操作制御
	//
	
	//左クリックで弾を発射
	/*
	if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_LEFT)) {
		ballet_interval_ = 0;
		scene_->objects_.emplace_back(new Bullet(sprite_->pos_, looking_));
	}
	if (tnl::Input::IsMouseDown(eMouse::LEFT)) {
		ballet_interval_++;
		if (ballet_interval_ > 10) {
			ballet_interval_ = 0;
			scene_->objects_.emplace_back(new Bullet(sprite_->pos_, looking_));
		}
	}
	*/

	//コンボ（遠隔）
	if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_RIGHT) && comboM_counter_ == 0) {
		switch (combo_counter_) {
		case 0:
			scene_->objects_.emplace_back(new Combo1(scene_));
			combo_counter_++;
			break;
		case 1:
			if (combo_timer_ > COMBO_INTERVAL_) {
				scene_->objects_.emplace_back(new Combo2(scene_));
				combo_timer_ = 0;
				combo_counter_++;
			}
			break;
		case 2:
			if (combo_timer_ > COMBO_INTERVAL_) {
				scene_->objects_.emplace_back(new Combo3(scene_));
				combo_timer_ = 0;
				combo_counter_++;
			}
			break;
		}
	}

	if (combo_counter_ > 0) {
		combo_timer_++;
		if (combo_timer_ > COMBO_RECEPTION_) {
			combo_timer_ = 0;
			combo_counter_ = 0;
		}
	}

	//コンボ（近接）
	if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_LEFT) && combo_counter_ == 0) {
		switch (comboM_counter_) {
		case 0:
			if (tnl::Input::IsKeyDown(eKeys::KB_LSHIFT)) break;
			scene_->objects_.emplace_back(new ComboM1(scene_));
			comboM_counter_++;
			break;
		case 1:
			if (comboM_timer_ > COMBO_INTERVAL_) {
				scene_->objects_.emplace_back(new ComboM2(scene_));
				comboM_timer_ = 0;
				comboM_counter_++;
			}
			break;
		case 2:
			if (comboM_timer_ > COMBO_INTERVAL_) {
				scene_->objects_.emplace_back(new ComboM3(scene_));
				comboM_timer_ = 0;
				comboM_counter_++;
			}
			break;
		case 3:
			if (comboM_timer_ > COMBO_INTERVAL_) {
				scene_->objects_.emplace_back(new ComboM4(scene_));
				comboM_timer_ = 0;
				comboM_counter_++;
			}
		}
	}

	if (comboM_counter_ > 0) {
		comboM_timer_++;
		if (comboM_timer_ > COMBO_RECEPTION_) {
			comboM_timer_ = 0;
			comboM_counter_ = 0;
		}
	}

	//接近攻撃
	if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_LEFT) && tnl::Input::IsKeyDown(eKeys::KB_LSHIFT)
		&& combo_counter_ == 0 && comboM_counter_ == 0) {
		scene_->objects_.emplace_back(new ComboS1(scene_));
		comboM_timer_ = 0;
		comboM_counter_ = 2;
	}

	//魔力の自動回復
	if (scene_->magic_ == scene_->magic_prev_) {
		scene_->magic_timer_++;
		if (scene_->magic_timer_ >= scene_->MAGIC_TIME_) {
			scene_->magic_++;
			scene_->magic_timer_ = scene_->MAGIC_TIME_;
		}
	}
	else {
		scene_->magic_timer_ = 0;
	}
	if (scene_->magic_ >= scene_->magic_max_) scene_->magic_ = scene_->magic_max_;
	scene_->magic_prev_ = scene_->magic_;

	sprite_->update(delta_time);
}

Bullet::Bullet(const tnl::Vector3& pos, const tnl::Vector3& dir) {
	tag_ = GameObj::eBullet;
	size_ = SIZE_;
	mesh_ = dxe::Mesh::CreateSphere(SIZE_);
	mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	mesh_->pos_ = pos;
	looking_ = dir;
}

void Bullet::update(float delta_time)
{
	//飛んでいく処理
	mesh_->pos_ += looking_ * SPEED_;
	distance_ += SPEED_;
	//一定距離進んだら消える
	if (distance_ > RANGE_) alive_ = false;
}

Enemy::Enemy(ScenePlay* scene, const tnl::Vector3& pos) {
	tag_ = GameObj::eEnemy;
	scene_ = scene;
	size_ = SIZE_;
	mesh_ = dxe::Mesh::CreateBox(SIZE_);
	mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/test.jpg"));
	mesh_->pos_ = pos;
};

void Enemy::update(float delta_time)
{
	//プレイヤーを追いかける
	looking_ = scene_->player_->sprite_->pos_ - mesh_->pos_;
	looking_.normalize();
	mesh_->pos_ += looking_ * SPEED_;
	//HPが0になったら消える
	if (hp_ <= 0) {
		hp_ = 0;
		alive_ = false;
	}
}

EnemySprite::EnemySprite(ScenePlay* scene, const tnl::Vector3& pos) {
	tag_ = GameObj::eEnemy;
	scene_ = scene;
	size_ = SIZE_;
	sprite_ = new AnimSprite3D(scene_->camera_);
	sprite_->regist(SPRITE_W_, SPRITE_H_, "walk_front", "graphics/c1_anim_up.png", tnl::SeekUnit::ePlayMode::REPEAT, 1.0f, 4, SPRITE_H_, 0);
	sprite_->regist(SPRITE_W_, SPRITE_H_, "walk_back", "graphics/c1_anim_down.png", tnl::SeekUnit::ePlayMode::REPEAT, 1.0f, 4, SPRITE_H_, 0);
	sprite_->regist(SPRITE_W_, SPRITE_H_, "walk_left", "graphics/c1_anim_left.png", tnl::SeekUnit::ePlayMode::REPEAT, 1.0f, 4, SPRITE_H_, 0);
	sprite_->regist(SPRITE_W_, SPRITE_H_, "walk_right", "graphics/c1_anim_right.png", tnl::SeekUnit::ePlayMode::REPEAT, 1.0f, 4, SPRITE_H_, 0);
	sprite_->setCurrentAnim("walk_front");
	sprite_->pos_ = pos;
	target_ = scene_->home_;
};

void EnemySprite::update(float delta_time) {
	//アニメーション制御
	int t = tnl::GetXzRegionPointAndOBB(
		scene_->camera_->pos_
		, sprite_->pos_
		, { SPRITE_W_, SPRITE_H_, SPRITE_W_ }
	, sprite_->rot_);

	std::string anim_names[4] = {
		"walk_back", "walk_right", "walk_front", "walk_left"
	};
	sprite_->setCurrentAnim(anim_names[t]);
	//ターゲットを追いかける
	if (hp_ != hp_max_) target_ = scene_->player_; //ターゲットの切り替え
	if (target_ == scene_->player_) looking_ = scene_->player_->sprite_->pos_ - sprite_->pos_;
	if (target_ == scene_->home_) looking_ = scene_->home_->mesh_->pos_ - sprite_->pos_;
	looking_.y = 0;
	if (looking_.length() > DIR_) {
		looking_.normalize();
		sprite_->pos_ += looking_ * SPEED_;
		sprite_->rot_.slerp(tnl::Quaternion::LookAtAxisY(sprite_->pos_, sprite_->pos_ + looking_), 0.3f);
	}
	else {
		looking_.normalize();
		sprite_->rot_.slerp(tnl::Quaternion::LookAtAxisY(sprite_->pos_, sprite_->pos_ + looking_), 0.3f);
		//攻撃
		elapsed_++;
		if (elapsed_ > INTERVAL_) {
			elapsed_ = 0;
			scene_->objects_.emplace_back(new ComboE1(scene_, this));
		}
	}
	sprite_->update(delta_time);
	//被弾
	if (prev_hp_ != hp_) {
		damaged_t_ = 12;
	}
	if (damaged_t_ > 0) {
		damaged_t_--;
		sprite_->pos_ += -looking_ * 6.0f;
	}
	if (damaged_t_ <= 0) damaged_t_ = 0;
	prev_hp_ = hp_;

	//HPが0になったら消える
	if (hp_ <= 0) {
		hp_ = 0;
		alive_ = false;
	}
}

Cloud::Cloud(ScenePlay* scene) {
	tag_ = GameObj::eCloud;
	scene_ = scene;
	size_ = SIZE_;
	mesh_ = dxe::Mesh::CreatePlane({ SPRITE_W_, SPRITE_H_, 0 });
	mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/cloud.png"));
	mesh_->pos_ = { 0,0,0 };
}

void Cloud::update(float delta_time) {
	tnl::Vector3 dir;
	dir = mesh_->pos_ - scene_->camera_->pos_;
	dir.normalize();
	mesh_->rot_q_.slerp(tnl::Quaternion::LookAtAxisY(mesh_->pos_, mesh_->pos_ + dir), 0.3f);
}

Home::Home(ScenePlay* scene) {
	tag_ = GameObj::eHome;
	scene_ = scene;
	size_ = SIZE_;
	mesh_ = dxe::Mesh::CreatePlane({ SPRITE_W_, SPRITE_H_, 0 });
	mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/bed.png"));
	mesh_->pos_ = { 0,0,0 };
}

void Home::update(float delta_time) {
	tnl::Vector3 dir;
	dir = mesh_->pos_ - scene_->camera_->pos_;
	dir.normalize();
	mesh_->rot_q_.slerp(tnl::Quaternion::LookAtAxisY(mesh_->pos_, mesh_->pos_ + dir), 0.3f);
}

//範囲コンボ
Combo1::Combo1(ScenePlay* scene) {
	scene_ = scene;
	if (scene_->magic_ < MAGIC_) {
		alive_ = false;
		return;
	}
	//基本パラメータ
	tag_ = GameObj::eAttack;
	size_ = SIZE_;
	mesh_ = dxe::Mesh::CreatePlane({ SPRITE_W_, SPRITE_H_, 0 });
	mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/1/1_001.png"));
	scene_->magic_ -= MAGIC_;
	
	//座標・角度の設定
	looking_ = scene_->player_->looking_;
	mesh_->pos_ = scene_->player_->sprite_->pos_ + looking_ * DIS_;
	float th = std::atan(looking_.x / looking_.z);
	tnl::Vector3 qy = { looking_.x, 1, looking_.z };
	tnl::Vector3 rot = -looking_.cross(qy);
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, th);
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis(rot, tnl::ToRadian(90));
};

void Combo1::update(float delta_time) {
	if (!alive_) return;
	mesh_->pos_ += looking_ * 2.0f;
	elapsed_++;
	if (elapsed_ > FRAME_TIME_) {
		elapsed_ = 0;
		frame_++;
		switch (frame_) {
		case 1:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/1/1_002.png"));
			break;
		case 2:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/1/1_003.png"));
			Attack(DAMAGE_);
			break;
		case 3:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/1/1_004.png"));
			break;
		case 4:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/1/1_005.png"));
			break;
		case 5:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/1/1_006.png"));
			break;
		case 6:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/1/1_007.png"));
			break;
		case 7:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/1/1_008.png"));
			break;
		case 8:
			alive_ = false;
			break;
		}
	}
}

Combo2::Combo2(ScenePlay* scene) {
	scene_ = scene;
	if (scene_->magic_ < MAGIC_) {
		alive_ = false;
		return;
	}
	//基本パラメータ
	tag_ = GameObj::eAttack;
	size_ = SIZE_;
	mesh_ = dxe::Mesh::CreatePlane({ SPRITE_W_, SPRITE_H_, 0 });
	mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/2/2_001.png"));
	scene_->magic_ -= MAGIC_;

	//座標・角度の設定
	looking_ = scene_->player_->looking_;
	mesh_->pos_ = scene_->player_->sprite_->pos_ + looking_ * DIS_;
	float th = std::atan(looking_.x / looking_.z);
	tnl::Vector3 qy = { looking_.x, 1, looking_.z };
	tnl::Vector3 rot = -looking_.cross(qy);
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, th);
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis(rot, tnl::ToRadian(90));
};

void Combo2::update(float delta_time) {
	if (!alive_) return;
	mesh_->pos_ += looking_ * 2.0f;
	elapsed_++;
	if (elapsed_ > FRAME_TIME_) {
		elapsed_ = 0;
		frame_++;
		switch (frame_) {
		case 1:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/2/2_002.png"));
			break;
		case 2:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/2/2_003.png"));
			break;
		case 3:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/2/2_004.png"));
			Attack(DAMAGE_);
			break;
		case 4:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/2/2_005.png"));
			break;
		case 5:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/2/2_006.png"));
			break;
		case 6:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/2/2_007.png"));
			break;
		case 7:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/2/2_008.png"));
			break;
		case 8:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/2/2_009.png"));
			break;
		case 9:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/2/2_010.png"));
			break;
		case 10:
			alive_ = false;
			break;
		}
	}
}

Combo3::Combo3(ScenePlay* scene) {
	scene_ = scene;
	if (scene_->magic_ < MAGIC_) {
		alive_ = false;
		return;
	}
	//基本パラメータ
	tag_ = GameObj::eAttack;
	size_ = SIZE_;
	mesh_ = dxe::Mesh::CreatePlane({ SPRITE_W_, SPRITE_H_, 0 });
	mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/3/3_001.png"));
	scene_->magic_ -= MAGIC_;

	//座標・角度の設定
	looking_ = scene_->player_->looking_;
	mesh_->pos_ = scene_->player_->sprite_->pos_ + looking_ * DIS_;
	float th = std::atan(looking_.x / looking_.z);
	tnl::Vector3 qy = { looking_.x, 1, looking_.z };
	tnl::Vector3 rot = -looking_.cross(qy);
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, th);
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis(rot, tnl::ToRadian(90));
};

void Combo3::update(float delta_time) {
	if (!alive_) return;
	mesh_->pos_ += looking_ * 2.0f;
	elapsed_++;
	if (elapsed_ > FRAME_TIME_) {
		elapsed_ = 0;
		frame_++;
		switch (frame_) {
		case 1:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/3/3_002.png"));
			break;
		case 2:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/3/3_003.png"));
			break;
		case 3:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/3/3_004.png"));
			break;
		case 4:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/3/3_005.png"));
			break;
		case 5:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/3/3_006.png"));
			Attack(DAMAGE_);
			break;
		case 6:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/3/3_007.png"));
			break;
		case 7:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/3/3_008.png"));
			break;
		case 8:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/3/3_009.png"));
			break;
		case 9:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/3/3_010.png"));
			break;
		case 10:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/3/3_011.png"));
			break;
		case 11:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/3/3_012.png"));
			break;
		case 12:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/3/3_013.png"));
			break;
		case 13:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/3/3_014.png"));
			break;
		case 14:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/3/3_015.png"));
			break;
		case 15:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/3/3_016.png"));
			break;
		case 16:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/3/3_017.png"));
			break;
		case 17:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/3/3_018.png"));
			break;
		case 18:
			alive_ = false;
			break;
		}
	}
}


//近接コンボ
ComboM1::ComboM1(ScenePlay* scene) {
	scene_ = scene;
	if (scene_->magic_ < MAGIC_) {
		alive_ = false;
		return;
	}
	//基本パラメータ
	tag_ = GameObj::eAttack;
	size_ = SIZE_;
	mesh_ = dxe::Mesh::CreatePlane({ SPRITE_W_, SPRITE_H_, 0 });
	mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_001.png"));
	scene_->magic_ -= MAGIC_;

	//座標・角度の設定
	looking_ = scene_->player_->looking_;
	mesh_->pos_ = scene_->player_->sprite_->pos_ + looking_ * DIS_;
	float th = std::atan(looking_.x / looking_.z);
	tnl::Vector3 qy = { looking_.x, 1, looking_.z };
	tnl::Vector3 rot = -looking_.cross(qy);
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, th);
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis(rot, tnl::ToRadian(90));
};

void ComboM1::update(float delta_time) {
	if (!alive_) return;
	mesh_->pos_ = scene_->player_->sprite_->pos_ + looking_ * DIS_;
	elapsed_++;
	if (elapsed_ > FRAME_TIME_) {
		elapsed_ = 0;
		frame_++;
		switch (frame_) {
		case 1:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_002.png"));
			break;
		case 2:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_003.png"));
			break;
		case 3:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_004.png"));
			break;
		case 4:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_005.png"));
			break;
		case 5:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_006.png"));
			Attack(DAMAGE_);
			break;
		case 6:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_007.png"));
			break;
		case 7:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_008.png"));
			break;
		case 8:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_009.png"));
			break;
		case 9:
			alive_ = false;
			break;
		}
	}
}

ComboM2::ComboM2(ScenePlay* scene) {
	scene_ = scene;
	if (scene_->magic_ < MAGIC_) {
		alive_ = false;
		return;
	}
	//基本パラメータ
	tag_ = GameObj::eAttack;
	size_ = SIZE_;
	mesh_ = dxe::Mesh::CreatePlane({ SPRITE_W_, SPRITE_H_, 0 });
	mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m2/2_001.png"));
	scene_->magic_ -= MAGIC_;

	//座標・角度の設定
	looking_ = scene_->player_->looking_;
	mesh_->pos_ = scene_->player_->sprite_->pos_ + looking_ * DIS_;
	float th = std::atan(looking_.x / looking_.z);
	tnl::Vector3 qy = { looking_.x, 1, looking_.z };
	tnl::Vector3 rot = -looking_.cross(qy);
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, th);
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis(rot, tnl::ToRadian(90));
};

void ComboM2::update(float delta_time) {
	if (!alive_) return;
	mesh_->pos_ = scene_->player_->sprite_->pos_ + looking_ * DIS_;
	elapsed_++;
	if (elapsed_ > FRAME_TIME_) {
		elapsed_ = 0;
		frame_++;
		switch (frame_) {
		case 1:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m2/2_002.png"));
			break;
		case 2:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m2/2_003.png"));
			break;
		case 3:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m2/2_004.png"));
			break;
		case 4:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m2/2_005.png"));
			break;
		case 5:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m2/2_006.png"));
			Attack(DAMAGE_);
			break;
		case 6:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m2/2_007.png"));
			break;
		case 7:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m2/2_008.png"));
			break;
		case 8:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m2/2_009.png"));
			break;
		case 9:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m3/3_001.png"));
			break;
		case 10:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m3/3_002.png"));
			break;
		case 11:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m3/3_003.png"));
			break;
		case 12:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m3/3_004.png"));
			break;
		case 13:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m3/3_005.png"));
			Attack(DAMAGE_);
			break;
		case 14:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m3/3_006.png"));
			break;
		case 15:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m3/3_007.png"));
			break;
		case 16:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m3/3_008.png"));
			break;
		case 17:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m3/3_009.png"));
			break;
		case 18:
			alive_ = false;
			break;
		}
	}
}

ComboM3::ComboM3(ScenePlay* scene) {
	scene_ = scene;
	if (scene_->magic_ < MAGIC_) {
		alive_ = false;
		return;
	}
	//基本パラメータ
	tag_ = GameObj::eAttack;
	size_ = SIZE_;
	mesh_ = dxe::Mesh::CreatePlane({ SPRITE_W_, SPRITE_H_, 0 });
	mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_001.png"));
	scene_->magic_ -= MAGIC_;

	//座標・角度の設定
	looking_ = scene_->player_->looking_;
	mesh_->pos_ = scene_->player_->sprite_->pos_ + looking_ * DIS_;
	float th = std::atan(looking_.x / looking_.z);
	tnl::Vector3 qy = { looking_.x, 1, looking_.z };
	tnl::Vector3 rot = -looking_.cross(qy);
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, th);
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis(rot, tnl::ToRadian(90));
};

void ComboM3::update(float delta_time) {
	if (!alive_) return;
	mesh_->pos_ = scene_->player_->sprite_->pos_ + looking_ * DIS_;
	elapsed_++;
	if (elapsed_ > FRAME_TIME_) {
		elapsed_ = 0;
		frame_++;
		switch (frame_) {
		case 1:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_002.png"));
			break;
		case 2:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_003.png"));
			break;
		case 3:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_004.png"));
			break;
		case 4:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_005.png"));
			break;
		case 5:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_006.png"));
			Attack(DAMAGE_);
			break;
		case 6:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_007.png"));
			break;
		case 7:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_008.png"));
			break;
		case 8:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_009.png"));
			break;
		case 9:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m4/4_001.png"));
			break;
		case 10:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m4/4_002.png"));
			break;
		case 11:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m4/4_003.png"));
			break;
		case 12:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m4/4_004.png"));
			break;
		case 13:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m4/4_005.png"));
			Attack(DAMAGE_);
			break;
		case 14:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m4/4_006.png"));
			break;
		case 15:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m4/4_007.png"));
			break;
		case 16:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m4/4_008.png"));
			break;
		case 17:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m4/4_009.png"));
			break;
		case 18:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_001.png"));
			break;
		case 19:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_002.png"));
			break;
		case 20:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_003.png"));
			break;
		case 21:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_004.png"));
			break;
		case 22:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_005.png"));
			break;
		case 23:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_006.png"));
			Attack(DAMAGE_);
			break;
		case 24:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_007.png"));
			break;
		case 25:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_008.png"));
			break;
		case 26:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_009.png"));
			break;
		case 27:
			alive_ = false;
			break;
		}
	}
}

ComboM4::ComboM4(ScenePlay* scene) {
	scene_ = scene;
	if (scene_->magic_ < MAGIC_) {
		alive_ = false;
		return;
	}
	//基本パラメータ
	tag_ = GameObj::eAttack;
	size_ = SIZE_;
	mesh_ = dxe::Mesh::CreatePlane({ SPRITE_W_, SPRITE_H_, 0 });
	mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m5/5_001.png"));
	scene_->magic_ -= MAGIC_;

	//座標・角度の設定
	looking_ = scene_->player_->looking_;
	mesh_->pos_ = scene_->player_->sprite_->pos_ + looking_ * DIS_;
	float th = std::atan(looking_.x / looking_.z);
	tnl::Vector3 qy = { looking_.x, 1, looking_.z };
	tnl::Vector3 rot = -looking_.cross(qy);
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, th);
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis(rot, tnl::ToRadian(90));
};

void ComboM4::update(float delta_time) {
	if (!alive_) return;
	mesh_->pos_ = scene_->player_->sprite_->pos_ + looking_ * DIS_;
	elapsed_++;
	if (elapsed_ > FRAME_TIME_) {
		elapsed_ = 0;
		frame_++;
		switch (frame_) {
		case 1:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m5/5_002.png"));
			break;
		case 2:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m5/5_003.png"));
			break;
		case 3:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m5/5_004.png"));
			break;
		case 4:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m5/5_005.png"));
			break;
		case 5:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m5/5_006.png"));
			Attack(DAMAGE_);
			break;
		case 6:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m5/5_007.png"));
			break;
		case 7:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m5/5_008.png"));
			break;
		case 8:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m5/5_009.png"));
			Attack(DAMAGE_);
			break;
		case 9:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m5/5_010.png"));
			break;
		case 10:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m5/5_011.png"));
			break;
		case 11:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m5/5_012.png"));
			Attack(DAMAGE_);
			break;
		case 12:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m5/5_013.png"));
			break;
		case 13:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m5/5_014.png"));
			break;
		case 14:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m5/5_015.png"));
			Attack(DAMAGE_);
			break;
		case 15:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m5/5_016.png"));
			break;
		case 16:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m5/5_017.png"));
			break;
		case 17:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m5/5_018.png"));
			break;
		case 18:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m5/5_019.png"));
			break;
		case 19:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m5/5_020.png"));
			break;
		case 20:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m5/5_021.png"));
			break;
		case 21:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m5/5_022.png"));
			break;
		case 22:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m5/5_023.png"));
			break;
		case 23:
			alive_ = false;
			break;
		}
	}
}

//接近コンボ
ComboS1::ComboS1(ScenePlay* scene) {
	scene_ = scene;
	if (scene_->magic_ < MAGIC_) {
		alive_ = false;
		return;
	}

	//基本パラメータ
	tag_ = GameObj::eAttack;
	size_ = SIZE_;
	mesh_ = dxe::Mesh::CreatePlane({ SPRITE_W_, SPRITE_H_, 0 });
	mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/s1/1_001.png"));
	scene_->magic_ -= MAGIC_;

	//座標・角度の設定
	looking_ = scene_->player_->looking_;
	mesh_->pos_ = scene_->player_->sprite_->pos_ + looking_ * DIS_;
	float th = std::atan(looking_.x / looking_.z);
	tnl::Vector3 qy = { looking_.x, 1, looking_.z };
	tnl::Vector3 rot = -looking_.cross(qy);
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, th);
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis(rot, tnl::ToRadian(90));
};

void ComboS1::update(float delta_time) {
	if (!alive_) return;
	elapsed_++;
	if (elapsed_ > FRAME_TIME_) {
		elapsed_ = 0;
		frame_++;
		switch (frame_) {
		case 1:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/s1/1_002.png"));
			break;
		case 2:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/s1/1_003.png"));
			break;
		case 3:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/s1/1_004.png"));
			Attack(DAMAGE_);
			scene_->player_->sprite_->pos_ += scene_->player_->looking_ * DIS_ * 2.0f;
			break;
		case 4:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/s1/1_005.png"));
			break;
		case 5:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/s1/1_006.png"));
			break;
		case 6:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/s1/1_007.png"));
			break;
		case 7:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/s1/1_008.png"));
			break;
		case 8:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/s1/1_009.png"));
			break;
		case 9:
			alive_ = false;
			break;
		}
	}
}


//敵ひっかき攻撃
ComboE1::ComboE1(ScenePlay* scene, GameObj* object) {
	//基本パラメータ
	scene_ = scene;
	tag_ = GameObj::eEnemyAttack;
	target_ = object;
	size_ = SIZE_;
	mesh_ = dxe::Mesh::CreatePlane({ SPRITE_W_, SPRITE_H_, 0 });
	mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/e1/1_001.png"));

	//座標・角度の設定
	looking_ = target_->looking_;
	mesh_->pos_ = target_->sprite_->pos_ + looking_ * DIS_;
	float th = std::atan(looking_.x / looking_.z);
	tnl::Vector3 qy = { looking_.x, 1, looking_.z };
	tnl::Vector3 rot = -looking_.cross(qy);
	if (rot.x == 0 && rot.y == 0 && rot.z == 0) {
		rot = { 1,0,0 };
	}
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis({ 0,0,1 }, tnl::ToRadian(180));
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, th);
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis(rot, tnl::ToRadian(90));
};

void ComboE1::update(float delta_time) {
	elapsed_++;
	if (elapsed_ > FRAME_TIME_) {
		elapsed_ = 0;
		frame_++;
		switch (frame_) {
		case 1:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/e1/1_002.png"));
			EnemyAttack(DAMAGE_, scene_->player_, scene_->home_);
			break;
		case 2:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/e1/1_003.png"));
			break;
		case 3:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/e1/1_004.png"));
			break;
		case 4:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/e1/1_005.png"));
			break;
		case 5:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/e1/1_006.png"));
			break;
		case 6:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/e1/1_007.png"));
			break;
		case 7:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/e1/1_008.png"));
			break;
		case 8:
			alive_ = false;
			break;
		}
	}
}

void GameObj::Attack(int damage) {
	for (auto enemy : scene_->objects_) {
		if (enemy->tag_ != GameObj::eEnemy) continue;
		if (enemy->mesh_ != nullptr) {
			if (tnl::IsIntersectSphere(enemy->mesh_->pos_, enemy->size_, mesh_->pos_, size_)) {
				enemy->hp_ -= damage;
			}
		}
		if (enemy->sprite_ != nullptr) {
			if (tnl::IsIntersectSphere(enemy->sprite_->pos_, enemy->size_, mesh_->pos_, size_)) {
				enemy->hp_ -= damage;
			}
		}
	}
}

void GameObj::EnemyAttack(int damage, GameObj* player, GameObj* home) {
	if (tnl::IsIntersectSphere(player->sprite_->pos_, player->size_, mesh_->pos_, size_)) {
		player->hp_ -= damage;
	}
	if (tnl::IsIntersectSphere(home->mesh_->pos_, home->size_, mesh_->pos_, size_)) {
		scene_->home_hp_ -= damage;
	}
}

//------------------------------------------------------------------
//
// UI
//

Ui::Ui(ScenePlay* scene) {
	scene_ = scene;
	//フォント読み込み
	font_pop_16 = LoadFontDataToHandle("font/Novelpop16.dft", 0);
	font_pop_32 = LoadFontDataToHandle("font/Novelpop32.dft", 0);
	//hpバー読み込み
	hp_bar_[eBlue] = LoadGraph("graphics/gauge_bar_blue.png");
	hp_bar_[eYellow] = LoadGraph("graphics/gauge_bar_yellow.png");
	hp_bar_[eRed] = LoadGraph("graphics/gauge_bar_pink.png");
	hp_back_ = LoadGraph("graphics/gauge_background.png");
	hp_frame_ = LoadGraph("graphics/gauge_frame.png");
	//アイコン読み込み
	heart_ = LoadGraph("graphics/heart.png");
	star_ = LoadGraph("graphics/star.png");
	//時間読み込み
	time_icon_ = LoadGraph("graphics/time.png");
}

void Ui::update(float delta_time) {

}

void Ui::render() {
	//拠点HP表示
	DrawGauge(HOME_HP_X_, HOME_HP_Y_, 156, 8, 2, scene_->home_hp_, scene_->home_hp_max_, eYellow);
	//プレイヤーHP表示
	DrawGauge(PLAYER_GAUGE_X_, PLAYER_GAUGE_Y1_, 156, 8, 1.5f, scene_->player_->hp_, scene_->player_->hp_max_, eRed);
	DrawGauge(PLAYER_GAUGE_X_, PLAYER_GAUGE_Y2_, 156, 8, 1.5f, scene_->magic_, scene_->magic_max_, eBlue);
	DrawRotaGraph(PLAYER_ICON_X_, PLAYER_GAUGE_Y1_, 0.07f, 0, heart_, true);
	DrawRotaGraph(PLAYER_ICON_X_, PLAYER_GAUGE_Y2_, 0.1f, 0, star_, true);
	//制限時間表示
	DrawFormatStringToHandle(TIME_X_, TIME_Y_, 0, font_pop_32, "%d", scene_->stage_->time_);
	DrawRotaGraph(TIME_ICON_X_, TIME_ICON_Y_, 0.1f, 0, time_icon_, true);
}

void Ui::DrawGauge(int x, int y, int width, int height, double exrate, int num, int max, int color) {
	int gauge_x1 = x - width * exrate / 2;
	int gauge_y1 = y - height * exrate / 2;
	int gauge_y2 = y + height * exrate / 2;
	int gauge_x2 = gauge_x1 + width * exrate *  ((double)num / (double)max);
	DrawRotaGraph(x, y, exrate, 0, hp_back_, true);
	DrawExtendGraph(gauge_x1, gauge_y1, gauge_x2, gauge_y2, hp_bar_[color], true);
	DrawRotaGraph(x, y, exrate, 0, hp_frame_, true);
}

//------------------------------------------------------------------
//
// ステージ
//

void StageBase::RandomPop(int num) {
	int enemy_pop_ = rand() % 100;
	double th = ((double)enemy_pop_ / (double)100) * 3.14f * 2;
	float x = scene_->FIELD_R_ * cos(th);
	float y = scene_->FIELD_R_ * sin(th);
	for (int i = 0; i < num; ++i) {
		enemy_pop_ = rand() % 200 - 100;
		x = x + enemy_pop_;
		y = y + enemy_pop_;
		for (auto enemy : scene_->objects_) {
			if (enemy->tag_ != GameObj::eEnemy) continue;
			if (enemy->id_ != enemyStock_) continue;
			enemy->sprite_->pos_ = { x,0,y };
			enemy->moving_ = true;
			enemyStock_--;
		}
	}
}

Stage1::Stage1(ScenePlay* scene) {
	scene_ = scene;
	stageNum_ = 1;
	for (int i = 0; i < ENEMY_NUM_; ++i) {
		scene_->objects_.emplace_back(new EnemySprite(scene_, { 0,-500,0 }))->moving_ = false;
		scene_->objects_.back()->update(0);
		enemyStock_++;
		scene_->objects_.back()->id_ = enemyStock_;
	}
}

void Stage1::update(float delta_time) {
	elapsed_++;
	if (elapsed_ > 60) {
		elapsed_ = 0;
		time_--;
	}

	if (time_ % 6 == 1 && elapsed_ == 0) {
		RandomPop(3);
	}
}
