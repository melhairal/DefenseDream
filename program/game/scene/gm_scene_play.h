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

//------------------------------------------------------------------
//
// �Q�[���I�u�W�F�N�g
//
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
	~Cloud(){}
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

// ========== �U��(�R���{) ===========
class PlayerComboBase :public GameObj {
public:
	PlayerComboBase(){}
	virtual ~PlayerComboBase(){}
	virtual void update(float delta_time) {}

	void initialize(ScenePlay* scene, int magic, float size, float w, float h, float d); //��{���̐ݒ�
	void Attack(int damage); //�_���[�W����p�֐�

	int elapsed_ = 0;
	int frame_ = 0;
};

//�͈̓R���{
class Combo1 :public PlayerComboBase {
public:
	Combo1(ScenePlay* scene);
	~Combo1(){}
	void update(float delta_time) override;

	const float SPRITE_W_ = 120.0f; //�摜�T�C�Y
	const float SPRITE_H_ = 120.0f;
	const float DIS_ = 70.0f; //�v���C���[����̐����ʒu
	const float SIZE_ = 80.0f; //�����蔻��̑傫��
	const int DAMAGE_ = 20; //�U����
	const int MAGIC_ = 20; //�����
	const int FRAME_TIME_ = 4; //1�t���[���̍Đ�����
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
};

// ========== �G�̍U�� ==========
class EnemyComboBase :public GameObj {
public:
	EnemyComboBase() {}
	virtual ~EnemyComboBase() {}
	virtual void update(float delta_time) {}

	void initialize(ScenePlay* scene, GameObj* object, float size, float w, float h, float d); //��{���̐ݒ�
	void Attack(int damage, GameObj* player, GameObj* home); //�_���[�W����p�֐�

	GameObj* target_ = nullptr; //�U���Ώ�
	int elapsed_ = 0; //���Ԍv��
	int frame_ = 0; //���݂̃A�j���[�V�����t���[��
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
};

//------------------------------------------------------------------
//
// UI
//
class Ui {
public:
	ScenePlay* scene_ = nullptr; //�V�[���̕ϐ����擾���邽�߂̃|�C���^
	Ui(ScenePlay* scene);
	~Ui(){}
	void update(float delta_time);
	void render();
	void DrawGauge(int x, int y, int width, int height, double exrate, int num, int max, int color); //�σQ�[�W��`��

	enum {
		eBlue,
		eYellow,
		eRed
	};

	//�t�H���g
	int font_pop_16 = 0;
	int font_pop_32 = 0;
	//HP�Q�[�W
	int hp_bar_[3] = { 0,0,0 };
	int hp_back_ = 0;
	int hp_frame_ = 0;
	//�A�C�R��
	int heart_ = 0;
	int star_ = 0;
	//����
	int time_icon_ = 0;

	// ==== ���W ====
	//���_HP
	const int HOME_HP_X_ = DXE_WINDOW_WIDTH / 2;
	const int HOME_HP_Y_ = DXE_WINDOW_HEIGHT * 0.1f;
	//�v���C���[�Q�[�W
	const int PLAYER_GAUGE_X_ = DXE_WINDOW_WIDTH * 0.17f;
	const int PLAYER_GAUGE_Y1_ = DXE_WINDOW_HEIGHT * 0.9f;
	const int PLAYER_GAUGE_Y2_ = PLAYER_GAUGE_Y1_ + 30;
	const int PLAYER_ICON_X_ = PLAYER_GAUGE_X_ - 140;
	//����
	const int TIME_X_ = DXE_WINDOW_WIDTH / 2;
	const int TIME_Y_ = DXE_WINDOW_HEIGHT * 0.02f;
	const int TIME_ICON_X_ = 16 + TIME_X_ - 50;
	const int TIME_ICON_Y_ = 16 + TIME_Y_;


};


//------------------------------------------------------------------
//
// �X�e�[�W
//
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