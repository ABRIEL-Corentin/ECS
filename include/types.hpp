////////////////////////
//
//  Created on Mon Jun 12 2023
//
//  types.hpp
//
////////////////////////

#pragma once

#include <unordered_map>
#include <map>
#include <any>
#include <optional>
#include <vector>
#include <functional>
#include <typeindex>
#include <memory>

namespace ecs
{
    class ISystem;

    using Entity = std::size_t;
    using Entities = std::vector<Entity>;
    using ComponentsContainer = std::unordered_map<std::type_index, std::any>;
    using Systems = std::vector<std::unique_ptr<ISystem>>;
    using SystemsContainer = std::map<Entity, Systems>;

    template<typename ... COMPONENTS>
    using System = void (*)(COMPONENTS &...);

    template<typename T>
    using Component = std::optional<T>;

    template<typename T>
    using Components = std::vector<Component<T>>;
}
