#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../gm_object.h"
class ScenePlay;

class PlayerComboBase :public GameObj {
public:
	PlayerComboBase() {}
	virtual ~PlayerComboBase() {}
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
	~Combo1() {}
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