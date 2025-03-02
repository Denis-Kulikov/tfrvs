#include "task.hpp"

std::vector<Task> countingSort(std::vector<Task>& tasks) {
    int max_t = 0;
    for (std::size_t i = 0; i < tasks.size(); ++i) 
        max_t = std::max(max_t, tasks[i].t);

    std::vector<std::vector<Task>> count(max_t + 1);
    for (const auto& task : tasks) 
        count[task.t].push_back(task);

    std::vector<Task> sortedTasks;
    for (int i = max_t; i >= 1; --i) {
        for (const auto& task : count[i]) {
            sortedTasks.push_back(task);
        }
    }

    return sortedTasks;
}
