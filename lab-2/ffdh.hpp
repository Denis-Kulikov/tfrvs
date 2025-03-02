#include <iostream>
#include <bitset>
#include <fstream>
#include <vector>
#include "task.hpp"
#include "node.hpp"

class FFDH {
public:
    using Mask = std::bitset<32>;

    class Node {
    public:
        Node() : reserved(0), machine_mask(0) {}
        int reserved;
        Mask machine_mask;
    };

    FFDH(const FFDH&) = delete;
    FFDH(FFDH&& other) noexcept {
        tree = std::move(other.tree);
        leaves = other.leaves;
        n = other.n;
    }

    FFDH(int n_, std::size_t num_leaves) {
        n = n_;
        leaves = 1;
        while (leaves < num_leaves) leaves *= 2;
        std::size_t size = 2 * leaves - 1;
        tree = std::vector<Node>(size, Node());
    }

    FFDH(int n_) {
        n = n_;
        leaves = 2;
        std::size_t size = 2 * leaves - 1;
        tree = std::vector<Node>(size, Node());
    }

    FFDH& operator=(const FFDH&) = delete;
    FFDH& operator=(FFDH&& other) noexcept {
        if (this != &other) {
            tree = std::move(other.tree);
            leaves = other.leaves;
        }
        return *this;
    }

    void start(std::vector<Task>& tasks) {
        tasks = countingSort(tasks);
        for (auto &task : tasks) {
            insert(task);
            // std::cout << "Size " << size() << std::endl;
            // for (int i = size() - leaves; i < size(); ++i) 
            //     std::cout << tree[i].machine_mask << std::endl;
            // std::cout << std::endl;
        }
    }

    std::size_t size() { return tree.size(); }

    void printTreeSerialized(std::ostream& out, std::vector<Node>& tree, std::size_t index, std::size_t size) {
        if (index >= size) {
            out << "# ";
            return;
        }
        if (tree[index].reserved == 0) {
            out << "* ";
        } else {
            out << tree[index].reserved << " ";
        }
        printTreeSerialized(out, tree, index * 2 + 1, size);
        printTreeSerialized(out, tree, index * 2 + 2, size);
    }

    void saveTreeToFile(const std::string& filename) {
        std::ofstream out(filename);
        if (!out) {
            std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
            return;
        }
        printTreeSerialized(out, tree, 0, size());
        out << std::endl;
        out.close();
    }

    void printTree() {
        saveTreeToFile("tree.txt");
    }

    void insert(Task& task) {
        if (task.t == 0 || task.r == 0) return;
        if (!enough(0, task.r)) expand(); 
        insert(task, 0);
    }

protected:
    void update_parents(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            tree[parent].reserved = std::min(tree[parent * 2 + 1].reserved, tree[parent * 2 + 2].reserved);
            index = parent;
        }
    }

    // кол-ва машин на уровне достаточно
    bool enough(int index, int r) {
        return tree[index].reserved + r <= n;
    }

    // true - не подходит
    bool check_mask(int index, Mask mask, int length) {
        if (length <= 0) return false;
        if ((tree[index].machine_mask & mask).any()) {
            // std::cout << "MASK " << mask << std::endl; 
            // std::cout << "MASK " << tree[index].machine_mask << std::endl; 
            // std::cout << "MASK " << &tree[index].machine_mask << std::endl; 
            return true;
        } 

        return check_mask(index + 1, mask, length - 1);
    }

    // должно возвращать индекс бита с которого доступна вставнка, -1 если вставка не доступна
    int can_fit(Task& task, int index) {
        int start = index;
        int end = std::min(index + task.t - 1, static_cast<int>(size()) - 1);

        for (int i = start; i <= end; ++i) { 
            if (!enough(i, task.r)) return -2; 
        }

        int max_shift = n - task.r;
        int max_lvl = std::min(task.t, end - start + 1);
        Mask mask = (1u << task.r) - 1;

        for (int shift = 0; shift <= max_shift; ++shift, mask <<= 1) {
            if (!check_mask(start, mask, max_lvl))
                return shift;
        }

        return -1;
    }

    void insert_cascade(const Task& task, int index, int t, Mask mask) {
        tree[index].reserved += task.r;
        tree[index].machine_mask |= mask;
        update_parents(index);

        if (--t > 0) {
            if (++index >= size()) index = expand(index);
            insert_cascade(task, index, t, mask);
        }
    }

    bool insert(Task& task, int index) {
        if (!enough(index, task.r)) return true; 

        if (index >= (leaves - 1)) {
            int bits = can_fit(task, index);
            if (bits < 0) return true;

            task.schedule.tau = index - leaves + 1;
            for (auto& x : task.schedule.x)
                x = bits + (&x - &task.schedule.x[0]);

            Mask mask = (1u << task.r) - 1;
            mask <<= bits;
            insert_cascade(task, index, task.t, mask);

            return false;
        }

        if (insert(task, index * 2 + 1))
            return insert(task, index * 2 + 2);

        return false;
    }

    static void copy_recursive(std::vector<Node>& first, std::vector<Node>& last, std::size_t last_size, std::size_t first_index, std::size_t last_index) {
        if (last_index >= last_size) return;

        first[first_index] = last[last_index];

        copy_recursive(first, last, last_size, first_index * 2 + 1, last_index * 2 + 1);
        copy_recursive(first, last, last_size, first_index * 2 + 2, last_index * 2 + 2);
    }

    static void copy_to_left_subtree(FFDH* first, FFDH* last) {
        if (last->size() == 0 || first->size() < (last->leaves * 2 - 1)) return;
        copy_recursive(first->tree, last->tree, last->size(), 1, 0);
    }

    // index - leaf
    int expand(int index) {
        index -= leaves - 1;
        expand(); 
        return index += leaves - 1;
    }

    void expand() {
        FFDH new_TournamentTree(n, leaves * 2);
        copy_to_left_subtree(&new_TournamentTree, this);
        *this = std::move(new_TournamentTree);
    }

private:
    std::vector<Node> tree;
    std::size_t leaves;
    int n;
};
