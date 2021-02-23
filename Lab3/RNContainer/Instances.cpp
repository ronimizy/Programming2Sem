//
// Created by Георгий Круглов on 22.02.2021.
//

#ifndef RNCONTAINER_INSTANCES
#define RNCONTAINER_INSTANCES

#include "../RNContainer.h"
#include "../Node.h"

#include <vector>

/** LinkedMap<std::string, int> **/
template
class RNContainer::LinkedMap<std::string, int>;

/** LinkedMap<std::string, LinkedMap<std::string, int>> **/
template
class RNContainer::LinkedMap<std::string, RNContainer::LinkedMap<std::string, int>>;

/** LinkedMap<std::string, LinkedMap<std::string, std::vector<Node>>> **/
template
class RNContainer::LinkedMap<std::string, RNContainer::LinkedMap<std::string, std::vector<Node>>>;

#endif //RNCONTAINER_INSTANCES