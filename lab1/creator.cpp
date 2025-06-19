// creator.cpp
#include <iostream>
#include <fstream>
#include <cstring>

struct Employee {
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: creator <filename> <number_of_records>" << std::endl;
        return 1;
    }

    const char* filename = argv[1];
    int numRecords = std::stoi(argv[2]);

    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }

    for (int i = 0; i < numRecords; ++i) {
        Employee emp;
        std::cout << "Enter employee ID: ";
        std::cin >> emp.num;
        std::cout << "Enter employee name: ";
        std::cin >> emp.name;
        std::cout << "Enter hours worked: ";
        std::cin >> emp.hours;
        outFile.write(reinterpret_cast<char*>(&emp), sizeof(Employee));
    }

    outFile.close();
    return 0;
}
