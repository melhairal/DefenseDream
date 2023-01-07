#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../gm_object.h"
class ScenePlay;

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