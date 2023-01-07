#include "gm_object_player.h"
#include "gm_object_combo.h"
#include "../gm_camera.h"
#include "../model/gm_anim_sprite3d.h"
#include "../scene/gm_scene_play.h"

Player::Player(ScenePlay* scene) {
	tag_ = GameObj::ePlayer;
	scene_ = scene;
	size_ = SIZE_;

	//�v���C���[����
	sprite_ = new AnimSprite3D(scene_->camera_);
	sprite_->regist(SPRITE_W_, SPRITE_H_, "walk_front", "graphics/c1_anim_up.png", tnl::SeekUnit::ePlayMode::REPEAT, 1.0f, 4, SPRITE_H_, 0);
	sprite_->regist(SPRITE_W_, SPRITE_H_, "walk_back", "graphics/c1_anim_down.png", tnl::SeekUnit::ePlayMode::REPEAT, 1.0f, 4, SPRITE_H_, 0);
	sprite_->regist(SPRITE_W_, SPRITE_H_, "walk_left", "graphics/c1_anim_left.png", tnl::SeekUnit::ePlayMode::REPEAT, 1.0f, 4, SPRITE_H_, 0);
	sprite_->regist(SPRITE_W_, SPRITE_H_, "walk_right", "graphics/c1_anim_right.png", tnl::SeekUnit::ePlayMode::REPEAT, 1.0f, 4, SPRITE_H_, 0);
	sprite_->setCurrentAnim("walk_front");
	sprite_->pos_ = { 50,0,0 };

	//�v���C���[�T�[�N������
	mesh_ = dxe::Mesh::CreatePlane({ SPRITE_W_ * 2, SPRITE_W_ * 2, 0 });
	mesh_->rot_q_ *= tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	mesh_->pos_ = { sprite_->pos_.x, sprite_->pos_.y - SPRITE_H_ / 2, sprite_->pos_.z };
	mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/circle.png"));
}

void Player::update(float delta_time) {
	//------------------------------------------------------------------
	//
	// �ړ�����
	//

	 //�X�v���C�g�̃A�b�v�f�[�g(�X�v���C�g�ɑ΂���J�����̈ʒu�ƃX�v���C�g�̌�������)
	updateSprite(delta_time, SPRITE_W_, SPRITE_H_);
	playerMove();

	//------------------------------------------------------------------
	//
	// �}�E�X�|�C���^����
	//

	updateMouse(delta_time);

	//------------------------------------------------------------------
	//
	// �v���C���[���쐧��
	//

	//�R���{(�ߐ�)
	comboLeft();

	//�R���{(���u)
	comboRight();

	//�ڋߍU��
	comboShiht();

	//------------------------------------------------------------------
	//
	// ���̑�����
	//
	
	//��e
	recievDamage();

	//���͂̎�����
	healMagic();

	//�X�v���C�g�̃A�b�v�f�[�g
	sprite_->update(delta_time);
}

void Player::playerMove() {
	//�ړ��x�N�g��
	tnl::Vector3 move_v = { 0,0,0 };

	//�l�����̃x�N�g��
	tnl::Vector3 dir[4] = {
		scene_->camera_->front().xz(),
		scene_->camera_->right().xz(),
		scene_->camera_->back().xz(),
		scene_->camera_->left().xz(),
	};

	//�L�[���͂���ړ��x�N�g���Ɏl�����x�N�g����������
	tnl::Input::RunIndexKeyDown([&](uint32_t idx) {
		move_v += dir[idx];
		}, eKeys::KB_W, eKeys::KB_D, eKeys::KB_S, eKeys::KB_A);

	int speed = SPEED_;

	//����
	if (tnl::Input::IsKeyDown(eKeys::KB_LSHIFT) && scene_->magic_ > 0) {
		scene_->magic_--;
		speed = SPRINT_;
	}

	//����
	if (tnl::Input::IsKeyDown(eKeys::KB_W, eKeys::KB_D, eKeys::KB_S, eKeys::KB_A)) {
		move_v.normalize();
		prev_pos_ = sprite_->pos_;
		sprite_->pos_ += move_v * speed;
	}

	//�T�[�N�����v���C���[�ɒǏ]������
	mesh_->pos_ = { sprite_->pos_.x, sprite_->pos_.y - SPRITE_H_ / 2, sprite_->pos_.z };
}

void Player::updateMouse(float delta_time) {
	//�}�E�X�̈ʒu���擾
	tnl::Vector3 msv = tnl::Input::GetMousePosition();

	//�}�E�X�̈ʒu���烌�C�𐶐�
	tnl::Vector3 ray = tnl::Vector3::CreateScreenRay(
		msv.x
		, msv.y
		, scene_->camera_->screen_w_
		, scene_->camera_->screen_h_
		, scene_->camera_->view_
		, scene_->camera_->proj_);

	//���������ʒu�̃x�N�g��
	tnl::Vector3 hit;

	//�}�E�X�J�[�\���̕���������
	if (tnl::IsIntersectLinePlane(scene_->camera_->pos_, scene_->camera_->pos_ + (ray * 10000.0f), { 0, scene_->FIELD_H_, 0 }, { 0, 1, 0 }, &hit)) {
		sprite_->rot_.slerp(tnl::Quaternion::LookAtAxisY(sprite_->pos_, hit), 0.3f);
		mesh_->rot_q_ = tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90)) * sprite_->rot_;
	}

	//�����Ă�����̃x�N�g�����擾
	looking_ = hit - sprite_->pos_;
	looking_.y = 0;
	looking_.normalize();
}

