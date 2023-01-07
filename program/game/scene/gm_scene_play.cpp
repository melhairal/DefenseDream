#include "../gm_manager.h"
#include "../gm_camera.h"
#include "../model/gm_anim_sprite3d.h"
#include "gm_scene_play.h"
#include "gm_scene_result.h"
#include "../gm_object.h"

tnl::Quaternion	fix_rot;

ScenePlay::~ScenePlay() {
	delete camera_;
	delete field_;
	delete ui_;
	delete stage_;

	for (auto object : objects_) delete object;
}

void ScenePlay::initialzie() {
	//�J��������
	camera_ = new GmCamera();
	camera_->pos_ = { 0, 150, -300 };
	camera_->far_ = 2000.0f;
	//�v���C���[����
	player_ = objects_.emplace_back(new Player(this));
	//���_�𐶐�
	home_ = objects_.emplace_back(new Home(this));
	//�X�e�[�W�𐶐�
	field_ = dxe::Mesh::CreateDisk(FIELD_R_ * 1.1f);
	field_->setTexture(dxe::Texture::CreateFromFile("graphics/ground.jpg"));
	field_->rot_q_ *= tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(90));
	field_->pos_ = { 0, FIELD_H_, 0 };
	//�_�𐶐�(�t�B�[���h�O��)
	for (int i = 0; i < CLOUD_DENSITY_; ++i) {
		double th = ((double)i / (double)CLOUD_DENSITY_) * 3.14f * 2;
		float x = FIELD_R_ * cos(th);
		float y = FIELD_R_ * sin(th);
		objects_.emplace_back(new Cloud(this))->mesh_->pos_ = {x, 0, y};
	}
	//�w�i�𐶐�
	backGround_ = LoadGraph("graphics/sky.jpg");
	//UI�𐶐�
	ui_ = new Ui(this);
	//�X�e�[�W�𐶐�
	stage_ = new Stage1(this);
}

void ScenePlay::update(float delta_time)
{
	GameManager* mgr = GameManager::GetInstance();

	//------------------------------------------------------------------
	//
	// �X�e�[�W����
	//

	stage_->update(delta_time);

	//------------------------------------------------------------------
	//
	// �I�u�W�F�N�g����
	//

	//�S�ẴI�u�W�F�N�g�̃A�b�v�f�[�g
	for (auto object : objects_) {
		if(object->moving_)	object->update(delta_time);
	}
	//�I�u�W�F�N�g�̐����t���O��false�ɂȂ�����f���[�g
	auto it_object = objects_.begin();
	while (it_object != objects_.end()) {
		if (!(*it_object)->alive_) {
			delete (*it_object);
			it_object = objects_.erase(it_object);
			continue;
		}
		it_object++;
	}
	// �J�����ɋ߂����Ƀ\�[�g(�߂��I�u�W�F�N�g����`�悷�邽��)
	objects_.sort([&](const GameObj* l, const GameObj* r) {
		float ld = 0;
		float rd = 0;
		if (l->mesh_ != nullptr) ld = (camera_->pos_ - l->mesh_->pos_).length();
		if (l->sprite_ != nullptr) ld = (camera_->pos_ - l->sprite_->pos_).length();
		if (r->mesh_ != nullptr) rd = (camera_->pos_ - r->mesh_->pos_).length();
		if (r->sprite_ != nullptr) rd = (camera_->pos_ - r->sprite_->pos_).length();
		return ld > rd;
		});

	//------------------------------------------------------------------
	//
	// �J��������
	//

	//�J������]
	tnl::Vector3 rot[2] = {
		{ 0, tnl::ToRadian(1.0f), 0 },
		{ 0, -tnl::ToRadian(1.0f), 0 }
	};
	tnl::Input::RunIndexKeyDown([&](uint32_t idx) {
		camera_->free_look_angle_xy_ += rot[idx];
		}, eKeys::KB_E, eKeys::KB_Q);
	//�J�����̃Y�[��
	int wheel = GetMouseWheelRotVol();
	if (wheel < 0) {
		camera_->target_distance_ += 30.0f;
	}
	if (wheel > 0) {
		camera_->target_distance_ -= 30.0f;
	}
	//�J�����Y�[���̏��
	if (camera_->target_distance_ < 200) {
		camera_->target_distance_ = 200;
	}
	if (camera_->target_distance_ > 400) {
		camera_->target_distance_ = 400;
	}
	camera_->target_ = player_->sprite_->pos_;

	//------------------------------------------------------------------
	//
	// ���̑�(�f�o�b�O��)
	//

	//�_(��)/���_�̏Փ˔���
	for (auto object : objects_) {
		if (object->tag_ != GameObj::eCloud && object->tag_ != GameObj::eHome) continue;
		if (tnl::IsIntersectAABB(player_->sprite_->pos_, { player_->size_,player_->size_,player_->size_ }, object->mesh_->pos_, { object->size_,object->size_,object->size_ })) {
			tnl::GetCorrectPositionIntersectAABB(player_->prev_pos_, { player_->size_,player_->size_,player_->size_ }, object->mesh_->pos_, { object->size_,object->size_,object->size_ }, player_->sprite_->pos_);
		}
	}

	//�G�����e�X�g�p
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_Z)) {
		objects_.emplace_back(new EnemySprite(this, { 0,0,0 }));
	}

	//�V�[���؂�ւ�
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		mgr->chengeScene(new SceneResult());
	}
}

