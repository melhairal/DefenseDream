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