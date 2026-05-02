#pragma once
#include "core/IFileChecker.h"
#include <unordered_map>

/**
 * @file SizeChecker.h
 * @brief Чекер изменения размера файла
 */

/// \brief Отслеживает изменение размера файла (реализация IFileChecker).
///
/// Для несуществующих файлов hasChanged() всегда возвращает false,
/// чтобы не конкурировать с ExistenceChecker при удалении файла.
class SizeChecker : public IFileChecker {
public:
    /// \brief Проверяет, изменился ли размер файла.
    /// \param path Путь к файлу.
    /// \return true если размер отличается от предыдущей проверки.
    bool hasChanged(const std::string& path) override;

    /// \brief Возвращает событие "size changed to N bytes".
    /// \param path Путь к файлу.
    FileEvent getEvent(const std::string& path) const override;

private:
    /// Предыдущий размер файла в байтах
    std::unordered_map<std::string, uintmax_t> prevSize_;
};
