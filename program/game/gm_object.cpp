#include "gm_object.h"

GameObj::~GameObj() {
	if (mesh_ != nullptr) delete mesh_;
	if (sprite_ != nullptr) delete sprite_;
}