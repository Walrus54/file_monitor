#pragma once
#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include "core/IFileChecker.h"
#include "core/INotifier.h"

/**
 * @file FileMonitor.h
 * @brief Оркестратор мониторинга файлов
 */

/// \brief Оркестратор мониторинга файлов (SRP, DIP).
///
/// Запускает фоновый поток, который с заданным интервалом обходит список файлов
/// и передаёт каждый путь всем зарегистрированным чекерам.
/// При обнаружении изменения вызывает INotifier::notify().
///
/// Принцип DIP: класс зависит только от абстракций IFileChecker и INotifier;
/// конкретные типы создаются снаружи и передаются через конструктор.
class FileMonitor {
public:
    /// \brief Конструктор.
    /// \param checkers Набор стратегий проверки (ownership через shared_ptr).
    /// \param notifier Получатель событий.
    FileMonitor(std::vector<std::shared_ptr<IFileChecker>> checkers,
                std::shared_ptr<INotifier> notifier);

    /// \brief Деструктор. Если мониторинг активен — останавливает его.
    ~FileMonitor();

    // Запрет копирования: класс владеет потоком
    FileMonitor(const FileMonitor&)            = delete;
    FileMonitor& operator=(const FileMonitor&) = delete;

    /// \brief Добавляет файл в список наблюдаемых.
    /// \param path Путь к файлу (дубликаты игнорируются).
    /// \return true если файл добавлен, false если уже отслеживается.
    bool addFile(const std::string& path);

    /// \brief Удаляет файл из списка наблюдаемых.
    /// \param path Путь к файлу.
    /// \return true если файл удалён, false если не был в списке.
    bool removeFile(const std::string& path);

    /// \brief Запускает фоновый поток мониторинга.
    /// Повторный вызов без stop() игнорируется.
    void start();

    /// \brief Останавливает фоновый поток и дожидается его завершения.
    void stop();

    /// \brief Выводит в лог список наблюдаемых файлов.
    void listFiles() const;

private:
    std::vector<std::string>                   files_;    ///< Список наблюдаемых путей
    std::vector<std::shared_ptr<IFileChecker>> checkers_; ///< Стратегии проверки
    std::shared_ptr<INotifier>                 notifier_; ///< Получатель событий
    std::thread                                worker_;   ///< Фоновый поток
    std::atomic<bool>                          running_;  ///< Флаг активности цикла
    mutable std::mutex                         mutex_;    ///< Защита files_

    /// \brief Основной цикл фонового потока.
    void run();

    /// \brief Единичный обход всех файлов через все чекеры.
    void checkFiles();
};
