#include "gm_object_combo.h"
#include "../gm_camera.h"
#include "../model/gm_anim_sprite3d.h"
#include "../scene/gm_scene_play.h"

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

	//画像の取得
	getImage();
	mesh_->setTexture(dxe::Texture::CreateFromFile(*it));
	it++;
}

void PlayerComboBase::animation(int frame_time) {
	elapsed_++;
	if (elapsed_ > frame_time) {
		elapsed_ = 0;
		frame_++;
		if (it != images_.end()) {
			mesh_->setTexture(dxe::Texture::CreateFromFile(*it));
			it++;
		}
		else {
			alive_ = false;
		}
	}
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
};

void Combo1::update(float delta_time) {
	if (!alive_) return;
	mesh_->pos_ += looking_ * 2.0f;

	animation(FRAME_TIME_);

	if (elapsed_ == 0) {
		if (frame_ == 2) {
			Attack(DAMAGE_);
		}
	}
}

Combo2::Combo2(ScenePlay* scene) {
	initialize(scene, MAGIC_, SIZE_, SPRITE_W_, SPRITE_H_, DIS_);
};

void Combo2::update(float delta_time) {
	if (!alive_) return;
	mesh_->pos_ += looking_ * 2.0f;

	animation(FRAME_TIME_);

	if (elapsed_ == 0) {
		if (frame_ == 2) {
			Attack(DAMAGE_);
		}
	}
}

Combo3::Combo3(ScenePlay* scene) {
	initialize(scene, MAGIC_, SIZE_, SPRITE_W_, SPRITE_H_, DIS_);
};

void Combo3::update(float delta_time) {
	if (!alive_) return;
	mesh_->pos_ += looking_ * 2.0f;

	animation(FRAME_TIME_);

	if (elapsed_ == 0) {
		if (frame_ == 4) {
			Attack(DAMAGE_);
		}
	}
}

//近接コンボ
ComboM1::ComboM1(ScenePlay* scene) {
	initialize(scene, MAGIC_, SIZE_, SPRITE_W_, SPRITE_H_, DIS_);
};

void ComboM1::update(float delta_time) {
	if (!alive_) return;
	mesh_->pos_ = scene_->player_->sprite_->pos_ + looking_ * DIS_;

	animation(FRAME_TIME_);

	if (elapsed_ == 0) {
		if (frame_ == 5) {
			Attack(DAMAGE_);
		}
	}
}

ComboM2::ComboM2(ScenePlay* scene) {
	initialize(scene, MAGIC_, SIZE_, SPRITE_W_, SPRITE_H_, DIS_);
};

void ComboM2::update(float delta_time) {
	if (!alive_) return;
	mesh_->pos_ = scene_->player_->sprite_->pos_ + looking_ * DIS_;

	animation(FRAME_TIME_);

	if (elapsed_ == 0) {
		if (frame_ == 4 || frame_ == 13) {
			Attack(DAMAGE_);
		}
	}
}

ComboM3::ComboM3(ScenePlay* scene) {
	initialize(scene, MAGIC_, SIZE_, SPRITE_W_, SPRITE_H_, DIS_);
};

void ComboM3::update(float delta_time) {
	if (!alive_) return;
	mesh_->pos_ = scene_->player_->sprite_->pos_ + looking_ * DIS_;

	animation(FRAME_TIME_);

	if (elapsed_ == 0) {
		if (frame_ == 4 || frame_ == 13 || frame_ == 22) {
			Attack(DAMAGE_);
		}
	}
}

ComboM4::ComboM4(ScenePlay* scene) {
	initialize(scene, MAGIC_, SIZE_, SPRITE_W_, SPRITE_H_, DIS_);
};

void ComboM4::update(float delta_time) {
	if (!alive_) return;
	mesh_->pos_ = scene_->player_->sprite_->pos_ + looking_ * DIS_;

	animation(FRAME_TIME_);

	if (elapsed_ == 0) {
		if (frame_ == 5 || frame_ == 8 || frame_ == 11 || frame_ == 14) {
			Attack(DAMAGE_);
		}
	}
}

//接近コンボ
ComboS1::ComboS1(ScenePlay* scene) {
	initialize(scene, MAGIC_, SIZE_, SPRITE_W_, SPRITE_H_, DIS_);
};

void ComboS1::update(float delta_time) {
	if (!alive_) return;

	animation(FRAME_TIME_);

	if(elapsed_ == 0){
		if (frame_ == 2) {
			Attack(DAMAGE_);
			scene_->player_->sprite_->pos_ += scene_->player_->looking_ * DIS_ * 2.0f;
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

	//画像の取得
	getImage();
	mesh_->setTexture(dxe::Texture::CreateFromFile(*it));
	it++;
}

void EnemyComboBase::animation(int frame_time) {
	elapsed_++;
	if (elapsed_ > frame_time) {
		elapsed_ = 0;
		frame_++;
		if (it != images_.end()) {
			mesh_->setTexture(dxe::Texture::CreateFromFile(*it));
			it++;
		}
		else {
			alive_ = false;
		}
	}
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
};

void ComboE1::update(float delta_time) {
	if (!alive_) return;

	animation(FRAME_TIME_);

	if (elapsed_ == 0) {
		if (frame_ == 2) {
			Attack(DAMAGE_, scene_->player_, scene_->home_);
		}
	}
}