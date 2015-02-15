#ifndef CMAP_H
#define CMAP_H

// C++ standard includes
#include <map>

// int2ssl includes
#include "../Hacks/Types.h"

// Third party includes

template<typename A, typename B, typename C,typename D>
class CMap
{
    std::map<A, C> _map;

    public:
        CMap()
        {
        }

        uint32_t GetSize() const
        {
            return _map.size();
        }

        void RemoveAll()
        {
            _map.clear();
        }

        void SetAt(uint32_t offset, C value)
        {
            if (_map.find(offset) != _map.end())
            {
                _map.at(offset) = value;
            }

            _map.insert(std::pair<A, C>(offset, value));
        }

        bool Lookup(uint32_t offset, C& value) const
        {
            auto it = _map.find(offset);
            if (it != _map.end())
            {
                value = it->second;
                return true;
            }
            return false;
        }

        void InitHashTable(uint32_t n)
        {

        }
};

#endif // CMAP_H
