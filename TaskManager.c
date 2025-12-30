#define _CRT_SECURE_NO_WARNINGS
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

typedef enum {
    LOW,
    MEDIUM,
    HIGH,
    CRITICAL
} Priority;

typedef struct {
    char name[100];
    time_t creation_date;
    char assignee[50];
    Status status;
    Priority priority;
    int estimated_hours;
    int actual_hours;
} Task;

int show_all_tasks(Task* tasks, int task_count);
int save_to_file(Task* tasks, int task_count);
int search_by_status(Task* tasks, int task_count);
int search_by_assignee_and_time(Task* tasks, int task_count);
int sort_tasks(Task* tasks, int task_count);
int show_menu();
int add_task(Task* tasks, int task_count);
int load_from_file(Task* tasks, int task_count, int task_capacity);

int main() {
    system("chcp 1251");

    Task* tasks = NULL;
    int task_count = 0;
    int task_capacity = 0;
    int result;

    task_capacity = 50;
    tasks = malloc(task_capacity * sizeof(Task));
    if (tasks == NULL) {
        printf("Ошибка выделения памяти!\n");
        return 1;
    }

    int choice;

    do {
        choice = show_menu();

        switch (choice) {
        case 1:
            result = show_all_tasks(tasks, task_count);
            printf("Отображено задач: %d\n", result);
            break;

        case 2:
            if (task_count >= task_capacity) {
                task_capacity = task_capacity * 2;
                Task* new_tasks = realloc(tasks, task_capacity * sizeof(Task));
                if (new_tasks == NULL) {
                    printf("Ошибка выделения памяти!\n");
                    task_capacity = task_capacity / 2;
                    break;
                }
                tasks = new_tasks;
                printf("Массив увеличен до %d задач\n", task_capacity);
            }

            result = add_task(tasks, task_count);
            if (result == 1) {
                task_count++;
                printf("Задача добавлена в массив! Всего задач: %d\n", task_count);
            }
            else {
                printf("Ошибка при добавлении задачи!\n");
            }
            break;

        case 3:
            result = search_by_status(tasks, task_count);
            printf("Найдено задач по статусу: %d\n", result);
            break;

        case 4:
            result = search_by_assignee_and_time(tasks, task_count);
            printf("Найдено задач по исполнителю и времени: %d\n", result);
            break;

        case 5:
            result = sort_tasks(tasks, task_count);
            if (result == 1) {
                printf("Сортировка выполнена успешно.\n");
            }
            else {
                printf("Сортировка не выполнена.\n");
            }
            break;

        case 6:
            result = save_to_file(tasks, task_count);
            printf("Сохранено задач в файл: %d\n", result);
            break;

        case 7:
            result = load_from_file(tasks, task_count, task_capacity);
            if (result > task_count) {
                printf("Загружено %d задач. Всего теперь: %d\n", result - task_count, result);
                task_count = result;
            }
            else {
                printf("Загружено задач: %d\n", result - task_count);
            }
            break;

        case 8:
        {
            int n;
            printf("Сколько задач сгенерировать? ");
            scanf("%d", &n);
            getchar();

            srand(time(NULL));
            const char* names[] = { "приложение", "отчет", "сайт", "базу", "тест", "код", "документ", "модуль" };
            const char* verbs[] = { "Разработать", "Создать", "Написать", "Протестировать", "Обновить", "Исправить" };
            const char* people_name[] = { "Иван", "Петр", "Анна", "Мария", "Алексей", "Елена" };
            const char* people_surname[] = { "Галич", "Короленко", "Шастун", "Евтушенко", "Долгих", "Ткач" };

            if (task_count + n > task_capacity) {
                task_capacity = task_count + n;
                Task* new_tasks = realloc(tasks, task_capacity * sizeof(Task));
                if (new_tasks == NULL) {
                    printf("Ошибка выделения памяти!\n");
                    break;
                }
                tasks = new_tasks;
            }

            for (int i = 0; i < n; i++) {
                sprintf(tasks[task_count].name, "%s %s",
                    verbs[rand() % 6],
                    names[rand() % 8]);

                tasks[task_count].creation_date = time(NULL) - (rand() % 30 * 86400);

                sprintf(tasks[task_count].assignee, "%s %s",
                    people_surname[rand() % 6],
                    people_name[rand() % 6]);

                tasks[task_count].status = rand() % 4;
                tasks[task_count].priority = rand() % 4;
                tasks[task_count].estimated_hours = 1 + rand() % 10;
                tasks[task_count].actual_hours = 1 + rand() % 10;

                task_count++;
            }
            printf("Сгенерировано %d задач. Всего теперь: %d\n", n, task_count);
            break;
        }

        case 0:
            printf("Выход из программы...\n");
            break;

        default:
            printf("Неверный выбор!\n");
        }

        if (choice != 0) {
            printf("\nНажмите Enter для продолжения...");
            getchar();
        }

    } while (choice != 0);

    if (tasks != NULL) {
        free(tasks);
        printf("Память освобождена\n");
    }

    return 0;
}

