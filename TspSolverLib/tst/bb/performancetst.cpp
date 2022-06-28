#include <gtest/gtest.h>

#include <bb/branchandbound.h>
#include <tspsolverlib.h>

#include <filesystem>
#include <fstream>
#include <chrono>

std::vector<std::string> split(std::string strToSplit, char delimeter) {
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
tspsolver::SquareMatrix<T> loadMatrix(const std::filesystem::path& path, size_t size) {
    std::ifstream stream(path);
    tspsolver::SquareMatrix<T> result(size);
    if(stream.is_open()) {
        std::string str;
        std::vector<T> data;
        while (std::getline(stream, str)) {
            auto elems = split(str, ' ');
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

TEST(BBPerfTest, test42) {

    /*
    for(size_t i = 5; i < 50; ++i) {
        auto matrix = loadMatrix<int>("graphs\\" + std::to_string(i) + ".txt", i);
        tspsolver::BBSolver solver(matrix, 1);

        auto t1 = std::chrono::steady_clock::now();
        auto result = solver.solve();
        auto t2 = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        std::cout <<"Size: " << i << " " <<result.weight << " time: "<<elapsed_ms.count() << "\n";
    }
    */

    auto matrix = loadMatrix<int>("graphs\\special\\fri26_d.txt", 26);
    tspsolver::BBSolver solver(matrix, 1);

    auto t1 = std::chrono::steady_clock::now();
    auto result = solver.solve();
    auto t2 = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout  << " time: "<<elapsed_ms.count() << "\n";
  //  EXPECT_EQ(result.weight, 73);
}