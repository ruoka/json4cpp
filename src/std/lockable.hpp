#pragma once

#include <mutex>

namespace ext {

template<class Class>
struct lockable : public Class, public std::mutex
{
    using Class::Class;

    lockable() = default;

    lockable(lockable<Class>&& lc) : Class{std::move(lc)}, mutex{}
    {
        if(!lc.try_lock()) std::terminate(); // Do not move locked objects!
    }
};

} // namespace ext
