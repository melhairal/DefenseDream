#include "gm_object_player.h"
#include "gm_object_combo.h"
#include "../gm_camera.h"
#include "../model/gm_anim_sprite3d.h"
#include "../scene/gm_scene_play.h"

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

	 //スプライトのアップデート(スプライトに対するカメラの位置とスプライトの向き制御)
	updateSprite(delta_time, SPRITE_W_, SPRITE_H_);
	playerMove();

	//------------------------------------------------------------------
	//
	// マウスポインタ制御
	//

	updateMouse(delta_time);

	//------------------------------------------------------------------
	//
	// プレイヤー操作制御
	//

	//コンボ(近接)
	comboLeft();

	//コンボ(遠隔)
	comboRight();

	//接近攻撃
	comboShiht();

	//------------------------------------------------------------------
	//
	// その他制御
	//
	
	//被弾
	recievDamage();

	//魔力の自動回復
	healMagic();

	//スプライトのアップデート
	sprite_->update(delta_time);
}

void Player::playerMove() {
	//移動ベクトル
	tnl::Vector3 move_v = { 0,0,0 };

	//四方向のベクトル
	tnl::Vector3 dir[4] = {
		scene_->camera_->front().xz(),
		scene_->camera_->right().xz(),
		scene_->camera_->back().xz(),
		scene_->camera_->left().xz(),
	};

	//キー入力から移動ベクトルに四方向ベクトルを加える
	tnl::Input::RunIndexKeyDown([&](uint32_t idx) {
		move_v += dir[idx];
		}, eKeys::KB_W, eKeys::KB_D, eKeys::KB_S, eKeys::KB_A);

	int speed = SPEED_;

	//走り
	if (tnl::Input::IsKeyDown(eKeys::KB_LSHIFT) && scene_->magic_ > 0) {
		scene_->magic_--;
		speed = SPRINT_;
	}

	//歩き
	if (tnl::Input::IsKeyDown(eKeys::KB_W, eKeys::KB_D, eKeys::KB_S, eKeys::KB_A)) {
		move_v.normalize();
		prev_pos_ = sprite_->pos_;
		sprite_->pos_ += move_v * speed;
	}

	//サークルをプレイヤーに追従させる
	mesh_->pos_ = { sprite_->pos_.x, sprite_->pos_.y - SPRITE_H_ / 2, sprite_->pos_.z };
}

void Player::updateMouse(float delta_time) {
	//マウスの位置を取得
	tnl::Vector3 msv = tnl::Input::GetMousePosition();

	//マウスの位置からレイを生成
	tnl::Vector3 ray = tnl::Vector3::CreateScreenRay(
		msv.x
		, msv.y
		, scene_->camera_->screen_w_
		, scene_->camera_->screen_h_
		, scene_->camera_->view_
		, scene_->camera_->proj_);

	//当たった位置のベクトル
	tnl::Vector3 hit;

	//マウスカーソルの方向を向く
	if (tnl::IsIntersectLinePlane(scene_->camera_->pos_, scene_->camera_->pos_ + (ray * 10000.0f), { 0, scene_->FIELD_H_, 0 }, { 0, 1, 0 }, &hit)) {
		sprite_->rot_.slerp(tnl::Quaternion::LookAtAxisY(sprite_->pos_, hit), 0.3f);
		mesh_->rot_q_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90)) * sprite_->rot_;
	}

	//向いてる方向のベクトルを取得
	looking_ = hit - sprite_->pos_;
	looking_.y = 0;
	looking_.normalize();
}

