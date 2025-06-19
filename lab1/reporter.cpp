#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <iomanip> // Для форматирования

struct Employee {
    int num;
    char name[10];
    double hours;
};

bool compareEmployees(const Employee& a, const Employee& b) {
    return a.num < b.num;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: reporter <input_filename> <output_filename> <hourly_rate>" << std::endl;
        return 1;
    }

    const char* inputFilename = argv[1];
    const char* outputFilename = argv[2];
    double hourlyRate = std::stod(argv[3]);

    std::ifstream inFile(inputFilename, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error opening file: " << inputFilename << std::endl;
        return 1;
    }

    std::vector<Employee> employees;
    Employee emp;
    while (inFile.read(reinterpret_cast<char*>(&emp), sizeof(Employee))) {
        employees.push_back(emp);
    }
    inFile.close();

    std::sort(employees.begin(), employees.end(), compareEmployees);

    std::ofstream outFile(outputFilename);
    if (!outFile) {
        std::cerr << "Error opening file: " << outputFilename << std::endl;
        return 1;
    }

    outFile << "Report for file \"" << inputFilename << "\"" << std::endl;
    outFile << "Employee ID, Name, Hours, Salary" << std::endl;
    for (const auto& emp : employees) {
        double salary = emp.hours * hourlyRate;
        outFile << emp.num << ", " << emp.name << ", "
                << std::fixed << std::setprecision(6) << emp.hours << ", "
                << std::fixed << std::setprecision(6) << salary << std::endl;
    }

    outFile.close();
    return 0;
}