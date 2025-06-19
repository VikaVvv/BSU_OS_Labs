#include "array_utils.h"
#include <iostream>
#include <thread>
#include <chrono>

void findMinMax(const std::vector<int>& arr, int& min, int& max) {
    if (arr.empty()) return;

    min = arr[0];
    max = arr[0];

    for (int num : arr) {
        if (num < min) min = num;
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        if (num > max) max = num;
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        //эта функцяи искусственно замедляет цикл чтобы продемонстрировать работу потоков и для синхронизации

    }
         std::cout << "Минимальный элемент: " << min << std::endl;
         std::cout << "Максимальный элемент: " << max << std::endl;

  
}

double findAverage(const std::vector<int>& arr) {
    if (arr.empty()) return 0.0;

    double sum = 0;
    for (int num : arr) {
        sum += num;
        std::this_thread::sleep_for(std::chrono::milliseconds(12));
    }
    
        std::cout << "Среднее значение: " << int(sum / arr.size()) << std::endl;
    return sum / arr.size();
}

void replaceMinMaxWithAverage(std::vector<int>& arr, double average) {
    int min, max;
    findMinMax(arr, min, max);

    for (int& num : arr) {
        if (num == min || num == max) {
            num = static_cast<int>(average);
        }
    }
}