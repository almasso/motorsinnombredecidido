#include "BaseResourceHandler.h"

BaseResourceHandler::BaseResourceHandler() = default;

BaseResourceHandler::~BaseResourceHandler() = default;

void BaseResourceHandler::init(ResourceMemoryManager* memoryManager) {
    _resourceMemoryManager = memoryManager;
}

void BaseResourceHandler::shutdown() {
    delete this;
}
