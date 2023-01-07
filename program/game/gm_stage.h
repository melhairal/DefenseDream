#include "../dxlib_ext/dxlib_ext.h"
class ScenePlay;

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