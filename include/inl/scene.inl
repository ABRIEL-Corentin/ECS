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
        m_entities.push_back(m_current_entity++);
        return m_entities.back();
    }

    inline void Scene::destroyEntity(const Entity &entity)
    {
        std::remove(m_entities.begin(), m_entities.end(), entity);
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
    }

    template<typename T>
    bool Scene::hasComponent(const Entity &entity) const
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
}
