#include "../dxlib_ext/dxlib_ext.h"
class ScenePlay;

class StageBase {
public:
	ScenePlay* scene_ = nullptr; //�V�[���̕ϐ����擾���邽�߂̃|�C���^
	StageBase() {}
	virtual ~StageBase() {}
	virtual void update(float delta_time) {}
	void RandomPop(int num);

	int stageNum_ = 0; //�X�e�[�W�ԍ�
	int time_ = 60; //��������
	int elapsed_ = 0; //���ԃJ�E���g�p
	int enemyStock_ = 0; //�c��̓G�̐�

};

class Stage1 : public StageBase {
public:
	Stage1(ScenePlay* scene);
	~Stage1() {}
	void update(float delta_time) override;

	const int ENEMY_NUM_ = 30; //�o������G�̐�
};