void ScenePlay::render()
{
	camera_->update();

	//�X�e�[�W�̕`��
	DrawGridGround(camera_, 50, 20);
	DrawRotaGraph(DXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT / 2, 2.5f, 0, backGround_, true);
	field_->render(camera_);
	//�S�I�u�W�F�N�g�̕`��
	for (auto object : objects_) {
		if (object->mesh_ != nullptr) object->mesh_->render(camera_);
		if (object->sprite_ != nullptr) object->sprite_->render(camera_);
	}

	ui_->render();

	DrawStringEx(100, 100, 0, "%d", stage_->enemyStock_);
}

//------------------------------------------------------------------
//
// UI
//

Ui::Ui(ScenePlay* scene) {
	scene_ = scene;
	//�t�H���g�ǂݍ���
	font_pop_16 = LoadFontDataToHandle("font/Novelpop16.dft", 0);
	font_pop_32 = LoadFontDataToHandle("font/Novelpop32.dft", 0);
	//hp�o�[�ǂݍ���
	hp_bar_[eBlue] = LoadGraph("graphics/gauge_bar_blue.png");
	hp_bar_[eYellow] = LoadGraph("graphics/gauge_bar_yellow.png");
	hp_bar_[eRed] = LoadGraph("graphics/gauge_bar_pink.png");
	hp_back_ = LoadGraph("graphics/gauge_background.png");
	hp_frame_ = LoadGraph("graphics/gauge_frame.png");
	//�A�C�R���ǂݍ���
	heart_ = LoadGraph("graphics/heart.png");
	star_ = LoadGraph("graphics/star.png");
	//���ԓǂݍ���
	time_icon_ = LoadGraph("graphics/time.png");
}

void Ui::update(float delta_time) {

}

void Ui::render() {
	//���_HP�\��
	DrawGauge(HOME_HP_X_, HOME_HP_Y_, 156, 8, 2, scene_->home_hp_, scene_->home_hp_max_, eYellow);
	//�v���C���[HP�\��
	DrawGauge(PLAYER_GAUGE_X_, PLAYER_GAUGE_Y1_, 156, 8, 1.5f, scene_->player_->hp_, scene_->player_->hp_max_, eRed);
	DrawGauge(PLAYER_GAUGE_X_, PLAYER_GAUGE_Y2_, 156, 8, 1.5f, scene_->magic_, scene_->magic_max_, eBlue);
	DrawRotaGraph(PLAYER_ICON_X_, PLAYER_GAUGE_Y1_, 0.07f, 0, heart_, true);
	DrawRotaGraph(PLAYER_ICON_X_, PLAYER_GAUGE_Y2_, 0.1f, 0, star_, true);
	//�������ԕ\��
	DrawFormatStringToHandle(TIME_X_, TIME_Y_, 0, font_pop_32, "%d", scene_->stage_->time_);
	DrawRotaGraph(TIME_ICON_X_, TIME_ICON_Y_, 0.1f, 0, time_icon_, true);
}

void Ui::DrawGauge(int x, int y, int width, int height, double exrate, int num, int max, int color) {
	int gauge_x1 = x - width * exrate / 2;
	int gauge_y1 = y - height * exrate / 2;
	int gauge_y2 = y + height * exrate / 2;
	int gauge_x2 = gauge_x1 + width * exrate *  ((double)num / (double)max);
	DrawRotaGraph(x, y, exrate, 0, hp_back_, true);
	DrawExtendGraph(gauge_x1, gauge_y1, gauge_x2, gauge_y2, hp_bar_[color], true);
	DrawRotaGraph(x, y, exrate, 0, hp_frame_, true);
}

//------------------------------------------------------------------
//
// �X�e�[�W
//

void StageBase::RandomPop(int num) {
	int enemy_pop_ = rand() % 100;
	double th = ((double)enemy_pop_ / (double)100) * 3.14f * 2;
	float x = scene_->FIELD_R_ * cos(th);
	float y = scene_->FIELD_R_ * sin(th);
	for (int i = 0; i < num; ++i) {
		enemy_pop_ = rand() % 200 - 100;
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

	if (time_ % 6 == 1 && elapsed_ == 0) {
		RandomPop(3);
	}
}
