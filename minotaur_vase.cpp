#include <iostream>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>

using namespace std;

const int N = 5; // Number of guests

mutex m;
condition_variable cv; // Condition variable to synchronize guests
queue<int> guests; // Queue to keep track of guests waiting to see the vase
bool showroom_busy = false; // Flag to indicate if the showroom is busy
int guests_counter = 0; // Counter to keep track of number of guests that have seen the vase
thread t[N]; // Threads for guests

void guest(int id) {
    unique_lock<mutex> lock(m);
    guests.push(id);
    cv.wait(lock, [] { return !showroom_busy; });
    guests.pop();
    guests_counter++;
    showroom_busy = true;
    lock.unlock();

    // Guest enters the showroom and views the vase
    cout << "Guest " << id << " enters the showroom." << endl;
    this_thread::sleep_for(chrono::seconds(1));

    // Guest leaves the showroom
    lock.lock();
    cout << "Guest " << id << " leaves the showroom and signals the next guest to enter." << endl;
    showroom_busy = false;
    lock.unlock();
    cv.notify_one();

    if (guests_counter == N) {
        cout << "All guests have seen the vase." << endl;
    }
}

int main() {
    cout << "The minotaur opens the showroom door" << endl;
    cout << "There are " << N << " guests waiting to see the vase." << endl;

    for (int i = 0; i < N; i++) {
        t[i] = thread(guest, i);
    }

    for (int i = 0; i < N; i++) {
        t[i].join();
    }

    return 0;
}
