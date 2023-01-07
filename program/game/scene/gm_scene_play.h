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

	GmCamera* camera_ = nullptr; //ゲームカメラ
	GameObj* player_ = nullptr; //プレイヤーを示すポインタ
	GameObj* home_ = nullptr; //拠点を示すポインタ

	int backGround_ = 0; //マップ背景
	dxe::Mesh* field_ = nullptr; //マップ床
	std::list<GameObj*> objects_; //オブジェクトリスト
	Ui* ui_ = nullptr; //UI
	StageBase* stage_ = nullptr; //ステージ

	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

	const float FIELD_H_ = -30.0f; //地面の高さ座標
	const int FIELD_R_ = 1000; //フィールドの半径
	const int CLOUD_DENSITY_ = 200; //雲の密度

	// ==== ゲーム内ステータス ====
	int home_hp_ = 500; //拠点HP
	int home_hp_max_ = home_hp_; //拠点最大HP

	int magic_ = 200; //MP
	int magic_prev_ = 0; //直前のMP(自然回復計算用)
	int magic_timer_ = 0; //自然回復時間カウント
	const int MAGIC_TIME_ = 100; //自然回復がはじまる時間
	int magic_max_ = magic_; //最大MP
};

//------------------------------------------------------------------
//
// UI
//
class Ui {
public:
	ScenePlay* scene_ = nullptr; //シーンの変数を取得するためのポインタ
	Ui(ScenePlay* scene);
	~Ui(){}
	void update(float delta_time);
	void render();
	void DrawGauge(int x, int y, int width, int height, double exrate, int num, int max, int color); //可変ゲージを描画

	enum {
		eBlue,
		eYellow,
		eRed
	};

	//フォント
	int font_pop_16 = 0;
	int font_pop_32 = 0;
	//HPゲージ
	int hp_bar_[3] = { 0,0,0 };
	int hp_back_ = 0;
	int hp_frame_ = 0;
	//アイコン
	int heart_ = 0;
	int star_ = 0;
	//時間
	int time_icon_ = 0;

	// ==== 座標 ====
	//拠点HP
	const int HOME_HP_X_ = DXE_WINDOW_WIDTH / 2;
	const int HOME_HP_Y_ = DXE_WINDOW_HEIGHT * 0.1f;
	//プレイヤーゲージ
	const int PLAYER_GAUGE_X_ = DXE_WINDOW_WIDTH * 0.17f;
	const int PLAYER_GAUGE_Y1_ = DXE_WINDOW_HEIGHT * 0.9f;
	const int PLAYER_GAUGE_Y2_ = PLAYER_GAUGE_Y1_ + 30;
	const int PLAYER_ICON_X_ = PLAYER_GAUGE_X_ - 140;
	//時間
	const int TIME_X_ = DXE_WINDOW_WIDTH / 2;
	const int TIME_Y_ = DXE_WINDOW_HEIGHT * 0.02f;
	const int TIME_ICON_X_ = 16 + TIME_X_ - 50;
	const int TIME_ICON_Y_ = 16 + TIME_Y_;


};


//------------------------------------------------------------------
//
// ステージ
//
class StageBase {
public:
	ScenePlay* scene_ = nullptr; //シーンの変数を取得するためのポインタ
	StageBase() {}
	virtual ~StageBase() {}
	virtual void update(float delta_time) {}
	void RandomPop(int num);

	int stageNum_ = 0; //ステージ番号
	int time_ = 60; //制限時間
	int elapsed_ = 0; //時間カウント用
	int enemyStock_ = 0; //残りの敵の数

};

class Stage1 : public StageBase {
public:
	Stage1(ScenePlay* scene);
	~Stage1() {}
	void update(float delta_time) override;

	const int ENEMY_NUM_ = 30; //出現する敵の数
};