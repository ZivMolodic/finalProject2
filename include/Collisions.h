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

    void tennisWithMan(GameObject& tennis, GameObject& man)
    {
        Tennis& tennisBall = dynamic_cast<Tennis&>(tennis);
        RaftMan& raftMan = dynamic_cast<RaftMan&>(man);
        
        //auto explosion = Explosion(tennisBall.getPosition(), 100.f);

        raftMan.handleObjectile(&tennisBall);
    }

    void objectileWithDownRaft(Objectile* obj, GameObject& raft)
    {
        DownRaft& raftBlock = dynamic_cast<DownRaft&>(raft);

        obj->handleCollision(raftBlock.getRectangle());
    }

    void objectileWithUpRaft(Objectile* obj, GameObject& raft)
    {
        UpRaft& raftBlock = dynamic_cast<UpRaft&>(raft);

        obj->handleCollision(raftBlock.getRectangle());
    }

    void grenadeWithDownRaft(GameObject& grenade, GameObject& raft)
    {
        Grenade& objectile = dynamic_cast<Grenade&>(grenade);

        objectileWithDownRaft(&objectile, raft);
    }

    void grenadeWithUpRaft(GameObject& grenade, GameObject& raft)
    {
        Grenade& objectile = dynamic_cast<Grenade&>(grenade);

        objectileWithUpRaft(&objectile, raft);
    }

    void tennisWithDownRaft(GameObject& tennis, GameObject& raft)
    {
        Tennis& objectile = dynamic_cast<Tennis&>(tennis);

        objectileWithDownRaft(&objectile, raft);
    }

    void tennisWithUpRaft(GameObject& tennis, GameObject& raft)
    {
        Tennis& objectile = dynamic_cast<Tennis&>(tennis);

        objectileWithUpRaft(&objectile, raft);
    }

    void missileWithDownRaft(GameObject& missile, GameObject& raft)
    {
        Missile& objectile = dynamic_cast<Missile&>(missile);

        objectileWithDownRaft(&objectile, raft);
    }

    void missileWithUpRaft(GameObject& missile, GameObject& raft)
    {
        Missile& objectile = dynamic_cast<Missile&>(missile);

        objectileWithUpRaft(&objectile, raft);
    }

    void guidedMissileWithDownRaft(GameObject& missile, GameObject& raft)
    {
        GuidedMissile& objectile = dynamic_cast<GuidedMissile&>(missile);

        objectileWithDownRaft(&objectile, raft);
    }

    void guidedMissileWithUpRaft(GameObject& missile, GameObject& raft)
    {
        GuidedMissile& objectile = dynamic_cast<GuidedMissile&>(missile);

        objectileWithUpRaft(&objectile, raft);
    }

    void manWithExplosion(GameObject& man, GameObject& explosion)
    {
        RaftMan& raftMan = dynamic_cast<RaftMan&>(man);
        Explosion& exp = dynamic_cast<Explosion&>(explosion);

        raftMan.handleExplosion(exp);
    }

    void DownRaftWithExplosion(GameObject& upRaft, GameObject& explosion)
    {
        DownRaft& raft = dynamic_cast<DownRaft&>(upRaft);
        Explosion& exp = dynamic_cast<Explosion&>(explosion);

        raft.handleExplosion(exp);
    }

    void UpRaftWithExplosion(GameObject& upRaft, GameObject& explosion)
    {
        UpRaft& raft = dynamic_cast<UpRaft&>(upRaft);
        Explosion& exp = dynamic_cast<Explosion&>(explosion);

        raft.handleExplosion(exp);
    }


    using collisionFunc = void (*) (GameObject& object1, GameObject& object2);
    using CollisionMap = std::unordered_map<std::string, collisionFunc>;


    std::unique_ptr<CollisionMap> CreateMap() {
        std::unique_ptr<CollisionMap> cm = std::make_unique<CollisionMap>();

        (*cm)[std::string(typeid(RaftMan).name()) + std::string(typeid(DownRaft).name())] = manWithDownRaft;
        (*cm)[std::string(typeid(RaftMan).name()) + std::string(typeid(UpRaft).name())] = manWithUpRaft;
        (*cm)[std::string(typeid(Grenade).name()) + std::string(typeid(DownRaft).name())] = grenadeWithDownRaft;
        (*cm)[std::string(typeid(Grenade).name()) + std::string(typeid(UpRaft).name())] = grenadeWithUpRaft;
        (*cm)[std::string(typeid(Missile).name()) + std::string(typeid(DownRaft).name())] = missileWithDownRaft;
        (*cm)[std::string(typeid(Missile).name()) + std::string(typeid(UpRaft).name())] = missileWithUpRaft;
        (*cm)[std::string(typeid(Tennis).name()) + std::string(typeid(DownRaft).name())] = tennisWithDownRaft;
        (*cm)[std::string(typeid(Tennis).name()) + std::string(typeid(UpRaft).name())] = tennisWithUpRaft;
        (*cm)[std::string(typeid(Tennis).name()) + std::string(typeid(RaftMan).name())] = tennisWithMan;
        (*cm)[std::string(typeid(GuidedMissile).name()) + std::string(typeid(DownRaft).name())] = guidedMissileWithDownRaft;
        (*cm)[std::string(typeid(GuidedMissile).name()) + std::string(typeid(UpRaft).name())] = guidedMissileWithUpRaft;
        (*cm)[std::string(typeid(RaftMan).name()) + std::string(typeid(Explosion).name())] = manWithExplosion;
        (*cm)[std::string(typeid(UpRaft).name()) + std::string(typeid(Explosion).name())] = UpRaftWithExplosion;
        (*cm)[std::string(typeid(DownRaft).name()) + std::string(typeid(Explosion).name())] = DownRaftWithExplosion;
        
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