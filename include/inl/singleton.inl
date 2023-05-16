////////////////////////
//
//  Created on Tue May 16 2023
//
//  singleton.inl
//
////////////////////////

#include "singleton.hpp"

template<typename T>
T Singleton<T>::m_instance = T();