int add_task(Task* tasks, int task_count) {
    if (task_count < 0) {
        return 0;
    }

    Task new_task;

    printf("\n=== Добавление новой задачи ===\n");

    printf("Введите название задачи: ");
    fgets(new_task.name, sizeof(new_task.name), stdin);
    new_task.name[strcspn(new_task.name, "\n")] = 0;

    new_task.creation_date = time(NULL);

    printf("Введите имя исполнителя: ");
    fgets(new_task.assignee, sizeof(new_task.assignee), stdin);
    new_task.assignee[strcspn(new_task.assignee, "\n")] = 0;

    printf("Выберите статус задачи (0-3): ");
    int status_choice;
    scanf("%d", &status_choice);
    new_task.status = (Status)status_choice;

    printf("Выберите приоритет задачи (0-3): ");
    int priority_choice;
    scanf("%d", &priority_choice);
    new_task.priority = (Priority)priority_choice;

    printf("Введите оценку времени в часах: ");
    scanf("%d", &new_task.estimated_hours);

    printf("Введите фактическое время в часах: ");
    scanf("%d", &new_task.actual_hours);
    getchar();

    if (strlen(new_task.name) == 0) {
        printf("Ошибка: название задачи не может быть пустым!\n");
        return 0;
    }

    if (new_task.estimated_hours <= 0) {
        printf("Ошибка: оценка времени должна быть положительной!\n");
        return 0;
    }

    tasks[task_count] = new_task;

    printf("\nЗадача '%s' успешно создана!\n", new_task.name);
    return 1;
}

int show_all_tasks(Task* tasks, int task_count) {
    printf("\n=== Список всех задач ===\n");

    if (task_count == 0) {
        printf("Задач нет.\n");
        return 0;
    }

    for (int i = 0; i < task_count; i++) {
        printf("\n--- Задача #%d ---\n", i + 1);
        printf("Название: %s\n", tasks[i].name);

        struct tm* timeinfo = localtime(&tasks[i].creation_date);
        printf("Дата: %02d.%02d.%04d\n",
            timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);

        printf("Исполнитель: %s\n", tasks[i].assignee);

        printf("Статус: ");
        switch (tasks[i].status) {
        case NEW: printf("Новая"); break;
        case IN_PROGRESS: printf("В работе"); break;
        case COMPLETED: printf("Завершена"); break;
        case CANCELLED: printf("Отменена"); break;
        }

        printf("\nПриоритет: ");
        switch (tasks[i].priority) {
        case LOW: printf("Низкий"); break;
        case MEDIUM: printf("Средний"); break;
        case HIGH: printf("Высокий"); break;
        case CRITICAL: printf("Критический"); break;
        }

        printf("\nОценка времени: %d часов\n", tasks[i].estimated_hours);
        printf("Фактическое время: %d часов\n", tasks[i].actual_hours);
    }

    return task_count;
}

