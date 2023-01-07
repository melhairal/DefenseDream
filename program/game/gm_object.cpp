#include "gm_object.h"
#include "gm_camera.h"
#include "model/gm_anim_sprite3d.h"
#include "scene/gm_scene_play.h"

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

// ========== 攻撃(コンボ) ==========

void PlayerComboBase::initialize(ScenePlay* scene, int magic, float size, float w, float h, float d) {
	scene_ = scene;
	if (scene_->magic_ < magic) {
		alive_ = false;
		return;
	}
	//基本パラメータ
	tag_ = GameObj::eAttack;
	size_ = size;
	mesh_ = dxe::Mesh::CreatePlane({ w, h, 0 });
	scene_->magic_ -= magic;

	//座標・角度の設定
	looking_ = scene_->player_->looking_;
	mesh_->pos_ = scene_->player_->sprite_->pos_ + looking_ * d;
	float th = std::atan(looking_.x / looking_.z);
	tnl::Vector3 qy = { looking_.x, 1, looking_.z };
	tnl::Vector3 rot = -looking_.cross(qy);
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, th);
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis(rot, tnl::ToRadian(90));
}

void PlayerComboBase::Attack(int damage) {
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

//範囲コンボ
Combo1::Combo1(ScenePlay* scene) {
	initialize(scene, MAGIC_, SIZE_, SPRITE_W_, SPRITE_H_, DIS_);
	if (alive_) mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/1/1_001.png"));
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
	initialize(scene, MAGIC_, SIZE_, SPRITE_W_, SPRITE_H_, DIS_);
	if (alive_) mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/2/2_001.png"));
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
	initialize(scene, MAGIC_, SIZE_, SPRITE_W_, SPRITE_H_, DIS_);
	if (alive_) mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/3/3_001.png"));
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
	initialize(scene, MAGIC_, SIZE_, SPRITE_W_, SPRITE_H_, DIS_);
	if (alive_) mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_001.png"));
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
	initialize(scene, MAGIC_, SIZE_, SPRITE_W_, SPRITE_H_, DIS_);
	if (alive_) mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m2/2_001.png"));
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
	initialize(scene, MAGIC_, SIZE_, SPRITE_W_, SPRITE_H_, DIS_);
	if (alive_) mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m1/1_001.png"));
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
	initialize(scene, MAGIC_, SIZE_, SPRITE_W_, SPRITE_H_, DIS_);
	if (alive_) mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/m5/5_001.png"));
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
	initialize(scene, MAGIC_, SIZE_, SPRITE_W_, SPRITE_H_, DIS_);
	if (alive_) mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/s1/1_001.png"));
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

// ========== 敵の攻撃 ==========
void EnemyComboBase::initialize(ScenePlay* scene, GameObj* object, float size, float w, float h, float d) {
	//基本パラメータ
	scene_ = scene;
	tag_ = GameObj::eEnemyAttack;
	target_ = object;
	size_ = size;
	mesh_ = dxe::Mesh::CreatePlane({ w, h, 0 });

	//座標・角度の設定
	looking_ = target_->looking_;
	mesh_->pos_ = target_->sprite_->pos_ + looking_ * d;
	float th = std::atan(looking_.x / looking_.z);
	tnl::Vector3 qy = { looking_.x, 1, looking_.z };
	tnl::Vector3 rot = -looking_.cross(qy);
	if (rot.x == 0 && rot.y == 0 && rot.z == 0) {
		rot = { 1,0,0 };
	}
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis({ 0,0,1 }, tnl::ToRadian(180));
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis({ 0,1,0 }, th);
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis(rot, tnl::ToRadian(90));
}

void EnemyComboBase::Attack(int damage, GameObj* player, GameObj* home) {
	if (tnl::IsIntersectSphere(player->sprite_->pos_, player->size_, mesh_->pos_, size_)) {
		player->hp_ -= damage;
	}
	if (tnl::IsIntersectSphere(home->mesh_->pos_, home->size_, mesh_->pos_, size_)) {
		scene_->home_hp_ -= damage;
	}
}

//ひっかき攻撃
ComboE1::ComboE1(ScenePlay* scene, GameObj* object) {
	initialize(scene, object, SIZE_, SPRITE_W_, SPRITE_H_, DIS_);
	mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/e1/1_001.png"));
};

void ComboE1::update(float delta_time) {
	elapsed_++;
	if (elapsed_ > FRAME_TIME_) {
		elapsed_ = 0;
		frame_++;
		switch (frame_) {
		case 1:
			mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/combo/e1/1_002.png"));
			Attack(DAMAGE_, scene_->player_, scene_->home_);
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