# 🗿 Philosophers - 42 Project

This project is a simulation of the classic **Dining Philosophers problem**, implemented in C using **threads (pthreads)** and **mutexes** for synchronization.

## 🛠️ Project Goal

- Simulate multiple philosophers sitting at a table.
- Each philosopher alternates between **thinking**, **eating**, and **sleeping**.
- Each needs **two forks** to eat, and the forks are **shared** with neighbors.
- Handle all edge cases like **deadlocks**, **race conditions**, and **timing**.

---

## ⚙️ Compilation

```bash
make
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]

```
## Example
```bash
./philo 5 800 200 200
```
## 🧪 Arguments

| Argument Position | Meaning                                           |
| ----------------- | ------------------------------------------------- |
| 1                 | Number of philosophers                            |
| 2                 | Time to die (ms)                                  |
| 3                 | Time to eat (ms)                                  |
| 4                 | Time to sleep (ms)                                |
| 5 *(optional)*    | Number of meals before simulation ends (optional) |

## 🧵 How It Works
- Each philosopher is a thread.

- Forks are mutexes.

- Philosophers try to pick up forks (mutex lock), eat, then release them (mutex unlock).

- A monitor thread checks:

  - If any philosopher died (time since last meal > time_to_die)

  - If all philosophers ate enough (optional)

## 🔐 Protection & Sync
- print_mtx → Prevents printing at same time

- stop_mtx → Protects the simulation status

- full_mtx → Tracks how many have eaten enough

- mtx_meal → Protects last_meal_time

## 🧠 Key Concepts
- Race Condition: multiple threads accessing/modifying shared memory at the same time

- Deadlock: each philosopher locks one fork and waits forever

- Mutex: ensures mutual exclusion (1 thread at a time)

- Thread Synchronization: coordinated execution without conflict

