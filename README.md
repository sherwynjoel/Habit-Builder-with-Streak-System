# 🎯 Habit Builder with Streak System

The **Habit Builder with Streak System** is a robust, cross-platform productivity application developed entirely in **C++** using object-oriented principles. Its main purpose is to help users maintain daily and weekly goals by leveraging a built-in gamification loop that grants dynamic point multipliers and exclusive badges. The system safely manages data using standard memory libraries.


---

## 🚀 Features

### 📋 Habit Management
- **Daily Habits**: Track tasks that need to be completed every single day.
- **Weekly Habits**: Set customized targets (e.g., 3x a week) for habits that don't require daily execution.
- **Dynamic Tracking**: Easily add, view, and remove habits from your active list.

### 🔥 Gamification & Streak System
- **Streak Tracking**: Automatically counts consecutive days/weeks of completion.
- **Point Rewards**: Earn **10 base points** per completion, plus a **+2 point bonus** for every day added to your current streak!
- **Achievement Badges**:
  - 🥉 `[Getting Started]` (3+ Day Streak)
  - 🥈 `[Consistent]` (7+ Day Streak)
  - 🥇 `[Pro]` (14+ Day Streak)
  - 🏆 `[Champion]` (21+ Day Streak)
  - 👑 `[LEGEND]` (30+ Day Streak)

### 📊 Detailed Statistics
- View your overall progress including current streak, longest streak, total completions, and current rewards for each habit.

---

## 📂 Project Files & Their Purpose

Here is an in-depth look at why each of the 5 key files exists and what role it plays in the application:

### 1. 📄 [habit.h](file:///c:/Users/Sherwyn%20joel/OneDrive/Desktop/Habit%20Builder%20with%20Streak%20System/habit.h)
- **Purpose**: Serves as the core data blueprint for habits.
- **Details**: It implements the abstract base class `Habit` as well as the specialized `DailyHabit` and `WeeklyHabit` subclasses. It isolates how completions, streak records, and badge rewards are stored and verified.

### 2. 📄 [user.h](file:///c:/Users/Sherwyn%20joel/OneDrive/Desktop/Habit%20Builder%20with%20Streak%20System/user.h)
- **Purpose**: Controls memory allocation and the user portfolio.
- **Details**: Encapsulates overall points metrics. It stores collections of dynamic habit instances safely using C++ `std::shared_ptr` wrappers, allowing for runtime polymorphism without resource leaks.

### 3. 📄 [tracker.h](file:///c:/Users/Sherwyn%20joel/OneDrive/Desktop/Habit%20Builder%20with%20Streak%20System/tracker.h)
- **Purpose**: Coordinates the CLI interface.
- **Details**: Functions as the presentation tier. It parses selection entries safely, mitigates input stream corruption vulnerabilities, and bridges interactions with backend storage classes.

### 4. 📄 [main.cpp](file:///c:/Users/Sherwyn%20joel/OneDrive/Desktop/Habit%20Builder%20with%20Streak%20System/main.cpp)
- **Purpose**: Entry runtime logic.
- **Details**: Bootstraps environment prerequisites and routes workflow streams.

### 5. 📄 [README.md](file:///c:/Users/Sherwyn%20joel/OneDrive/Desktop/Habit%20Builder%20with%20Streak%20System/README.md)
- **Purpose**: Setup documentation and developer onboarding overview.


---

## 🛠️ How to Set Up & Run (New Machine Guide)

To run the project from scratch on any computer, complete these steps:

### 1. Download & Clone
Open your terminal application and execute:
```bash
git clone https://github.com/sherwynjoel/Habit-Builder-with-Streak-System.git
```

### 2. Enter Workspace
```bash
cd Habit-Builder-with-Streak-System
```

### 3. Prerequisites
- Ensure a reliable C++ compiler suite supporting standard **C++11** constraints operates appropriately.

### 4. Compilation

Open your terminal/command prompt in the project directory and run:

```bash
g++ main.cpp -o habit_builder
```

### Execution
Run the compiled executable:

#### Windows
```bash
habit_builder.exe
```

#### Linux / macOS
```bash
./habit_builder
```

---

## 🎮 How to Use

1. **Launch the App**: Enter your name to initialize your profile.
2. **Add Habits**: Use option `2` to create your first habit. Choose between Daily or Weekly.
3. **Check-in**: Use option `3` to mark a habit as complete for the day.
4. **Track Progress**: Use options `1`, `4`, and `5` to keep an eye on your stats and unlocked badges.
5. **Simulate Time (Dev Tool)**: Use option `7` to simulate moving to the next day and reset your daily flags for testing.

---
*Keep building great habits!*

---

## 🎓 A-to-Z Academic Breakdown (For College Presentation)

This section provides an architectural breakdown of the project to help you understand the core Computer Science concepts implemented.

### 📋 1. Detailed Feature Breakdown

#### A. Habit Management (Daily vs. Weekly)
The project distinguishes between two types of habits:
1. **Daily Habits**: Tasks you must do every day (e.g., "Drink 3L Water").
2. **Weekly Habits**: Tasks with a frequency target (e.g., "Go to the Gym 3x/week").

