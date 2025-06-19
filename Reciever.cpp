#include <iostream>
#include <fstream>
#include <string>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

const int MAX_MSG_LEN = 20;

struct SharedData {
    sem_t input_ready;
    sem_t output_ready;
    sem_t mutex;
    char message[MAX_MSG_LEN + 1];
};

void createSenderProcesses(const std::string& file_name, int num_senders) {
    for (int i = 0; i < num_senders; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            execl("./Sender", "Sender", file_name.c_str(), nullptr);
            perror("execl failed");
            exit(1);
        } else if (pid < 0) {
            perror("fork failed");
            exit(1);
        }
    }
}

int main() {
    std::string file_name;
    int num_notes, num_senders;

    std::cout << "Enter binary file name: ";
    std::cin >> file_name;
    std::cout << "Enter number of notes: ";
    std::cin >> num_notes;
    std::cout << "Enter number of Sender Processes: ";
    std::cin >> num_senders;

    // Создаем файл
    std::fstream file(file_name, std::ios::out);
    file.close();

    // Инициализация разделяемой памяти
    int shm_fd = shm_open("/shared_mem", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(SharedData));
    SharedData* shared = (SharedData*)mmap(NULL, sizeof(SharedData), 
                                         PROT_READ | PROT_WRITE, 
                                         MAP_SHARED, shm_fd, 0);

    // Инициализация семафоров
    sem_init(&shared->input_ready, 1, 0);
    sem_init(&shared->output_ready, 1, num_notes);
    sem_init(&shared->mutex, 1, 1);

    createSenderProcesses(file_name, num_senders);

    // Основной цикл обработки сообщений
    while (true) {
        std::cout << "\nEnter 1 to read message\nEnter 0 to exit\n";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            sem_wait(&shared->input_ready);
            sem_wait(&shared->mutex);

            std::cout << "Received message: " << shared->message << std::endl;

            sem_post(&shared->mutex);
            sem_post(&shared->output_ready);
        } else if (choice == 0) {
            break;
        }
    }

    // Очистка ресурсов
    sem_destroy(&shared->input_ready);
    sem_destroy(&shared->output_ready);
    sem_destroy(&shared->mutex);
    munmap(shared, sizeof(SharedData));
    shm_unlink("/shared_mem");

    return 0;
}