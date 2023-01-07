#include "gm_object_enemy.h"
#include "gm_object_combo.h"
#include "../gm_camera.h"
#include "../model/gm_anim_sprite3d.h"
#include "../scene/gm_scene_play.h"

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