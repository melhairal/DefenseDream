#pragma once
#include "../dxlib_ext/dxlib_ext.h"
class ScenePlay;
class AnimSprite3D;

class GameObj {
public:
	ScenePlay* scene_ = nullptr; //�V�[���̕ϐ����擾���邽�߂̃|�C���^
	GameObj() {}
	virtual ~GameObj();
	virtual void update(float delta_time) {}

	// =========== �֐� ==========

	void updateSprite(float delta_time, float w, float h); //�X�v���C�g�̃A�b�v�f�[�g
	void recievDamage(float power, int timer, int& counter); //��e���[�V����
	void hitWall(GameObj* object); //�ǔ���

	// ========== ��{�X�e�[�^�X ==========

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

	dxe::Mesh* mesh_ = nullptr; //����(���b�V��)
	AnimSprite3D* sprite_ = nullptr; //����(�X�v���C�g)

	bool moving_ = true; //����t���O
	bool alive_ = true; //�����t���O
	float size_ = 0.0f; //�����蔻��p�T�C�Y
	int hp_max_ = 100; //�ő�HP
	int hp_ = 100; //HP
	int prev_hp_ = hp_; //��e����p

	tnl::Vector3 looking_ = { 1,0,0 }; //���Ă�������̃x�N�g��
	tnl::Vector3 prev_pos_ = { 0,0,0 }; //�␳�p���W
};