#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <sys/wait.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <vector>
#include <sstream>
#include <iomanip>

// Функция для выполнения команды в дочернем процессе
void executeCommand(const std::string& command) {
    pid_t pid = fork(); // Создаем дочерний процесс
    if (pid == 0) {
        // Дочерний процесс
        execl("/bin/sh", "sh", "-c", command.c_str(), (char*)NULL);
        // Если execl завершился с ошибкой
        std::cerr << "execl failed: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Ошибка при создании дочернего процесса
        std::cerr << "fork failed: " << strerror(errno) << std::endl;
    } else {
        // Родительский процесс ожидает завершения дочернего
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            std::cerr << "Command failed with exit code: " << WEXITSTATUS(status) << std::endl;
        }
    }
}

// Структура для хранения данных о сотруднике
struct Employee {
    int num;
    char name[10];
    double hours;
};

// Функция для вывода содержимого бинарного файла
void printBinaryFile(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    Employee emp;
    while (inFile.read(reinterpret_cast<char*>(&emp), sizeof(Employee))) {
        std::cout << "Employee ID: " << emp.num
                  << ", Name: " << emp.name
                  << ", Hours: " << emp.hours << std::endl;
    }

    if (!inFile.eof() && inFile.fail()) {
        std::cerr << "Error reading file: " << filename << std::endl;
    }

    inFile.close();
}

// Функция для вывода содержимого текстового файла (отчета)
void printReportFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        std::cout << line << std::endl;
    }

    inFile.close();
}

// Функция для логирования в файл
void logToFile(const std::string& message, const std::string& logFilename = "log.txt") {
    std::ofstream logFile(logFilename, std::ios::app);
    if (!logFile) {
        std::cerr << "Error opening log file: " << logFilename << std::endl;
        return;
    }

    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);
    logFile << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S") << " - " << message << std::endl;
    logFile.close();
}

// Функция для проверки ввода числа
bool validateInput(int value, const std::string& prompt) {
    if (value <= 0) {
        std::cerr << prompt << " must be a positive number." << std::endl;
        return false;
    }
    return true;
}

// Функция для проверки ввода числа с плавающей точкой
bool validateInput(double value, const std::string& prompt) {
    if (value <= 0) {
        std::cerr << prompt << " must be a positive number." << std::endl;
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    std::string binaryFilename;
    int numRecords;
    std::string reportFilename;
    double hourlyRate;

    // Обработка аргументов командной строки
    if (argc == 5) {
        binaryFilename = argv[1];
        numRecords = std::stoi(argv[2]);
        reportFilename = argv[3];
        hourlyRate = std::stod(argv[4]);
    } else {
        // Ввод имени бинарного файла и количества записей
        std::cout << "Enter binary file name: ";
        std::cin >> binaryFilename;
        std::cout << "Enter number of records: ";
        std::cin >> numRecords;

        // Ввод имени файла отчета и почасовой ставки
        std::cout << "Enter report file name: ";
        std::cin >> reportFilename;
        std::cout << "Enter hourly rate: ";
        std::cin >> hourlyRate;
    }

    // Валидация ввода
    if (!validateInput(numRecords, "Number of records") || !validateInput(hourlyRate, "Hourly rate")) {
        return EXIT_FAILURE;
    }

    // Запуск программы creator
    std::string creatorCommand = "./creator " + binaryFilename + " " + std::to_string(numRecords);
    std::cout << "Running creator: " << creatorCommand << std::endl;
    logToFile("Running creator: " + creatorCommand);
    executeCommand(creatorCommand);

    // Вывод содержимого бинарного файла
    std::cout << "Contents of binary file:" << std::endl;
    printBinaryFile(binaryFilename);

    // Запуск программы reporter
    std::string reporterCommand = "./reporter " + binaryFilename + " " + reportFilename + " " + std::to_string(hourlyRate);
    std::cout << "Running reporter: " << reporterCommand << std::endl;
    logToFile("Running reporter: " + reporterCommand);
    executeCommand(reporterCommand);

    // Вывод содержимого файла отчета
    std::cout << "Report:" << std::endl;
    printReportFile(reportFilename);

    return 0;
}