#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../gm_object.h"
class ScenePlay;

class PlayerComboBase :public GameObj {
public:
	PlayerComboBase() {}
	virtual ~PlayerComboBase() {}
	virtual void update(float delta_time) {}

	// =========== �֐� ==========

	void initialize(ScenePlay* scene, int magic, float size, float w, float h, float d); //��{���̐ݒ�
	virtual void getImage() {} //�A�j���[�V�����摜�Z�b�g
	void animation(int frame_time); //�A�j���[�V�����̍X�V
	void Attack(int damage); //�_���[�W����p�֐�

	// ========== ��{�X�e�[�^�X ==========

	int elapsed_ = 0;
	int frame_ = 0;
	std::list<std::string> images_; //�A�j���[�V�������X�g
	std::list<std::string>::iterator it; //�A�j���[�V�������X�g�p�C�e���[�^
};

//�͈̓R���{
class Combo1 :public PlayerComboBase {
public:
	Combo1(ScenePlay* scene);
	~Combo1() {}
	void update(float delta_time) override;

	const float SPRITE_W_ = 120.0f; //�摜�T�C�Y
	const float SPRITE_H_ = 120.0f;
	const float DIS_ = 70.0f; //�v���C���[����̐����ʒu
	const float SIZE_ = 80.0f; //�����蔻��̑傫��
	const int DAMAGE_ = 20; //�U����
	const int MAGIC_ = 20; //�����
	const int FRAME_TIME_ = 4; //1�t���[���̍Đ�����

	void getImage() override {
		// ====================================
		//  �����ɃA�j���[�V�����̉摜����ׂ�
		// ====================================
		images_.emplace_back("graphics/combo/1/1_001.png");
		images_.emplace_back("graphics/combo/1/1_002.png");
		images_.emplace_back("graphics/combo/1/1_003.png");
		images_.emplace_back("graphics/combo/1/1_004.png");
		images_.emplace_back("graphics/combo/1/1_005.png");
		images_.emplace_back("graphics/combo/1/1_006.png");
		images_.emplace_back("graphics/combo/1/1_007.png");
		images_.emplace_back("graphics/combo/1/1_008.png");
		it = images_.begin();
	}
};

class Combo2 :public PlayerComboBase {
public:
	Combo2(ScenePlay* scene);
	~Combo2() {}
	void update(float delta_time) override;

	const float SPRITE_W_ = 120.0f; //�摜�T�C�Y
	const float SPRITE_H_ = 120.0f;
	const float DIS_ = 70.0f; //�v���C���[����̐����ʒu
	const float SIZE_ = 80.0f; //�����蔻��̑傫��
	const int DAMAGE_ = 40; //�U����
	const int MAGIC_ = 30; //�����
	const int FRAME_TIME_ = 4; //1�t���[���̍Đ�����

	void getImage() override {
		// ====================================
		//  �����ɃA�j���[�V�����̉摜����ׂ�
		// ====================================
		images_.emplace_back("graphics/combo/2/2_001.png");
		images_.emplace_back("graphics/combo/2/2_002.png");
		images_.emplace_back("graphics/combo/2/2_003.png");
		images_.emplace_back("graphics/combo/2/2_004.png");
		images_.emplace_back("graphics/combo/2/2_005.png");
		images_.emplace_back("graphics/combo/2/2_006.png");
		images_.emplace_back("graphics/combo/2/2_007.png");
		images_.emplace_back("graphics/combo/2/2_008.png");
		images_.emplace_back("graphics/combo/2/2_009.png");
		images_.emplace_back("graphics/combo/2/2_010.png");
		it = images_.begin();
	}
};

class Combo3 :public PlayerComboBase {
public:
	Combo3(ScenePlay* scene);
	~Combo3() {}
	void update(float delta_time) override;

	const float SPRITE_W_ = 360.0f; //�摜�T�C�Y
	const float SPRITE_H_ = 120.0f;
	const float DIS_ = 70.0f; //�v���C���[����̐����ʒu
	const float SIZE_ = 120.0f; //�����蔻��̑傫��
	const int DAMAGE_ = 60; //�U����
	const int MAGIC_ = 40; //�����
	const int FRAME_TIME_ = 4; //1�t���[���̍Đ�����

