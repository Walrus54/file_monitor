#pragma once
#include "core/IFileChecker.h"
#include <unordered_map>

/**
 * @file ExistenceChecker.h
 * @brief Чекер появления и удаления файла
 */

/// \brief Отслеживает создание и удаление файла (реализация IFileChecker).
///
/// Хранит предыдущее состояние (exists / not-exists) для каждого пути.
/// Первый вызов hasChanged() записывает базовый снимок, событие не генерируется.
class ExistenceChecker : public IFileChecker {
public:
    /// \brief Проверяет, изменился ли факт существования файла.
    /// \param path Путь к файлу.
    /// \return true при переходе exists → not-exists или обратно.
    bool hasChanged(const std::string& path) override;

    /// \brief Возвращает событие "created" или "deleted".
    /// \param path Путь к файлу.
    FileEvent getEvent(const std::string& path) const override;

private:
    /// Предыдущее состояние: true = файл существовал на прошлой проверке
    std::unordered_map<std::string, bool> prevState_;
};
