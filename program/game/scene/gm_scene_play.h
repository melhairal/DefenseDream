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
// ゲームオブジェクト
//
class GameObj {
public:
	ScenePlay* scene_ = nullptr; //シーンの変数を取得するためのポインタ
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
	int tag_ = GameObj::eNone; //オブジェクトを識別するタグ
	int id_ = -1; //同じタグ内での識別用の番号

	dxe::Mesh* mesh_ = nullptr;
	AnimSprite3D* sprite_ = nullptr;

	bool moving_ = true; //動作フラグ
	bool alive_ = true; //生存フラグ
	float size_ = 0.0f; //当たり判定用サイズ
	int hp_max_ = 100; //最大HP
	int hp_ = 100; //HP
	int prev_hp_ = hp_; //被弾判定用

	tnl::Vector3 looking_ = { 1,0,0 }; //見ている方向のベクトル
	tnl::Vector3 prev_pos_ = { 0,0,0 }; //補正用座標
};

//プレイヤー
class Player :public GameObj {
public:
	Player(ScenePlay* scene);
	~Player() {}
	void update(float delta_time) override;

	const float SPRITE_W_ = 32.0f; //画像サイズ
	const float SPRITE_H_ = 48.0f;
	const float SIZE_ = 30.0f; //当たり判定の大きさ
	const float SPEED_ = 3.0f; //移動速度
	const float SPRINT_ = 5.0f; //ダッシュ速度
	const int COMBO_RECEPTION_ = 100; //コンボ受付猶予
	const int COMBO_INTERVAL_ = 40; //コンボ間隔

	int combo_counter_ = 0; //今のコンボの段階(遠隔)
	int combo_timer_ = 0; //コンボ受付時間
	int comboM_counter_ = 0; //今のコンボの段階(近接)
	int comboM_timer_ = 0; //コンボ受付時間
	int damaged_t_ = 0; //ノックバックモーション計測用
};

//敵(メッシュ)
class Enemy :public GameObj {
public:
	Enemy(ScenePlay* scene, const tnl::Vector3& pos);
	~Enemy() {}
	void update(float delta_time) override;

	const float SIZE_ = 30.0f; //当たり判定の大きさ
	const float SPEED_ = 2.0f; //移動速度

	GameObj* target_ = nullptr;
};

//敵(スプライト)
class EnemySprite :public GameObj {
public:
	EnemySprite(ScenePlay* scene, const tnl::Vector3& pos);
	~EnemySprite() {}
	void update(float delta_time) override;

	const float SPRITE_W_ = 32.0f; //画像サイズ
	const float SPRITE_H_ = 48.0f;
	const float SIZE_ = 30.0f; //当たり判定の大きさ
	const float SPEED_ = 2.0f; //移動速度
	const float DIR_ = 50.0f; //停止距離
	int damaged_t_ = 0; //ノックバックモーション計測用
	int elapsed_ = 0; //時間計測用
	const int INTERVAL_ = 60; //攻撃間隔

	GameObj* target_ = nullptr;
};

//雲(外周)
class Cloud :public GameObj {
public:
	Cloud(ScenePlay* scene);
	~Cloud(){}
	void update(float delta_time) override;

	const float SPRITE_W_ = 40.0f; //画像サイズ
	const float SPRITE_H_ = 25.0f;
	const float SIZE_ = 30.0f; //当たり判定の大きさ
};

//拠点
class Home :public GameObj {
public:
	Home(ScenePlay* scene);
	~Home() {}
	void update(float delta_time) override;

	const float SPRITE_W_ = 50.0f; //画像サイズ
	const float SPRITE_H_ = 50.0f;
	const float SIZE_ = 50.0f; //当たり判定の大きさ
};

// ========== 攻撃(コンボ) ===========
class PlayerComboBase :public GameObj {
public:
	PlayerComboBase(){}
	virtual ~PlayerComboBase(){}
	virtual void update(float delta_time) {}

	void initialize(ScenePlay* scene, int magic, float size, float w, float h, float d); //基本情報の設定
	void Attack(int damage); //ダメージ判定用関数

	int elapsed_ = 0;
	int frame_ = 0;
};

//範囲コンボ
class Combo1 :public PlayerComboBase {
public:
	Combo1(ScenePlay* scene);
	~Combo1(){}
	void update(float delta_time) override;

	const float SPRITE_W_ = 120.0f; //画像サイズ
	const float SPRITE_H_ = 120.0f;
	const float DIS_ = 70.0f; //プレイヤーからの生成位置
	const float SIZE_ = 80.0f; //当たり判定の大きさ
	const int DAMAGE_ = 20; //攻撃力
	const int MAGIC_ = 20; //消費魔力
	const int FRAME_TIME_ = 4; //1フレームの再生時間
};

class Combo2 :public PlayerComboBase {
public:
	Combo2(ScenePlay* scene);
	~Combo2() {}
	void update(float delta_time) override;

