#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <random>

class MarkerThread {
public:
    MarkerThread(int id, std::vector<int>& array, std::mutex& array_mutex, 
                 std::condition_variable& cv, std::atomic<bool>& start_signal,
                 std::atomic<bool>& terminate_signal, std::atomic<bool>& continue_signal);
    
    void run();
    void stop();
    int get_id() const;
    int get_marked_count() const;
    void reset_marked_count();
    void test_run() { run(); }
private:
    int id;
    std::vector<int>& array;
    std::mutex& array_mutex;
    std::condition_variable& cv;
    std::atomic<bool>& start_signal;
    std::atomic<bool>& terminate_signal;
    std::atomic<bool>& continue_signal;
    std::atomic<int> marked_count{0};
    std::vector<bool> marked_indices;
    std::mt19937 rng;
};

void marker_thread_func(MarkerThread& marker);
