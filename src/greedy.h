#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <Instance.h>
#include <Solution.h>
#include <map>
using namespace std;

vector<int> best_ratio(int t, GAPInstance *inst);

Solution greedy(GAPInstance *inst);

