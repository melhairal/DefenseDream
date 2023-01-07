#pragma once
#include "../dxlib_ext/dxlib_ext.h"
class ScenePlay;

class Ui {
public:
	ScenePlay* scene_ = nullptr; //�V�[���̕ϐ����擾���邽�߂̃|�C���^
	Ui(ScenePlay* scene);
	~Ui() {}
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

	// ========== ���W ==========
	
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