	const float SPRITE_W_ = 120.0f; //画像サイズ
	const float SPRITE_H_ = 120.0f;
	const float DIS_ = 70.0f; //プレイヤーからの生成位置
	const float SIZE_ = 80.0f; //当たり判定の大きさ
	const int DAMAGE_ = 40; //攻撃力
	const int MAGIC_ = 30; //消費魔力
	const int FRAME_TIME_ = 4; //1フレームの再生時間
};

class Combo3 :public PlayerComboBase {
public:
	Combo3(ScenePlay* scene);
	~Combo3() {}
	void update(float delta_time) override;

	const float SPRITE_W_ = 360.0f; //画像サイズ
	const float SPRITE_H_ = 120.0f;
	const float DIS_ = 70.0f; //プレイヤーからの生成位置
	const float SIZE_ = 120.0f; //当たり判定の大きさ
	const int DAMAGE_ = 60; //攻撃力
	const int MAGIC_ = 40; //消費魔力
	const int FRAME_TIME_ = 4; //1フレームの再生時間
};

//近接コンボ
class ComboM1 :public PlayerComboBase {
public:
	ComboM1(ScenePlay* scene);
	~ComboM1() {}
	void update(float delta_time) override;

	const float SPRITE_W_ = 180.0f; //画像サイズ
	const float SPRITE_H_ = 120.0f;
	const float DIS_ = 40.0f; //プレイヤーからの生成位置
	const float SIZE_ = 40.0f; //当たり判定の大きさ
	const int DAMAGE_ = 20; //攻撃力
	const int MAGIC_ = 10; //消費魔力
	const int FRAME_TIME_ = 1; //1フレームの再生時間
};

class ComboM2 :public PlayerComboBase {
public:
	ComboM2(ScenePlay* scene);
	~ComboM2() {}
	void update(float delta_time) override;

	const float SPRITE_W_ = 180.0f; //画像サイズ
	const float SPRITE_H_ = 120.0f;
	const float DIS_ = 40.0f; //プレイヤーからの生成位置
	const float SIZE_ = 40.0f; //当たり判定の大きさ
	const int DAMAGE_ = 30; //攻撃力
	const int MAGIC_ = 20; //消費魔力
	const int FRAME_TIME_ = 1; //1フレームの再生時間
};

class ComboM3 :public PlayerComboBase {
public:
	ComboM3(ScenePlay* scene);
	~ComboM3() {}
	void update(float delta_time) override;

	const float SPRITE_W_ = 180.0f; //画像サイズ
	const float SPRITE_H_ = 120.0f;
	const float DIS_ = 40.0f; //プレイヤーからの生成位置
	const float SIZE_ = 40.0f; //当たり判定の大きさ
	const int DAMAGE_ = 20; //攻撃力
	const int MAGIC_ = 20; //消費魔力
	const int FRAME_TIME_ = 1; //1フレームの再生時間
};

class ComboM4 :public PlayerComboBase {
public:
	ComboM4(ScenePlay* scene);
	~ComboM4() {}
	void update(float delta_time) override;

	const float SPRITE_W_ = 160.0f; //画像サイズ
	const float SPRITE_H_ = 120.0f;
	const float DIS_ = 40.0f; //プレイヤーからの生成位置
	const float SIZE_ = 40.0f; //当たり判定の大きさ
	const int DAMAGE_ = 20; //攻撃力
	const int MAGIC_ = 40; //消費魔力
	const int FRAME_TIME_ = 1; //1フレームの再生時間
};

//接近コンボ
class ComboS1 :public PlayerComboBase {
public:
	ComboS1(ScenePlay* scene);
	~ComboS1() {}
	void update(float delta_time) override;

	const float SPRITE_W_ = 320.0f; //画像サイズ
	const float SPRITE_H_ = 240.0f;
	const float DIS_ = 120.0f; //プレイヤーからの生成位置
	const float SIZE_ = 100.0f; //当たり判定の大きさ
	const int DAMAGE_ = 60; //攻撃力
	const int MAGIC_ = 80; //消費魔力
	const int FRAME_TIME_ = 1; //1フレームの再生時間
};

// ========== 敵の攻撃 ==========
class EnemyComboBase :public GameObj {
public:
	EnemyComboBase() {}
	virtual ~EnemyComboBase() {}
	virtual void update(float delta_time) {}

	void initialize(ScenePlay* scene, GameObj* object, float size, float w, float h, float d); //基本情報の設定
	void Attack(int damage, GameObj* player, GameObj* home); //ダメージ判定用関数

	GameObj* target_ = nullptr; //攻撃対象
	int elapsed_ = 0; //時間計測
	int frame_ = 0; //現在のアニメーションフレーム
};

//敵ひっかき攻撃
class ComboE1 :public EnemyComboBase {
public:
	ComboE1(ScenePlay* scene, GameObj* object);
	~ComboE1() {}
	void update(float delta_time) override;

	const float SPRITE_W_ = 80.0f; //画像サイズ
	const float SPRITE_H_ = 80.0f;
	const float DIS_ = 40.0f; //スプライトからの生成位置
	const float SIZE_ = 40.0f; //当たり判定の大きさ
	const int DAMAGE_ = 10; //攻撃力
	const int FRAME_TIME_ = 1; //1フレームの再生時間
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