#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>

using namespace std;

const int N = 5; // Number of guests

mutex mtx;
condition_variable cv; // Condition variable to synchronize guests
bool cake_left = true; // Flag to indicate if cake is left at the end of the labyrinth
int counter = 0; // Counter to keep track of number of guests that have gone through the labyrinth
bool all_guests_entered_labyrinth = false; // Flag to indicate if all guests have gone through the labyrinth
int selected_guest = -1; // Guest that is currently in the labyrinth

void guest(int id) {
    bool been_to_labyrinth = false;
    unique_lock<mutex> lck(mtx);
    while (!all_guests_entered_labyrinth) {
        if (id != selected_guest) {
            cv.wait(lck);
            continue;
        }

        cout << "Guest " << id << " enters the labyrinth." << endl;

        if (id == 0) {
            if (cake_left) {
                cout << "Guest 0 found the cupcake and left the labyrinth." << endl;
            } else {
                counter++;
                if (counter == N - 1) {
                    cout << "Guest 0 announces that all " << N << " guests have entered the labyrinth." << endl;
                    all_guests_entered_labyrinth = true;
                } else {
                    cout << "Guest 0 found the cupcake was eaten and asked the servants to bring another one." << endl;
                    cake_left = true;
                }
            }
        } else {
            if (cake_left && !been_to_labyrinth) {
                cout << "Guest " << id << " ate the cupcake and left the labyrinth." << endl;
                cake_left = false;
                been_to_labyrinth = true;
            } else {
                cout << "Guest " << id << " left the labyrinth without eating the cupcake." << endl;
            }
        }
        cv.wait_for(lck, chrono::seconds(1)); // Wait for 1 second before the next guest enters the labyrinth
    }
}

int main() {
    thread guests[N];

    // Invite guests to the party
    for (int i = 0; i < N; i++) {
        guests[i] = thread(guest, i);
    }

    cout << "The minotaur starts the party with " << N << " guests." << endl;

    // Randomly pick guests to enter the labyrinth until the party is over
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, N - 1);
    while (!all_guests_entered_labyrinth) {
        selected_guest = dis(gen);
        cv.notify_all(); // Notify all guests that a guest has entered the labyrinth
        this_thread::sleep_for(chrono::seconds(1));
    }

    cv.notify_all(); // Notify all guests that the party is over

    // Wait for all guests to leave the party
    for (int i = 0; i < N; i++) {
        guests[i].join();
    }

    cout << "The minotaur ends the party." << endl;

    return 0;
}
