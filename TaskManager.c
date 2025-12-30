/*
* Автор: Маркина Е.А., студентка группы бТИИ-251
* 
 * Файл содержит реализацию консольного приложения
 * "Система управления задачами".
 * Программа позволяет создавать, хранить, просматривать,
 * сортировать, искать задачи, а также сохранять и загружать
 * их из текстовых файлов.
 */

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

/*
 * Функция main является точкой входа в программу.
 * Управляет главным меню, вызовом операций и памятью.
 * Входные параметры: отсутствуют.
 * Возвращаемое значение: 0 - успешное завершение, 1 - ошибка выделения памяти.
 */
int main() {
    system("chcp 1251");

    Task* tasks = NULL;
    int task_count = 0;
    int task_capacity = 0;
    int result;

    task_capacity = 10;
    tasks = malloc(task_capacity * sizeof(Task));
    if (tasks == NULL) {
        printf("Ошибка выделения памяти!\n");
        return 1;
    }

    int choice;

    do {
        choice = show_menu();

        switch (choice) {
        case 1: /* Показать все задачи */
            result = show_all_tasks(tasks, task_count);
            printf("Отображено задач: %d\n", result);
            break;

        case 2: /* Добавить задачу */
            if (task_count >= task_capacity) {
                task_capacity = task_capacity * 2;
                Task* new_tasks = realloc(tasks, task_capacity * sizeof(Task));
                if (new_tasks == NULL) {
                    printf("Ошибка выделения памяти!\n");
                    task_capacity = task_capacity / 2;
                    break;
                }
                tasks = new_tasks;
            }

            result = add_task(tasks, task_count);
            if (result == 1) {
                task_count++;
                printf("\nЗадача успешно создана! Всего задач: %d\n", task_count);
            }
            else {
                printf("Ошибка при добавлении задачи!\n");
            }
            break;

        case 3: /* Поиск по статусу */
            result = search_by_status(tasks, task_count);
            printf("\nНайдено задач по статусу: %d\n", result);
            break;

        case 4: /* Поиск по исполнителю и времени */
            result = search_by_assignee_and_time(tasks, task_count);
            printf("\nНайдено задач по исполнителю и времени: %d\n", result);
            break;

        case 5: /* Сортировка задач */
            result = sort_tasks(tasks, task_count);
            if (result == 1) {
                printf("Сортировка выполнена успешно.\n");
            }
            else {
                printf("Сортировка не требуется\n");
            }
            break;

        case 6: /* Сохранение в файл */
            result = save_to_file(tasks, task_count);
            printf("Сохранено задач в файл: %d\n", result);
            break;

        case 7: /* Загрузка из файла */
            result = load_from_file(tasks, task_count, task_capacity);
            if (result > task_count) {
                /* Если загружены новые задачи, обновляем счётчик */
                printf("Загружено %d задач. Всего теперь: %d\n", result - task_count, result);
                task_count = result;
            }
            else {
                printf("Загружено задач: %d\n", result - task_count);
            }
            break;

        case 0: /* Выход из программы */
            printf("Выход из программы...\n");
            break;

        default: /* Обработка некорректного ввода */
            printf("Неверный выбор!\n");
        }

        if (choice != 0) {
            printf("\nНажмите Enter для продолжения...");
            getchar();
        }

    } while (choice != 0);

    if (tasks != NULL) {
        free(tasks);
    }

    return 0;
}

/*
 * Функция add_task добавляет новую задачу в массив.
 * Входные параметры:
 *   tasks - указатель на массив структур Task.
 *   task_count - текущее количество задач в массиве (индекс для новой задачи).
 * Возвращаемое значение:
 *   1 - задача успешно добавлена.
 *   0 - ошибка ввода данных или недопустимый индекс.
 */
int add_task(Task* tasks, int task_count) {
    if (task_count < 0) {
        return 0;
    }

    Task new_task;

    printf("\n=== Добавление новой задачи ===\n");

    printf("Введите название задачи: ");
    fgets(new_task.name, sizeof(new_task.name), stdin);
    /* Удаление символа новой строки '\n', который fgets добавляет в буфер */
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

    return 1;
}

/*
 * Функция show_all_tasks выводит на экран информацию о всех задачах.
 * Входные параметры:
 *   tasks - указатель на массив структур Task.
 *   task_count - количество задач для отображения.
 * Возвращаемое значение:
 *   task_count - количество отображённых задач.
 *   0 - если массив пуст.
 */
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
        /* Вывод даты в формате ДД.ММ.ГГГГ.
           tm_mon отсчитывается от 0, поэтому +1.
           tm_year - количество лет, прошедших с 1900 года, поэтому +1900. */
        printf("Дата: %02d.%02d.%04d\n",
            timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);

        printf("Исполнитель: %s\n", tasks[i].assignee);

        printf("Статус: ");
        /* Преобразование числового кода статуса в текстовую строку */
        switch (tasks[i].status) {
        case NEW: printf("Новая"); break;
        case IN_PROGRESS: printf("В работе"); break;
        case COMPLETED: printf("Завершена"); break;
        case CANCELLED: printf("Отменена"); break;
        }

        printf("\nПриоритет: ");
        /* Преобразование числового кода приоритета в текстовую строку */
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

