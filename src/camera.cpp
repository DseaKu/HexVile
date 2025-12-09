#include "camera.h"
#include "raylib.h"

GameCamera::GameCamera() {
  camera.offset = (Vector2){0, 0};
  camera.rotation = 0.0f;
  camera.target = (Vector2){0, 0};
  camera.zoom = 0.0f;
}

GameCamera::~GameCamera() {}

void GameCamera::FollowPosition(Vector2 *position) {
  targetPosition = position;
  camera.target = *targetPosition;
}

void GameCamera::Update() { camera.target = *targetPosition; }
