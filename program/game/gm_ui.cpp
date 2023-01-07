#include "gm_ui.h"
#include "gm_stage.h"
#include "gm_object.h"
#include "scene/gm_scene_play.h"

Ui::Ui(ScenePlay* scene) {
	scene_ = scene;
	//フォント読み込み
	font_pop_16 = LoadFontDataToHandle("font/Novelpop16.dft", 0);
	font_pop_32 = LoadFontDataToHandle("font/Novelpop32.dft", 0);
	//hpバー読み込み
	hp_bar_[eBlue] = LoadGraph("graphics/gauge_bar_blue.png");
	hp_bar_[eYellow] = LoadGraph("graphics/gauge_bar_yellow.png");
	hp_bar_[eRed] = LoadGraph("graphics/gauge_bar_pink.png");
	hp_back_ = LoadGraph("graphics/gauge_background.png");
	hp_frame_ = LoadGraph("graphics/gauge_frame.png");
	//アイコン読み込み
	heart_ = LoadGraph("graphics/heart.png");
	star_ = LoadGraph("graphics/star.png");
	//時間読み込み
	time_icon_ = LoadGraph("graphics/time.png");
}

void Ui::update(float delta_time) {

}

void Ui::render() {
	//拠点HP表示
	DrawGauge(HOME_HP_X_, HOME_HP_Y_, 156, 8, 2, scene_->home_hp_, scene_->home_hp_max_, eYellow);
	//プレイヤーHP表示
	DrawGauge(PLAYER_GAUGE_X_, PLAYER_GAUGE_Y1_, 156, 8, 1.5f, scene_->player_->hp_, scene_->player_->hp_max_, eRed);
	DrawGauge(PLAYER_GAUGE_X_, PLAYER_GAUGE_Y2_, 156, 8, 1.5f, scene_->magic_, scene_->magic_max_, eBlue);
	DrawRotaGraph(PLAYER_ICON_X_, PLAYER_GAUGE_Y1_, 0.07f, 0, heart_, true);
	DrawRotaGraph(PLAYER_ICON_X_, PLAYER_GAUGE_Y2_, 0.1f, 0, star_, true);
	//制限時間表示
	DrawFormatStringToHandle(TIME_X_, TIME_Y_, 0, font_pop_32, "%d", scene_->stage_->time_);
	DrawRotaGraph(TIME_ICON_X_, TIME_ICON_Y_, 0.1f, 0, time_icon_, true);
}

void Ui::DrawGauge(int x, int y, int width, int height, double exrate, int num, int max, int color) {
	int gauge_x1 = x - width * exrate / 2;
	int gauge_y1 = y - height * exrate / 2;
	int gauge_y2 = y + height * exrate / 2;
	int gauge_x2 = gauge_x1 + width * exrate * ((double)num / (double)max);
	DrawRotaGraph(x, y, exrate, 0, hp_back_, true);
	DrawExtendGraph(gauge_x1, gauge_y1, gauge_x2, gauge_y2, hp_bar_[color], true);
	DrawRotaGraph(x, y, exrate, 0, hp_frame_, true);
}