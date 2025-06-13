/**
 * @file
 * @brief Программа для сравнительного анализа производительности различных алгоритмов сортировки.
 * 
 * Данная программа считывает данные о лотерейных билетах из файлов,
 * сортирует их с использованием четырех разных алгоритмов (std::sort, сортировка выбором,
 * пузырьковая сортировка, пирамидальная сортировка) и замеряет время их выполнения.
 * Результаты замеров сохраняются в файл для последующего анализа.
 */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm> 
#include <utility>
#include <stdexcept>
#include <random>
#include <time.h>

/**
 * @class LotteryTicket
 * @brief Класс для представления лотерейного билета.
 *
 * Хранит информацию о номере билета, его стоимости, дате розыгрыша и сумме выигрыша.
 * Также перегружает операторы сравнения для корректной работы алгоритмов сортировки.
 */
class LotteryTicket {
public:
    long long ticketNumber; ///< Номер билета.
    int cost;               ///< Стоимость билета.
    std::string lotteryDate;///< Дата проведения лотереи в формате "YYYY-MM-DD".
    int winAmount;          ///< Сумма выигрыша.

    /**
     * @brief Конструктор класса LotteryTicket.
     * @param num Номер билета.
     * @param c Стоимость билета.
     * @param date Дата проведения лотереи.
     * @param win Сумма выигрыша.
     */
    LotteryTicket(long long num, int c, std::string date, int win)
        : ticketNumber(num), cost(c), lotteryDate(date), winAmount(win) {}

    /**
     * @brief Оператор сравнения "меньше".
     * @details Сравнение происходит сначала по дате лотереи (по возрастанию), 
     *          затем по сумме выигрыша (по убыванию), и в последнюю очередь
     *          по номеру билета (по возрастанию).
     * @param other Другой объект LotteryTicket для сравнения.
     * @return true, если текущий объект меньше, чем other, иначе false.
     */
    bool operator<(const LotteryTicket& other) const {
        if (lotteryDate != other.lotteryDate) {
            return lotteryDate < other.lotteryDate;
        }
        if (winAmount != other.winAmount) {
            return winAmount > other.winAmount; // Сортировка по убыванию
        }
        return ticketNumber < other.ticketNumber;
    }

    /** @brief Оператор сравнения "больше". */
    bool operator>(const LotteryTicket& other) const { return other < *this; }
    /** @brief Оператор сравнения "меньше или равно". */
    bool operator<=(const LotteryTicket& other) const { return !(*this > other); }
    /** @brief Оператор сравнения "больше или равно". */
    bool operator>=(const LotteryTicket& other) const { return !(*this < other); }
};

/**
 * @brief Перегрузка оператора вывода для класса LotteryTicket.
 * @param os Поток вывода.
 * @param ticket Объект LotteryTicket для вывода.
 * @return Ссылка на поток вывода.
 */
std::ostream& operator<<(std::ostream& os, const LotteryTicket& ticket) {
    os << ticket.ticketNumber << "," << ticket.cost << "," << ticket.lotteryDate << "," << ticket.winAmount;
    return os;
}

// --- Алгоритмы сортировки ---

/**
 * @brief Реализация сортировки выбором.
 * @tparam T Тип элементов в векторе. Должен поддерживать оператор '<'.
 * @param arr Вектор элементов для сортировки. Сортируется на месте.
 */
template<typename T>
void selectionSort(std::vector<T>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            std::swap(arr[i], arr[min_idx]);
        }
    }
}

/**
 * @brief Реализация пузырьковой сортировки.
 * @tparam T Тип элементов в векторе. Должен поддерживать оператор '>'.
 * @param arr Вектор элементов для сортировки. Сортируется на месте.
 */
template<typename T>
void bubbleSort(std::vector<T>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

/**
 * @brief Вспомогательная функция для heapSort. Преобразует поддерево с корнем `i` в двоичную кучу.
 * @tparam T Тип элементов в векторе. Должен поддерживать оператор '>'.
 * @param arr Вектор, представляющий двоичную кучу.
 * @param n Размер кучи.
 * @param i Индекс корневого узла поддерева.
 */
template<typename T>
void heapify(std::vector<T>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

/**
 * @brief Реализация пирамидальной сортировки (Heap Sort).
 * @tparam T Тип элементов в векторе. Должен поддерживать операторы сравнения.
 * @param arr Вектор элементов для сортировки. Сортируется на месте.
 */
template<typename T>
void heapSort(std::vector<T>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

/**
 * @brief Считывает данные о лотерейных билетах из файла.
 * @param filename Имя файла для чтения.
 * @return Вектор объектов LotteryTicket, прочитанных из файла.
 * @throws std::runtime_error Если не удалось открыть файл.
 */
std::vector<LotteryTicket> readTicketsFromFile(const std::string& filename) {
    std::vector<LotteryTicket> tickets;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string item;
        long long num;
        int cost, win;
        std::string date;

        std::getline(ss, item, ','); num = std::stoll(item);
        std::getline(ss, item, ','); cost = std::stoi(item);
        std::getline(ss, item, ','); date = item;
        std::getline(ss, item, ','); win = std::stoi(item);
        
        tickets.emplace_back(num, cost, date, win);
    }
    return tickets;
}

/**
 * @brief Записывает данные о лотерейных билетах в файл.
 * @param filename Имя файла для записи.
 * @param tickets Вектор объектов LotteryTicket для записи.
 * @throws std::runtime_error Если не удалось открыть файл для записи.
 */
void writeTicketsToFile(const std::string& filename, const std::vector<LotteryTicket>& tickets) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }
    for (const auto& ticket : tickets) {
        file << ticket << "\n";
    }
}

