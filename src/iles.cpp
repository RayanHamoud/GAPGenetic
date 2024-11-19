#include <iles.h>

void island::evolve(){
    population.evolution(N_max);
}

vector<Solution> island::selectMigrants(){
    std::random_device rd;             // Définition de rd
    std::mt19937 gen(rd()); 
    // Sélection aléatoire d'au plus 10 % de la population pour migrer
    int N = population.vec.size();
    std::vector<int> numbers(N);
    for (int i = 0; i < N; ++i) {
        numbers[i] = i;
    }
    vector<Solution> migrants;
    std::shuffle(numbers.begin(), numbers.end(), gen);
    int nb_migrants = getRandomInt(N/10);

    for (int k = 0; k < nb_migrants; k++){
        Solution migrant = population.vec[numbers[k]];
        if (!(migrant == population.best)){
            migrants.push_back(population.vec[numbers[k]]);
            //population.vec.erase(remove(population.vec.begin(), population.vec.end(), migrant), population.vec.end());   
        }   
    }
    return migrants;

}


void island::migrateFrom(vector<Solution> migrants){
    int nb_migrants = migrants.size();
    if (population.vec.size()>nb_migrants){
        for (int i = 0; i< nb_migrants; i++){
        population.remplacement(migrants[i]);
        }
    }
};

Solution run(int numIslands, int generations, int pop_size, int N_max, GAPInstance* inst, bool implant){
    std::vector<island> islands;
    int max_non_ameliorant = 20;
    for (int i = 0; i < numIslands; ++i) {
        islands.emplace_back(pop_size, N_max, inst);
    }
    Solution best_faisable = islands[0].population.best;

    std::vector<std::thread> threads;
    auto start = std::chrono::high_resolution_clock::now();
    int k = 0;
    for (int gen = 0; gen < generations; ++gen) {
        if (k > max_non_ameliorant){
            cout<<"Derniere gen ameliorante "<<gen - k<<endl;
            break;
            
        }

        threads.clear();

        for (int i = 0; i < numIslands; ++i) {
            threads.emplace_back(&island::evolve, &islands[i]);
        }
        for (auto& th : threads) th.join();

        bool non_ameliore = true;
        for (int i = 0; i < numIslands; ++i){
            if (i==numIslands-1){
                vector<Solution>migrants = islands[i].selectMigrants();
                islands[0].migrateFrom(migrants);
            }
            else{
                vector<Solution>migrants = islands[i].selectMigrants();
                islands[i+1].migrateFrom(migrants);
            }

            if (islands[i].population.best.fitness>best_faisable.fitness and islands[i].population.best.unfitness==0){
                best_faisable = islands[i].population.best;
                non_ameliore = false;
            }
        }
        if (non_ameliore){
            k+=1;
        }
        else{k=0;}

    }

    return best_faisable;
};