/*
 * Функция save_to_file сохраняет массив задач в текстовый файл.
 * Формат строки в файле: Название|ДД.ММ.ГГГГ|Исполнитель|СТАТУС|ПРИОРИТЕТ|Оценка|Факт_время
 * Входные параметры:
 *   tasks - указатель на массив структур Task.
 *   task_count - количество задач для сохранения.
 * Возвращаемое значение:
 *   task_count - количество сохранённых задач.
 *   0 - ошибка открытия файла или пустой массив.
 */
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
        /* Конвертация статуса из перечисления в строку для хранения в файле */
        switch (tasks[i].status) {
        case NEW: strcpy(status_str, "NEW"); break;
        case IN_PROGRESS: strcpy(status_str, "IN_PROGRESS"); break;
        case COMPLETED: strcpy(status_str, "COMPLETED"); break;
        case CANCELLED: strcpy(status_str, "CANCELLED"); break;
        }

        char priority_str[20];
        /* Конвертация приоритета из перечисления в строку для хранения в файле */
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
    return task_count;
}

/*
 * Функция load_from_file загружает задачи из текстового файла в массив.
 * Входные параметры:
 *   tasks - указатель на массив структур Task.
 *   task_count - текущее количество задач в массиве.
 *   task_capacity - максимальная ёмкость массива (сколько задач можно добавить).
 * Возвращаемое значение:
 *   task_count + loaded - новое общее количество задач после загрузки.
 *   task_count - в случае ошибки открытия файла или если файл пуст/некорректен.
 */
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

            /* Коррекция загруженных значений даты для соответствия структуре tm */
            timeinfo.tm_mon -= 1;
            timeinfo.tm_year -= 1900;
            tasks[index].creation_date = mktime(&timeinfo);

            /* Обратная конвертация строкового статуса в значение перечисления */
            if (strcmp(status_str, "NEW") == 0) tasks[index].status = NEW;
            else if (strcmp(status_str, "IN_PROGRESS") == 0) tasks[index].status = IN_PROGRESS;
            else if (strcmp(status_str, "COMPLETED") == 0) tasks[index].status = COMPLETED;
            else if (strcmp(status_str, "CANCELLED") == 0) tasks[index].status = CANCELLED;

            /* Обратная конвертация строкового приоритета в значение перечисления */
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
        return task_count + loaded;  // Возвращаем общее количество
    }
    else {
        printf("Файл пуст или имеет неверный формат\n");
        return task_count;  // Возвращаем исходное количество
    }
}

/*
 * Функция search_by_status находит и выводит задачи с заданным статусом.
 * Входные параметры:
 *   tasks - указатель на массив структур Task.
 *   task_count - количество задач в массиве.
 * Возвращаемое значение:
 *   found_count - количество найденных задач.
 *   0 - если задачи не найдены или введён неверный статус.
 */
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

    /* Валидация ввода: номер статуса должен быть в диапазоне перечисления */
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

/*
 * Функция search_by_assignee_and_time находит задачи по исполнителю
 * и максимальной оценке времени (оценка <= max_hours).
 * Входные параметры:
 *   tasks - указатель на массив структур Task.
 *   task_count - количество задач в массиве.
 * Возвращаемое значение:
 *   found_count - количество найденных задач.
 */
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

/*
 * Функция sort_tasks сортирует массив задач по трём критериям (многоуровневая сортировка):
 * 1. estimated_hours (оценка времени) - по возрастанию.
 * 2. Если оценки равны: priority (приоритет) - по убыванию (CRITICAL > HIGH > ...).
 * 3. Если и приоритеты равны: status (статус) - по убыванию (CANCELLED > ... > NEW).
 * Алгоритм: пузырьковая сортировка (Bubble Sort).
 * Входные параметры:
 *   tasks - указатель на массив структур Task.
 *   task_count - количество задач в массиве.
 * Возвращаемое значение:
 *   1 - массив был отсортирован.
 *   0 - сортировка не требовалась (задач меньше двух).
 */
int sort_tasks(Task* tasks, int task_count) {
    printf("\n=== Сортировка задач ===\n");

    if (task_count < 2) {
        return 0;
    }

    /* Пузырьковая сортировка */
    for (int i = 0; i < task_count - 1; i++) {
        for (int j = 0; j < task_count - i - 1; j++) {

            /* Уровень 1: Сравнение по полю estimated_hours (по возрастанию) */
            if (tasks[j].estimated_hours > tasks[j + 1].estimated_hours) {
                Task temp = tasks[j];
                tasks[j] = tasks[j + 1];
                tasks[j + 1] = temp;
            }
            /* Если оценки времени равны, переходим к сравнению приоритетов */
            else if (tasks[j].estimated_hours == tasks[j + 1].estimated_hours) {
                /* Уровень 2: Сравнение по полю priority (по убыванию).
                   В перечислении меньшему числу соответствует более высокий приоритет. */
                if (tasks[j].priority < tasks[j + 1].priority) {
                    Task temp = tasks[j];
                    tasks[j] = tasks[j + 1];
                    tasks[j + 1] = temp;
                }
                /* Если и приоритеты равны, переходим к сравнению статусов */
                else if (tasks[j].priority == tasks[j + 1].priority) {
                    /* Уровень 3: Сравнение по полю status (по убыванию).
                       В перечислении большему числу соответствует более "завершённый" статус. */
                    if (tasks[j].status > tasks[j + 1].status) {
                        Task temp = tasks[j];
                        tasks[j] = tasks[j + 1];
                        tasks[j + 1] = temp;
                    }
                }
            }
        }
    }

    return 1;
}

/*
 * Функция show_menu отображает главное текстовое меню программы.
 * Входные параметры: отсутствуют.
 * Возвращаемое значение:
 *   choice - целое число, соответствующее выбранному пользователем пункту меню.
 */
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