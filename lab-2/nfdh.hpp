#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include "task.hpp"
#include "node.hpp"


class NFDH {
public:
    NFDH(int n_) : number_machine(n_), reserved(n_), top_lvl(0) {}

    void start(std::vector<Task>& tasks) {
        tasks = countingSort(tasks);
        std::for_each(tasks.begin(), tasks.end(), [&](Task& task) { insert(task); });
    }

private:
    void jump(const int t) {
        low_lvl = top_lvl;
        top_lvl += t;
        reserved = 0;
    }

    bool can_fit(const int r) const {
        return reserved + r <= number_machine;
    }

    void place_task(Task::Schedule& schedule) {
        schedule.tau = low_lvl;
        for (auto& x : schedule.x)
            x = reserved++;
    }

    void insert(Task& task) {
        if (!can_fit(task.r)) jump(task.t);
        place_task(task.schedule);
    }

    int low_lvl;
    int top_lvl;
    int number_machine;
    int reserved;
};

