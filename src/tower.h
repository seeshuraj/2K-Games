// Copyright 2K Games, Engineering Graduate Program. All rights reserved. DO NOT redistribute.

#ifndef TOWER_H
#define TOWER_H

#include <chrono>
#include <vector>
#include <cmath>
#include "bullet.h"
#include "enemy.h"
#include "texture.h"
#include "vector2d.h"
#include "constants.h"

class Tower {
public:
    Vector2D position;
    std::chrono::steady_clock::time_point lastFireTime;
    Texture texture = Texture('T', Color::GREEN);
    
    Tower(int x, int y, int rateOfFire);
    
    void Update(std::vector<Enemy*> enemies, std::vector<Bullet>& bullets);
    bool GetAutoFire() const;
    void SetAutoFire(bool autoFire);
    void ManualFire(std::vector<Enemy*> enemies, std::vector<Bullet>& bullets);

private:
    int rateOfFire;
    bool bAutoFire;
    
    int CalculateDistance(const Vector2D& pos1, const Vector2D& pos2) const;
    Enemy* FindClosestEnemy(const std::vector<Enemy*>& enemies) const;
    void FireAtEnemy(const std::vector<Enemy*>& enemies, std::vector<Bullet>& bullets);
    void FireBulletAtEnemy(Enemy& enemy, std::vector<Bullet>& bullets);
};

#endif // TOWER_H
