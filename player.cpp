#include "player.h"
#include <vector>
#include <math.h>

Player::Player(Texture& texture, const int& speed, const double& angularSpeed, Vector2u Screen, const int& w, const int& h) : screen(Screen) {
  player.setTexture(&texture);
  player.setSize(Vector2f(w, h));
    Player::speed = speed;
    Player::angularSpeed = angularSpeed;
    player.setOrigin(w / 2, h / 2);
    player.setPosition(500, 500);
    anim_end = false;
}

void Player::Move(bool forward) { 
    int directionX = player.getOrigin().x;
    int directionY = player.getOrigin().y;
    float posX = player.getPosition().x;
    float posY = player.getPosition().y;
    int absPosX = abs((int)posX);
    if (forward) {
        player.move(-directionX * sin(-player.getRotation() * PI / 180) * speed / 3000, -directionY * cos(-player.getRotation() * PI / 180) * speed / 3000);
    }
    else {
        player.move(directionX * sin(-player.getRotation() * PI / 180) * speed / 3000, directionY * cos(-player.getRotation() * PI / 180) * speed / 3000);
    }
    if (posX >= screen.x) player.setPosition((int)posX % screen.x, posY);
    else if (posX < -player.getLocalBounds().width / 2) player.setPosition(screen.x + posX / 4, posY);
    else if (posY >= screen.y) player.setPosition(posX, (int)posY % screen.y);
    else if (posY < -player.getLocalBounds().height / 2) player.setPosition(posX, screen.y + posY / 4);
}

void Player::flyForward(float time, float& currentFrame) {
    currentFrame += 0.005 * time;
    if (currentFrame < 0.5) {
        player.setTextureRect(IntRect(4, 0, 114, 109));
    }
    if (currentFrame > 0.5 && currentFrame < 1.0) {
        player.setTextureRect(IntRect(121, 0, 114, 109));
    }
    if (currentFrame > 1.0 && currentFrame < 1.5) {
        player.setTextureRect(IntRect(4, 111, 114, 109));
    }
    if (currentFrame > 1.5 && currentFrame < 2.0) {
        player.setTextureRect(IntRect(121, 111, 114, 109));
    }
    if (currentFrame > 2.0) {
        player.setTextureRect(IntRect(238, 111, 114, 109));
    }
    if (currentFrame > 2.5) currentFrame = 0.25;
    Move(true);
}

void Player::flyBack(const float& time) {
    player.setTextureRect(IntRect(4, 0, 114, 109));
}

void Player::flyLeft(const float& time) { 
    player.rotate(-angularSpeed);
}

void Player::flyRight(const float& time) { 
    player.rotate(angularSpeed);
}

void Player::flyForwardAndBack() { 
    Move(false);
}

void Player::standartCondition() { 
    player.setTextureRect(IntRect(4, 0, 114, 109));
}

Vector2f Player::getPos() {
    return player.getPosition();
}

void Player::Destroy(std::vector<Texture>& textures) {
  int currentFrame = dest_anim_clock.getElapsedTime().asMilliseconds() / 16;
  dest_sprite.setPosition(pos_die);
  int anim_speed = 2;
  for (ptrdiff_t i = 0; i < textures.size() * anim_speed; i += anim_speed) {
    if (currentFrame >= textures.size() * anim_speed) {
      anim_end = true;
      break;
    }
    if (currentFrame > i && currentFrame < i + anim_speed) {
      dest_sprite.setTexture(textures[i / (anim_speed * 2)]);
    }
  }
}