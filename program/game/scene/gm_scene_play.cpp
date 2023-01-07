#include "../gm_manager.h"
#include "../gm_camera.h"
#include "../model/gm_anim_sprite3d.h"
#include "gm_scene_play.h"
#include "gm_scene_result.h"
#include "../gm_object.h"
#include "../gm_ui.h"
#include "../gm_stage.h"
#include "../object/gm_object_player.h"
#include "../object/gm_object_enemy.h"
#include "../object/gm_object_combo.h"
#include "../object/gm_object_other.h"

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
	int home_hp_ = std::clamp(home_hp_, 0, home_hp_max_);

	//------------------------------------------------------------------
	//
	// �I�u�W�F�N�g����
	//

	updateObject(delta_time);

	//------------------------------------------------------------------
	//
	// �J��������
	//
	
	updateCamera(delta_time);

	//------------------------------------------------------------------
	//
	// ���̑�(�f�o�b�O��)
	//

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
	DrawGridGround(camera_, 50, 20); //�O���b�h�̍쐬
	DrawRotaGraph(DXE_WINDOW_WIDTH / 2, DXE_WINDOW_HEIGHT / 2, 2.5f, 0, backGround_, true); //�w�i�̕`��
	field_->render(camera_); //���̕`��

	//�S�I�u�W�F�N�g�̕`��
	for (auto object : objects_) {
		if (object->mesh_ != nullptr) object->mesh_->render(camera_);
		if (object->sprite_ != nullptr) object->sprite_->render(camera_);
	}

	//UI�̕`��
	ui_->render();


	//------------------------------------------------------------------
	//
	// ���̑�(�f�o�b�O��)
	//

	DrawStringEx(100, 100, 0, "%d", stage_->enemyStock_);
}

void ScenePlay::updateObject(float delta_time) {
	//�S�ẴI�u�W�F�N�g�̃A�b�v�f�[�g
	for (auto object : objects_) {
		if (object->moving_)	object->update(delta_time);
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
}

void ScenePlay::updateCamera(float delta_time) {
	//�J������]
	tnl::Vector3 rot[2] = {
		{ 0, tnl::ToRadian(CAM_ROT_SPEED_), 0 },
		{ 0, -tnl::ToRadian(CAM_ROT_SPEED_), 0 }
	};
	tnl::Input::RunIndexKeyDown([&](uint32_t idx) {
		camera_->free_look_angle_xy_ += rot[idx];
		}, eKeys::KB_E, eKeys::KB_Q);

	//�J�����̃Y�[��
	int wheel = GetMouseWheelRotVol();
	if (wheel < 0) {
		camera_->target_distance_ += CAM_ZOOM_SPEED_;
	}
	if (wheel > 0) {
		camera_->target_distance_ -= CAM_ZOOM_SPEED_;
	}

	//�J�����Y�[���̏���E����
	camera_->target_distance_ = std::clamp(camera_->target_distance_, CAM_ZOOM_MIN_, CAM_ZOOM_MAX_);

	camera_->target_ = player_->sprite_->pos_;
}