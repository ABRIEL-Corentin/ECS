////////////////////////
//
//  Created on Mon Jun 12 2023
//
//  world.cpp
//
////////////////////////

#include "world.hpp"

namespace ecs
{
    Entity World::createEntity()
    {
        if (_deleted_entities.size())
            return *_deleted_entities.erase(_deleted_entities.begin());
        return _current_entity++;
    }

    void World::deleteEntity(const Entity &entity)
    {
        _deleted_entities.push_back(entity);
    }

    void World::clear()
    {
        _components.clear();
        _systems.clear();
        _current_entity = 0;
        _deleted_entities.clear();
    }

    void World::launchSystems()
    {
        for (auto systems = _systems.begin(); systems != _systems.end(); ++systems)
            for (auto system = systems->second.begin(); system != systems->second.end(); ++system)
                (*system)->launch(systems->first);
    }
}
