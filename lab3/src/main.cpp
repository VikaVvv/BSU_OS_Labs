#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include "marker_thread.h"

using namespace std;

int main() {
    // 1. Захватить память под массив целых чисел
    int array_size;
    cout << "Enter array size: ";
    cin >> array_size;
    vector<int> array(array_size, 0);

    // 2. Инициализировать элементы массива нулями (уже сделано при создании)

    // 3. Запросить количество потоков marker
    int num_threads;
    cout << "Enter number of marker threads: ";
    cin >> num_threads;

    // Подготовка синхронизации
    mutex array_mutex;
    condition_variable cv;
    atomic<bool> start_signal(false);
    atomic<bool> terminate_signal(false);
    atomic<bool> continue_signal(false);

    // Создание потоков marker
    vector<unique_ptr<MarkerThread>> markers;
    vector<thread> threads;
    vector<atomic<bool>> thread_stopped(num_threads);

    for (int i = 0; i < num_threads; ++i) {
        markers.emplace_back(make_unique<MarkerThread>(
            i + 1, array, array_mutex, cv, start_signal, terminate_signal, continue_signal));
        threads.emplace_back(marker_thread_func, ref(*markers.back()));
        thread_stopped[i] = false;
    }

    // 5. Дать сигнал на начало работы всех потоков marker
    {
        lock_guard<mutex> lock(array_mutex);
        start_signal = true;
    }
    cv.notify_all();

    // 6. Основной цикл управления потоками
    while (true) {
        // 6.1 Ждать, пока все потоки marker не подадут сигналы о невозможности продолжения
        {
            unique_lock<mutex> lock(array_mutex);
            cv.wait(lock, [&]() {
                for (int i = 0; i < num_threads; ++i) {
                    if (!thread_stopped[i] && markers[i]->get_marked_count() == 0) {
                        return false;
                    }
                }
                return true;
            });
        }

        // 6.2 Вывести содержимое массива на консоль
        cout << "Current array state:" << endl;
        for (int val : array) {
            cout << val << " ";
        }
        cout << endl;

        // 6.3 Запросить порядковый номер потока для завершения
        int thread_to_terminate;
        cout << "Enter thread number to terminate (1-" << num_threads << "): ";
        cin >> thread_to_terminate;
        if (thread_to_terminate < 1 || thread_to_terminate > num_threads) {
            cout << "Invalid thread number!" << endl;
            continue;
        }

        // 6.4 Подать сигнал на завершение выбранному потоку
        {
            lock_guard<mutex> lock(array_mutex);
            terminate_signal = true;
        }
        cv.notify_all();

        // 6.5 Ждать завершение выбранного потока
        threads[thread_to_terminate - 1].join();

        // 6.6 Вывести содержимое массива на консоль
        cout << "Array after thread " << thread_to_terminate << " termination:" << endl;
        for (int val : array) {
            cout << val << " ";
        }
        cout << endl;

        // Проверить, остались ли активные потоки
        bool all_terminated = true;
        for (int i = 0; i < num_threads; ++i) {
            if (i != thread_to_terminate - 1 && threads[i].joinable()) {
                all_terminated = false;
                break;
            }
        }

        if (all_terminated) {
            break;
        }

        // 6.7 Подать сигнал на продолжение работы оставшимся потокам
        {
            lock_guard<mutex> lock(array_mutex);
            terminate_signal = false;
            continue_signal = true;
        }
        cv.notify_all();

        // Сбросить счетчики для оставшихся потоков
        for (int i = 0; i < num_threads; ++i) {
            if (i != thread_to_terminate - 1 && threads[i].joinable()) {
                markers[i]->reset_marked_count();
            }
        }
    }

    cout << "All marker threads have finished. Program exiting." << endl;
    return 0;
}

void marker_thread_func(MarkerThread& marker) {
    marker.run();
}
/*Белорусский государственный университет
Факультет прикладной математики и информатики
Кафедра технологии программирования
Доц. Побегайло А.П.
Лабораторная работа №3. ( 6 часов: 10.03.20 – 07.04.20 )
Тема: «Синхронизация потоков при помощи критических секций и событий.
Обработка тупиков».
Задача. Написать программу для консольного процесса, который состоит из потока main и
нескольких экземпляров потока marker.
Поток main должен выполнять следующие действия:
1. Захватить память под массив целых чисел, размерность которого вводится с консоли.
2. Инициализировать элементы массива нулями.
3. Запросить количество потоков marker, которые требуется запустить.
4. Запустить заданное количество экземпляров потока marker. В качестве параметра
каждому экземпляру потока marker передать его порядковый номер в запуске.
5. Дать сигнал на начало работы всех потоков marker.
6. Выполнять в цикле следующие действия:
6.1. Ждать, пока все потоки marker не подадут сигналы о невозможности
продолжения своей работы.
6.2. Вывести содержимое массива на консоль.
6.3. Запросить с консоли порядковый номер потока marker, которому будет подан
сигнал на завершение своей работы.
6.4. Подать потоку marker, номер которого получен в пункте 6.3, сигнал на
завершение работы.
6.5. Ждать завершение работы потока marker, которому был подан сигнал на
завершение работы в пункте 6.4.
6.6. Вывести содержимое массива на консоль.
6.7. Подать сигнал на продолжение работы, оставшимся потокам marker.
7. Завершить свою работу после завершения работы всех потоков marker.
Поток marker должен выполнять следующие действия:
1. Начать работу по сигналу от потока main.
2. Инициализировать генерацию последовательности случайных чисел. Для этого
использовать функцию srand, которой передать в качестве аргумента свой
порядковый номер.
3. Работать циклически, выполняя на каждом цикле следующие действия:
3.1. Генерировать случайное число, используя функцию rand.
3.2. Разделить это число по модулю на размерность массива.
3.3. Если элемент массива, индекс которого равен результату деления, равен нулю, то
выполнить следующие действия:
3.3.1. Поспать 5 миллисекунд.
3.3.2. Занести в элемент, индекс которого вычислен, свой порядковый номер.
3.3.3. Поспать 5 миллисекунд.
3.3.4. Продолжить исполнение цикла 3.
3.4. В противном случае:
3.4.1. Вывести на консоль следующую информацию:
- свой порядковый номер;
- количество помеченных элементов;
- индекс элемента массива, который невозможно пометить.
3.4.2. Дать сигнал потоку main на невозможность продолжения своей работы.
3.4.3. Ждать ответный сигнал на продолжение или завершение работы от потока
main.
4. Если получен сигнал на завершение работы, то выполнить следующие действия:
4.1. Заполнить нулями в массиве все элементы, которые он пометил.
4.2. Завершить работу.
5. Если получен сигнал на продолжение работы, то продолжить исполнение цикла из
пункта 3.*/