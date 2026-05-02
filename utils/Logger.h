#pragma once
#include <fstream>
#include <mutex>
#include <string>

/**
 * @file Logger.h
 * @brief Потокобезопасный Singleton-логгер
 */

/// \brief Уровни серьёзности лог-сообщений
enum class LogLevel {
    INFO,    ///< Информационное сообщение
    WARNING, ///< Предупреждение
    ERROR    ///< Ошибка
};

/// \brief Потокобезопасный логгер (паттерн Singleton, реализация Meyers).
///
/// getInstance() использует static local variable: инициализация происходит
/// ровно один раз, даже при конкурентных вызовах (стандарт C++11, §6.7).
/// Копирование и присваивание запрещены.
///
/// Вывод идёт в stdout; при необходимости — дополнительно в файл.
class Logger {
public:
    /// \brief Возвращает единственный экземпляр логгера.
    static Logger& getInstance();

    Logger(const Logger&)            = delete;
    Logger& operator=(const Logger&) = delete;

    /// \brief Записывает сообщение с временной меткой и уровнем.
    /// \param level  Уровень серьёзности.
    /// \param message Текст сообщения.
    void log(LogLevel level, const std::string& message);

    /// \brief Дополнительно перенаправляет вывод в файл (режим append).
    /// \param path Путь к лог-файлу.
    void setOutputFile(const std::string& path);

private:
    Logger();
    ~Logger();

    /// \brief Формирует строку временной метки в формате "YYYY-MM-DD HH:MM:SS".
    static std::string timestamp();

    /// \brief Преобразует уровень в строку "INFO" / "WARN" / "ERROR".
    static std::string levelToString(LogLevel level);

    std::ofstream file_;   ///< Файловый поток (опционально)
    bool          toFile_; ///< Флаг: писать ли в файл
    std::mutex    mutex_;  ///< Защита от гонки при параллельном логировании
};
