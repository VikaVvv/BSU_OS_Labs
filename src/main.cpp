#include "array_utils.h"
#include <iostream>
#include <vector>
#include <thread>

int main() {
    int size;
    std::cout << "Введите размер массива: ";
    std::cin >> size;

    std::vector<int> arr(size);
    std::cout << "Введите элементы массива: ";
    for (int i = 0; i < size; ++i) {
        std::cin >> arr[i];
    }

    int min, max;
    double average;

    // Создаем потоки
    std::thread minMaxThread([&]() { findMinMax(arr, min, max); });
    std::thread averageThread([&]() { average = findAverage(arr); });
// так как переменные захвачены по ссылке их изменения видны в основном потоке 
    // Ожидаем завершения потоков
    minMaxThread.join(); //блокирует выполнение основного потока до тех пор, пока поток,
    // для которого он вызван, не завершит свою работу.
    averageThread.join();

    // Выводим результаты
     // Заменяем min и max на среднее значение
    replaceMinMaxWithAverage(arr, average);

    // Выводим измененный массив
    std::cout << "Измененный массив: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}