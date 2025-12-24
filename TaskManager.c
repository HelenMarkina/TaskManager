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

Task* init_tasks_array(int* task_count, int* task_capacity, int initial_size);
int add_task_to_array(Task** tasks, int* task_count, int* task_capacity, Task new_task);
int free_tasks_array(Task* tasks, int* task_count, int* task_capacity);
Task* generate_test_data(Task* old_tasks, int* task_count, int* task_capacity, int n);

int add_task(Task** tasks, int* task_count, int* task_capacity);
int show_all_tasks(Task* tasks, int task_count);
int save_to_file(Task* tasks, int task_count);
Task* load_from_file(Task* old_tasks, int* task_count, int* task_capacity);
int search_by_status(Task* tasks, int task_count);
int search_by_assignee_and_time(Task* tasks, int task_count);
int sort_tasks(Task* tasks, int task_count);
int show_menu();

int main() {
    system("chcp 1251");

    Task* tasks = NULL;
    int task_count = 0;
    int task_capacity = 0;

    tasks = init_tasks_array(&task_count, &task_capacity, 10);
    if (tasks == NULL) {
        return 1;
    }

    int choice;

    do {
        choice = show_menu();

        switch (choice) {
        case 1:
            show_all_tasks(tasks, task_count);
            break;

        case 2:
            add_task(&tasks, &task_count, &task_capacity);
            break;

        case 3:
            search_by_status(tasks, task_count);
            break;

        case 4:
            search_by_assignee_and_time(tasks, task_count);
            break;

        case 5:
            sort_tasks(tasks, task_count);
            break;

        case 6:
            save_to_file(tasks, task_count);
            break;

        case 7:
            tasks = load_from_file(tasks, &task_count, &task_capacity);
            if (tasks == NULL) {
                tasks = init_tasks_array(&task_count, &task_capacity, 10);
            }
            break;

        case 9:
        {
            int n;
            printf("Сколько задач сгенерировать? ");
            scanf("%d", &n);
            getchar();

            tasks = generate_test_data(tasks, &task_count, &task_capacity, n);
            if (tasks != NULL) {
                printf("Успешно сгенерировано %d задач\n", task_count);
            }
            else {
                printf("Ошибка генерации!\n");
            }
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

    free_tasks_array(tasks, &task_count, &task_capacity);

    return 0;
}

Task* generate_test_data(Task* old_tasks, int* task_count, int* task_capacity, int n) {
    srand(time(NULL));
    const char* names[] = { "приложение", "отчет", "сайт", "базу", "тест", "код", "документ", "модуль" };
    const char* verbs[] = { "Разработать", "Создать", "Написать", "Протестировать", "Обновить", "Исправить" };
    const char* people_name[] = { "Иван", "Петр", "Анна", "Мария", "Алексей", "Елена" };
    const char* people_surname[] = { "Галич", "Короленко", "Шастун", "Евтушенко", "Долгих", "Ткач" };

    if (old_tasks != NULL) {
        free(old_tasks);
    }

    Task* tasks = init_tasks_array(task_count, task_capacity, n);
    if (tasks == NULL) {
        return NULL;
    }

    int generated = 0;
    for (int i = 0; i < n; i++) {
        Task task;

        sprintf(task.name, "%s %s",
            verbs[rand() % 6],
            names[rand() % 8]);

        task.creation_date = time(NULL) - (rand() % 30 * 86400);

        sprintf(task.assignee, "%s %s",
            people_surname[rand() % 6],
            people_name[rand() % 6]);

        task.status = rand() % 4;
        task.priority = rand() % 4;
        task.estimated_hours = 1 + rand() % 10;
        task.actual_hours = 1 + rand() % 10;

        add_task_to_array(&tasks, task_count, task_capacity, task);
        generated++;
    }

    return tasks;
}

Task* init_tasks_array(int* task_count, int* task_capacity, int initial_size) {
    Task* tasks = (Task*)malloc(initial_size * sizeof(Task));
    if (tasks == NULL) {
        printf("Ошибка выделения памяти!\n");
        return NULL;
    }

    *task_count = 0;
    *task_capacity = initial_size;

    return tasks;
}

int free_tasks_array(Task* tasks, int* task_count, int* task_capacity) {
    if (tasks != NULL) {
        free(tasks);
        *task_count = 0;
        *task_capacity = 0;
        printf("Память освобождена\n");
        return 1;
    }
    return 0;
}


int add_task_to_array(Task** tasks, int* task_count, int* task_capacity, Task new_task) {

    if (*task_count >= *task_capacity) {
        int new_capacity = (*task_capacity) * 2;
        if (new_capacity == 0) new_capacity = 10;

        Task* new_tasks = (Task*)realloc(*tasks, new_capacity * sizeof(Task));
        if (new_tasks == NULL) {
            printf("Ошибка перевыделения памяти!\n");
            return *task_count;
        }

        *tasks = new_tasks;
        *task_capacity = new_capacity;
        printf("Массив увеличен до %d задач\n", new_capacity);
    }


    (*tasks)[*task_count] = new_task;
    (*task_count)++;

    return *task_count;
}


int add_task(Task** tasks, int* task_count, int* task_capacity) {
    printf("\n=== Добавление новой задачи ===\n");

    Task new_task;

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


    int result = add_task_to_array(tasks, task_count, task_capacity, new_task);

    if (result > 0) {
        printf("Задача успешно добавлена! Всего задач: %d\n\n", *task_count);
        return 1;
    }
    return 0;
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

    char filename[100];
    printf("Введите имя файла: ");
    scanf("%s", filename);
    getchar();

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка создания файла!\n");
        return 0;
    }

    int saved = 0;
    for (int i = 0; i < task_count; i++) {
        struct tm* timeinfo = localtime(&tasks[i].creation_date);

        // Статус в текст
        char status_str[20];
        switch (tasks[i].status) {
        case NEW: strcpy(status_str, "NEW"); break;
        case IN_PROGRESS: strcpy(status_str, "IN_PROGRESS"); break;
        case COMPLETED: strcpy(status_str, "COMPLETED"); break;
        case CANCELLED: strcpy(status_str, "CANCELLED"); break;
        }

        // Приоритет в текст
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

        saved++;
    }

    fclose(file);
    printf("Сохранено %d задач в файл '%s'\n", saved, filename);
    return saved;
}


Task* load_from_file(Task* old_tasks, int* task_count, int* task_capacity) {
    printf("\n=== Загрузка из файла ===\n");

    char filename[100];
    printf("Введите имя файла: ");
    scanf("%s", filename);
    getchar();

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка открытия файла!\n");
        return old_tasks;
    }

    if (old_tasks != NULL) {
        free(old_tasks);
    }

    Task* tasks = init_tasks_array(task_count, task_capacity, 10);
    if (tasks == NULL) {
        fclose(file);
        return NULL;
    }

    char line[256];
    int loaded = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        Task task;
        struct tm timeinfo = { 0 };
        char status_str[20], priority_str[20];

        if (sscanf(line, "%[^|]|%d.%d.%d|%[^|]|%[^|]|%[^|]|%d|%d",
            task.name,
            &timeinfo.tm_mday,
            &timeinfo.tm_mon,
            &timeinfo.tm_year,
            task.assignee,
            status_str,
            priority_str,
            &task.estimated_hours,
            &task.actual_hours) == 9) {

            timeinfo.tm_mon -= 1;
            timeinfo.tm_year -= 1900;
            task.creation_date = mktime(&timeinfo);

            // Статус
            if (strcmp(status_str, "NEW") == 0) task.status = NEW;
            else if (strcmp(status_str, "IN_PROGRESS") == 0) task.status = IN_PROGRESS;
            else if (strcmp(status_str, "COMPLETED") == 0) task.status = COMPLETED;
            else if (strcmp(status_str, "CANCELLED") == 0) task.status = CANCELLED;

            // Приоритет
            if (strcmp(priority_str, "LOW") == 0) task.priority = LOW;
            else if (strcmp(priority_str, "MEDIUM") == 0) task.priority = MEDIUM;
            else if (strcmp(priority_str, "HIGH") == 0) task.priority = HIGH;
            else if (strcmp(priority_str, "CRITICAL") == 0) task.priority = CRITICAL;

            add_task_to_array(&tasks, task_count, task_capacity, task);
            loaded++;
        }
    }

    fclose(file);

    if (loaded == 0) {
        free(tasks);
        printf("Файл пуст или имеет неверный формат\n");
        return NULL;
    }

    printf("Загружено %d задач из файла '%s'\n", loaded, filename);
    return tasks;
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

    Status search_status = (Status)status_choice;
    int found_count = 0;

    printf("\nЗадачи со статусом '");
    switch (search_status) {
    case NEW: printf("Новая"); break;
    case IN_PROGRESS: printf("В работе"); break;
    case COMPLETED: printf("Завершена"); break;
    case CANCELLED: printf("Отменена"); break;
    }
    printf("':\n");

    for (int i = 0; i < task_count; i++) {
        if (tasks[i].status == search_status) {
            found_count++;
            printf("\n--- Задача #%d ---\n", found_count);
            printf("Название: %s\n", tasks[i].name);

            struct tm* timeinfo = localtime(&tasks[i].creation_date);
            printf("Дата: %02d.%02d.%04d\n",
                timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);

            printf("Исполнитель: %s\n", tasks[i].assignee);
            printf("Оценка времени: %d часов\n", tasks[i].estimated_hours);
            printf("Фактическое время: %d часов\n", tasks[i].actual_hours);
        }
    }

    if (found_count == 0) {
        printf("Задачи с указанным статусом не найдены.\n");
    }
    else {
        printf("\nНайдено задач: %d\n", found_count);
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
    printf("Введите максимальное время выполнения (часов): ");
    scanf("%d", &max_hours);
    getchar();

    int found_count = 0;

    printf("\nЗадачи исполнителя '%s' со сроком до %d часов:\n", assignee, max_hours);

    for (int i = 0; i < task_count; i++) {
        if (strcmp(tasks[i].assignee, assignee) == 0 &&
            tasks[i].estimated_hours <= max_hours) {

            found_count++;
            printf("\n--- Задача #%d ---\n", found_count);
            printf("Название: %s\n", tasks[i].name);

            struct tm* timeinfo = localtime(&tasks[i].creation_date);
            printf("Дата: %02d.%02d.%04d\n",
                timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);

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
    }

    if (found_count == 0) {
        printf("Задачи не найдены.\n");
    }
    else {
        printf("\nНайдено задач: %d\n", found_count);
    }

    return found_count;
}


int sort_tasks(Task* tasks, int task_count) {
    printf("\n=== Многоуровневая сортировка ===\n");

    if (task_count < 2) {
        printf("Сортировка не требуется\n");
        return 0;
    }

    printf("Сортируем по: время → приоритет → статус\n");

    // пузырьковая сортировка
    for (int i = 0; i < task_count - 1; i++) {
        for (int j = 0; j < task_count - i - 1; j++) {

            if (tasks[j].estimated_hours > tasks[j + 1].estimated_hours) {
                Task temp = tasks[j];
                tasks[j] = tasks[j + 1];
                tasks[j + 1] = temp;
            }

            else if (tasks[j].estimated_hours == tasks[j + 1].estimated_hours) {
                if (tasks[j].priority < tasks[j + 1].priority) {
                    Task temp = tasks[j];
                    tasks[j] = tasks[j + 1];
                    tasks[j + 1] = temp;
                }
                else if (tasks[j].priority == tasks[j + 1].priority) {
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

