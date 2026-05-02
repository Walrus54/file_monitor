#pragma once
#include "core/INotifier.h"

/**
 * @file ConsoleNotifier.h
 * @brief Нотификатор, выводящий события через Logger
 */

/// \brief Нотификатор с выводом через Logger (реализация INotifier).
///
/// Делегирует вывод Logger::getInstance(), который обеспечивает
/// потокобезопасность и опциональную запись в файл.
class ConsoleNotifier : public INotifier {
public:
    /// \brief Логирует событие с уровнем INFO.
    /// \param event Событие изменения файла.
    void notify(const FileEvent& event) override;
};
