#include "checkers/RestorationChecker.h"
#include <filesystem>

namespace fs = std::filesystem;

bool RestorationChecker::hasChanged(const std::string& path) {
    bool exists = fs::exists(path);
    auto it = state_.find(path);

    if (it == state_.end()) {
        // Первый вызов: инициализируем машину состояний без генерации события
        state_[path] = exists ? State::PRESENT : State::ABSENT;
        return false;
    }

    State prev = it->second;
    State cur  = exists ? State::PRESENT : State::ABSENT;

    if (prev == State::PRESENT && cur == State::ABSENT) {
        // Файл был — теперь его нет: переходим в режим ожидания восстановления
        it->second     = State::ABSENT;
        lastEvent_[path] = "deleted — watching for restoration";
        return true;
    }
    if (prev == State::ABSENT && cur == State::PRESENT) {
        // Файл отсутствовал и снова появился: восстановление подтверждено
        it->second     = State::PRESENT;
        lastEvent_[path] = "restored";
        return true;
    }

    return false; // состояние не изменилось
}

FileEvent RestorationChecker::getEvent(const std::string& path) const {
    auto it = lastEvent_.find(path);
    return {path, it != lastEvent_.end() ? it->second : "no event"};
}
