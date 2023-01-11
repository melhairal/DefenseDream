#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../gm_object.h"
class ScenePlay;

//�G(���b�V��)
class Enemy :public GameObj {
public:
	Enemy(ScenePlay* scene, const tnl::Vector3& pos);
	~Enemy() {}
	void update(float delta_time) override;

	const float SIZE_ = 30.0f; //�����蔻��̑傫��
	const float SPEED_ = 2.0f; //�ړ����x

	GameObj* target_ = nullptr;
};

//�G(�X�v���C�g)
class EnemySprite :public GameObj {
public:
	EnemySprite(ScenePlay* scene, const tnl::Vector3& pos);
	~EnemySprite() {}
	void update(float delta_time) override;

	// =========== �֐� ==========

	void enemyMove(float delta_time);

	// ========== ��{�X�e�[�^�X ==========

	const float SPRITE_W_ = 32.0f; //�摜�T�C�Y
	const float SPRITE_H_ = 48.0f;
	const float SIZE_ = 30.0f; //�����蔻��̑傫��
	const float SPEED_ = 2.0f; //�ړ����x
	const float DIR_ = 50.0f; //��~����
	const float KNOCK_BACK_ = 6.0f; //�m�b�N�o�b�N��
	const int KNOCK_BACK_TIMER_ = 12; //�m�b�N�o�b�N����
	const int INTERVAL_ = 60; //�U���Ԋu

	int knock_back_counter_ = 0; //�m�b�N�o�b�N���[�V�����v���p
	int elapsed_ = 0; //���Ԍv���p

	GameObj* target_ = nullptr;
};
