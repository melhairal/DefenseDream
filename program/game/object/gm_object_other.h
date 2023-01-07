#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../gm_object.h"
class ScenePlay;

// �_(�O��)
class Cloud :public GameObj {
public:
	Cloud(ScenePlay* scene);
	~Cloud() {}
	void update(float delta_time) override;

	const float SPRITE_W_ = 40.0f; //�摜�T�C�Y
	const float SPRITE_H_ = 25.0f;
	const float SIZE_ = 30.0f; //�����蔻��̑傫��
};

//���_
class Home :public GameObj {
public:
	Home(ScenePlay* scene);
	~Home() {}
	void update(float delta_time) override;

	const float SPRITE_W_ = 50.0f; //�摜�T�C�Y
	const float SPRITE_H_ = 50.0f;
	const float SIZE_ = 50.0f; //�����蔻��̑傫��
};