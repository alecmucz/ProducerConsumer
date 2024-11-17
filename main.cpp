#include <iostream>
#include <pthread.h>
using namespace std;

#define N 10    //size of the shared buffer

int sharedBuffer[N]{};
pthread_mutex_t mtx;

// Producer: Enters data into the shared buffer
void* produce(void* arg) {
    pthread_mutex_lock(&mtx);
    for (int i = 0; i < N; i++) {
        sharedBuffer[i] = i;
    }
    pthread_mutex_unlock(&mtx);
    return nullptr;
}

// Consumer: Reads from the shared buffer
void* consume(void* arg) {
    int* arr = new int[N];
    pthread_mutex_lock(&mtx);
    cout << "[";
    for (int i = 0; i < N; i++) {
        arr[i] = sharedBuffer[i];
        cout << arr[i] << ",";
    }
    cout << "]" << endl;
    pthread_mutex_unlock(&mtx);
    delete[] arr;
    return nullptr;
}

int main() {
    pthread_t pid1, pid2;

    if (pthread_mutex_init(&mtx, nullptr)) {
        cout << "Failed to initialize mutex lock" << endl;
        return 1;
    }
    cout << "Initialized Mutex Lock" << endl;

    if (pthread_create(&pid1, nullptr, produce, nullptr) != 0) {
        cout << "Failed to create Process A: Producer" << endl;
        return 1;
    }
    cout << "Created Process A: Producer" << endl;

    if (pthread_create(&pid2, nullptr, consume, nullptr) != 0) {
        cout << "Failed to create Process B: Consumer" << endl;
        return 1;
    }
    cout << "Created Process B: Consumer" << endl;

    pthread_join(pid1, nullptr);
    pthread_join(pid2, nullptr);
    pthread_mutex_destroy(&mtx);

    return 0;
}
