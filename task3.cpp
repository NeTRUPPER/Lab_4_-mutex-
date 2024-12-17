#include <iostream>
#include <mutex>
#include <chrono>
#include <thread>
#include <vector>

using namespace std;

// Класс для представления вилок
class Forks {
public:
    Forks() = default; // Конструктор по умолчанию
    mutex mtx; // Мьютекс для каждой вилки
};

// Функция, в которой философ ест, блокируя вилки
void eating(Forks& rightFork, Forks& leftFork, int philosopherNumber) {
    // Блокируем обе вилки с помощью unique_lock для обеспечения синхронизации
    unique_lock<mutex> rightLock(rightFork.mtx);
    unique_lock<mutex> leftLock(leftFork.mtx);
    
    // Философ начинает есть
    cout << "Философ " << philosopherNumber << " ест\n";
    
    // Устанавливаем время для еды (2 секунды)
    chrono::milliseconds eatingTime(2000);
    this_thread::sleep_for(eatingTime);
    
    // Философ заканчивает есть
    cout << "Философ " << philosopherNumber << " доел\n";
}

int main() {
    const int philosophers = 5; // Количество философов
    vector<Forks> forks(philosophers); // Вектор вилок для философов
    vector<thread> threads; // Вектор потоков для каждого философа
    
    // Начинаем с того, что первый философ думает
    cout << "Философ " << 1 << " думает\n"; 
    
    // Первый философ использует первую и последнюю вилку
    threads.emplace_back(eating, ref(forks[0]), ref(forks[philosophers - 1]), 1);
    
    // Для остальных философов создаём потоки с использованием соответствующих вилок
    for (int i = 1; i < philosophers; i++) {
        cout << "Философ " << i + 1 << " думает\n";
        // Каждый философ использует текущую вилку и предыдущую
        threads.emplace_back(eating, ref(forks[i]), ref(forks[i - 1]), i + 1);
    }
    
    // Ожидаем завершения всех потоков
    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
