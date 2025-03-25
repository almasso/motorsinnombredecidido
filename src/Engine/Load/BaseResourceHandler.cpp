#include "BaseResourceHandler.h"

BaseResourceHandler::BaseResourceHandler() :
    _resourceMemoryManager(nullptr) {
}

BaseResourceHandler::~BaseResourceHandler() = default;

void BaseResourceHandler::init(ResourceMemoryManager* memoryManager) {
    _resourceMemoryManager = memoryManager;
}

void BaseResourceHandler::shutdown() {
    delete this;
}
