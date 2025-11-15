// Copyright 2K Games, Engineering Graduate Program. All rights reserved. DO NOT redistribute.

#include "tower.h"
#include "logger.h"
#include <cmath>

Tower::Tower(int x, int y, int rateOfFire)
    : position({x, y}),
      rateOfFire(rateOfFire),
      lastFireTime(std::chrono::steady_clock::now()),
      bAutoFire(true) {}

void Tower::Update(std::vector<Enemy*> enemies, std::vector<Bullet>& bullets) {
    if (bAutoFire) {
        FireAtEnemy(enemies, bullets);
    }
}

bool Tower::GetAutoFire() const {
    return bAutoFire;
}

void Tower::SetAutoFire(bool autoFire) {
    bAutoFire = autoFire;
}

void Tower::ManualFire(std::vector<Enemy*> enemies, std::vector<Bullet>& bullets) {
    FireAtEnemy(enemies, bullets);
}

int Tower::CalculateDistance(const Vector2D& pos1, const Vector2D& pos2) const {
    int dx = std::abs(pos1.x - pos2.x);
    int dy = std::abs(pos1.y - pos2.y);
    return dx + dy;
}

Enemy* Tower::FindClosestEnemy(const std::vector<Enemy*>& enemies) const {
    if (enemies.empty()) {
        return nullptr;
    }
    
    Enemy* closestEnemy = enemies[0];
    int minDistance = CalculateDistance(closestEnemy->GetPosition(), position);
    
    for (size_t i = 1; i < enemies.size(); ++i) {
        int distance = CalculateDistance(enemies[i]->GetPosition(), position);
        if (distance < minDistance) {
            minDistance = distance;
            closestEnemy = enemies[i];
        }
    }
    
    return closestEnemy;
}

void Tower::FireAtEnemy(const std::vector<Enemy*>& enemies, std::vector<Bullet>& bullets) {
    if (enemies.empty()) {
        return;
    }
    
    auto currentTime = std::chrono::steady_clock::now();
    auto timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastFireTime);
    
    if (timeElapsed >= std::chrono::milliseconds(rateOfFire)) {
        Enemy* closestEnemy = FindClosestEnemy(enemies);
        
        if (closestEnemy != nullptr) {
            if (bAutoFire) {
                int distance = CalculateDistance(closestEnemy->GetPosition(), position);
                if (distance <= ENEMYRANGE) {
                    FireBulletAtEnemy(*closestEnemy, bullets);
                    lastFireTime = currentTime;
                }
            } else {
                FireBulletAtEnemy(*closestEnemy, bullets);
                lastFireTime = currentTime;
            }
        }
    }
}

void Tower::FireBulletAtEnemy(Enemy& enemy, std::vector<Bullet>& bullets) {
    Vector2D direction = {0, 0};
    
    if (enemy.GetPosition().x > position.x) direction.x = 1;
    if (enemy.GetPosition().x < position.x) direction.x = -1;
    
    if (enemy.GetPosition().y > position.y) direction.y = 1;
    if (enemy.GetPosition().y < position.y) direction.y = -1;
    
    bullets.emplace_back(position, direction);
}
