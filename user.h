
#pragma once
#include "habit.h"
#include <vector>
#include <memory>
#include <algorithm>

// ---------------------------------------------
//  Class: User
//  Encapsulates the user's profile and habits
// ---------------------------------------------
class User {
private:
    string              username;
    int                 totalPoints;
    vector<shared_ptr<Habit>> habits;
    int                 nextId;

public:
    explicit User(const string& username)
        : username(username), totalPoints(0), nextId(1) {}

    // -- Habit Management ---------------------
    void addHabit(shared_ptr<Habit> h) {
        habits.push_back(move(h));
    }

    bool removeHabit(int id) {
        auto it = find_if(habits.begin(), habits.end(),
                          [id](const shared_ptr<Habit>& h){ return h->getId() == id; });
        if (it == habits.end()) return false;
        habits.erase(it);
        return true;
    }

    shared_ptr<Habit> findHabit(int id) {
        for (auto& h : habits)
            if (h->getId() == id) return h;
        return nullptr;
    }

    // Mark habit complete and award points
    bool completeHabit(int id) {
        auto h = findHabit(id);
        if (!h) {
            cout << "  [!] Habit not found.\n";
            return false;
        }
        bool ok = h->markComplete();
        if (ok) {
            int pts = 10 + h->getStreak() * 2;   // bonus for streak
            totalPoints += pts;
            cout << "  OK Habit '" << h->getName() << "' marked complete!\n";
            cout << "  Current Streak: " << h->getStreak() << " | "
                 << h->getReward() << "\n";
            cout << "  + Points earned this session: +" << pts
                 << "  (Total: " << totalPoints << ")\n";
        }
        return ok;
    }

    // Reset daily completions (simulate next day)
    void simulateNewDay() {
        for (auto& h : habits) h->resetCompletion();
        cout << "  [OK] New day simulated -- all daily habits reset.\n";
    }

    // -- Display ------------------------------
    void displayProfile() const {
        cout << "\n  +------------------------------+\n";
        cout << "  |  User : " << left << setw(20) << username << "|\n";
        cout << "  |  Points: " << left << setw(19) << totalPoints << "|\n";
        cout << "  |  Habits: " << left << setw(19) << habits.size() << "|\n";
        cout << "  +------------------------------+\n";
    }

    void displayAllHabits() const {
        if (habits.empty()) {
            cout << "  No habits yet. Add one!\n";
            return;
        }
        cout << "\n  -- Your Habits ------------------------------\n";
        for (const auto& h : habits) h->display();
        cout << "  ---------------------------------------------\n";
    }

    void displayStats() const {
        if (habits.empty()) {
            cout << "  No habits to show stats for.\n";
            return;
        }
        cout << "\n  +-----------------------------------------------+\n";
        cout << "  |             HABIT STATISTICS                  |\n";
        cout << "  +-----------------------------------------------+\n";
        cout << "  " << left << setw(20) << "Habit"
             << setw(12) << "Type"
             << setw(10) << "Streak"
             << setw(14) << "Best Streak"
             << setw(8)  << "Total"
             << "Reward\n";
        cout << "  " << string(74, '-') << "\n";
        for (const auto& h : habits) {
            cout << "  " << left << setw(20) << h->getName()
                 << setw(12) << h->getType()
                 << setw(10) << h->getStreak()
                 << setw(14) << h->getLongestStreak()
                 << setw(8)  << h->getTotalCompletions()
                 << h->getReward() << "\n";
        }
        cout << "\n  Total Points: " << totalPoints << "\n";
    }

    // -- Getters ------------------------------
    string           getUsername()   const { return username; }
    int              getNextId()           { return nextId++; }
    int              getPoints()     const { return totalPoints; }
    int              getHabitCount() const { return (int)habits.size(); }
};
