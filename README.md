# 🎯 Habit Builder with Streak System

A robust C++ console application designed to help you build, track, and maintain positive daily and weekly habits. Stay motivated with a built-in gamification system that rewards your consistency with points and exclusive badges!

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

## 📂 Project Structure

- 📄 `main.cpp`: The entry point of the application.
- 📄 `habit.h`: Contains the core `Habit` base class and derived `DailyHabit` & `WeeklyHabit` classes.
- 📄 `user.h`: Manages the user profile, points, and collection of habits.
- 📄 `tracker.h`: The main menu engine and user interaction controller.

---

## 🛠️ How to Build & Run

### Prerequisites
- A C++ compiler supporting **C++11** or higher (e.g., GCC, Clang, or MSVC).

### Compilation
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
