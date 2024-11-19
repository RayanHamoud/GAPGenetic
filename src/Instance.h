#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class GAPInstance
{
public:
    int M;                        // Number of agents
    int T;                        // Number of tasks
    vector<vector<int>> profit;     // profit matrix: n rows and m columns
    vector<vector<int>> resource; // Resource usage matrix: n rows and m columns
    vector<int> capacities;       // Capacities of each agent
    vector<vector<int>> delta;
    bool max;

    // Constructor that reads the data from a given file
    GAPInstance(const string &filename, int instance_number, bool maximization = true)
    {
        max = maximization;
        ifstream file(filename);
        if (!file.is_open())
        {
            cerr << "Error: Could not open file " << filename << endl;
            exit(1);
        }
        int dummy;
        // Read the number of agents
        file >> dummy;

        int current_instance = 0;

        // Loop through the file to find the correct instance
        while (current_instance < instance_number)
        {
            // Skip over the first line of each instance (dummy values)
            file >> M >> T;

            // Skip the profit and resource matrix for the current instance
            for (int i = 0; i < M; ++i)
            {
                for (int j = 0; j < T; ++j)
                {
                    file >> dummy; // Skip profit values
                }
            }

            for (int i = 0; i < M; ++i)
            {
                for (int j = 0; j < T; ++j)
                {
                    file >> dummy; // Skip resource values
                }
            }

            // Skip the capacities
            for (int i = 0; i < M; ++i)
            {
                file >> dummy;
            }

            ++current_instance;
        }

        file >> M;

        // Read the number of tasks and the constraint value
        file >> T;

        // Resize the matrices to fit the data
        profit.resize(M, vector<int>(T));
        resource.resize(M, vector<int>(T));

        // Read the profit matrix
        for (int i = 0; i < M; ++i)
        {
            for (int j = 0; j < T; ++j)
            {
                file >> profit[i][j];
                if (!maximization)
                {
                    profit[i][j] = -profit[i][j];
                }
            }
        }

        // Read the resource matrix
        for (int i = 0; i < M; ++i)
        {
            for (int j = 0; j < T; ++j)
            {
                file >> resource[i][j];
            }
        }

        // Read the capacities of each agent
        capacities.resize(M);
        for (int i = 0; i < M; ++i)
        {
            file >> capacities[i];
        }

        file.close();

        // Create the modified profit matrix (same size as the original profit matrix)
        delta.resize(M, vector<int>(T));
        int S = 0;
        // Step 1: For each task, find the maximum profit among all agents
        for (int j = 0; j < T; ++j)
        {
            int max_cost_for_task_j = -1; // Initialize with a very low value

            // Find the maximum profit for task j among all agents
            for (int i = 0; i < M; ++i)
            {
                if (profit[i][j] > max_cost_for_task_j)
                {

                    max_cost_for_task_j = profit[i][j];
                }
            }
            S += max_cost_for_task_j;
            // Step 2: Compute delta(i, j) for each agent i for task j
            for (int i = 0; i < M; ++i)
            {
                delta[i][j] = -max_cost_for_task_j + profit[i][j];
            }
        }
        cout << "   K is equal to " << S << endl;
    }

    // Method to display the instance data (for debugging purposes)
    void display()
    {
        cout << "Number of Agents (M): " << M << endl;
        cout << "Number of Tasks (T): " << T << endl;

        cout << "\nCost Matrix:" << endl;
        for (const auto &row : profit)
        {
            for (const auto &val : row)
            {
                cout << val << " ";
            }
            cout << endl;
        }

        cout << "\nResource Matrix:" << endl;
        for (const auto &row : resource)
        {
            for (const auto &val : row)
            {
                cout << val << " ";
            }
            cout << endl;
        }

        cout << "\nCapacities:" << endl;
        for (const auto &cap : capacities)
        {
            cout << cap << " ";
        }
        cout << endl;
    }
};