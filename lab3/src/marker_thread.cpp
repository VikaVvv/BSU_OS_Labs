#include "marker_thread.h"
#include <iostream>
#include <chrono>

MarkerThread::MarkerThread(int id, std::vector<int>& array, std::mutex& array_mutex, 
                           std::condition_variable& cv, std::atomic<bool>& start_signal,
                           std::atomic<bool>& terminate_signal, std::atomic<bool>& continue_signal)
    : id(id), array(array), array_mutex(array_mutex), cv(cv), 
      start_signal(start_signal), terminate_signal(terminate_signal), 
      continue_signal(continue_signal), marked_indices(array.size(), false) {
    rng.seed(id); // Инициализация генератора случайных чисел
}

void MarkerThread::run() {
    std::unique_lock<std::mutex> lock(array_mutex);
    
    // Ждем сигнала на начало работы
    cv.wait(lock, [this]() { return start_signal.load(); });

    while (true) {
        // Генерация случайного индекса
        int index = rng() % array.size();

        if (array[index] == 0) {
            // 3.3.1 Поспать 5 миллисекунд
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            
            // 3.3.2 Записать свой номер в элемент
            array[index] = id;
            marked_count++;
            marked_indices[index] = true;
            
            // 3.3.3 Поспать еще 5 миллисекунд
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        } else {
            // 3.4.1 Вывод информации
            std::cout << "Thread " << id << " cannot mark element at index " << index 
                      << ". Total marked: " << marked_count << std::endl;
            
            // 3.4.2 Сигнал о невозможности продолжения
            lock.unlock();
            cv.notify_all();
            
            // 3.4.3 Ждем ответный сигнал
            lock.lock();
            cv.wait(lock, [this]() { 
                return terminate_signal.load() || continue_signal.load(); 
            });
            
            if (terminate_signal.load()) {
                // 4.1 Заполнить нулями помеченные элементы
                for (size_t i = 0; i < array.size(); ++i) {
                    if (marked_indices[i]) {
                        array[i] = 0;
                    }
                }
                
                // 4.2 Завершить работу
                std::cout << "Thread " << id << " terminating." << std::endl;
                return;
            } else if (continue_signal.load()) {
                continue_signal = false;
                marked_count = 0;
                marked_indices.assign(array.size(), false);
                continue;
            }
        }
    }
}

void MarkerThread::stop() {
    // Метод для остановки потока (если нужно)
}

int MarkerThread::get_id() const {
    return id;
}

int MarkerThread::get_marked_count() const {
    return marked_count.load();
}

void MarkerThread::reset_marked_count() {
    marked_count = 0;
    marked_indices.assign(array.size(), false);
}