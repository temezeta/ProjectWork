#include "misc.h"

namespace Teemu {
Course::ResourceMap negativeResourceMap(Course::ResourceMap resourcemap)
{
    auto newResource = resourcemap;
    for(auto resource: newResource){
        newResource.at(resource.first) = resource.second * -1;
    }
    return newResource;
}

}
