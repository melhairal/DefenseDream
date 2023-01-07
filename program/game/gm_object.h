#pragma once
#include "../dxlib_ext/dxlib_ext.h"
class ScenePlay;
class AnimSprite3D;

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
	~Cloud() {}
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