#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../gm_object.h"
class ScenePlay;

//�v���C���[
class Player :public GameObj {
public:
	Player(ScenePlay* scene);
	~Player() {}
	void update(float delta_time) override;

	// =========== �֐� ==========

	void playerMove(); //�ړ�����
	void updateMouse(float delta_time); //�}�E�X���쐧��
	void comboLeft(); //�R���{���N���b�N
	void comboRight(); //�R���{�E�N���b�N
	void comboShiht(); //�R���{�V�t�g�L�[
	void healMagic(); //���͎��R��

	// ========== ��{�X�e�[�^�X ==========

	const float SPRITE_W_ = 32.0f; //�摜�T�C�Y
	const float SPRITE_H_ = 48.0f;
	const float SIZE_ = 30.0f; //�����蔻��̑傫��
	const float SPEED_ = 3.0f; //�ړ����x
	const float SPRINT_ = 5.0f; //�_�b�V�����x
	const int COMBO_RECEPTION_ = 100; //�R���{��t�P�\
	const int COMBO_INTERVAL_ = 40; //�R���{�Ԋu
	const float KNOCK_BACK_ = 6.0f; //�m�b�N�o�b�N��
	const int KNOCK_BACK_TIMER_ = 12; //�m�b�N�o�b�N����

	int combo_counter_ = 0; //���̃R���{�̒i�K(���u)
	int combo_timer_ = 0; //�R���{��t����
	int comboM_counter_ = 0; //���̃R���{�̒i�K(�ߐ�)
	int comboM_timer_ = 0; //�R���{��t����
	int knock_back_counter_ = 0; //�m�b�N�o�b�N���[�V�����v���p
};