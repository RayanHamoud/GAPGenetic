#pragma once
#include <fstream>
#include <filesystem>

#include <unordered_map>
#include <cmath>

// Assuming GAPInstance and other related classes are defined elsewhere
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <Instance.h>
#include <Solution.h>
#include <map>
#include <greedy.h>
#include <random>
#include <voisin.h>
#include<montee.h>
#include<chrono>
#include "thread"
#include "regex"
#include <iles.h>

void benchmark(const std::string &data_folder, const std::string &output_file)
{
    std::ofstream csv_file(output_file);
    csv_file << "Instance,Cost\n";

    std::regex pattern(R"(gap(\d+)\.txt$)"); // Pattern to match files like "gap5.txt"
    std::smatch match;

    for (const auto &entry : std::filesystem::directory_iterator(data_folder))
    {
        std::string file_path = entry.path().string();
        std::string file_name = entry.path().filename().string();

        // Check if the instance name ends with a number to determine the boolean flag
        bool is_number = (std::regex_search(file_name, match, pattern));

        int instance_count = is_number ? 4 : 5;

        for (int i = 0; i <= instance_count; ++i)
        {
            GAPInstance gap(file_path, i, is_number);
            string instance_name;
            if (is_number)
            {
                instance_name = file_name.substr(0, file_name.size() - 4) + "_c" +
                                std::to_string(gap.M) + std::to_string(gap.T) + +"-" + std::to_string(i + 1);
            }
            else
            {
                instance_name = file_name.substr(0, file_name.size() - 4) + "_" + file_name[3] + std::to_string(gap.M) + "-" + std::to_string(gap.T);
            }

            cout<<"Starting instance : "<<instance_name<<endl;
            // Construct GAPInstance with the current file and appropriate parameters

            // Generate an initial solution and perform tabu search
            int N_max = 200; //Nombre de fois qu'on s'autorise à ne pas améliorer la solution
            int pop_size = 100; 
            int numIslands = 8;
            int generations = 200;
            Solution gt = run(numIslands, generations, pop_size, N_max, &gap,false);


            //Record the cost of the best feasible solution
            if (gt.isFeasible())
            {
                int total_cost = gt.computeProfit();

                if (!is_number)
                {
                    total_cost = -total_cost;
                }
                csv_file << instance_name << "," << total_cost << "\n";
                std::cout << "Instance: "
                          << " | Cost: " << total_cost << std::endl;
            }
            else
            {
                csv_file << instance_name << ",infeasible\n";
                std::cout << "Instance: " << instance_name
                          << " | Solution is infeasible" << std::endl;
            }
        }
    }

    csv_file.close();
    std::cout << "Benchmark completed. Results saved to " << output_file << std::endl;
}

void processBenchmarkFiles(const std::string &benchmarkFile, const std::string &bestKnownFile, const std::string &outputFile)
{
    std::unordered_map<std::string, int> benchmarkResults;
    std::unordered_map<std::string, int> bestKnownValues;

    // Read benchmark results
    {
        std::ifstream file(benchmarkFile);
        std::string line;

        // Skip header
        std::getline(file, line);

        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            std::string instance;
            int cost;
            std::getline(ss, instance, ',');
            ss >> cost;
            benchmarkResults[instance] = cost;
        }
    }

    // Read best known values
    {
        std::ifstream file(bestKnownFile);
        std::string line;

        // Skip header
        std::getline(file, line);

        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            std::string instance;
            int cost;
            std::getline(ss, instance, ',');
            ss >> cost;
            bestKnownValues[instance] = cost;
        }
    }

    // Write merged results
    {
        std::ofstream outFile(outputFile);
        outFile << "Instance,Cost_Benchmark,Cost_Best,Gap\n";

        for (const auto &entry : benchmarkResults)
        {
            const std::string &instance = entry.first;
            int benchmarkCost = entry.second;
            int bestKnownCost = bestKnownValues.count(instance) ? bestKnownValues.at(instance) : -1;
            double gap =  (static_cast<double>(bestKnownCost - benchmarkCost)) * 100 / bestKnownCost ;

            outFile << instance << "," << benchmarkCost << "," << bestKnownCost << "," << std::abs(gap) << "\n";
        }

        outFile.close();
    }

    std::cout << "Merged results saved to '" << outputFile << "'" << std::endl;
}