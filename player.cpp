#include "player.h"
#include <vector>

Player::Player(const std::vector<Texture>& players, const int& speed, const double& angularSpeed, const int& w, const int& h) {
    playerTextures = players;
    Player::speed = speed;
    Player::angularSpeed = angularSpeed;
    player.setTexture(playerTextures[0]);
    player.setOrigin(w / 2, h / 2);
    player.setPosition(500, 500);
}

Player::Player(Sprite sprite, const int& speed, const double& angularSpeed, const int& w, const int& h) : player(sprite) {
    Player::speed = speed;
    Player::angularSpeed = angularSpeed;
    player.setOrigin(w / 2, h / 2);
    player.setPosition(500, 500);
}

void Player::Move(bool forward) {
    if (forward) {
        player.move(-player.getOrigin().x * sin(-player.getRotation() * PI / 180) * speed / 3000, -player.getOrigin().y * cos(-player.getRotation() * PI / 180) * speed / 3000);
    }
    else {
        player.move(player.getOrigin().x * sin(-player.getRotation() * PI / 180) * speed / 3000, player.getOrigin().y * cos(-player.getRotation() * PI / 180) * speed / 3000);
    }
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