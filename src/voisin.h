#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <Instance.h>
#include <Solution.h>
#include <map>
using namespace std;


class Vois_1 {
    public : 
    string name;
    Solution sol;
    vector<Solution> voisins;
    GAPInstance *instance;
    Vois_1 (Solution solution, GAPInstance *inst) : sol(solution), instance (inst){}
    Vois_1 (string nom, Solution solution, GAPInstance *inst) : name(nom), sol(solution), instance (inst){}
       
    vector<Solution> generate() ;

};