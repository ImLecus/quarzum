#pragma once
#include "../ast/astnode.hpp"

class IdentationManager {
public:
    Container* getLastLayer(){
        return layers.at(layers.size() - 1);
    }
    /*
    * Opens an indentation of type T. While this indentation is
    * the last layer, it will contain all the next nodes as childs.
    */
    template<typename T>
    void open(T* layer){
        ++identationLayer;
        layers.push_back(layer);
    }

    void close(){
        --identationLayer;
        Container* lastLayer = getLastLayer();
        layers.pop_back();
        getLastLayer()->add(lastLayer);
    }

    bool hasLayers(){
        return identationLayer > 1;
    }

private:
    u_int8_t identationLayer; 
    std::vector<Container*> layers;
};
