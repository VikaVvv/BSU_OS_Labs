#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN // This tells Doctest to provide a main() function
#include "doctest.h" // Include Doctest header
#include <fstream>
#include <cstring>

struct Employee {
    int num;
    char name[10];
    double hours;
};

TEST_CASE("Create binary file") {
    const char* filename = "test_employees.bin";
    std::ofstream outFile(filename, std::ios::binary);
    Employee emp = {1, "John", 40};
    outFile.write(reinterpret_cast<char*>(&emp), sizeof(Employee));
    outFile.close();

    std::ifstream inFile(filename, std::ios::binary);
    Employee readEmp;
    inFile.read(reinterpret_cast<char*>(&readEmp), sizeof(Employee));
    inFile.close();

    CHECK(readEmp.num == 1);
    CHECK(std::strcmp(readEmp.name, "John") == 0);
    CHECK(readEmp.hours == doctest::Approx(40));

    std::remove(filename);
}

TEST_CASE("Create report file") {
  const char* inputFilename = "test_employees.bin";
    const char* outputFilename = "test_report.txt";
    double hourlyRate = 20.0;

    std::ofstream outFile(inputFilename, std::ios::binary);
    Employee emp = {1, "John", 40};
    outFile.write(reinterpret_cast<char*>(&emp), sizeof(Employee));
    outFile.close();

    std::ifstream inFile(inputFilename, std::ios::binary);
    std::ofstream reportFile(outputFilename);
    Employee readEmp;
    inFile.read(reinterpret_cast<char*>(&readEmp), sizeof(Employee));
    double salary = readEmp.hours * hourlyRate;
    reportFile << "Report for file \"" << inputFilename << "\"" << std::endl;
    reportFile << "Employee ID, Name, Hours, Salary" << std::endl;
    reportFile << readEmp.num << ", " << readEmp.name << ", " << readEmp.hours << ", " << salary << std::endl;
    inFile.close();
    reportFile.close();

    std::ifstream reportInFile(outputFilename);
    std::string line;
    std::getline(reportInFile, line);
    CHECK(line == "Report for file \"test_employees.bin\"");
    std::getline(reportInFile, line);
    CHECK(line == "Employee ID, Name, Hours, Salary");
    std::getline(reportInFile, line);
    CHECK(line == "1, John, 40, 800"); // Теперь этот CHECK пройдет успешно
    reportInFile.close();

    std::remove(inputFilename);
    std::remove(outputFilename);
}