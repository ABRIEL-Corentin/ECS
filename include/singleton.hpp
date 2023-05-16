////////////////////////
//
//  Created on Tue May 16 2023
//
//  singleton.hpp
//
////////////////////////

#pragma once

template<typename T>
class Singleton
{
    public:
        inline static T &getInstance() { return m_instance; }

    private:
        static T m_instance;
};

#include "inl/singleton.inl"
