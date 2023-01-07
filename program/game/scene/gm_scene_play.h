#pragma once
#include "gm_scene_base.h"

class GmCamera;
class AnimSprite3D;
class GameObj;
class Ui;
class StageBase;

class ScenePlay : public SceneBase {
public:
	ScenePlay() {}
	~ScenePlay();

	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

	// =========== �֐� ==========
	
	void updateObject(float delta_time); //�I�u�W�F�N�g����
	void updateCamera(float delta_time); //�J��������

	// ========== ��{�X�e�[�^�X ==========

	GmCamera* camera_ = nullptr; //�Q�[���J����
	GameObj* player_ = nullptr; //�v���C���[�������|�C���^
	GameObj* home_ = nullptr; //���_�������|�C���^

	int backGround_ = 0; //�}�b�v�w�i
	dxe::Mesh* field_ = nullptr; //�}�b�v��
	std::list<GameObj*> objects_; //�I�u�W�F�N�g���X�g
	Ui* ui_ = nullptr; //UI
	StageBase* stage_ = nullptr; //�X�e�[�W

	const float FIELD_H_ = -30.0f; //�n�ʂ̍������W
	const int FIELD_R_ = 1000; //�t�B�[���h�̔��a
	const int CLOUD_DENSITY_ = 200; //�_�̖��x

	const float CAM_ROT_SPEED_ = 1.0f; //�J������]���x
	const float CAM_ZOOM_SPEED_ = 30.0f; //�J�����Y�[�����x
	const float CAM_ZOOM_MAX_ = 400.0f; //�J�����Y�[������
	const float CAM_ZOOM_MIN_ = 200.0f; //�J�����Y�[�����

	// ========= �Q�[�����X�e�[�^�X =========

	int home_hp_ = 500; //���_HP
	int home_hp_max_ = home_hp_; //���_�ő�HP

	int magic_ = 200; //MP
	int magic_prev_ = 0; //���O��MP(���R�񕜌v�Z�p)
	int magic_timer_ = 0; //���R�񕜎��ԃJ�E���g
	const int MAGIC_TIME_ = 100; //���R�񕜂��͂��܂鎞��
	int magic_max_ = magic_; //�ő�MP
};