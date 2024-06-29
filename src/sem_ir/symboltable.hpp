#pragma once
#include "../Quarzum.h"

struct SymbolInfo {
    char type;
    std::string name;
    std::string dataType;
    std::string scope;
    uint8_t args;
};

struct SymbolTable {
    inline void insert(const std::string& name, SymbolInfo info){
        table[name] = info;
    }
    inline void remove(const std::string& name){
        table.erase(name);
    }
    SymbolInfo* find(const std::string& name){
        auto it = table.find(name);
        if(it != table.end()){
            return &(it->second);
        }
        return nullptr;
    }
    std::unordered_map<std::string, SymbolInfo> table;
};

struct ScopedSymbolTable {
    inline void enterScope(){
        scopes.push_back(SymbolTable());
    }
    inline void leaveScope(){
        if(not scopes.empty()){
            scopes.pop_back();
        }
    }
    inline void insert(const std::string& name, SymbolInfo info){
        if(not scopes.empty()){
            scopes.back().insert(name, info);
        }
    }
    SymbolInfo* find(const std::string& name){
        for(auto it = scopes.rbegin(); it != scopes.rend(); ++it){
            if(it->find(name) != nullptr){
                return it->find(name);
            }
        }
        return nullptr;
    }
    std::vector<SymbolTable> scopes;
};

ScopedSymbolTable symbolTable;