int save_to_file(Task* tasks, int task_count) {
    printf("\n=== Сохранение в файл ===\n");

    if (task_count == 0) {
        printf("Нет задач для сохранения.\n");
        return 0;
    }

    char filename[100];
    printf("Введите имя файла: ");
    scanf("%s", filename);
    getchar();

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка создания файла!\n");
        return 0;
    }

    for (int i = 0; i < task_count; i++) {
        struct tm* timeinfo = localtime(&tasks[i].creation_date);

        char status_str[20];
        switch (tasks[i].status) {
        case NEW: strcpy(status_str, "NEW"); break;
        case IN_PROGRESS: strcpy(status_str, "IN_PROGRESS"); break;
        case COMPLETED: strcpy(status_str, "COMPLETED"); break;
        case CANCELLED: strcpy(status_str, "CANCELLED"); break;
        }

        char priority_str[20];
        switch (tasks[i].priority) {
        case LOW: strcpy(priority_str, "LOW"); break;
        case MEDIUM: strcpy(priority_str, "MEDIUM"); break;
        case HIGH: strcpy(priority_str, "HIGH"); break;
        case CRITICAL: strcpy(priority_str, "CRITICAL"); break;
        }

        fprintf(file, "%s|%02d.%02d.%04d|%s|%s|%s|%d|%d\n",
            tasks[i].name,
            timeinfo->tm_mday,
            timeinfo->tm_mon + 1,
            timeinfo->tm_year + 1900,
            tasks[i].assignee,
            status_str,
            priority_str,
            tasks[i].estimated_hours,
            tasks[i].actual_hours);
    }

    fclose(file);
    printf("Сохранено %d задач в файл '%s'\n", task_count, filename);
    return task_count;
}

int load_from_file(Task* tasks, int task_count, int task_capacity) {
    printf("\n=== Загрузка из файла ===\n");

    char filename[100];
    printf("Введите имя файла: ");
    scanf("%s", filename);
    getchar();

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка открытия файла!\n");
        return task_count;  // Возвращаем текущее количество
    }

    int loaded = 0;
    char line[256];
    int current_index = task_count;  // Начинаем добавлять после существующих задач

    while (fgets(line, sizeof(line), file) != NULL) {
        if (current_index >= task_capacity) {
            printf("Достигнут максимальный размер массива. Загружено %d задач.\n", loaded);
            break;
        }

        struct tm timeinfo = { 0 };
        char status_str[20], priority_str[20];
        int index = current_index;  // Используем текущий индекс

        if (sscanf(line, "%[^|]|%d.%d.%d|%[^|]|%[^|]|%[^|]|%d|%d",
            tasks[index].name,
            &timeinfo.tm_mday,
            &timeinfo.tm_mon,
            &timeinfo.tm_year,
            tasks[index].assignee,
            status_str,
            priority_str,
            &tasks[index].estimated_hours,
            &tasks[index].actual_hours) == 9) {

            timeinfo.tm_mon -= 1;
            timeinfo.tm_year -= 1900;
            tasks[index].creation_date = mktime(&timeinfo);

            // Статус
            if (strcmp(status_str, "NEW") == 0) tasks[index].status = NEW;
            else if (strcmp(status_str, "IN_PROGRESS") == 0) tasks[index].status = IN_PROGRESS;
            else if (strcmp(status_str, "COMPLETED") == 0) tasks[index].status = COMPLETED;
            else if (strcmp(status_str, "CANCELLED") == 0) tasks[index].status = CANCELLED;

            // Приоритет
            if (strcmp(priority_str, "LOW") == 0) tasks[index].priority = LOW;
            else if (strcmp(priority_str, "MEDIUM") == 0) tasks[index].priority = MEDIUM;
            else if (strcmp(priority_str, "HIGH") == 0) tasks[index].priority = HIGH;
            else if (strcmp(priority_str, "CRITICAL") == 0) tasks[index].priority = CRITICAL;

            loaded++;
            current_index++;
        }
    }

    fclose(file);

    if (loaded > 0) {
        printf("Загружено %d задач из файла '%s'\n", loaded, filename);
        return task_count + loaded;  // Возвращаем общее количество
    }
    else {
        printf("Файл пуст или имеет неверный формат\n");
        return task_count;  // Возвращаем исходное количество
    }
}