void Player::comboLeft() {
	//左クリックした時&右クリックがコンボ中でないとき
	if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_LEFT) && combo_counter_ == 0) {
		
		//コンボ数のチェック
		switch (comboM_counter_) {
		case 0:
			if (tnl::Input::IsKeyDown(eKeys::KB_LSHIFT)) break;
			scene_->objects_.emplace_back(new ComboM1(scene_));
			comboM_counter_++;
			break;

		case 1:
			//コンボ受付時間のチェック
			if (comboM_timer_ > COMBO_INTERVAL_) {
				scene_->objects_.emplace_back(new ComboM2(scene_));
				comboM_timer_ = 0;
				comboM_counter_++;
			}
			break;

		case 2:
			//コンボ受付時間のチェック
			if (comboM_timer_ > COMBO_INTERVAL_) {
				scene_->objects_.emplace_back(new ComboM3(scene_));
				comboM_timer_ = 0;
				comboM_counter_++;
			}
			break;

		case 3:
			//コンボ受付時間のチェック
			if (comboM_timer_ > COMBO_INTERVAL_) {
				scene_->objects_.emplace_back(new ComboM4(scene_));
				comboM_timer_ = 0;
				comboM_counter_++;
			}
		}
	}

	//コンボ受付時間のカウント
	if (comboM_counter_ > 0) {
		comboM_timer_++;
		if (comboM_timer_ > COMBO_RECEPTION_) {
			comboM_timer_ = 0;
			comboM_counter_ = 0;
		}
	}
}

void Player::comboRight() {
	//右クリックした時&左クリックがコンボ中でないとき
	if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_RIGHT) && comboM_counter_ == 0) {

		//コンボ数のチェック
		switch (combo_counter_) {
		case 0:
			scene_->objects_.emplace_back(new Combo1(scene_));
			combo_counter_++;
			break;

		case 1:
			//コンボ受付時間のチェック
			if (combo_timer_ > COMBO_INTERVAL_) {
				scene_->objects_.emplace_back(new Combo2(scene_));
				combo_timer_ = 0;
				combo_counter_++;
			}
			break;

		case 2:
			//コンボ受付時間のチェック
			if (combo_timer_ > COMBO_INTERVAL_) {
				scene_->objects_.emplace_back(new Combo3(scene_));
				combo_timer_ = 0;
				combo_counter_++;
			}
			break;
		}
	}

	//コンボ受付時間のカウント
	if (combo_counter_ > 0) {
		combo_timer_++;
		if (combo_timer_ > COMBO_RECEPTION_) {
			combo_timer_ = 0;
			combo_counter_ = 0;
		}
	}
}

void Player::comboShiht() {
	//シフトキーを押しながら左クリックをしたとき&コンボが回っていないとき
	if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_LEFT) && tnl::Input::IsKeyDown(eKeys::KB_LSHIFT)
		&& combo_counter_ == 0 && comboM_counter_ == 0) {
		scene_->objects_.emplace_back(new ComboS1(scene_));
		comboM_timer_ = 0;
		comboM_counter_ = 2;
	}
}

void Player::recievDamage() {
	//HPが減ったとき
	if (prev_hp_ > hp_) {
		//カウンターをセット
		damaged_t_ = 12;
	}

	//カウンターの秒数分のけぞる
	if (damaged_t_ > 0) {
		damaged_t_--;
		sprite_->pos_ += -looking_ * 6.0f;
	}

	//hpの上限下限
	hp_ = std::clamp(hp_, 0, hp_max_);

	//hp状態の記憶
	prev_hp_ = hp_;
}

void Player::healMagic() {
	//魔力の状態に変動がない時
	if (scene_->magic_ == scene_->magic_prev_) {
		scene_->magic_timer_++;

		//一定時間たったら回復
		if (scene_->magic_timer_ >= scene_->MAGIC_TIME_) {
			scene_->magic_++;
			scene_->magic_timer_ = scene_->MAGIC_TIME_;
		}
	}
	else {
		//タイマーをリセット
		scene_->magic_timer_ = 0;
	}

	//魔力の上限下限
	scene_->magic_ = std::clamp(scene_->magic_, 0, scene_->magic_max_);

	//魔力の状態を記憶
	scene_->magic_prev_ = scene_->magic_;
}