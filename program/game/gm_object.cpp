#include "gm_object.h"
#include "gm_camera.h"
#include "model/gm_anim_sprite3d.h"
#include "scene/gm_scene_play.h"

GameObj::~GameObj() {
	if (mesh_ != nullptr) delete mesh_;
	if (sprite_ != nullptr) delete sprite_;
}

void GameObj::updateSprite(float delta_time, float w, float h) {
	//スプライトに対してカメラがどの面にあるか
	int t = tnl::GetXzRegionPointAndOBB(
		scene_->camera_->pos_
		, sprite_->pos_
		, { w, h, w }
	, sprite_->rot_);

	//アニメーションの名前を登録
	std::string anim_names[4] = {
		"walk_back", "walk_right", "walk_front", "walk_left"
	};
	sprite_->setCurrentAnim(anim_names[t]);
}

void GameObj::recievDamage(float power, int timer, int& counter) {
	//HPが減ったとき
	if (prev_hp_ > hp_) {
		//カウンターをセット
		counter = timer;
	}

	//カウンターの秒数分のけぞる
	if (counter > 0) {
		counter--;
		sprite_->pos_ += -looking_ * power;
	}

	//hpの上限下限
	hp_ = std::clamp(hp_, 0, hp_max_);

	//hp状態の記憶
	prev_hp_ = hp_;
}

void GameObj::hitWall(GameObj* object) {
	//objectが自分に接触したら押し戻す
	if (tnl::IsIntersectAABB(object->sprite_->pos_, { object->size_,object->size_,object->size_ }, mesh_->pos_, { size_,size_,size_ })) {
		tnl::GetCorrectPositionIntersectAABB(object->prev_pos_, { object->size_,object->size_,object->size_ }, mesh_->pos_, { size_,size_,size_ }, object->sprite_->pos_);
	}
}