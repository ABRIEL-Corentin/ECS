////////////////////////
//
//  Created on Mon May 15 2023
//
//  main.cpp
//
////////////////////////

#include "scene.hpp"

using Transform = struct Transform {
    double pos_x;
    double pos_y;
    double pos_z;
};

std::ostream &operator<<(std::ostream &stream, const Transform &transform)
{
    return stream << transform.pos_x << " " << transform.pos_y << " " << transform.pos_z;
}

Transform &operator>>(std::istream &stream, Transform &transform)
{
    stream >> transform.pos_x;
    stream >> transform.pos_y;
    stream >> transform.pos_z;
    return transform;
}

template<typename T>
void put_components(const ecs::Components<T> &components)
{
    std::cout << "components:" << std::endl;
    for (auto component = components.begin(); component != components.end(); ++component) {
        if (component->has_value())
            std::cout << "  " << **component << std::endl;
        else
            std::cout << "  " << "null" << std::endl;
    }
    std::cout << std::endl;
}

void velocity(double, float &, int &)
{
    std::cout << "call velocity" << std::endl;
}

int main()
{
    ecs::Scene scene = ecs::Scene();

    std::cout << std::boolalpha;

    for (ssize_t i = 10 - 1; i >= 0; --i) {
        scene.addComponent<float>(i);
        scene.addComponent<int>(i);
        scene.addSystem(i, velocity);
    }

    scene.launchSystems();

    return 0;
}
