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

	GmCamera* camera_ = nullptr; //�Q�[���J����
	GameObj* player_ = nullptr; //�v���C���[�������|�C���^
	GameObj* home_ = nullptr; //���_�������|�C���^

	int backGround_ = 0; //�}�b�v�w�i
	dxe::Mesh* field_ = nullptr; //�}�b�v��
	std::list<GameObj*> objects_; //�I�u�W�F�N�g���X�g
	Ui* ui_ = nullptr; //UI
	StageBase* stage_ = nullptr; //�X�e�[�W

	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

	const float FIELD_H_ = -30.0f; //�n�ʂ̍������W
	const int FIELD_R_ = 1000; //�t�B�[���h�̔��a
	const int CLOUD_DENSITY_ = 200; //�_�̖��x

	// ==== �Q�[�����X�e�[�^�X ====
	int home_hp_ = 500; //���_HP
	int home_hp_max_ = home_hp_; //���_�ő�HP

	int magic_ = 200; //MP
	int magic_prev_ = 0; //���O��MP(���R�񕜌v�Z�p)
	int magic_timer_ = 0; //���R�񕜎��ԃJ�E���g
	const int MAGIC_TIME_ = 100; //���R�񕜂��͂��܂鎞��
	int magic_max_ = magic_; //�ő�MP
};