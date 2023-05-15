////////////////////////
//
//  Created on Mon May 15 2023
//
//  scene.cpp
//
////////////////////////

#include "scene.hpp"

namespace ecs
{
    void Scene::clear()
    {
        m_current_entity = 0;
        m_components.clear();
        m_systems.clear();
    }

    void Scene::launchSystems()
    {
        for (auto systems = m_systems.begin(); systems != m_systems.end(); ++systems)
            for (auto system = systems->second.begin(); system != systems->second.end(); ++system)
                (*system)->launch(systems->first);
    }
}