	void getImage() override {
		// ====================================
		//  �����ɃA�j���[�V�����̉摜����ׂ�
		// ====================================
		images_.emplace_back("graphics/combo/3/3_001.png");
		images_.emplace_back("graphics/combo/3/3_002.png");
		images_.emplace_back("graphics/combo/3/3_003.png");
		images_.emplace_back("graphics/combo/3/3_004.png");
		images_.emplace_back("graphics/combo/3/3_005.png");
		images_.emplace_back("graphics/combo/3/3_006.png");
		images_.emplace_back("graphics/combo/3/3_007.png");
		images_.emplace_back("graphics/combo/3/3_008.png");
		images_.emplace_back("graphics/combo/3/3_009.png");
		images_.emplace_back("graphics/combo/3/3_010.png");
		images_.emplace_back("graphics/combo/3/3_011.png");
		images_.emplace_back("graphics/combo/3/3_012.png");
		images_.emplace_back("graphics/combo/3/3_013.png");
		images_.emplace_back("graphics/combo/3/3_014.png");
		images_.emplace_back("graphics/combo/3/3_015.png");
		images_.emplace_back("graphics/combo/3/3_016.png");
		images_.emplace_back("graphics/combo/3/3_017.png");
		images_.emplace_back("graphics/combo/3/3_018.png");
		it = images_.begin();
	}
};

//�ߐڃR���{
class ComboM1 :public PlayerComboBase {
public:
	ComboM1(ScenePlay* scene);
	~ComboM1() {}
	void update(float delta_time) override;

	const float SPRITE_W_ = 180.0f; //�摜�T�C�Y
	const float SPRITE_H_ = 120.0f;
	const float DIS_ = 40.0f; //�v���C���[����̐����ʒu
	const float SIZE_ = 40.0f; //�����蔻��̑傫��
	const int DAMAGE_ = 20; //�U����
	const int MAGIC_ = 10; //�����
	const int FRAME_TIME_ = 1; //1�t���[���̍Đ�����

	void getImage() override {
		// ====================================
		//  �����ɃA�j���[�V�����̉摜����ׂ�
		// ====================================
		images_.emplace_back("graphics/combo/m1/1_001.png");
		images_.emplace_back("graphics/combo/m1/1_002.png");
		images_.emplace_back("graphics/combo/m1/1_003.png");
		images_.emplace_back("graphics/combo/m1/1_004.png");
		images_.emplace_back("graphics/combo/m1/1_005.png");
		images_.emplace_back("graphics/combo/m1/1_006.png");
		images_.emplace_back("graphics/combo/m1/1_007.png");
		images_.emplace_back("graphics/combo/m1/1_008.png");
		images_.emplace_back("graphics/combo/m1/1_009.png");
		it = images_.begin();
	}
};

class ComboM2 :public PlayerComboBase {
public:
	ComboM2(ScenePlay* scene);
	~ComboM2() {}
	void update(float delta_time) override;

	const float SPRITE_W_ = 180.0f; //�摜�T�C�Y
	const float SPRITE_H_ = 120.0f;
	const float DIS_ = 40.0f; //�v���C���[����̐����ʒu
	const float SIZE_ = 40.0f; //�����蔻��̑傫��
	const int DAMAGE_ = 30; //�U����
	const int MAGIC_ = 20; //�����
	const int FRAME_TIME_ = 1; //1�t���[���̍Đ�����

	void getImage() override {
		// ====================================
		//  �����ɃA�j���[�V�����̉摜����ׂ�
		// ====================================
		images_.emplace_back("graphics/combo/m2/2_001.png");
		images_.emplace_back("graphics/combo/m2/2_002.png");
		images_.emplace_back("graphics/combo/m2/2_003.png");
		images_.emplace_back("graphics/combo/m2/2_004.png");
		images_.emplace_back("graphics/combo/m2/2_005.png");
		images_.emplace_back("graphics/combo/m2/2_006.png");
		images_.emplace_back("graphics/combo/m2/2_007.png");
		images_.emplace_back("graphics/combo/m2/2_008.png");
		images_.emplace_back("graphics/combo/m2/2_009.png");
		images_.emplace_back("graphics/combo/m3/3_001.png");
		images_.emplace_back("graphics/combo/m3/3_002.png");
		images_.emplace_back("graphics/combo/m3/3_003.png");
		images_.emplace_back("graphics/combo/m3/3_004.png");
		images_.emplace_back("graphics/combo/m3/3_005.png");
		images_.emplace_back("graphics/combo/m3/3_006.png");
		images_.emplace_back("graphics/combo/m3/3_007.png");
		images_.emplace_back("graphics/combo/m3/3_008.png");
		images_.emplace_back("graphics/combo/m3/3_009.png");
		it = images_.begin();
	}
};

