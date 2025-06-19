#ifndef ARRAY_UTILS_H
#define ARRAY_UTILS_H

#include <vector>
#include <thread>

void findMinMax(const std::vector<int>& arr, int& min, int& max);
double findAverage(const std::vector<int>& arr);
void replaceMinMaxWithAverage(std::vector<int>& arr, double average);

#endif // ARRAY_UTILS_H