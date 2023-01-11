#pragma once
#include "../dxlib_ext/dxlib_ext.h"
class ScenePlay;
class AnimSprite3D;

class GameObj {
public:
	ScenePlay* scene_ = nullptr; //シーンの変数を取得するためのポインタ
	GameObj() {}
	virtual ~GameObj();
	virtual void update(float delta_time) {}

	// =========== 関数 ==========

	void updateSprite(float delta_time, float w, float h); //スプライトのアップデート
	void recievDamage(float power, int timer, int& counter); //被弾モーション
	void hitWall(GameObj* object); //壁判定

	// ========== 基本ステータス ==========

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

	dxe::Mesh* mesh_ = nullptr; //実体(メッシュ)
	AnimSprite3D* sprite_ = nullptr; //実体(スプライト)

	bool moving_ = true; //動作フラグ
	bool alive_ = true; //生存フラグ
	float size_ = 0.0f; //当たり判定用サイズ
	int hp_max_ = 100; //最大HP
	int hp_ = 100; //HP
	int prev_hp_ = hp_; //被弾判定用

	tnl::Vector3 looking_ = { 1,0,0 }; //見ている方向のベクトル
	tnl::Vector3 prev_pos_ = { 0,0,0 }; //補正用座標
};