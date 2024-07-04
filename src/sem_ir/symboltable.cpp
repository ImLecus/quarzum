#pragma once
#include "../Quarzum.h"

struct SymbolInfo {
    char type; // 'v' for variable, 'f' for function
    string name;
    string dataType;
    string scope;
    uint8 args;
};

struct SymbolTable {
    inline void insert(const string& name, SymbolInfo info){
        table[name] = info;
    }
    inline void remove(const string& name){
        table.erase(name);
    }
    SymbolInfo* find(const string& name){
        auto it = table.find(name);
        if(it != table.end()){
            return &(it->second);
        }
        return nullptr;
    }
    std::unordered_map<string, SymbolInfo> table;
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
    inline void insert(const string& name, SymbolInfo info){
        if(not scopes.empty()){
            scopes.back().insert(name, info);
        }
    }
    SymbolInfo* find(const string& name){
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