/**
 * @brief Запускает и измеряет время выполнения std::sort, выводит результат в консоль и в файл.
 * @param tickets Вектор лотерейных билетов для сортировки. Передается по значению.
 */
void benchmarkStdSort(std::vector<LotteryTicket> tickets) {
    auto start = std::chrono::high_resolution_clock::now();
    std::sort(tickets.begin(), tickets.end());
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Algorithm: std::sort"
              << ", Time: " << duration.count() << " ms" << std::endl;
    
    std::string output_filename = "sorted_std_sort_" + std::to_string(tickets.size()) + ".csv";
    writeTicketsToFile(output_filename, tickets);
}

/**
 * @brief Измеряет время выполнения std::sort.
 * @param tickets Вектор лотерейных билетов для сортировки. Передается по значению.
 * @return Время выполнения сортировки в миллисекундах.
 */
long long measureStdSort(std::vector<LotteryTicket> tickets) {
    auto start = std::chrono::high_resolution_clock::now();
    
    std::sort(tickets.begin(), tickets.end());
    
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

/**
 * @brief Измеряет время выполнения сортировки выбором и записывает результат в файл.
 * @param tickets Вектор лотерейных билетов для сортировки. Передается по значению.
 * @return Время выполнения сортировки в миллисекундах.
 */
long long measureSelectionSort(std::vector<LotteryTicket> tickets) {
    auto start = std::chrono::high_resolution_clock::now();
    
    selectionSort(tickets);
    
    auto end = std::chrono::high_resolution_clock::now();

    std::string filename = "lottery_selection_sort_" + std::to_string(tickets.size());
    std::ofstream out;          
    out.open(filename);
    for(int i = 0; i < tickets.size(); i++){
        out << tickets[i] << std::endl;
    }
    out.close(); 

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

/**
 * @brief Измеряет время выполнения пузырьковой сортировки и записывает результат в файл.
 * @param tickets Вектор лотерейных билетов для сортировки. Передается по значению.
 * @return Время выполнения сортировки в миллисекундах.
 */
long long measureBubbleSort(std::vector<LotteryTicket> tickets) {
    auto start = std::chrono::high_resolution_clock::now();

    bubbleSort(tickets);
    
    auto end = std::chrono::high_resolution_clock::now();

    std::string filename = "lottery_bubble_sort_" + std::to_string(tickets.size());
    std::ofstream out;         
    out.open(filename);
    for(int i = 0; i < tickets.size(); i++){
        out << tickets[i] << std::endl;
    }
    out.close(); 

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

/**
 * @brief Измеряет время выполнения пирамидальной сортировки и записывает результат в файл.
 * @param tickets Вектор лотерейных билетов для сортировки. Передается по значению.
 * @return Время выполнения сортировки в миллисекундах.
 */
long long measureHeapSort(std::vector<LotteryTicket> tickets) {
    auto start = std::chrono::high_resolution_clock::now();

    heapSort(tickets);

    auto end = std::chrono::high_resolution_clock::now();

    std::string filename = "lottery_heap_sort_" + std::to_string(tickets.size());
    std::ofstream out;          
    out.open(filename);
    for(int i = 0; i < tickets.size(); i++){
        out << tickets[i] << std::endl;
    }
    out.close(); 

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

/**
 * @brief Главная функция программы.
 * @details Функция выполняет следующие шаги:
 * 1. Определяет размеры наборов данных для тестирования.
 * 2. Читает данные о лотерейных билетах из соответствующих файлов.
 * 3. Запускает замеры времени для четырех алгоритмов сортировки (std::sort, Bubble, Selection, Heap) на каждом наборе данных.
 * 4. Записывает результаты замеров (размер массива и время для каждого алгоритма) в файл "time_sorts.txt".
 * @return 0 в случае успешного выполнения.
 */
int main(){
    std::vector<std::string> filenames;
    std::vector<int> arr_size = {100, 500, 1000, 2500, 5000, 7500, 10000, 12500, 15000, 20000, 30000, 40000, 50000, 60000, 80000, 100000};

    // Загрузка данных
    std::vector<std::vector<LotteryTicket>> arrs_tickets;
    for(auto el: arr_size){
        filenames.push_back("lottery_" + std::to_string(el) + ".txt");
    }

    for(auto name: filenames){
        arrs_tickets.push_back(readTicketsFromFile(name));
    }

    // Замеры скорости 
    std::vector<long long> std_sort_times;
    for(auto arr : arrs_tickets){
        std_sort_times.push_back(measureStdSort(arr));
    }

    std::vector<long long> std_bubble_times;
    for(auto arr : arrs_tickets){
        std_bubble_times.push_back(measureBubbleSort(arr));
    }

    std::vector<long long> std_selection_times;
    for(auto arr : arrs_tickets){
        std_selection_times.push_back(measureSelectionSort(arr));
    }

    std::vector<long long> std_heap_times;
    for(auto arr : arrs_tickets){
        std_heap_times.push_back(measureHeapSort(arr));
    }

    // записываем результаты замеров в файл
    std::ofstream out;          // поток для записи
    out.open("time_sorts.txt");
    for(int i = 0; i < std_sort_times.size(); i++){
        out << arr_size[i] << '\t' << std_sort_times[i] << '\t' << std_bubble_times[i] << '\t' << std_selection_times[i] << '\t' << std_heap_times[i] << std::endl;
    }
    out.close(); 
}
