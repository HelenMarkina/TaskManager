#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum {
    NEW,
    IN_PROGRESS,
    COMPLETED,
    CANCELLED
} Status;

struct Task {
    char name[100];
    time_t creation_date;
    char assignee[50];
    enum Status status;
    int estimated_hours;
    int actual_hours;
};

struct Task tasks[100];
int task_count = 0;

// Функции для работы с данными
void print_date(time_t t) {
    struct tm* timeinfo = localtime(&t);
    printf("%02d.%02d.%04d %02d:%02d",
        timeinfo->tm_mday,
        timeinfo->tm_mon + 1,
        timeinfo->tm_year + 1900,
        timeinfo->tm_hour,
        timeinfo->tm_min);
}

void print_status(Status s) {
    switch (s) {
    case NEW: printf("Новая"); break;
    case IN_PROGRESS: printf("В работе"); break;
    case COMPLETED: printf("Завершена"); break;
    case CANCELLED: printf("Отменена"); break;
    default: printf("Неизвестно");
    }
}

time_t input_date() {
    struct tm timeinfo;
    memset(&timeinfo, 0, sizeof(struct tm));

    printf("Введите день (1-31): ");
    scanf("%d", &timeinfo.tm_mday);
    printf("Введите месяц (1-12): ");
    scanf("%d", &timeinfo.tm_mon);
    timeinfo.tm_mon--; // месяц от 0 до 11
    printf("Введите год (например, 2024): ");
    scanf("%d", &timeinfo.tm_year);
    timeinfo.tm_year -= 1900; // год с 1900
    printf("Введите час (0-23): ");
    scanf("%d", &timeinfo.tm_hour);
    printf("Введите минуту (0-59): ");
    scanf("%d", &timeinfo.tm_min);
    getchar(); // очистка буфера

    timeinfo.tm_sec = 0;

    return mktime(&timeinfo);
}

//Получение текущей даты и времени
time_t get_current_date() {
    return time(NULL);
}


void add_task() {
    if (task_count >= 100) {
        printf("Достигнут лимит задач (100)!\n");
        return;
    }

    printf("\n=== Добавление новой задачи ===\n");

    struct Task new_task;

    // Название задачи
    printf("Введите название задачи: ");
    fgets(new_task.name, sizeof(new_task.name), stdin);
    new_task.name[strcspn(new_task.name, "\n")] = 0; // удаляем символ новой строки

    // Дата создания (можно выбрать вручную или использовать системную)
    char choice;
    printf("Использовать текущую дату и время? (y/n): ");
    scanf("%c", &choice);
    getchar(); // очистка буфера

    if (choice == 'y' || choice == 'Y') {
        new_task.creation_date = get_current_date();
        printf("Установлена текущая дата: ");
        print_date(new_task.creation_date);
        printf("\n");
    }
    else {
        printf("Введите дату создания задачи:\n");
        new_task.creation_date = input_date();
    }

    // Исполнитель
    printf("Введите имя исполнителя: ");
    fgets(new_task.assignee, sizeof(new_task.assignee), stdin);
    new_task.assignee[strcspn(new_task.assignee, "\n")] = 0;

    // Статус
    printf("Выберите статус задачи:\n");
    printf("0 - Новая\n1 - В работе\n2 - Завершена\n3 - Отменена\n");
    printf("Ваш выбор (0-3): ");
    int status_choice;
    scanf("%d", &status_choice);
    new_task.status = (enum Status)status_choice;

    // Оценка времени
    printf("Введите оценку времени в часах: ");
    scanf("%d", &new_task.estimated_hours);

    // Фактическое время
    printf("Введите фактическое время в часах: ");
    scanf("%d", &new_task.actual_hours);

    getchar();

    tasks[task_count] = new_task;
    task_count++;

    printf("Задача успешно добавлена!\n\n");
}

