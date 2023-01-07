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