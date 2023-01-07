#pragma once
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

	void initialzie() override;
	void update(float delta_time) override;
	void render() override;

	// =========== 関数 ==========
	
	void updateObject(float delta_time); //オブジェクト制御
	void updateCamera(float delta_time); //カメラ制御

	// ========== 基本ステータス ==========

	GmCamera* camera_ = nullptr; //ゲームカメラ
	GameObj* player_ = nullptr; //プレイヤーを示すポインタ
	GameObj* home_ = nullptr; //拠点を示すポインタ

	int backGround_ = 0; //マップ背景
	dxe::Mesh* field_ = nullptr; //マップ床
	std::list<GameObj*> objects_; //オブジェクトリスト
	Ui* ui_ = nullptr; //UI
	StageBase* stage_ = nullptr; //ステージ

	const float FIELD_H_ = -30.0f; //地面の高さ座標
	const int FIELD_R_ = 1000; //フィールドの半径
	const int CLOUD_DENSITY_ = 200; //雲の密度

	const float CAM_ROT_SPEED_ = 1.0f; //カメラ回転速度
	const float CAM_ZOOM_SPEED_ = 30.0f; //カメラズーム速度
	const float CAM_ZOOM_MAX_ = 400.0f; //カメラズーム下限
	const float CAM_ZOOM_MIN_ = 200.0f; //カメラズーム上限

	// ========= ゲーム内ステータス =========

	int home_hp_ = 500; //拠点HP
	int home_hp_max_ = home_hp_; //拠点最大HP

	int magic_ = 200; //MP
	int magic_prev_ = 0; //直前のMP(自然回復計算用)
	int magic_timer_ = 0; //自然回復時間カウント
	const int MAGIC_TIME_ = 100; //自然回復がはじまる時間
	int magic_max_ = magic_; //最大MP
};