void view_all_tasks() {
    printf("\n=== Список всех задач ===\n");

    if (task_count == 0) {
        printf("Задач нет.\n");
        return;
    }

    for (int i = 0; i < task_count; i++) {
        printf("\n--- Задача #%d ---\n", i + 1);
        printf("Название: %s\n", tasks[i].name);
        printf("Дата создания: ");
        print_date(tasks[i].creation_date);
        printf("\n");
        printf("Исполнитель: %s\n", tasks[i].assignee);
        printf("Статус: ");
        print_status(tasks[i].status);
        printf("\n");
        printf("Оценка времени: %d часов\n", tasks[i].estimated_hours);
        printf("Фактическое время: %d часов\n", tasks[i].actual_hours);
        printf("\n");
    }
}

void search_by_range() {
    printf("\n=== Поиск по диапазону времени ===\n");
    printf("В разработке\n\n");
}

void sort_data() {
    printf("\n=== Сортировка данных ===\n");
    printf("В разработке\n\n");
}

void save_to_file() {
    printf("\n=== Сохранение в файл ===\n");
    printf("В разработке\n\n");
}

void load_from_file() {
    printf("\n=== Загрузка из файла ===\n");
    printf("В разработке\n\n");
}


void print_menu() {
    printf("================================\n");
    printf("   СИСТЕМА УПРАВЛЕНИЯ ЗАДАЧАМИ   \n");
    printf("================================\n");
    printf("1. Просмотреть все задачи\n");
    printf("2. Добавить новую задачу\n");
    printf("3. Поиск по диапазону времени\n");
    printf("4. Отсортировать задачи\n");
    printf("5. Сохранить данные в файл\n");
    printf("6. Загрузить данные из файла\n");
    printf("0. Выход\n");
    printf("================================\n");
    printf("Выберите пункт меню: ");
}


int main() {
    int choice;

    // Для поддержки русского языка в консоли Windows
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    struct tm timeinfo1, timeinfo2;

    // Тестовая задача 1 (15 мая 2025, 10:30)
    memset(&timeinfo1, 0, sizeof(struct tm));
    timeinfo1.tm_mday = 15;
    timeinfo1.tm_mon = 4; // май (0-11)
    timeinfo1.tm_year = 125; // 2024-1900
    timeinfo1.tm_hour = 10;
    timeinfo1.tm_min = 30;
    timeinfo1.tm_isdst = -1;

    struct Task test_task1 = {
        "Создать прототип программы",
        mktime(&timeinfo1),
        "Иванов И.И.",
        IN_PROGRESS,
        10,
        5
    };

    // Тестовая задача 2 (10 мая 2025, 14:00)
    memset(&timeinfo2, 0, sizeof(struct tm));
    timeinfo2.tm_mday = 10;
    timeinfo2.tm_mon = 4; // май
    timeinfo2.tm_year = 125; // 2024-1900
    timeinfo2.tm_hour = 14;
    timeinfo2.tm_min = 0;
    timeinfo2.tm_isdst = -1;

    struct Task test_task2 = {
        "Написать документацию",
        mktime(&timeinfo2),
        "Петров П.П.",
        NEW,
        5,
        0
    };

    tasks[0] = test_task1;
    tasks[1] = test_task2;
    task_count = 2;

    do {
        print_menu();
        scanf("%d", &choice);
        getchar();

        switch (choice) {
        case 1:
            view_all_tasks();
            break;
        case 2:
            add_task();
            break;
        case 3:
            search_by_range();
            break;
        case 4:
            sort_data();
            break;
        case 5:
            save_to_file();
            break;
        case 6:
            load_from_file();
            break;
        case 0:
            printf("Выход из программы...\n");
            break;
        default:
            printf("Неверный выбор! Попробуйте снова.\n");
        }

        if (choice != 0) {
            printf("Нажмите Enter для продолжения...");
            getchar();
        }

    } while (choice != 0);

    return 0;
}