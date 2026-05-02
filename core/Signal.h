#pragma once
#include <functional>
#include <vector>

/**
 * @file Signal.h
 * @brief Лёгкий механизм сигнал-слот без внешних зависимостей
 */

/// \brief Сигнал — список слотов, вызываемых при emit().
///
/// Использование:
/// \code
/// Signal<FileEvent> fileChanged;
/// fileChanged.connect([](const FileEvent& e) { /* слот */ });
/// fileChanged.emit(event);
/// \endcode
///
/// \tparam Args Типы аргументов, передаваемых в слоты при emit().
template<typename... Args>
class Signal {
public:
    /// Тип слота — любой callable с подходящей сигнатурой
    using Slot = std::function<void(Args...)>;

    /// \brief Подключает слот к сигналу.
    /// \param slot Функция, метод или лямбда, вызываемые при emit().
    void connect(Slot slot) {
        slots_.push_back(std::move(slot));
    }

    /// \brief Испускает сигнал — вызывает все подключённые слоты.
    /// \param args Аргументы, передаваемые в каждый слот.
    void emit(Args... args) const {
        for (const auto& slot : slots_) {
            slot(args...);
        }
    }

private:
    std::vector<Slot> slots_; ///< Зарегистрированные слоты
};
