#pragma once
#include "core/IFileChecker.h"
#include <string>
#include <unordered_map>

/**
 * @file RestorationChecker.h
 * @brief Чекер восстановления файла после удаления
 */

/// \brief Отслеживает цикл удаления и повторного появления файла.
///
/// Машина состояний с тремя состояниями:
/// - UNKNOWN  : начальное состояние, снимок ещё не сделан
/// - PRESENT  : файл существует
/// - ABSENT   : файл был удалён, ждём восстановления
///
/// События:
/// - PRESENT → ABSENT  : "deleted — watching for restoration"
/// - ABSENT  → PRESENT : "restored"
///
/// Отличие от ExistenceChecker: не реагирует на первичное появление (UNKNOWN → PRESENT),
/// фиксирует только повторное появление после подтверждённого удаления.
class RestorationChecker : public IFileChecker {
public:
    /// \brief Проверяет переход PRESENT↔ABSENT.
    /// \param path Путь к файлу.
    /// \return true при удалении или восстановлении.
    bool hasChanged(const std::string& path) override;

    /// \brief Возвращает последнее событие удаления или восстановления.
    /// \param path Путь к файлу.
    FileEvent getEvent(const std::string& path) const override;

private:
    /// \brief Состояния машины состояний
    enum class State { UNKNOWN, PRESENT, ABSENT };

    std::unordered_map<std::string, State>              state_;     ///< Текущее состояние файла
    mutable std::unordered_map<std::string, std::string> lastEvent_; ///< Кэш описания события
};
