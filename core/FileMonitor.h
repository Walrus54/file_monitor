#pragma once
#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include "core/IFileChecker.h"
#include "core/Signal.h"
#include "utils/Config.h"

/**
 * @file FileMonitor.h
 * @brief Оркестратор мониторинга файлов
 */

/// \brief Оркестратор мониторинга файлов (SRP, DIP).
///
/// Запускает фоновый поток, который с заданным интервалом обходит список
/// файлов и прогоняет их через чекеры. При обнаружении изменения испускает
/// сигнал fileChanged — все подключённые слоты вызываются автоматически.
///
/// \code
/// FileMonitor monitor(checkers, config);
/// monitor.fileChanged.connect([](const FileEvent& e) { /* слот */ });
/// monitor.start();
/// \endcode
class FileMonitor {
public:
    /// \brief Сигнал, испускаемый при каждом обнаруженном изменении файла.
    /// Слоты подключаются через fileChanged.connect(...) до вызова start().
    Signal<FileEvent> fileChanged;

    /// \brief Конструктор.
    /// \param checkers Набор стратегий проверки (ownership через shared_ptr).
    /// \param config   Параметры мониторинга.
    FileMonitor(std::vector<std::shared_ptr<IFileChecker>> checkers,
                Config                                     config = {});

    /// \brief Деструктор. Если мониторинг активен — останавливает его.
    ~FileMonitor();

    FileMonitor(const FileMonitor&)            = delete;
    FileMonitor& operator=(const FileMonitor&) = delete;

    /// \brief Добавляет файл в список наблюдаемых.
    /// \return true если добавлен, false если уже отслеживается.
    bool addFile(const std::string& path);

    /// \brief Удаляет файл из списка наблюдаемых.
    /// \return true если удалён, false если не был в списке.
    bool removeFile(const std::string& path);

    /// \brief Запускает фоновый поток мониторинга.
    void start();

    /// \brief Останавливает фоновый поток и дожидается его завершения.
    void stop();

    /// \brief Выводит в лог список наблюдаемых файлов.
    void listFiles() const;

private:
    Config                                     config_;   ///< Параметры мониторинга
    std::vector<std::string>                   files_;    ///< Наблюдаемые пути
    std::vector<std::shared_ptr<IFileChecker>> checkers_; ///< Стратегии проверки
    std::thread                                worker_;   ///< Фоновый поток
    std::atomic<bool>                          running_;  ///< Флаг активности цикла
    mutable std::mutex                         mutex_;    ///< Защита files_

    void run();
    void checkFiles();
};
