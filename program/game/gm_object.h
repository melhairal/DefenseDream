#pragma once
#include "../dxlib_ext/dxlib_ext.h"
class ScenePlay;
class AnimSprite3D;

class GameObj {
public:
	ScenePlay* scene_ = nullptr; //�V�[���̕ϐ����擾���邽�߂̃|�C���^
	GameObj() {}
	virtual ~GameObj() {
		if (mesh_ != nullptr) delete mesh_;
		if (sprite_ != nullptr) delete sprite_;
	}
	virtual void update(float delta_time) {}

	enum {
		eNone = -1,
		ePlayer,
		eHome,
		eBullet,
		eEnemy,
		eCloud,
		eAttack,
		eEnemyAttack
	};
	int tag_ = GameObj::eNone; //�I�u�W�F�N�g�����ʂ���^�O
	int id_ = -1; //�����^�O���ł̎��ʗp�̔ԍ�

	dxe::Mesh* mesh_ = nullptr;
	AnimSprite3D* sprite_ = nullptr;

	bool moving_ = true; //����t���O
	bool alive_ = true; //�����t���O
	float size_ = 0.0f; //�����蔻��p�T�C�Y
	int hp_max_ = 100; //�ő�HP
	int hp_ = 100; //HP
	int prev_hp_ = hp_; //��e����p

	tnl::Vector3 looking_ = { 1,0,0 }; //���Ă�������̃x�N�g��
	tnl::Vector3 prev_pos_ = { 0,0,0 }; //�␳�p���W
};

//�v���C���[
class Player :public GameObj {
public:
	Player(ScenePlay* scene);
	~Player() {}
	void update(float delta_time) override;

	const float SPRITE_W_ = 32.0f; //�摜�T�C�Y
	const float SPRITE_H_ = 48.0f;
	const float SIZE_ = 30.0f; //�����蔻��̑傫��
	const float SPEED_ = 3.0f; //�ړ����x
	const float SPRINT_ = 5.0f; //�_�b�V�����x
	const int COMBO_RECEPTION_ = 100; //�R���{��t�P�\
	const int COMBO_INTERVAL_ = 40; //�R���{�Ԋu

	int combo_counter_ = 0; //���̃R���{�̒i�K(���u)
	int combo_timer_ = 0; //�R���{��t����
	int comboM_counter_ = 0; //���̃R���{�̒i�K(�ߐ�)
	int comboM_timer_ = 0; //�R���{��t����
	int damaged_t_ = 0; //�m�b�N�o�b�N���[�V�����v���p
};

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

	const float SPRITE_W_ = 32.0f; //�摜�T�C�Y
	const float SPRITE_H_ = 48.0f;
	const float SIZE_ = 30.0f; //�����蔻��̑傫��
	const float SPEED_ = 2.0f; //�ړ����x
	const float DIR_ = 50.0f; //��~����
	int damaged_t_ = 0; //�m�b�N�o�b�N���[�V�����v���p
	int elapsed_ = 0; //���Ԍv���p
	const int INTERVAL_ = 60; //�U���Ԋu

	GameObj* target_ = nullptr;
};

//�_(�O��)
class Cloud :public GameObj {
public:
	Cloud(ScenePlay* scene);
	~Cloud() {}
	void update(float delta_time) override;

	const float SPRITE_W_ = 40.0f; //�摜�T�C�Y
	const float SPRITE_H_ = 25.0f;
	const float SIZE_ = 30.0f; //�����蔻��̑傫��
};

//���_
class Home :public GameObj {
public:
	Home(ScenePlay* scene);
	~Home() {}
	void update(float delta_time) override;

	const float SPRITE_W_ = 50.0f; //�摜�T�C�Y
	const float SPRITE_H_ = 50.0f;
	const float SIZE_ = 50.0f; //�����蔻��̑傫��
};