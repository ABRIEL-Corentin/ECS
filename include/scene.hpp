////////////////////////
//
//  Created on Mon May 15 2023
//
//  scene.hpp
//
////////////////////////

#pragma once

#include "ecs.hpp"
#include "isystem.hpp"

namespace ecs
{
    using ComponentsContainer = std::unordered_map<std::type_index, std::any>;
    using SystemsContainer = std::unordered_map<Entity, std::vector<std::unique_ptr<ISystem>>>;

    class Scene
    {
        public:
            inline Entity createEntity();
            void clear();
            void launchSystems();

            template<typename T>
            Component<T> &getComponent(const Entity &entity);

            template<typename T>
            Components<T> &getComponents();

            template<typename T>
            Component<T> &addComponent(const Entity &entity);

            template<typename T>
            bool hasComponent(const Entity &entity);

            template<typename T>
            void removeComponent(const Entity &entity);

            template<typename T>
            void registerComponent();

            template<typename ... COMPONENTS>
            bool addSystem(const Entity &entity, System<COMPONENTS...> system);

            template<typename ... COMPONENTS>
            bool removeSystem(const Entity &entity, System<COMPONENTS...> system);

            template<typename ... COMPONENTS>
            bool hasSystem(const Entity &entity, System<COMPONENTS...> system);

        private:
            Entity m_current_entity;
            ComponentsContainer m_components;
            SystemsContainer m_systems;

            template<typename T>
            void removeDependSystems(const Entity &entity);
    };

    template<typename ... COMPONENTS>
    class SystemManager : public ISystem
    {
        public:
            SystemManager(Scene &scene, System<COMPONENTS...> system);

            void launch(const Entity &entity) override;
            bool requiredComponents(const std::type_info &type) const override;
            void *getPtr() const override;

        private:
            Scene &m_scene;
            System<COMPONENTS...> m_system;
    };
}

#include "inl/scene.inl"
