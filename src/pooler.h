// Copyright 2K Games, Engineering Graduate Program. All rights reserved. DO NOT redistribute.

#ifndef POOLER_H
#define POOLER_H

#include <list>
#include <vector>
#include "enemy.h"

class Pooler {
public:
    Pooler(size_t minimumPoolSize);
    ~Pooler();
    
    size_t PoolSize() const;
    Enemy* SpawnEnemy(const Vector2D& spawnPosition);
    void DespawnEnemy(Enemy* enemy);
    std::vector<Enemy*> GetActiveEnemies() const;
    void Clear();

private:
    std::list<Enemy*> freeEnemies;
    std::list<Enemy*> activeEnemies;
    size_t minimumPoolSize;
    
    void ExpandPool();
    Enemy* CreateEnemy();
};

#endif // POOLER_H
