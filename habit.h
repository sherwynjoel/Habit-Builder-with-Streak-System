
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
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
        sscanf(dateStr.c_str(), "%d-%d-%d", &y, &m, &d);
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
