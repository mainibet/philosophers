# Philosophers

A C language implementation of the classic Dining Philosophers problem, to explore and demonstrate concurrent programming concepts using POSIX threads (pthreads) and mutexes.
This project provides a robust simulation of philosophers attempting to eat, sleep, and think without deadlocking or starving.
---

## ✨ Problem statment
The Dining Philosophers problem is a classic synchronization problem in computer science. Imagine five philosophers seated around a circular table. 
Between each pair of philosophers is a single fork. To eat, a philosopher needs two forks: one from their left and one from their right. 
The challenge is to design a protocol that allows philosophers to eat without:

**Deadlock**: A state where no philosopher can proceed because each is waiting for a resource held by another.
**Starvation**: A state where a philosopher repeatedly loses the race for forks and never gets to eat.

This project implements a solution to this problem, ensuring all philosophers can eventually eat and the simulation terminates correctly.
---

## 🚀 Features
- **Multithreaded Simulation**: Each philosopher runs as an independent thread.
- **Mutex-based Synchronization**: Utilizes mutexes to protect shared resources (forks) and control access to critical sections (e.g., output).
- **Configurable Parameters**: The simulation can be configured via command-line arguments:
      - Number of philosophers.
      - Time a philosopher takes to die if not eating.
      - Time a philosopher takes to eat.
      - Time a philosopher takes to sleep.
      - (Optional) The number of times each philosopher must eat before the simulation ends.
- **Life Cycle Simulation**: Philosophers cycle through states: thinking, taking forks, eating, and sleeping.
- **Dedicated Monitor Thread**: A separate thread continuously monitors the philosophers' states to detect:
- If any philosopher starves (dies).
- If all philosophers have eaten the required number of meals (if specified).
- **Precise Time Tracking**: Logs events with timestamps relative to the simulation start.
- **Robust Error Handling**: Includes comprehensive checks for invalid arguments, memory allocation failures, and mutex initialization errors, ensuring graceful program termination.
- **Clean Resource Management**: Meticulous handling of thread joining and mutex destruction to prevent leaks and ensure proper cleanup.

---
## 🍽️ How It Works

**1. Philosophers as Threads**: Each philosopher is represented by a pthread that executes its own life_cycle routine.
**2. Forks as Mutexes**: Each fork is a pthread_mutex_t. To pick up a fork, a philosopher must lock its corresponding mutex.
**3. Ordered Fork Acquisition**: To prevent deadlock, philosophers pick up forks in a specific order (e.g., always taking the lower-indexed fork first, then the higher-indexed fork). 
This breaks the circular dependency that causes deadlocks.
**4. State Management**: Global mutexes (start_mutex, end_mutex, output_mutex) are used to synchronize the start of the simulation, control its termination, and ensure clean, non-interleaved output to the console.
**5. Monitoring**: A separate monitor_thread periodically checks each philosopher's last_meal timestamp against time_to_die to determine if they have starved. 
If a max_meals limit is set, it also checks if all philosophers have met their eating quota.

---

## ⚙️ Installation & Setup 

To compile and run the philo project:

1. Clone the repository:
```
git clone https://github.com/mainibet/philo.git
cd philo
```

2. Compile the project
```
  make
```
This will create the philo executable in the current directory.

---

## 💡 Usage

The program requires 4 or 5 command-line arguments:
```
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```
- `number_of_philosophers`: The number of philosophers (and forks).
- `time_to_die`: Time in milliseconds a philosopher has to live without eating.
- `time_to_eat`: Time in milliseconds a philosopher takes to eat.
- `time_to_sleep`: Time in milliseconds a philosopher takes to sleep.
- `[number_of_times_each_philosopher_must_eat]`: (Optional) If set, the simulation stops when all philosophers have eaten at least this many times. Otherwise, it runs until a philosopher dies.

**Examples**:

1. Basic simulation (runs until death):
```
./philo 5 800 200 200
```
(5 philosophers, die after 800ms, eat for 200ms, sleep for 200ms)

2. Simulation with meal limit:
```
./philo 4 410 200 200 7
```
(4 philosophers, die after 410ms, eat for 200ms, sleep for 200ms, stop after 7 meals each)

3. Single philosopher scenario:
```
./philo 1 800 200 200
```
(A single philosopher will pick up a fork and wait indefinitely for a second, then die, as expected in the problem.)

---

## 🧠 Development & Challenges 

This project was a significant challenge in understanding and implementing concurrent programming. 
Key aspects and learnings include:

- **Synchronization Complexity**: The primary challenge was preventing race conditions, deadlocks, and starvation. This required careful design and consistent application of mutexes for forks and shared program state.

- **Time Management**: Accurately tracking time in milliseconds and ensuring philosophers' actions (eating, sleeping) occur for the specified durations, while also checking for death conditions, was critical.

- **Thread Management**: Correctly creating, joining, and detaching threads, and ensuring all resources (mutexes, allocated memory) are properly cleaned up upon termination, regardless of how the simulation ends.

- **Argument Parsing & Error Handling**: Implementing robust validation for command-line arguments and providing clear error messages for invalid inputs or runtime failures (e.g., malloc failures, mutex initialization issues).

- **The "One Philosopher" Edge Case**: Special handling was required for the scenario with only one philosopher, as they can only ever acquire one fork and will inevitably starve.

Debugging Concurrency: Debugging multithreaded applications is inherently complex due to non-deterministic execution. 
This project heavily relied on careful logging and systematic reasoning to identify and resolve synchronization bugs.
---

## 📂File struct

```
.
├── philo/                   # Main project directory
│   ├── actions.c            # Implements philosopher actions (eating, sleeping, thinking, taking/releasing forks)
│   ├── args.c               # Handles command-line argument parsing
│   ├── args_utils.c         # Utility functions for argument parsing
│   ├── cleanup_error.c      # Functions for error handling and global program cleanup
│   ├── clean_up_utils.c     # Utility functions for cleaning up specific data structures (forks, philos)
│   ├── helper.c             # General helper functions (e.g., time, mutex initialization, fork ordering)
│   ├── init.c               # Initializes program data, philosophers, and mutexes
│   ├── life_cycle.c         # Contains the main routine for philosopher threads
│   ├── main.c               # Program entry point, sets up and runs the simulation
│   ├── macros.h             # Defines constants and status codes
│   ├── Makefile             # Builds the project
│   ├── monitor.c            # Implements the monitoring thread logic (checking for death/meal limits)
│   ├── output.c             # Handles synchronized output to the console
│   ├── philo.h              # Main header file with structs and function prototypes
│   └── simulation.c         # Manages the overall simulation flow (setup, start, end conditions)
└── obj/                     # Compiled object files (automatically generated)
```
## 👤 Autor
Alicia Betancourt ([mainibet](https://github.com/mainibet))
