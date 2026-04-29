
#pragma once
#include "user.h"
#include <limits>

// ---------------------------------------------
//  Class: Tracker
//  Main application controller / menu engine
// ---------------------------------------------
class Tracker {
private:
    User* user;

    // -- Input helpers ------------------------
    static int getInt(const string& prompt, int lo = INT_MIN, int hi = INT_MAX) {
        int v;
        while (true) {
            cout << prompt;
            if (cin >> v && v >= lo && v <= hi) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return v;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [!] Invalid input. Try again.\n";
        }
    }

    static string getString(const string& prompt) {
        string s;
        cout << prompt;
        getline(cin, s);
        // Trim leading/trailing spaces
        size_t start = s.find_first_not_of(" \t");
        size_t end   = s.find_last_not_of(" \t");
        return (start == string::npos) ? "" : s.substr(start, end - start + 1);
    }

    // -- Sub-menus ----------------------------
    void menuAddHabit() {
        cout << "\n  -- Add Habit ------------------------\n";
        cout << "  Habit type:\n";
        cout << "    1. Daily  (must do every day)\n";
        cout << "    2. Weekly (set a weekly target)\n";
        int type = getInt("  Choose (1/2): ", 1, 2);

        string name     = getString("  Habit name : ");
        string category = getString("  Category   : ");
        if (name.empty()) { cout << "  [!] Name cannot be empty.\n"; return; }

        int id = user->getNextId();
        if (type == 1) {
            user->addHabit(make_shared<DailyHabit>(id, name, category));
        } else {
            int target = getInt("  How many times per week? (1-7): ", 1, 7);
            user->addHabit(make_shared<WeeklyHabit>(id, name, category, target));
        }
        cout << "  OK Habit '" << name << "' added (ID=" << id << ").\n";
    }

    void menuMarkComplete() {
        user->displayAllHabits();
        if (user->getHabitCount() == 0) return;
        int id = getInt("  Enter Habit ID to mark complete: ", 1, INT_MAX);
        user->completeHabit(id);
    }

    void menuRemoveHabit() {
        user->displayAllHabits();
        if (user->getHabitCount() == 0) return;
        int id = getInt("  Enter Habit ID to remove: ", 1, INT_MAX);
        if (user->removeHabit(id))
            cout << "  OK Habit removed.\n";
        else
            cout << "  [!] ID not found.\n";
    }

    void menuSimulateDay() {
        cout << "\n  Simulating a new day (resets daily completion flags)...\n";
        user->simulateNewDay();
    }

    // -- Leaderboard / rewards summary --------
    void menuRewards() const {
        cout << "\n  +--------------------------------------+\n";
        cout << "  |           REWARDS & BADGES           |\n";
        cout << "  +--------------------------------------+\n";
        cout << "  Streak -> Badge\n";
        cout << "  -------------------------------------\n";
        cout << "    3+  days  ->  [Getting Started]\n";
        cout << "    7+  days  ->  [Consistent]\n";
        cout << "   14+  days  ->  [Pro]\n";
        cout << "   21+  days  ->  [Champion]\n";
        cout << "   30+  days  ->  [LEGEND]\n";
        cout << "  -------------------------------------\n";
        cout << "  Points: 10 base + 2 per streak day per completion\n";
        cout << "  Your Points : " << user->getPoints() << "\n";
    }

    // -- Main banner --------------------------
    static void printBanner() {
        cout << "\n";
        cout << "\n  +------------------------------------------+\n";
        cout << "  |        HABIT BUILDER + STREAK SYSTEM     |\n";
        cout << "  +------------------------------------------+\n";
    }

    static void printMenu() {
        cout << "\n  +--------------------------------------+\n";
        cout << "  |  1. View Habits                      |\n";
        cout << "  |  2. Add Habit                        |\n";
        cout << "  |  3. Mark Habit as Complete           |\n";
        cout << "  |  4. View Statistics                  |\n";
        cout << "  |  5. View Rewards & Badges            |\n";
        cout << "  |  6. Remove Habit                     |\n";
        cout << "  |  7. Simulate Next Day  (Dev Tool)    |\n";
        cout << "  |  0. Exit                             |\n";
        cout << "  +--------------------------------------+\n";
    }

public:
    explicit Tracker(User* u) : user(u) {}

    void run() {
        printBanner();
        user->displayProfile();

        while (true) {
            printMenu();
            int choice = getInt("  >> ", 0, 7);

            switch (choice) {
                case 1: user->displayAllHabits();  break;
                case 2: menuAddHabit();             break;
                case 3: menuMarkComplete();         break;
                case 4: user->displayStats();       break;
                case 5: menuRewards();              break;
                case 6: menuRemoveHabit();          break;
                case 7: menuSimulateDay();          break;
                case 0:
                    cout << "\n  Goodbye! Keep building great habits!\n\n";
                    return;
            }
        }
    }
};
