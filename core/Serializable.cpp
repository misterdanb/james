#include "Serializable.hpp"

using namespace james;
using namespace james::core;

std::ostream &operator<<(std::ostream &os, Serializable &serializable)
{
    serializable.Serialize(os);

    return os;
}

std::istream &operator>>(std::istream &is, Serializable &serializable)
{
    serializable.Deserialize(is);

    return is;
}

