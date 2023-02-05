# Multi-Threaded Minotaur Problems

###### Coded by Raciel Antela Pardo for COP4520

## Problem 1: Minotaur’s Birthday Party

The Minotaur invited N guests to his birthday party. When the guests arrived, he made
the following announcement.

The guests may enter his labyrinth, one at a time and only when he invites them to do
so. At the end of the labyrinth, the Minotaur placed a birthday cupcake on a plate. When
a guest finds a way out of the labyrinth, he or she may decide to eat the birthday
cupcake or leave it. If the cupcake is eaten by the previous guest, the next guest will find
the cupcake plate empty and may request another cupcake by asking the Minotaur’s
servants. When the servants bring a new cupcake the guest may decide to eat it or leave
it on the plate.

The Minotaur’s only request for each guest is to not talk to the other guests about her or
his visit to the labyrinth after the game has started. The guests are allowed to come up
with a strategy prior to the beginning of the game. There are many birthday cupcakes, so
the Minotaur may pick the same guests multiple times and ask them to enter the
labyrinth. Before the party is over, the Minotaur wants to know if all of his guests have
had the chance to enter his labyrinth. To do so, the guests must announce that they have
all visited the labyrinth at least once.

Now the guests must come up with a strategy to let the Minotaur know that every guest
entered the Minotaur’s labyrinth. It is known that there is already a birthday cupcake left
at the labyrinth’s exit at the start of the game. How would the guests do this and not
disappoint his generous and a bit temperamental host?

Create a program to simulate the winning strategy (protocol) where each guest is
represented by one running thread. In your program you can choose a concrete number
for N or ask the user to specify N at the start.

### Approach and Proof of Correctness

The approach taken to solve this problem is as follows:

1. All guests randomly enter the labyrinth one at a time.
2. Guest 0 keeps track of the number of guests that have entered the labyrinth by counting all the times the cupcake
   is eaten. Whenever Guest 0 enters the labyrinth, it checks the state of the cupcake and asks the servants to bring
   another one if it has been eaten by some other guest before.
3. The other guests enter the labyrinth, eat the cupcake if it is their first time entering the labyrinth, or leave the
   cupcake for the next guest if not.
4. The party ends once Guest 0 lets the minotaur know that all guests have entered the labyrinth.

The code uses a mutex and a condition variable to synchronize the threads representing the guests. The main thread
randomly selects a guest to enter the labyrinth and notifies all other threads through the condition variable. The
guests then wait on the condition variable until they are selected and enter the labyrinth. The selected guest then
performs the actions required and notifies the other guests to continue.

The code also uses a flag (<code>all_guests_entered_labyrinth</code>) to keep track of whether the party has ended or
not, and a counter (counter) to keep track of the number of guests that have entered the labyrinth. These variables are
protected by the mutex and updated in a thread-safe manner.

Overall, the code implements a solution that meets the requirements of the problem description and ensures the correct
behavior of the threads through the use of synchronization primitives and proper data management.

### Experimental Evaluation and Results

Initially, I was not sure how to keep track of a certain guest that entered the labyrinth. I considered
tracking it in an array, but then realized it was easier to just use a simple boolean variable scoped to each guest's
thread. This variable is set to true when the guest enters the labyrinth for the first time, and is used to determine
whether the guest should eat the cupcake or leave it for the next guest.

As for the approach, I tried to think of a more efficient way to keep track of the number of guests, but since
the guests cannot communicate after the party starts, I had to use a counter and let Guest 0 keep track of it. This
approach causes guests to enter the labyrinth more than once (which might be considered wasteful) but it ensures that
all guests have entered at least once.

### How to run

1. `cd` to this project folder
2. Run `g++ -std=c++20 -pthread minotaur_party.cpp && ./a.out` in the terminal
3. That's it!

## Problem 2: Minotaur’s Crystal Vase

The Minotaur decided to show his favorite crystal vase to his guests in a dedicated
showroom with a single door. He did not want many guests to gather around the vase
and accidentally break it. For this reason, he would allow only one guest at a time into
the showroom. He asked his guests to choose from one of three possible strategies for
viewing the Minotaur’s favorite crystal vase:

1) Any guest could stop by and check whether the showroom’s door is open at any time
   and try to enter the room. While this would allow the guests to roam around the castle
   and enjoy the party, this strategy may also cause large crowds of eager guests to gather
   around the door. A particular guest wanting to see the vase would also have no
   guarantee that she or he will be able to do so and when.
2) The Minotaur’s second strategy allowed the guests to place a sign on the door
   indicating when the showroom is available. The sign would read “AVAILABLE” or
   “BUSY.” Every guest is responsible to set the sign to “BUSY” when entering the
   showroom and back to “AVAILABLE” upon exit. That way guests would not bother trying
   to go to the showroom if it is not available.
3) The third strategy would allow the quests to line in a queue. Every guest exiting the
   room was responsible to notify the guest standing in front of the queue that the
   showroom is available. Guests were allowed to queue multiple times.

Which of these three strategies should the guests choose? Please discuss the advantages
and disadvantages.

Implement the strategy/protocol of your choice where each guest is represented by 1
running thread. You can choose a concrete number for the number of guests or ask the
user to specify it at the start.

### Approach and Proof of Correctness

Each guest was queued in order of arrival (0 to N-1) and a mutex and condition variable were used to synchronize the
threads. When a guest leaves the showroom after entering, it notifies the next guest in line through the condition
variable.

### Experimental Evaluation and Results

The third strategy would be the best choice for the guests, as it ensures fairness and order in the access to the
showroom. In this strategy, every guest waiting in line knows when it is their turn to enter the showroom, avoiding the
possibility of crowding around the door and potentially causing damage to the crystal vase.

However, the first strategy may allow for more freedom and flexibility for the guests, but it does not guarantee that
the guest will be able to view the vase and could lead to crowding and potential damage.

The second strategy eliminates the issue of crowding but relies on the guests to be responsible and trustworthy in
setting the sign to "BUSY" when entering the showroom and back to "AVAILABLE" upon exit, which may not always be the
case.

In summary, while each strategy has its own advantages and disadvantages, the third strategy of having a queue provides
the best solution in terms of fairness and order while also allowing guests to view the vase multiple times.

### How to run

1. `cd` to this project folder
2. Run `g++ -std=c++20 -pthread minotaur_vase.cpp && ./a.out` in the terminal
3. That's it!