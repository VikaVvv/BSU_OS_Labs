#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../src/marker_thread.h"

TEST_CASE("Basic MarkerThread test") {
    SUBCASE("Initialization") {
        std::vector<int> array(10, 0);
        std::mutex mtx;
        std::condition_variable cv;
        std::atomic<bool> start(false), terminate(false), cont(false);
        
        MarkerThread marker(1, array, mtx, cv, start, terminate, cont);
        
        CHECK(marker.get_id() == 1);
        CHECK(marker.get_marked_count() == 0);
    }
    
    SUBCASE("Thread safety") {
        // Добавьте тесты для проверки потокобезопасности
    }
}