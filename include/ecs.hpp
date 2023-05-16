////////////////////////
//
//  Created on Mon May 15 2023
//
//  ecs.hpp
//
////////////////////////

#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <unordered_map>
#include <optional>
#include <memory>
#include <any>
#include <typeindex>
#include <algorithm>
#include <filesystem>
#include <dlfcn.h>
#include <boost/type_index.hpp>

namespace ecs
{
    using Entity = std::size_t;
    using Entities = std::vector<Entity>;

    template<typename ... COMPONENTS>
    using System = void (*)(double, COMPONENTS &...);

    template<typename COMPONENT>
    using Component = std::optional<COMPONENT>;

    template<typename COMPONENT>
    using Components = std::vector<Component<COMPONENT>>;
}
