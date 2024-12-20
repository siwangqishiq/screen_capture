#include "ui/ui.h"
#include "ui/container.h"

namespace purple{
    PWidget FunContainer(std::vector<PWidget> children){
        std::shared_ptr<Container> container = std::make_shared<Container>();
        return container;
    }
}

