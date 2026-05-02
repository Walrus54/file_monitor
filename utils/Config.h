#pragma once
#include <mutex>
#include <string>
#include <unordered_map>

/**
 * @file Config.h
 * @brief Singleton-хранилище параметров приложения
 */

/// \brief Хранилище параметров приложения (паттерн Singleton, реализация Meyers).
///
/// Поддерживает загрузку из файла формата key=value.
/// Значения по умолчанию: poll_interval=1, log_level=INFO.
///
/// Интервал опроса перечитывается на каждой итерации цикла мониторинга,
/// что позволяет менять его без перезапуска приложения.
class Config final {
public:
    /// \brief Возвращает единственный экземпляр конфигурации.
    static Config& getInstance();

    Config(const Config&)            = delete;
    Config& operator=(const Config&) = delete;

    /// \brief Устанавливает значение параметра.
    /// \param key   Ключ.
    /// \param value Значение.
    void set(const std::string& key, const std::string& value);

    /// \brief Возвращает строковое значение параметра.
    /// \param key        Ключ.
    /// \param defaultVal Значение, возвращаемое при отсутствии ключа.
    std::string get(const std::string& key, const std::string& defaultVal = "") const;

    /// \brief Возвращает целочисленное значение параметра.
    /// \param key        Ключ.
    /// \param defaultVal Значение при отсутствии ключа или ошибке парсинга.
    int getInt(const std::string& key, int defaultVal = 0) const;

    /// \brief Загружает параметры из файла формата key=value.
    /// Строки, начинающиеся с '#', считаются комментариями и пропускаются.
    /// \param path Путь к конфигурационному файлу.
    void loadFromFile(const std::string& path);

private:
    Config();

    std::unordered_map<std::string, std::string> data_;  ///< Хранилище параметров
    mutable std::mutex                           mutex_; ///< Защита data_ от гонки
};
