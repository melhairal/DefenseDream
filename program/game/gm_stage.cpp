#include "gm_stage.h"
#include "gm_object.h"
#include "object/gm_object_enemy.h"
#include "gm_camera.h"
#include "model/gm_anim_sprite3d.h"
#include "scene/gm_scene_play.h"

void StageBase::RandomPop(int num) {
	int enemy_pop_ = rand() % 100;
	double th = ((double)enemy_pop_ / (double)100) * 3.14f * 2;
	float x = scene_->FIELD_R_ * cos(th);
	float y = scene_->FIELD_R_ * sin(th);
	for (int i = 0; i < num; ++i) {
		enemy_pop_ = rand() % 400 - 200;
		x = x + enemy_pop_;
		y = y + enemy_pop_;
		for (auto enemy : scene_->objects_) {
			if (enemy->tag_ != GameObj::eEnemy) continue;
			if (enemy->id_ != enemyStock_) continue;
			enemy->sprite_->pos_ = { x,0,y };
			enemy->moving_ = true;
			enemyStock_--;
		}
	}
}

Stage1::Stage1(ScenePlay* scene) {
	scene_ = scene;
	stageNum_ = 1;
	for (int i = 0; i < ENEMY_NUM_; ++i) {
		scene_->objects_.emplace_back(new EnemySprite(scene_, { 0,-500,0 }))->moving_ = false;
		scene_->objects_.back()->update(0);
		enemyStock_++;
		scene_->objects_.back()->id_ = enemyStock_;
	}
}

void Stage1::update(float delta_time) {
	elapsed_++;
	if (elapsed_ > 60) {
		elapsed_ = 0;
		time_--;
	}
	if (time_ <= 0) {
		time_ = 0;
	}
	if (time_ % 6 == 1 && elapsed_ == 0) {
		RandomPop(3);
	}
}
