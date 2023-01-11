#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../gm_object.h"
class ScenePlay;

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

	// =========== 関数 ==========

	void enemyMove(float delta_time);

	// ========== 基本ステータス ==========

	const float SPRITE_W_ = 32.0f; //画像サイズ
	const float SPRITE_H_ = 48.0f;
	const float SIZE_ = 30.0f; //当たり判定の大きさ
	const float SPEED_ = 2.0f; //移動速度
	const float DIR_ = 50.0f; //停止距離
	const float KNOCK_BACK_ = 6.0f; //ノックバック力
	const int KNOCK_BACK_TIMER_ = 12; //ノックバック時間
	const int INTERVAL_ = 60; //攻撃間隔

	int knock_back_counter_ = 0; //ノックバックモーション計測用
	int elapsed_ = 0; //時間計測用

	GameObj* target_ = nullptr;
};
