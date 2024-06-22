#pragma once
#include <unordered_map>
#include <string>
#include <vector>

struct SymbolInfo {
    char type;
    std::string name;
    std::string dataType;
    std::string scope;
};

struct SymbolTable {
    void insert(std::string name, SymbolInfo info){
        table[name] = info;
    }
    void remove(std::string name){
        table.erase(name);
    }
    SymbolInfo* find(std::string name){
        auto it = table.find(name);
        if(it != table.end()){
            return &(it->second);
        }
        return nullptr;
    }
    std::unordered_map<std::string, SymbolInfo> table;
};

struct ScopedSymbolTable {
    void enterScope(){
        scopes.push_back(SymbolTable());
    }
    void leaveScope(){
        if(not scopes.empty()){
            scopes.pop_back();
        }
    }
    void insert(std::string name, SymbolInfo info){
        if(not scopes.empty()){
            scopes.back().insert(name, info);
        }
    }
    SymbolInfo* find(std::string name){
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
