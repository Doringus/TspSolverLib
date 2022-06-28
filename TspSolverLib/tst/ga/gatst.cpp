#include <gtest/gtest.h>

#include <ga/ga.h>
#include <filesystem>
#include <fstream>

std::vector<std::string> split1(std::string strToSplit, char delimeter) {
    std::stringstream ss(strToSplit);
    std::string item;
    std::vector<std::string> splittedStrings;
    while (std::getline(ss, item, delimeter))
    {
        splittedStrings.push_back(item);
    }
    return splittedStrings;
}

template <typename T>
tspsolver::SquareMatrix<T> loadMatrix1(const std::filesystem::path& path, size_t size) {
    std::ifstream stream(path);
    tspsolver::SquareMatrix<T> result(size);
    if(stream.is_open()) {
        std::string str;
        std::vector<T> data;
        while (std::getline(stream, str)) {
            auto elems = split1(str, ' ');
            for(auto& e : elems) {
                int v = std::stoi(e);
                if(v == 0) {
                    v = tspsolver::infinity<int>;
                }
                data.push_back(v);
            }

        }
        std::copy(data.begin(), data.end(), result.begin());
    }
    return result;
}

template<typename Gene, typename Fitness>
class Log : public tspsolver::ga::ILogger<Gene, Fitness> {
public:
     void logIteration(const tspsolver::ga::Population<Gene, Fitness>& population) {
         auto it = std::max_element(population.begin(), population.end(), [](const auto& lhs, const auto& rhs){
             return lhs.getFitness() < rhs.getFitness();
         });
         fit.push_back((*it).getFitness());

       /*  auto it = std::accumulate(population.begin(), population.end(), 0.0, [](const auto& lhs, const auto& rhs){
             return lhs + rhs.getFitness();
         });
         fit.push_back(it / (double)population.size());
         */
     }
     std::vector<Fitness> fit;
};
template<typename Fitness>
void writeLog(const std::vector<Fitness>& fit) {
    std::ofstream file;
    file.open ("example.txt");
    for(int i = 0; i < fit.size(); ++i) {
        file << fit.at(i) << "\n";
    }

    file.close();
}

TEST(GATest, test1) {

    auto matrix = loadMatrix1<int>("graphs\\special\\att48_d.txt", 48);

    tspsolver::ga::Chromosome<int> g1(48);
    std::iota(g1.begin(), g1.end(), 0);
    std::vector<tspsolver::ga::Chromosome<int>> population;
    population.push_back(g1);
    std::random_device rd;
    std::mt19937 g(rd());
    while (population.size() < 120) {
       // std::shuffle(g1.begin(), g1.end(), g);
        std::next_permutation(g1.begin(), g1.end());
        population.push_back(g1);
    }
    std::vector<int> resultArr;
    for(size_t i = 0; i < 1; ++i) {
        auto log = std::make_shared<Log<int, double>>();
        tspsolver::ga::GeneticAlgorithm<int, double> alg(population, 0.05, 0.8, 2000, log);
        alg.setFitness([&matrix](const std::vector<int>& chromosome){
            double result = 0;
            for(size_t i = 0; i < chromosome.size() - 1; ++i) {
                result += matrix.at(chromosome.at(i), chromosome.at(i + 1));
            }
            result + matrix.at(chromosome.back(), chromosome.front());
            return 10000.0 / result;
        });
        auto random = std::make_shared<std::mt19937_64>();

       // alg.setSelection(std::make_shared<tspsolver::ga::RouletteSelection<int, double>>(random));
        alg.setSelection(std::make_shared<tspsolver::ga::TournamentSelection<int, double>>(random));
        alg.setCrossover(std::make_shared<tspsolver::ga::Ox2Crossover<int>>(random));
        alg.setMutation(std::make_shared<tspsolver::ga::InverseMutation<int>>(random));
        auto t1 = std::chrono::steady_clock::now();
        auto ind = alg.evolve();
        auto t2 = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        resultArr.push_back(elapsed_ms.count());
       writeLog(log->fit);
        double result = 0;
        for(size_t i = 0; i < ind.getChromosome().size() - 1; ++i) {
            result += matrix.at(ind.getChromosome().at(i), ind.getChromosome().at(i + 1));
        }
        result += matrix.at(ind.getChromosome().back(), ind.getChromosome().front());
        //resultArr.push_back(result);
    }
    auto sum = std::accumulate(resultArr.begin(), resultArr.end(), 0);
    double mid = sum / 50.0f;
    auto it = std::min_element(resultArr.begin(), resultArr.end());
    int minIt = *it;
    int j =0 ;
}