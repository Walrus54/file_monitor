#pragma once
#include "core/IFileChecker.h"

/**
 * @file INotifier.h
 * @brief Интерфейс получателя событий об изменениях файлов
 */

/// \brief Интерфейс получателя событий (ISP).
///
/// Отделяет способ доставки уведомлений от логики мониторинга.
/// Реализации могут выводить в консоль, писать в БД, слать в сеть и т.д.
class INotifier {
public:
    virtual ~INotifier() = default;

    /// \brief Обрабатывает событие изменения файла.
    /// \param event Событие, содержащее путь и описание изменения.
    virtual void notify(const FileEvent& event) = 0;
};
