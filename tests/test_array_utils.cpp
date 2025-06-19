#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "/Users/vvv/Documents/OS/lab2/doctest/doctest/doctest.h"
#include "../src/array_utils.h"

TEST_CASE("Тестирование функций для работы с массивом") {
    std::vector<int> arr = {-100, -100, 0, 50 , 144};

    SUBCASE("Поиск минимального и максимального элементов") {
        int min, max;
        findMinMax(arr, min, max);
        CHECK(min == -100);
        CHECK(max == 144);
    }

    SUBCASE("Поиск среднего значения") {
        double average = findAverage(arr);
        CHECK(average == -1);
    }

    SUBCASE("Замена min и max на среднее значение") {
        double average = findAverage(arr);
        replaceMinMaxWithAverage(arr, average);
        CHECK(arr[0] == -1); // Минимальный элемент заменен
        CHECK(arr[4] == -1); // Максимальный элемент заменен
    }
}