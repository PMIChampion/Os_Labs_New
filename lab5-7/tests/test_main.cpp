#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

// ------------------------------
// Класс TimerManager
// ------------------------------
class TimerManager {
public:
    void Start() {
        running = true;
        start_time = std::chrono::steady_clock::now();
    }

    long ElapsedMs() const {
        if (!running) return 0;
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();
    }

    void Stop() {
        running = false;
    }

private:
    bool running = false;
    std::chrono::time_point<std::chrono::steady_clock> start_time;
};

// ------------------------------
// Класс TreeManager
// Управляет "узлами" в дереве:
//  - Узел имеет id
//  - Есть корневой узел (-1)
//  - Можно создавать узлы с заданным родителем
//  - Можно удалять узлы и их потомков
//  - Можно проверять, доступен ли узел
// ------------------------------
class TreeManager {
public:
    TreeManager() {
        // Можно сразу создать корневой узел, если нужно
        CreateNode(-1, -1);
    }

    bool CreateNode(int child_id, int parent_id) {
        // Если узел уже существует, ошибка
        if (nodes.find(child_id) != nodes.end()) return false;
        // Если родитель не существует и это не корень
        if (parent_id != -1 && nodes.find(parent_id) == nodes.end()) return false;

        NodeInfo info;
        info.id = child_id;
        info.parent = parent_id;
        nodes[child_id] = info;

        // Добавляем child_id в список детей родителя
        if (parent_id != -1) {
            nodes[parent_id].children.insert(child_id);
        }

        return true;
    }

    bool RemoveNode(int id) {
        // Если узел не существует
        if (nodes.find(id) == nodes.end()) return false;

        // Рекурсивно удаляем всех потомков
        RemoveSubtree(id);

        return true;
    }

    bool IsNodeAccessible(int id) const {
        return nodes.find(id) != nodes.end();
    }

private:
    struct NodeInfo {
        int id;
        int parent;
        std::unordered_set<int> children;
    };

    std::unordered_map<int, NodeInfo> nodes;

    void RemoveSubtree(int id) {
        // Сначала удаляем всех детей
        for (auto c : nodes[id].children) {
            RemoveSubtree(c);
        }
        // Удаляем связь родителя
        int parent_id = nodes[id].parent;
        if (parent_id != -1) {
            nodes[parent_id].children.erase(id);
        }

        // Теперь удаляем сам узел
        nodes.erase(id);
    }
};


TEST(TimerManagerTest, StartStopCheck) {
    TimerManager tm;
    tm.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    long elapsed = tm.ElapsedMs();
    // Проверяем, что прошло хотя бы 50 мс
    EXPECT_GE(elapsed, 50);
    tm.Stop();
    // После остановки таймер должен возвращать 0
    EXPECT_EQ(tm.ElapsedMs(), 0);
}

TEST(TimerManagerTest, NoStartCheck) {
    TimerManager tm;
    // Без Start elapsed всегда 0
    EXPECT_EQ(tm.ElapsedMs(), 0);
    tm.Stop(); 
    EXPECT_EQ(tm.ElapsedMs(), 0);
}


TEST(TreeManagerTest, CreateRemoveCheck) {
    TreeManager tm;
    // По умолчанию корневой узел (-1) существует
    EXPECT_TRUE(tm.IsNodeAccessible(-1));

    // Создаём узел 10 от -1
    EXPECT_TRUE(tm.CreateNode(10, -1));
    EXPECT_TRUE(tm.IsNodeAccessible(10));

    // Создаём узел 11 от 10
    EXPECT_TRUE(tm.CreateNode(11, 10));
    EXPECT_TRUE(tm.IsNodeAccessible(11));

    // Удаляем узел 10, должно исчезнуть и 11
    EXPECT_TRUE(tm.RemoveNode(10));
    EXPECT_FALSE(tm.IsNodeAccessible(10));
    EXPECT_FALSE(tm.IsNodeAccessible(11));
}

TEST(TreeManagerTest, FailCases) {
    TreeManager tm;
    // Нельзя создать узел с уже существующим ID
    EXPECT_FALSE(tm.CreateNode(-1, -1));
    // Нельзя создать узел с несуществующим родителем (кроме корня)
    EXPECT_FALSE(tm.CreateNode(100, 9999));
    // Удаление несуществующего узла
    EXPECT_FALSE(tm.RemoveNode(9999));
}

// Главная функция запуска тестов
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
