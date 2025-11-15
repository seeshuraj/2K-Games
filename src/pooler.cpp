// Copyright 2K Games, Engineering Graduate Program. All rights reserved. DO NOT redistribute.

#include "pooler.h"
#include "constants.h"
#include <algorithm>

Pooler::Pooler(size_t minimumPoolSize) 
    : minimumPoolSize(minimumPoolSize) {
    for (size_t i = 0; i < minimumPoolSize; ++i) {
        freeEnemies.push_back(CreateEnemy());
    }
}

Pooler::~Pooler() {
    Clear();
}

size_t Pooler::PoolSize() const {
    return freeEnemies.size() + activeEnemies.size();
}

Enemy* Pooler::SpawnEnemy(const Vector2D& spawnPosition) {
    if (freeEnemies.empty()) {
        ExpandPool();
    }
    
    Enemy* enemy = freeEnemies.front();
    freeEnemies.pop_front();
    
    enemy->Activate(spawnPosition);
    activeEnemies.push_back(enemy);
    
    return enemy;
}

void Pooler::DespawnEnemy(Enemy* enemy) {
    if (enemy == nullptr) {
        return;
    }
    
    enemy->Deactivate();
    
    auto it = std::find(activeEnemies.begin(), activeEnemies.end(), enemy);
    if (it != activeEnemies.end()) {
        activeEnemies.erase(it);
    }
    
    freeEnemies.push_back(enemy);
}

std::vector<Enemy*> Pooler::GetActiveEnemies() const {
    return std::vector<Enemy*>(activeEnemies.begin(), activeEnemies.end());
}

void Pooler::Clear() {
    for (auto enemy : activeEnemies) {
        enemy->Deactivate();
        freeEnemies.push_back(enemy);
    }
    activeEnemies.clear();
    
    for (auto enemy : freeEnemies) {
        delete enemy;
    }
    freeEnemies.clear();
}

void Pooler::ExpandPool() {
    for (size_t i = 0; i < ENEMYSPAWNBULKSIZE; ++i) {
        freeEnemies.push_back(CreateEnemy());
    }
}

Enemy* Pooler::CreateEnemy() {
    return new Enemy();
}