class ComboM3 :public PlayerComboBase {
public:
	ComboM3(ScenePlay* scene);
	~ComboM3() {}
	void update(float delta_time) override;

	const float SPRITE_W_ = 180.0f; //�摜�T�C�Y
	const float SPRITE_H_ = 120.0f;
	const float DIS_ = 40.0f; //�v���C���[����̐����ʒu
	const float SIZE_ = 40.0f; //�����蔻��̑傫��
	const int DAMAGE_ = 20; //�U����
	const int MAGIC_ = 20; //�����
	const int FRAME_TIME_ = 1; //1�t���[���̍Đ�����

	void getImage() override {
		// ====================================
		//  �����ɃA�j���[�V�����̉摜����ׂ�
		// ====================================
		images_.emplace_back("graphics/combo/m1/1_001.png");
		images_.emplace_back("graphics/combo/m1/1_002.png");
		images_.emplace_back("graphics/combo/m1/1_003.png");
		images_.emplace_back("graphics/combo/m1/1_004.png");
		images_.emplace_back("graphics/combo/m1/1_005.png");
		images_.emplace_back("graphics/combo/m1/1_006.png");
		images_.emplace_back("graphics/combo/m1/1_007.png");
		images_.emplace_back("graphics/combo/m1/1_008.png");
		images_.emplace_back("graphics/combo/m1/1_009.png");
		images_.emplace_back("graphics/combo/m2/2_001.png");
		images_.emplace_back("graphics/combo/m2/2_002.png");
		images_.emplace_back("graphics/combo/m2/2_003.png");
		images_.emplace_back("graphics/combo/m2/2_004.png");
		images_.emplace_back("graphics/combo/m2/2_005.png");
		images_.emplace_back("graphics/combo/m2/2_006.png");
		images_.emplace_back("graphics/combo/m2/2_007.png");
		images_.emplace_back("graphics/combo/m2/2_008.png");
		images_.emplace_back("graphics/combo/m2/2_009.png");
		images_.emplace_back("graphics/combo/m1/1_001.png");
		images_.emplace_back("graphics/combo/m1/1_002.png");
		images_.emplace_back("graphics/combo/m1/1_003.png");
		images_.emplace_back("graphics/combo/m1/1_004.png");
		images_.emplace_back("graphics/combo/m1/1_005.png");
		images_.emplace_back("graphics/combo/m1/1_006.png");
		images_.emplace_back("graphics/combo/m1/1_007.png");
		images_.emplace_back("graphics/combo/m1/1_008.png");
		images_.emplace_back("graphics/combo/m1/1_009.png");
		it = images_.begin();
	}
};

class ComboM4 :public PlayerComboBase {
public:
	ComboM4(ScenePlay* scene);
	~ComboM4() {}
	void update(float delta_time) override;

	const float SPRITE_W_ = 160.0f; //�摜�T�C�Y
	const float SPRITE_H_ = 120.0f;
	const float DIS_ = 40.0f; //�v���C���[����̐����ʒu
	const float SIZE_ = 40.0f; //�����蔻��̑傫��
	const int DAMAGE_ = 20; //�U����
	const int MAGIC_ = 40; //�����
	const int FRAME_TIME_ = 1; //1�t���[���̍Đ�����

	void getImage() override {
		// ====================================
		//  �����ɃA�j���[�V�����̉摜����ׂ�
		// ====================================
		images_.emplace_back("graphics/combo/m5/5_001.png");
		images_.emplace_back("graphics/combo/m5/5_002.png");
		images_.emplace_back("graphics/combo/m5/5_003.png");
		images_.emplace_back("graphics/combo/m5/5_004.png");
		images_.emplace_back("graphics/combo/m5/5_005.png");
		images_.emplace_back("graphics/combo/m5/5_006.png");
		images_.emplace_back("graphics/combo/m5/5_007.png");
		images_.emplace_back("graphics/combo/m5/5_008.png");
		images_.emplace_back("graphics/combo/m5/5_009.png");
		images_.emplace_back("graphics/combo/m5/5_010.png");
		images_.emplace_back("graphics/combo/m5/5_011.png");
		images_.emplace_back("graphics/combo/m5/5_012.png");
		images_.emplace_back("graphics/combo/m5/5_013.png");
		images_.emplace_back("graphics/combo/m5/5_014.png");
		images_.emplace_back("graphics/combo/m5/5_015.png");
		images_.emplace_back("graphics/combo/m5/5_016.png");
		images_.emplace_back("graphics/combo/m5/5_017.png");
		images_.emplace_back("graphics/combo/m5/5_018.png");
		images_.emplace_back("graphics/combo/m5/5_019.png");
		images_.emplace_back("graphics/combo/m5/5_020.png");
		images_.emplace_back("graphics/combo/m5/5_021.png");
		images_.emplace_back("graphics/combo/m5/5_022.png");
		images_.emplace_back("graphics/combo/m5/5_023.png");
		it = images_.begin();
	}
};