**OOP Concept Used: Polymorphism & Inheritance**
- A base class [Habit](file:///c:/Users/Sherwyn%20joel/OneDrive/Desktop/Habit%20Builder%20with%20Streak%20System/habit.h#L13) defines the core structure.
- [DailyHabit](file:///c:/Users/Sherwyn%20joel/OneDrive/Desktop/Habit%20Builder%20with%20Streak%20System/habit.h#L114) and [WeeklyHabit](file:///c:/Users/Sherwyn%20joel/OneDrive/Desktop/Habit%20Builder%20with%20Streak%20System/habit.h#L130) inherit from it and define their own specific completion rules by overriding virtual functions.

#### B. The Streak System
A streak measures consecutive completions. 
- **Daily Streak Logic**: Compares the `lastCompletedDate` with `yesterdayStr()`. If they match, the streak continues; otherwise, it resets.
- **Weekly Streak Logic**: Tracks completions within the ISO week starting on Monday.

#### C. Gamification (Points & Badges)
- **Points Formula**: `10 base points + (Current Streak * 2)`.
- **Badges**: Awarded dynamically based on streak milestones (3, 7, 14, 21, and 30 days).

---

### 📂 2. Code Architecture (File by File)

#### 🗺️ Architecture Overview (ASCII Diagram)
```text
       +-------------------+
       |     main.cpp      |
       |   (Entry Point)   |
       +---------+---------+
                 |  Creates & Boots
                 v
       +-------------------+
       |     tracker.h     |
       | (Menu Controller) |
       +---------+---------+
                 |  Interacts With
                 v
       +-------------------+
       |      user.h       |
       |  (State Manager)  |
       +---------+---------+
                 |  Manages Collections of
                 v
       +-------------------+
       |      habit.h      |
       |  (Base Model)     |
       +----+--------------+
            |
            +-----------------------+
            |                       |
            v                       v
   +-------------------+   +-------------------+
   |    DailyHabit     |   |    WeeklyHabit    |
   | (Daily Schedule)  |   | (Target Tracking) |
   +-------------------+   +-------------------+
```


#### 📄 [habit.h](file:///c:/Users/Sherwyn%20joel/OneDrive/Desktop/Habit%20Builder%20with%20Streak%20System/habit.h) (The Core Data Model)
Defines the state of a habit.
- **Encapsulation**: Attributes like `streak` and `lastCompletedDate` are `protected` to prevent unauthorized external modification while allowing derived classes to access them.
- **Pure Virtual Functions**: `getType()` and `isCompletable()` force child classes to implement their own behavior.

#### 📄 [user.h](file:///c:/Users/Sherwyn%20joel/OneDrive/Desktop/Habit%20Builder%20with%20Streak%20System/user.h) (The Controller/State)
Manages the user's data and collection of habits.
- **Smart Pointers**: Uses `std::shared_ptr<Habit>` for the habit list. This ensures automatic memory cleanup and prevents memory leaks.
- **Vector Storage**: Uses `std::vector` for dynamic list resizing.

#### 📄 [tracker.h](file:///c:/Users/Sherwyn%20joel/OneDrive/Desktop/Habit%20Builder%20with%20Streak%20System/tracker.h) (The User Interface)
Handles terminal I/O (Menus, prompts, inputs).
- **Input Validation**: Employs robustness helpers like `getInt()` to clear the `cin` stream, avoiding infinite loops when a user enters text where a number is expected.

#### 📄 [main.cpp](file:///c:/Users/Sherwyn%20joel/OneDrive/Desktop/Habit%20Builder%20with%20Streak%20System/main.cpp) (The Entry Point)
Acts as the single file deployment hub containing the initialization of the application loop.

---

### ⚙️ 3. Step-by-Step Execution Workflow

To understand how the application handles operations end-to-end, follow this sequence:

#### Step 1: Initialization (How it Starts)
1. Execution kicks off in the `main()` function inside [main.cpp](file:///c:/Users/Sherwyn%20joel/OneDrive/Desktop/Habit%20Builder%20with%20Streak%20System/main.cpp).
2. It constructs the [User](file:///c:/Users/Sherwyn%20joel/OneDrive/Desktop/Habit%20Builder%20with%20Streak%20System/user.h#L12) record to establish point parameters.

#### Step 2: Input Gathering (How Input is Retrieved)
1. Custom validations filter incorrect characters gracefully.

#### Step 3: Business Logic (What Calculations Occur)
1. Milestones apply continuous calculation updates.

#### Step 4: Formatting (How Data Returns)
1. Detailed tables output modular summaries.


---


### 💡 3. Key Computer Science Concepts to Highlight
1. **Memory Management**: Automatic scope management via smart pointers.
2. **Abstract Classes**: `Habit` cannot be instantiated directly—it serves only as a blueprint.
3. **Time Manipulation**: Using standard library `<ctime>` functions to extract real-world calendar metrics.

---

## 🚀 Future Roadmap & Enhancements

To expand the features of this application for future deployment, consider the following enhancements:

1. **Persistent Database Storage**: Currently, habit configurations reside entirely in RAM during active iterations. Implementing file saves via JSON/CSV exports secures operational backups.
2. **Cross-Platform GUI Shells**: Upgrading visual assets smoothly.
