#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <memory>
#include <algorithm>
#include <limits>
#include <climits>
#include <cstdio>

using namespace std;

// ---------------------------------------------
//  Base Class: Habit
// ---------------------------------------------
class Habit {
protected:
    int    id;
    string name;
    string category;
    int    streak;
    int    longestStreak;
    int    totalCompletions;
    string lastCompletedDate;   // "YYYY-MM-DD"
    bool   completedToday;

    // Helper: today's date as "YYYY-MM-DD"
    static string todayStr() {
        time_t t = time(nullptr);
        tm* lt = localtime(&t);
        char buf[11];
        strftime(buf, sizeof(buf), "%Y-%m-%d", lt);
        return string(buf);
    }

    // Helper: yesterday's date as "YYYY-MM-DD"
    static string yesterdayStr() {
        time_t t = time(nullptr) - 86400;
        tm* lt = localtime(&t);
        char buf[11];
        strftime(buf, sizeof(buf), "%Y-%m-%d", lt);
        return string(buf);
    }

public:
    Habit(int id, const string& name, const string& category)
        : id(id), name(name), category(category),
          streak(0), longestStreak(0), totalCompletions(0),
          lastCompletedDate(""), completedToday(false) {}

    virtual ~Habit() = default;

    // Pure virtual: each habit type may have different frequency rules
    virtual string getType() const = 0;
    virtual bool   isCompletable() const = 0;  // can the user mark it today?

    // Mark the habit as done
    virtual bool markComplete() {
        if (!isCompletable()) {
            cout << "  [!] '" << name << "' is already completed for this period.\n";
            return false;
        }
        string today = todayStr();

        // Streak logic
        if (lastCompletedDate == yesterdayStr()) {
            streak++;                          // continued streak
        } else if (lastCompletedDate == today) {
            // already done (shouldn't reach here due to isCompletable)
        } else {
            streak = 1;                        // reset streak
        }

        if (streak > longestStreak) longestStreak = streak;
        totalCompletions++;
        lastCompletedDate = today;
        completedToday    = true;
        return true;
    }

    // Reset daily/weekly completion flag (called by Tracker at start)
    virtual void resetCompletion() { completedToday = false; }

    // -- Getters ------------------------------
    int         getId()              const { return id; }
    string      getName()            const { return name; }
    string      getCategory()        const { return category; }
    int         getStreak()          const { return streak; }
    int         getLongestStreak()   const { return longestStreak; }
    int         getTotalCompletions()const { return totalCompletions; }
    bool        isCompletedToday()   const { return completedToday; }
    string      getLastDate()        const { return lastCompletedDate; }

    // Reward badge based on streak
    string getReward() const {
        if (streak >= 30) return "[LEGEND] (30+ days)";
        if (streak >= 21) return "[Champion] (21+ days)";
        if (streak >= 14) return "[Pro] (14+ days)";
        if (streak >= 7)  return "[Consistent] (7+ days)";
        if (streak >= 3)  return "[Getting Started] (3+ days)";
        return "[Beginner]";
    }

    virtual void display() const {
        cout << "  [" << id << "] " << name
             << " (" << category << ") | Type: " << getType()
             << " | Streak: " << streak << " day(s)"
             << " | " << getReward()
             << (completedToday ? "  [DONE]" : "  [Pending]")
             << "\n";
    }
};

// ---------------------------------------------
//  Derived Class: DailyHabit
// ---------------------------------------------
class DailyHabit : public Habit {
public:
    DailyHabit(int id, const string& name, const string& category)
        : Habit(id, name, category) {}

    string getType() const override { return "Daily"; }

    // Completable if not done today
    bool isCompletable() const override {
        return !completedToday;
    }
};

// ---------------------------------------------
//  Derived Class: WeeklyHabit
// ---------------------------------------------
class WeeklyHabit : public Habit {
private:
    int completionsThisWeek;
    int targetPerWeek;
    string weekStart;   // Monday of current ISO week "YYYY-MM-DD"

    // Get Monday of the week containing the given date string
    static string getWeekStart(const string& dateStr) {
        // Parse date
        int y, m, d;
        if (sscanf(dateStr.c_str(), "%d-%d-%d", &y, &m, &d) != 3) {
            return "";
        }
        tm t{};
        t.tm_year = y - 1900; t.tm_mon = m - 1; t.tm_mday = d;
        mktime(&t);
        // Sunday=0 in tm, Monday=1..Saturday=6
        int dow = t.tm_wday;        // 0=Sun
        int daysToMon = (dow == 0) ? 6 : dow - 1;
        time_t base = mktime(&t) - (time_t)daysToMon * 86400;
        tm* monday = localtime(&base);
        char buf[11];
        strftime(buf, sizeof(buf), "%Y-%m-%d", monday);
        return string(buf);
    }

public:
    WeeklyHabit(int id, const string& name, const string& category, int targetPerWeek = 3)
        : Habit(id, name, category),
          completionsThisWeek(0), targetPerWeek(targetPerWeek), weekStart("") {}

    string getType() const override {
        return "Weekly(" + to_string(completionsThisWeek) + "/" + to_string(targetPerWeek) + ")";
    }

    bool isCompletable() const override {
        // Can complete if haven't hit weekly target yet
        string today = todayStr();
        string ws    = getWeekStart(today);
        // If a new week, always completable
        if (ws != weekStart) return true;
        return completionsThisWeek < targetPerWeek;
    }

    bool markComplete() override {
        if (!isCompletable()) {
            cout << "  [!] Weekly target (" << targetPerWeek << "x) already met for this week!\n";
            return false;
        }
        string today = todayStr();
        string ws    = getWeekStart(today);

        // New week -> reset weekly counter
        if (ws != weekStart) {
            completionsThisWeek = 0;
            weekStart = ws;
            // Only reset streak if they didn't complete at all last week
            if (lastCompletedDate != "" && getWeekStart(lastCompletedDate) != getWeekStart(yesterdayStr())) {
                // last completion was not in adjacent week
                if (getWeekStart(lastCompletedDate) != getWeekStart(ws)) {
                    // More than one week gap -- check using simple logic
                    // Simplification: if last week's start != previous week, reset
                    streak = 0;
                }
            }
        }

        completionsThisWeek++;
        totalCompletions++;
        lastCompletedDate = today;
        completedToday    = true;

        // Weekly streak: count completed weeks
        streak++;
        if (streak > longestStreak) longestStreak = streak;

        return true;
    }

    void resetCompletion() override {
        completedToday = false;
        // Weekly: don't reset completionsThisWeek daily
    }

    int getTarget()            const { return targetPerWeek; }
    int getCompletionsThisWeek() const { return completionsThisWeek; }
};

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

int main() {
    // ── Get username ─────────────────────────
    cout << "\n+------------------------------------------+\n";
    cout << "  |     *  HABIT BUILDER + STREAK SYSTEM    |\n";
    cout << "+------------------------------------------+\n";
    cout << "  Enter your name: ";
    string name;
    getline(cin, name);
    if (name.empty()) name = "User";

    User    user(name);
    Tracker tracker(&user);
    tracker.run();

    return 0;
}