int search_by_status(Task* tasks, int task_count) {
    printf("\n=== Поиск задач по статусу ===\n");

    printf("Выберите статус для поиска:\n");
    printf("0 - Новая\n");
    printf("1 - В работе\n");
    printf("2 - Завершена\n");
    printf("3 - Отменена\n");
    printf("Ваш выбор (0-3): ");

    int status_choice;
    scanf("%d", &status_choice);
    getchar();

    if (status_choice < 0 || status_choice > 3) {
        printf("Неверный статус!\n");
        return 0;
    }

    int found_count = 0;

    printf("\nРезультаты поиска:\n");

    for (int i = 0; i < task_count; i++) {
        if (tasks[i].status == status_choice) {
            found_count++;
            printf("\n--- Задача #%d ---\n", found_count);
            printf("Название: %s\n", tasks[i].name);
            printf("Исполнитель: %s\n", tasks[i].assignee);
            printf("Оценка времени: %d часов\n", tasks[i].estimated_hours);
        }
    }

    if (found_count == 0) {
        printf("Задачи не найдены.\n");
    }

    return found_count;
}

int search_by_assignee_and_time(Task* tasks, int task_count) {
    printf("\n=== Поиск по исполнителю и времени ===\n");

    char assignee[50];
    printf("Введите имя исполнителя: ");
    fgets(assignee, sizeof(assignee), stdin);
    assignee[strcspn(assignee, "\n")] = 0;

    int max_hours;
    printf("Введите максимальное время (часов): ");
    scanf("%d", &max_hours);
    getchar();

    int found_count = 0;

    printf("\nРезультаты поиска:\n");

    for (int i = 0; i < task_count; i++) {
        if (strcmp(tasks[i].assignee, assignee) == 0 &&
            tasks[i].estimated_hours <= max_hours) {

            found_count++;
            printf("\n--- Задача #%d ---\n", found_count);
            printf("Название: %s\n", tasks[i].name);
            printf("Статус: ");
            switch (tasks[i].status) {
            case NEW: printf("Новая"); break;
            case IN_PROGRESS: printf("В работе"); break;
            case COMPLETED: printf("Завершена"); break;
            case CANCELLED: printf("Отменена"); break;
            }
            printf("\nОценка времени: %d часов\n", tasks[i].estimated_hours);
        }
    }

    if (found_count == 0) {
        printf("Задачи не найдены.\n");
    }

    return found_count;
}

int sort_tasks(Task* tasks, int task_count) {
    printf("\n=== Сортировка задач ===\n");

    if (task_count < 2) {
        printf("Сортировка не требуется\n");
        return 0;
    }

    // Пузырьковая сортировка
    for (int i = 0; i < task_count - 1; i++) {
        for (int j = 0; j < task_count - i - 1; j++) {

            // По времени
            if (tasks[j].estimated_hours > tasks[j + 1].estimated_hours) {
                Task temp = tasks[j];
                tasks[j] = tasks[j + 1];
                tasks[j + 1] = temp;
            }
            // Если время одинаковое
            else if (tasks[j].estimated_hours == tasks[j + 1].estimated_hours) {
                // По приоритету
                if (tasks[j].priority < tasks[j + 1].priority) {
                    Task temp = tasks[j];
                    tasks[j] = tasks[j + 1];
                    tasks[j + 1] = temp;
                }
                // Если приоритет одинаковый
                else if (tasks[j].priority == tasks[j + 1].priority) {
                    // По статусу
                    if (tasks[j].status > tasks[j + 1].status) {
                        Task temp = tasks[j];
                        tasks[j] = tasks[j + 1];
                        tasks[j + 1] = temp;
                    }
                }
            }
        }
    }

    printf("Сортировка завершена!\n");
    return 1;
}

int show_menu() {
    printf("\n================================\n");
    printf("   СИСТЕМА УПРАВЛЕНИЯ ЗАДАЧАМИ   \n");
    printf("================================\n");
    printf("1. Показать все задачи\n");
    printf("2. Добавить задачу\n");
    printf("3. Поиск по статусу\n");
    printf("4. Поиск по исполнителю и времени\n");
    printf("5. Сортировать задачи\n");
    printf("6. Сохранить в файл\n");
    printf("7. Загрузить из файла\n");
    printf("0. Выход\n");
    printf("================================\n");
    printf("Выберите пункт меню: ");

    int choice;
    scanf("%d", &choice);
    getchar();
    return choice;
}