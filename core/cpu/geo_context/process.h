#pragma once
#include "string"
class GeoContextCPU;
class ProcessCPU{
public:
    using Process = void(*)(GeoContextCPU&, float);
    Process processFunction;

    std::string name = "Unnamed process";
    bool activated = false;

    ProcessCPU(Process processFunction, std::string name)
        :processFunction(processFunction),
        name(name)
        {}
    void update(GeoContextCPU & context, float delta){
        if (activated){
            processFunction(context, delta);
        }
    }
};