////////////////////////
//
//  Created on Mon Jun 12 2023
//
//  world.hpp
//
////////////////////////

#pragma once

#include "types.hpp"
#include "isystem.hpp"

namespace ecs
{
    class World
    {
        public:
            World() = default;

            Entity createEntity();
            void deleteEntity(const Entity &entity);
            void clear();
            void launchSystems();

            template<typename T>
            Component<T> &getComponent(const Entity &entity)
            {
                static Component<T> null_component = std::nullopt;
                Components<T> &components = getComponents<T>();

                if (components.size() <= entity)
                    return null_component;
                return components.at(entity);
            }

            template<typename T>
            Components<T> &getComponents()
            {
                auto components = _components.find(std::type_index(typeid(T)));

                if (components == _components.end()) {
                    registerComponent<T>();
                    return getComponents<T>();
                }
                return std::any_cast<Components<T> &>(components->second);
            }

            template<typename T>
            Component<T> &addComponent(const Entity &entity)
            {
                Components<T> &components = getComponents<T>();

                if (components.size() <= entity)
                    components.resize(entity + 1);
                components.at(entity).emplace();
                return components.at(entity);
            }

            template<typename T>
            bool hasComponent(const Entity &entity)
            {
                Components<T> &components = getComponents<T>();

                return components.size() > entity && components.at(entity) != std::nullopt;
            }

            template<typename T>
            void removeComponent(const Entity &entity)
            {
                Components<T> &components = getComponents<T>();

                if (components.size() <= entity)
                    return;
                components.at(entity).reset();
                removeDependSystems<T>(entity);
            }

            template<typename T>
            void registerComponent()
            {
                if (_components.find(std::type_index(typeid(T))) != _components.end())
                    return;

                _components.insert({std::type_index(typeid(T)), Components<T>()});
            }

            template<typename ... COMPONENTS>
            bool addSystem(const Entity &entity, System<COMPONENTS...> system)
            {
                bool status = true;

                ((status = hasComponent<COMPONENTS>(entity) ? status : false), ...);

                if (!status || !system)
                    return false;

                auto systems = _systems.find(entity);

                if (systems == _systems.end()) {
                    _systems.insert({entity, Systems()});
                    _systems.at(entity).push_back(std::make_unique<SystemManager<COMPONENTS...>>(*this, system));
                } else {
                    for (auto it = systems->second.begin(); it != systems->second.end(); ++it)
                        if ((*it)->getPtr() == system)
                            return false;

                    systems->second.push_back(std::make_unique<SystemManager<COMPONENTS...>>(*this, system));
                }
                return true;
            }

            template<typename ... COMPONENTS>
            bool removeSystem(const Entity &entity, System<COMPONENTS...> system)
            {
                auto systems = _systems.find(entity);

                if (systems == _systems.end() || !system)
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
            bool hasSystem(const Entity &entity, System<COMPONENTS...> system)
            {
                auto systems = _systems.find(entity);

                if (system == _systems.end() || !system)
                    return false;

                for (auto it = systems->second.begin(); it != systems->second.end(); ++it)
                    if ((*it)->getPtr() == system)
                        return true;
                return false;
            }

            template<typename T>
            void removeDependSystems(const Entity &entity)
            {
                auto systems = _systems.find(entity);

                if (systems == _systems.end())
                    return;

                for (std::size_t i = 0; i < systems->second.size(); ++i) {
                    if ((systems->second.at(i))->requiredComponent(typeid(T))) {
                        systems->second.erase(systems->second.begin() + i);
                        --i;
                    }
                }
            }

        private:
            ComponentsContainer _components;
            SystemsContainer _systems;
            Entity _current_entity;
            Entities _deleted_entities;

            template<typename ... COMPONENTS>
            class SystemManager : public ISystem
            {
                public:
                    SystemManager(World &world, System<COMPONENTS...> system)
                        : _world(world),
                          _system(system)
                    { }

                    virtual void launch(const Entity &entity) override
                    {
                        if (_system)
                            _system(*_world.getComponent<COMPONENTS>(entity)...);
                    }

                    virtual bool requiredComponent(const std::type_info &type) const override
                    {
                        bool status = false;

                        ((status = typeid(COMPONENTS) == type ? true : status), ...);
                        return status;
                    }

                    virtual void *getPtr() const override
                    {
                        return reinterpret_cast<void *>(_system);
                    }

                private:
                    World &_world;
                    System<COMPONENTS...> _system;
            };
    };
}
