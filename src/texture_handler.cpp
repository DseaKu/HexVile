#include "texture_handler.h"
#include "defines.h"
#include "raylib.h"
#include <iostream>

TextureHandler::TextureHandler() { textureAtlas = {0, 0, 0, 0, 0}; }

int TextureHandler::LoadAssets(const char *pathToAssest) {
  this->textureAtlas = LoadTexture(pathToAssest);
  // Catch error
  if (this->textureAtlas.id == 0) {
    std::cout << "Error loading texture atlas" << std::endl;
    return 1;
  }
  this->width = this->textureAtlas.width / Config::ASSEST_RESOLUTION;
  this->height = this->textureAtlas.height / Config::ASSEST_RESOLUTION;
  return 0;
}

void TextureHandler::UnloadAssets() { UnloadTexture(this->textureAtlas); }

void TextureHandler::Draw(Rectangle assetsRect, Rectangle destRect,
                          Vector2 origin, float rotation, Color color) {
  DrawTexturePro(this->textureAtlas, assetsRect, destRect, origin, rotation,
                 color);
}
