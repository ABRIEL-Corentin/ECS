////////////////////////
//
//  Created on Mon May 15 2023
//
//  scene.hpp
//
////////////////////////

#pragma once

#include "ecs.hpp"

namespace ecs
{
    using ComponentsContainer = std::unordered_map<std::type_index, std::any>;

    class Scene
    {
        public:
            Scene() = default;

            inline Entity createEntity();
            inline void destroyEntity(const Entity &entity);

            template<typename T>
            Component<T> &getComponent(const Entity &entity);

            template<typename T>
            Components<T> &getComponents();

            template<typename T>
            Component<T> &addComponent(const Entity &entity);

            template<typename T>
            bool hasComponent(const Entity &entity) const;

            template<typename T>
            void removeComponent(const Entity &entity);

            template<typename T>
            void registerComponent();

        private:
            Entity m_current_entity;
            Entities m_entities;
            ComponentsContainer m_components;
    };
}

#include "inl/scene.inl"
