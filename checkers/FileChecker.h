#pragma once
#include "core/IFileChecker.h"
#include <string>
#include <unordered_map>

/**
 * @file FileChecker.h
 * @brief Универсальный чекер состояния файла в трёх режимах
 */

/// \brief Универсальный чекер состояния файла (реализация IFileChecker).
///
/// Один класс — три режима работы, задаются при конструировании:
///
/// | Режим       | Что отслеживает                              | Событие                           |
/// |-------------|----------------------------------------------|-----------------------------------|
/// | Existence   | факт существования файла                     | "created" / "deleted"             |
/// | Size        | изменение размера существующего файла        | "size changed to N bytes"         |
/// | Restoration | удаление и повторное появление файла         | "deleted — watching..." / "restored" |
///
/// Использование:
/// \code
/// auto checker = std::make_shared<FileChecker>(FileChecker::Mode::Existence);
/// \endcode
class FileChecker : public IFileChecker {
public:
    /// \brief Режимы работы чекера
    enum class Mode {
        Existence,   ///< Отслеживание создания и удаления файла
        Size,        ///< Отслеживание изменения размера файла
        Restoration  ///< Отслеживание удаления и последующего восстановления
    };

    /// \brief Конструктор.
    /// \param mode Режим работы чекера.
    explicit FileChecker(Mode mode);

    /// \brief Проверяет, изменилось ли свойство файла, соответствующее режиму.
    ///
    /// Первый вызов всегда возвращает false (базовый снимок состояния).
    /// \param path Путь к файлу.
    /// \return true если изменение обнаружено.
    bool hasChanged(const std::string& path) override;

    /// \brief Возвращает описание последнего обнаруженного события.
    /// \param path Путь к файлу.
    FileEvent getEvent(const std::string& path) const override;

private:
    /// \brief Внутренние состояния для режима Restoration
    enum class RestorationState { UNKNOWN, PRESENT, ABSENT };

    Mode mode_; ///< Режим, выбранный при конструировании

    // Состояния для каждого режима (заполняются по необходимости)
    std::unordered_map<std::string, bool>             prevExists_;  ///< Existence: предыдущее наличие
    std::unordered_map<std::string, uintmax_t>        prevSize_;    ///< Size: предыдущий размер
    std::unordered_map<std::string, RestorationState> restState_;   ///< Restoration: состояние МСА
    mutable std::unordered_map<std::string, std::string> lastEvent_; ///< Кэш последнего события

    // Внутренние методы проверки для каждого режима
    bool checkExistence(const std::string& path);
    bool checkSize(const std::string& path);
    bool checkRestoration(const std::string& path);
};
