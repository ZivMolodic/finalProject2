#pragma once

#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include "GameObject.h"
#include "Resources.h"
#include "RaftMan.h"
#include "RaftBlock.h"

//this namespace handles all collisions creating double v-table
namespace {
    void manWithDownRaft(GameObject& man, GameObject& raft)
    {
        RaftMan& raftMan = dynamic_cast<RaftMan&>(man);
        DownRaft& raftBlock = dynamic_cast<DownRaft&>(raft);

        raftMan.handleCollision(raftBlock.getRectangle());
    }
    void manWithUpRaft(GameObject& man, GameObject& raft)
    {
        RaftMan& raftMan = dynamic_cast<RaftMan&>(man);
        UpRaft& raftBlock = dynamic_cast<UpRaft&>(raft);

        raftMan.handleCollision(raftBlock.getRectangle());
    }
    
    void objectileWithDownRaft(GameObject& obj, GameObject& raft)
    {
        Objectile& objectile = dynamic_cast<Objectile&>(obj);
        DownRaft& raftBlock = dynamic_cast<DownRaft&>(raft);

        objectile.handleCollision(raftBlock.getRectangle());
    }

    void objectileWithUpRaft(GameObject& obj, GameObject& raft)
    {
        Objectile& objectile = dynamic_cast<Objectile&>(obj);
        UpRaft& raftBlock = dynamic_cast<UpRaft&>(raft);

        objectile.handleCollision(raftBlock.getRectangle());
    }

    void manWithExplosion(GameObject& man, GameObject& explosion)
    {
        RaftMan& raftMan = dynamic_cast<RaftMan&>(man);
        Explosion& exp = dynamic_cast<Explosion&>(explosion);

        raftMan.handleExplosion(exp);
    }


    using collisionFunc = void (*) (GameObject& object1, GameObject& object2);
    using CollisionMap = std::unordered_map<std::string, collisionFunc>;


    std::unique_ptr<CollisionMap> CreateMap() {
        std::unique_ptr<CollisionMap> cm = std::make_unique<CollisionMap>();

        (*cm)[std::string(typeid(RaftMan).name()) + std::string(typeid(DownRaft).name())] = manWithDownRaft;
        (*cm)[std::string(typeid(RaftMan).name()) + std::string(typeid(UpRaft).name())] = manWithUpRaft;
        (*cm)[std::string(typeid(Objectile).name()) + std::string(typeid(DownRaft).name())] = objectileWithDownRaft;
        (*cm)[std::string(typeid(Objectile).name()) + std::string(typeid(UpRaft).name())] = objectileWithUpRaft;
        (*cm)[std::string(typeid(RaftMan).name()) + std::string(typeid(Explosion).name())] = manWithExplosion;
        //(*cm)[std::string(typeid(Pacman).name()) + std::string(typeid(Door).name())] = pacmanWithDoor;
        //(*cm)[std::string(typeid(Pacman).name()) + std::string(typeid(Key).name())] = pacmanWithKey;
        //(*cm)[std::string(typeid(Pacman).name()) + std::string(typeid(SuperPresent).name())] = pacmanWithSuperPresent;
        //(*cm)[std::string(typeid(Pacman).name()) + std::string(typeid(TimePresent).name())] = pacmanWithTimePresent;
        //(*cm)[std::string(typeid(Pacman).name()) + std::string(typeid(LivesPresent).name())] = pacmanWithLivesPresent;
        //(*cm)[std::string(typeid(Pacman).name()) + std::string(typeid(FreezePresent).name())] = pacmanWithFreezePresent;

        return cm;
    }

    collisionFunc lookup(const std::string& object1, const std::string& object2)
    {
        static std::unique_ptr<CollisionMap> map = CreateMap();
        std::string name = object1 + object2;
        auto itr = map->find(object1 + object2);
        if (itr == map->end())
            return nullptr;

        return itr->second;
    }


    void processCollision(GameObject& object1,
        GameObject& object2)
    {
        collisionFunc p2f = lookup(typeid(object1).name(),
            typeid(object2).name());
        if (p2f)
            p2f(object1, object2);
        else
        {
            p2f = lookup(typeid(object2).name(),
                typeid(object1).name());
            if (p2f)
                p2f(object2, object1);
        }
    }
}