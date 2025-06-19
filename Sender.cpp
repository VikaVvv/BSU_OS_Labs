#include <iostream>
#include <fstream>
#include <string>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <limits>

const int MAX_MSG_LEN = 20;

struct SharedData {
    sem_t input_ready;
    sem_t output_ready;
    sem_t mutex;
    char message[MAX_MSG_LEN + 1];
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file_name>\n";
        return 1;
    }

    // Открываем разделяемую память
    int shm_fd = shm_open("/shared_mem", O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        return 1;
    }

    SharedData* shared = (SharedData*)mmap(NULL, sizeof(SharedData), 
                                     PROT_READ | PROT_WRITE, 
                                     MAP_SHARED, shm_fd, 0);
    if (shared == MAP_FAILED) {
        perror("mmap failed");
        close(shm_fd);
        return 1;
    }

    while (true) {
        std::cout << "\nEnter 1 to send message\nEnter 0 to exit\n";
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера

        if (choice == 1) {
            sem_wait(&shared->output_ready);
            sem_wait(&shared->mutex);

            std::cout << "Enter message (max " << MAX_MSG_LEN << " chars): ";
            std::string msg;
            std::getline(std::cin, msg); // Безопасное чтение строки
            
            // Безопасное копирование
            size_t copy_len = std::min(msg.length(), static_cast<size_t>(MAX_MSG_LEN));
            msg.copy(shared->message, copy_len);
            shared->message[copy_len] = '\0';

            sem_post(&shared->mutex);
            sem_post(&shared->input_ready);
        } else if (choice == 0) {
            break;
        } else {
            std::cout << "Invalid choice!\n";
        }
    }

    munmap(shared, sizeof(SharedData));
    close(shm_fd);
    return 0;
}