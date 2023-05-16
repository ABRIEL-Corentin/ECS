////////////////////////
//
//  Created on Mon May 15 2023
//
//  scene.inl
//
////////////////////////

#include "scene.hpp"

namespace ecs
{
    inline Entity Scene::createEntity()
    {
        return m_current_entity++;
    }

    template<typename T>
    Component<T> &Scene::getComponent(const Entity &entity)
    {
        static Component<T> null_component = std::nullopt;
        Components<T> &components = getComponents<T>();

        if (components.size() <= entity)
            return null_component;
        return components.at(entity);
    }

    template<typename T>
    Components<T> &Scene::getComponents()
    {
        auto components = m_components.find(std::type_index(typeid(T)));

        if (components == m_components.end()) {
            registerComponent<T>();
            return getComponents<T>();
        }
        return std::any_cast<Components<T> &>(components->second);
    }

    template<typename T>
    Component<T> &Scene::addComponent(const Entity &entity)
    {
        Components<T> &components = getComponents<T>();

        if (components.size() <= entity)
            components.resize(entity + 1);
        return components.at(entity) = T();
    }

    template<typename T>
    void Scene::removeComponent(const Entity &entity)
    {
        Components<T> &components = getComponents<T>();

        if (components.size() <= entity)
            return;
        components.at(entity).reset();
        removeDependSystems<T>(entity);
    }

    template<typename T>
    bool Scene::hasComponent(const Entity &entity)
    {
        Components<T> &components = getComponents<T>();

        return components.size() > entity && components.at(entity) != std::nullopt;
    }

    template<typename T>
    void Scene::registerComponent()
    {
        if (m_components.find(std::type_index(typeid(T))) != m_components.end())
            return;

        m_components.insert({std::type_index(typeid(T)), Components<T>()});
    }

    template<typename ... COMPONENTS>
    bool Scene::addSystem(const Entity &entity, System<COMPONENTS...> system)
    {
        bool status = true;

        ((status = hasComponent<COMPONENTS>(entity) ? status : false), ...);

        if (!status || !system)
            return false;

        auto systems = m_systems.find(entity);

        if (systems == m_systems.end()) {
            m_systems.insert({entity, std::vector<std::unique_ptr<ISystem>>()});
            m_systems.at(entity).push_back(std::make_unique<SystemManager<COMPONENTS...>>(system));
        } else {
            for (auto it = systems->second.begin(); it != systems->second.end(); ++it)
                if ((*it)->getPtr() == system)
                    return false;

            systems->second.push_back(std::make_unique<SystemManager<COMPONENTS...>>(system));
        }
        return true;
    }

    template<typename ... COMPONENTS>
    bool Scene::removeSystem(const Entity &entity, System<COMPONENTS...> system)
    {
        auto systems = m_systems.find(entity);

        if (system == m_systems.end() || !system)
            return false;

        for (auto it = systems->second.begin(); it != systems->second.end(); ++it) {
            if ((*it)->getPtr() == system) {
                systems->second.erase(it);
                return true;
            }
        }
        return false;
    }

    template<typename ... COMPONENTS>
    bool Scene::hasSystem(const Entity &entity, System<COMPONENTS...> system)
    {
        auto systems = m_systems.find(entity);

        if (system == m_systems.end() || !system)
            return false;

        for (auto it = systems->second.begin(); it != systems->second.end(); ++it)
            if ((*it)->getPtr() == system)
                return true;
        return false;
    }

    template<typename T>
    void Scene::removeDependSystems(const Entity &entity)
    {
        auto systems = m_systems.find(entity);

        if (systems == m_systems.end())
            return;

        for (std::size_t i = 0; i < systems->second.size(); ++i) {
            if ((systems->second.at(i))->requiredComponents(typeid(T))) {
                systems->second.erase(systems->second.begin() + i);
                --i;
            }
        }
    }

    template<typename ... COMPONENTS>
    SystemManager<COMPONENTS...>::SystemManager(System<COMPONENTS...> system)
        : m_system(system)
    { }

    template<typename ... COMPONENTS>
    void SystemManager<COMPONENTS...>::launch(const Entity &entity)
    {
        if (m_system)
            m_system(1, *Scene::getInstance().getComponent<COMPONENTS>(entity)...);
    }

    template<typename ... COMPONENTS>
    bool SystemManager<COMPONENTS...>::requiredComponents(const std::type_info &type) const
    {
        bool status = false;

        ((status = typeid(COMPONENTS) == type ? true : status), ...);
        return status;
    }

    template<typename ... COMPONENTS>
    void *SystemManager<COMPONENTS...>::getPtr() const
    {
        return reinterpret_cast<void *>(m_system);
    }
}
