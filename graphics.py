import pandas as pd
import matplotlib.pyplot as plt

try:
    data = pd.read_csv('time_sorts.txt', delim_whitespace=True, header=None)
    # Присваиваем колонкам имена для удобства (необязательно, но улучшает читаемость)
    data.columns = ['X', 'Y1', 'Y2', 'Y3', 'Y4']

    # Шаг 2: Создание и настройка графика
    # Создаем фигуру и оси для графика. figsize задает размер окна с графиком.
    fig, ax = plt.subplots(figsize=(10, 6))

    # Рисуем каждую линию (каждый столбец Y против столбца X)
    # Используем разные маркеры и стили линий, чтобы графики легко различались
    ax.plot(data['X'], data['Y1'], marker='o', linestyle='-', label='std::sort()')
    ax.plot(data['X'], data['Y2'], marker='s', linestyle='--', label='Пузырьковая сортировка')
    ax.plot(data['X'], data['Y3'], marker='^', linestyle='-.', label='Сортировка выбором')
    ax.plot(data['X'], data['Y4'], marker='x', linestyle=':', label='Пирамидальная сортировка')

    # Шаг 3: Добавление элементов оформления
    ax.set_title('Графики зависимостей времени от размера входных данных') # Заголовок графика
    ax.set_xlabel('Размер входных данных')               # Подпись оси X
    ax.set_ylabel('Время выполнения, ms')               # Подпись оси Y
    ax.legend()                                 # Показать легенду (описание линий)
    ax.grid(True)                               # Включить сетку для удобства чтения
    ax.set_yscale('log')
    # Шаг 4: Отображение графика
    output_filename = 'time_graphics.png'
    plt.savefig(output_filename, dpi=300)
    plt.show()

except FileNotFoundError:
    print("Ошибка: файл 'test_times.txt' не найден. Убедитесь, что он находится в той же папке, что и скрипт.")

