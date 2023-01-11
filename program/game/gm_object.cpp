#include "gm_object.h"
#include "gm_camera.h"
#include "model/gm_anim_sprite3d.h"
#include "scene/gm_scene_play.h"

GameObj::~GameObj() {
	if (mesh_ != nullptr) delete mesh_;
	if (sprite_ != nullptr) delete sprite_;
}

void GameObj::updateSprite(float delta_time, float w, float h) {
	//�X�v���C�g�ɑ΂��ăJ�������ǂ̖ʂɂ��邩
	int t = tnl::GetXzRegionPointAndOBB(
		scene_->camera_->pos_
		, sprite_->pos_
		, { w, h, w }
	, sprite_->rot_);

	//�A�j���[�V�����̖��O��o�^
	std::string anim_names[4] = {
		"walk_back", "walk_right", "walk_front", "walk_left"
	};
	sprite_->setCurrentAnim(anim_names[t]);
}

void GameObj::recievDamage(float power, int timer, int& counter) {
	//HP���������Ƃ�
	if (prev_hp_ > hp_) {
		//�J�E���^�[���Z�b�g
		counter = timer;
	}

	//�J�E���^�[�̕b�����̂�����
	if (counter > 0) {
		counter--;
		sprite_->pos_ += -looking_ * power;
	}

	//hp�̏������
	hp_ = std::clamp(hp_, 0, hp_max_);

	//hp��Ԃ̋L��
	prev_hp_ = hp_;
}

void GameObj::hitWall(GameObj* object) {
	//object�������ɐڐG�����牟���߂�
	if (tnl::IsIntersectAABB(object->sprite_->pos_, { object->size_,object->size_,object->size_ }, mesh_->pos_, { size_,size_,size_ })) {
		tnl::GetCorrectPositionIntersectAABB(object->prev_pos_, { object->size_,object->size_,object->size_ }, mesh_->pos_, { size_,size_,size_ }, object->sprite_->pos_);
	}
}