//�ڋ߃R���{
class ComboS1 :public PlayerComboBase {
public:
	ComboS1(ScenePlay* scene);
	~ComboS1() {}
	void update(float delta_time) override;

	const float SPRITE_W_ = 320.0f; //�摜�T�C�Y
	const float SPRITE_H_ = 240.0f;
	const float DIS_ = 120.0f; //�v���C���[����̐����ʒu
	const float SIZE_ = 100.0f; //�����蔻��̑傫��
	const int DAMAGE_ = 60; //�U����
	const int MAGIC_ = 80; //�����
	const int FRAME_TIME_ = 1; //1�t���[���̍Đ�����

	void getImage() override{
		// ====================================
		//  �����ɃA�j���[�V�����̉摜����ׂ�
		// ====================================
		images_.emplace_back("graphics/combo/s1/1_001.png");
		images_.emplace_back("graphics/combo/s1/1_002.png");
		images_.emplace_back("graphics/combo/s1/1_003.png");
		images_.emplace_back("graphics/combo/s1/1_004.png");
		images_.emplace_back("graphics/combo/s1/1_005.png");
		images_.emplace_back("graphics/combo/s1/1_006.png");
		images_.emplace_back("graphics/combo/s1/1_007.png");
		images_.emplace_back("graphics/combo/s1/1_008.png");
		images_.emplace_back("graphics/combo/s1/1_009.png");
		it = images_.begin();
	}
};

// ========== �G�̍U�� ==========
class EnemyComboBase :public GameObj {
public:
	EnemyComboBase() {}
	virtual ~EnemyComboBase() {}
	virtual void update(float delta_time) {}

	// =========== �֐� ==========

	void initialize(ScenePlay* scene, GameObj* object, float size, float w, float h, float d); //��{���̐ݒ�
	virtual void getImage() {} //�A�j���[�V�����摜�Z�b�g
	void animation(int frame_time); //�A�j���[�V�����̍X�V
	void Attack(int damage, GameObj* player, GameObj* home); //�_���[�W����p�֐�

	// ========== ��{�X�e�[�^�X ==========

	GameObj* target_ = nullptr; //�U���Ώ�
	int elapsed_ = 0; //���Ԍv��
	int frame_ = 0; //���݂̃A�j���[�V�����t���[��
	std::list<std::string> images_; //�A�j���[�V�������X�g
	std::list<std::string>::iterator it; //�A�j���[�V�������X�g�p�C�e���[�^
};

//�G�Ђ������U��
class ComboE1 :public EnemyComboBase {
public:
	ComboE1(ScenePlay* scene, GameObj* object);
	~ComboE1() {}
	void update(float delta_time) override;

	const float SPRITE_W_ = 80.0f; //�摜�T�C�Y
	const float SPRITE_H_ = 80.0f;
	const float DIS_ = 40.0f; //�X�v���C�g����̐����ʒu
	const float SIZE_ = 40.0f; //�����蔻��̑傫��
	const int DAMAGE_ = 10; //�U����
	const int FRAME_TIME_ = 1; //1�t���[���̍Đ�����

	void getImage() override {
		// ====================================
		//  �����ɃA�j���[�V�����̉摜����ׂ�
		// ====================================
		images_.emplace_back("graphics/combo/e1/1_001.png");
		images_.emplace_back("graphics/combo/e1/1_002.png");
		images_.emplace_back("graphics/combo/e1/1_003.png");
		images_.emplace_back("graphics/combo/e1/1_004.png");
		images_.emplace_back("graphics/combo/e1/1_005.png");
		images_.emplace_back("graphics/combo/e1/1_006.png");
		images_.emplace_back("graphics/combo/e1/1_007.png");
		images_.emplace_back("graphics/combo/e1/1_008.png");
		it = images_.begin();
	}
};