void Player::comboLeft() {
	//���N���b�N������&�E�N���b�N���R���{���łȂ��Ƃ�
	if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_LEFT) && combo_counter_ == 0) {
		
		//�R���{���̃`�F�b�N
		switch (comboM_counter_) {
		case 0:
			if (tnl::Input::IsKeyDown(eKeys::KB_LSHIFT)) break;
			scene_->objects_.emplace_back(new ComboM1(scene_));
			comboM_counter_++;
			break;

		case 1:
			//�R���{��t���Ԃ̃`�F�b�N
			if (comboM_timer_ > COMBO_INTERVAL_) {
				scene_->objects_.emplace_back(new ComboM2(scene_));
				comboM_timer_ = 0;
				comboM_counter_++;
			}
			break;

		case 2:
			//�R���{��t���Ԃ̃`�F�b�N
			if (comboM_timer_ > COMBO_INTERVAL_) {
				scene_->objects_.emplace_back(new ComboM3(scene_));
				comboM_timer_ = 0;
				comboM_counter_++;
			}
			break;

		case 3:
			//�R���{��t���Ԃ̃`�F�b�N
			if (comboM_timer_ > COMBO_INTERVAL_) {
				scene_->objects_.emplace_back(new ComboM4(scene_));
				comboM_timer_ = 0;
				comboM_counter_++;
			}
		}
	}

	//�R���{��t���Ԃ̃J�E���g
	if (comboM_counter_ > 0) {
		comboM_timer_++;
		if (comboM_timer_ > COMBO_RECEPTION_) {
			comboM_timer_ = 0;
			comboM_counter_ = 0;
		}
	}
}

void Player::comboRight() {
	//�E�N���b�N������&���N���b�N���R���{���łȂ��Ƃ�
	if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_RIGHT) && comboM_counter_ == 0) {

		//�R���{���̃`�F�b�N
		switch (combo_counter_) {
		case 0:
			scene_->objects_.emplace_back(new Combo1(scene_));
			combo_counter_++;
			break;

		case 1:
			//�R���{��t���Ԃ̃`�F�b�N
			if (combo_timer_ > COMBO_INTERVAL_) {
				scene_->objects_.emplace_back(new Combo2(scene_));
				combo_timer_ = 0;
				combo_counter_++;
			}
			break;

		case 2:
			//�R���{��t���Ԃ̃`�F�b�N
			if (combo_timer_ > COMBO_INTERVAL_) {
				scene_->objects_.emplace_back(new Combo3(scene_));
				combo_timer_ = 0;
				combo_counter_++;
			}
			break;
		}
	}

	//�R���{��t���Ԃ̃J�E���g
	if (combo_counter_ > 0) {
		combo_timer_++;
		if (combo_timer_ > COMBO_RECEPTION_) {
			combo_timer_ = 0;
			combo_counter_ = 0;
		}
	}
}

void Player::comboShiht() {
	//�V�t�g�L�[�������Ȃ��獶�N���b�N�������Ƃ�&�R���{������Ă��Ȃ��Ƃ�
	if (tnl::Input::IsMouseTrigger(eMouseTrigger::IN_LEFT) && tnl::Input::IsKeyDown(eKeys::KB_LSHIFT)
		&& combo_counter_ == 0 && comboM_counter_ == 0) {
		scene_->objects_.emplace_back(new ComboS1(scene_));
		comboM_timer_ = 0;
		comboM_counter_ = 2;
	}
}

void Player::recievDamage() {
	//HP���������Ƃ�
	if (prev_hp_ > hp_) {
		//�J�E���^�[���Z�b�g
		damaged_t_ = 12;
	}

	//�J�E���^�[�̕b�����̂�����
	if (damaged_t_ > 0) {
		damaged_t_--;
		sprite_->pos_ += -looking_ * 6.0f;
	}

	//hp�̏������
	hp_ = std::clamp(hp_, 0, hp_max_);

	//hp��Ԃ̋L��
	prev_hp_ = hp_;
}

void Player::healMagic() {
	//���͂̏�Ԃɕϓ����Ȃ���
	if (scene_->magic_ == scene_->magic_prev_) {
		scene_->magic_timer_++;

		//��莞�Ԃ��������
		if (scene_->magic_timer_ >= scene_->MAGIC_TIME_) {
			scene_->magic_++;
			scene_->magic_timer_ = scene_->MAGIC_TIME_;
		}
	}
	else {
		//�^�C�}�[�����Z�b�g
		scene_->magic_timer_ = 0;
	}

	//���͂̏������
	scene_->magic_ = std::clamp(scene_->magic_, 0, scene_->magic_max_);

	//���͂̏�Ԃ��L��
	scene_->magic_prev_ = scene_->magic_;
}