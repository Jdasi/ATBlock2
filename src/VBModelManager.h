#pragma once

#include <map>
#include <memory>

#include "VBModel.h"

class Renderer;

class VBModelManager
{
public:
    VBModelManager(Renderer* _renderer);
    ~VBModelManager() = default;

    VBModel* getModel(const std::string& _name);

private:
    VBModel* createModelEntry(const std::string& _file_name);

    Renderer* renderer;

    std::map<std::string, std::unique_ptr<VBModel>> vb_models;

};
