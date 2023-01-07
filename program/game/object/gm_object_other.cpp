#include "gm_object_other.h"
#include "../gm_camera.h"
#include "../model/gm_anim_sprite3d.h"
#include "../scene/gm_scene_play.h"

Cloud::Cloud(ScenePlay* scene) {
	tag_ = GameObj::eCloud;
	scene_ = scene;
	size_ = SIZE_;
	mesh_ = dxe::Mesh::CreatePlane({ SPRITE_W_, SPRITE_H_, 0 });
	mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/cloud.png"));
	mesh_->pos_ = { 0,0,0 };
}

void Cloud::update(float delta_time) {
	tnl::Vector3 dir;
	dir = mesh_->pos_ - scene_->camera_->pos_;
	dir.normalize();
	mesh_->rot_q_.slerp(tnl::Quaternion::LookAtAxisY(mesh_->pos_, mesh_->pos_ + dir), 0.3f);
}

Home::Home(ScenePlay* scene) {
	tag_ = GameObj::eHome;
	scene_ = scene;
	size_ = SIZE_;
	mesh_ = dxe::Mesh::CreatePlane({ SPRITE_W_, SPRITE_H_, 0 });
	mesh_->setTexture(dxe::Texture::CreateFromFile("graphics/bed.png"));
	mesh_->pos_ = { 0,0,0 };
}

void Home::update(float delta_time) {
	tnl::Vector3 dir;
	dir = mesh_->pos_ - scene_->camera_->pos_;
	dir.normalize();
	mesh_->rot_q_.slerp(tnl::Quaternion::LookAtAxisY(mesh_->pos_, mesh_->pos_ + dir), 0.3f);
}