////////////////////////
//
//  Created on Mon Jun 12 2023
//
//  isystem.hpp
//
////////////////////////

#pragma once

#include "types.hpp"

namespace ecs
{
    class ISystem
    {
        public:
            virtual ~ISystem() = default;

            virtual void launch(const Entity &entity) = 0;
            virtual bool requiredComponent(const std::type_info &type) const = 0;
            virtual void *getPtr() const = 0;
    };
}
