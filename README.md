# File Monitor

Консольная утилита для мониторинга изменений файлов (существование, размер).

## Архитектура

Проект следует принципам **SOLID** и использует паттерн **Singleton**:

| Принцип       | Реализация                                                                                                                              |
| ------------- | --------------------------------------------------------------------------------------------------------------------------------------- |
| **SRP**       | `FileMonitor` — оркестрация; `Logger` — журналирование; `Config` — конфигурация; `ExistenceChecker`/`SizeChecker` — конкретные проверки |
| **OCP**       | Новый тип проверки добавляется без изменения `FileMonitor` — достаточно реализовать `IFileChecker`                                      |
| **LSP**       | `ExistenceChecker` и `SizeChecker` взаимозаменяемы везде, где ожидается `IFileChecker*`                                                 |
| **ISP**       | `IFileChecker` (2 метода) и `INotifier` (1 метод) — минимальные целевые интерфейсы                                                      |
| **DIP**       | `FileMonitor` зависит только от абстракций; конкретные типы создаются в `main.cpp`                                                      |
| **Singleton** | `Logger` и `Config` — потокобезопасные синглтоны (Meyers singleton, C++11)                                                              |

### Диаграмма классов

```mermaid
classDiagram
    class IFileChecker {
        <<interface>>
        +hasChanged(path: string) bool
        +getEvent(path: string) FileEvent
    }

    class INotifier {
        <<interface>>
        +notify(event: FileEvent) void
    }

    class FileEvent {
        +path: string
        +description: string
    }

    class FileMonitor {
        -files_: vector~string~
        -checkers_: vector~IFileChecker~
        -notifier_: INotifier
        -worker_: thread
        -running_: atomic~bool~
        -mutex_: mutex
        +addFile(path: string) bool
        +removeFile(path: string) bool
        +start() void
        +stop() void
        +listFiles() void
        -run() void
        -checkFiles() void
    }

    class ExistenceChecker {
        -prevState_: unordered_map
        +hasChanged(path: string) bool
        +getEvent(path: string) FileEvent
    }

    class SizeChecker {
        -prevSize_: unordered_map
        +hasChanged(path: string) bool
        +getEvent(path: string) FileEvent
    }

    class ConsoleNotifier {
        +notify(event: FileEvent) void
    }

    class Logger {
        <<singleton>>
        -file_: ofstream
        -mutex_: mutex
        +getInstance()$ Logger
        +log(level: LogLevel, msg: string) void
        +setOutputFile(path: string) void
    }

    class Config {
        <<singleton>>
        -data_: unordered_map
        -mutex_: mutex
        +getInstance()$ Config
        +get(key: string) string
        +getInt(key: string) int
        +loadFromFile(path: string) void
    }

    IFileChecker <|.. ExistenceChecker
    IFileChecker <|.. SizeChecker
    INotifier <|.. ConsoleNotifier
    FileMonitor o-- IFileChecker
    FileMonitor o-- INotifier
    ConsoleNotifier ..> Logger
    FileMonitor ..> Logger
    FileMonitor ..> Config
    IFileChecker ..> FileEvent
```

### Диаграмма последовательности (цикл мониторинга)

```mermaid
sequenceDiagram
    participant main
    participant FileMonitor
    participant ExistenceChecker
    participant SizeChecker
    participant ConsoleNotifier
    participant Logger

    main->>FileMonitor: start()
    activate FileMonitor
    FileMonitor-->>FileMonitor: spawn worker thread

    loop каждые poll_interval секунд
        FileMonitor->>ExistenceChecker: hasChanged(path)
        ExistenceChecker-->>FileMonitor: true / false
        alt changed
            FileMonitor->>ExistenceChecker: getEvent(path)
            ExistenceChecker-->>FileMonitor: FileEvent
            FileMonitor->>ConsoleNotifier: notify(event)
            ConsoleNotifier->>Logger: log(INFO, message)
            Logger-->>ConsoleNotifier: ok
        end

        FileMonitor->>SizeChecker: hasChanged(path)
        SizeChecker-->>FileMonitor: true / false
        alt changed
            FileMonitor->>SizeChecker: getEvent(path)
            SizeChecker-->>FileMonitor: FileEvent
            FileMonitor->>ConsoleNotifier: notify(event)
            ConsoleNotifier->>Logger: log(INFO, message)
            Logger-->>ConsoleNotifier: ok
        end
    end

    main->>FileMonitor: stop()
    FileMonitor-->>FileMonitor: running_ = false, join()
    deactivate FileMonitor
```

### Структура проекта

```
include/
  IFileChecker.h          — интерфейс проверки изменений
  INotifier.h             — интерфейс уведомлений
  FileMonitor.h           — оркестратор мониторинга
  ConsoleNotifier.h       — вывод событий в stdout
  Logger.h                — Singleton логгер (thread-safe)
  Config.h                — Singleton конфигурация
  checkers/
    ExistenceChecker.h    — проверка существования файла
    SizeChecker.h         — проверка изменения размера
src/                      — реализации
tests/                    — GoogleTest unit-тесты
```

## Сборка

```bash
cmake -B build
cmake --build build
```

## Тесты

```bash
cd build && ctest --output-on-failure
```

## Использование

```bash
./build/file_monitor [config.ini]
```

| Команда         | Описание                            |
| --------------- | ----------------------------------- |
| `add <path>`    | Добавить файл в мониторинг          |
| `remove <path>` | Удалить файл из мониторинга         |
| `start`         | Начать мониторинг (отдельный поток) |
| `stop`          | Остановить мониторинг               |
| `list`          | Список наблюдаемых файлов           |
| `quit`          | Выход                               |

### Пример config.ini

```ini
poll_interval=2
log_level=INFO
```
