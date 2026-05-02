#include "checkers/FileChecker.h"
#include <filesystem>

namespace fs = std::filesystem;

FileChecker::FileChecker(Mode mode) : mode_(mode) {}

bool FileChecker::hasChanged(const std::string& path) {
    switch (mode_) {
        case Mode::Existence:   return checkExistence(path);
        case Mode::Size:        return checkSize(path);
        case Mode::Restoration: return checkRestoration(path);
    }
    return false;
}

FileEvent FileChecker::getEvent(const std::string& path) const {
    auto it = lastEvent_.find(path);
    return {path, it != lastEvent_.end() ? it->second : "no event"};
}

// --- Режим Existence ---

bool FileChecker::checkExistence(const std::string& path) {
    bool cur = fs::exists(path);
    auto it  = prevExists_.find(path);
    if (it == prevExists_.end()) {
        // Первый вызов: запоминаем текущее состояние, событие не генерируем
        prevExists_[path] = cur;
        return false;
    }
    bool changed = (it->second != cur);
    if (changed)
        lastEvent_[path] = cur ? "created" : "deleted";
    it->second = cur;
    return changed;
}

// --- Режим Size ---

bool FileChecker::checkSize(const std::string& path) {
    // Несуществующий файл — зона ответственности режима Existence
    if (!fs::exists(path)) return false;
    uintmax_t cur = fs::file_size(path);
    auto it       = prevSize_.find(path);
    if (it == prevSize_.end()) {
        prevSize_[path] = cur;
        return false;
    }
    bool changed = (it->second != cur);
    if (changed)
        lastEvent_[path] = "size changed to " + std::to_string(cur) + " bytes";
    it->second = cur;
    return changed;
}

// --- Режим Restoration ---

bool FileChecker::checkRestoration(const std::string& path) {
    bool exists = fs::exists(path);
    auto it     = restState_.find(path);

    if (it == restState_.end()) {
        // Первый вызов: инициализируем машину состояний без генерации события
        restState_[path] = exists ? RestorationState::PRESENT : RestorationState::ABSENT;
        return false;
    }

    RestorationState prev = it->second;
    RestorationState cur  = exists ? RestorationState::PRESENT : RestorationState::ABSENT;

    if (prev == RestorationState::PRESENT && cur == RestorationState::ABSENT) {
        // Файл был — теперь удалён: переходим в режим ожидания восстановления
        it->second       = RestorationState::ABSENT;
        lastEvent_[path] = "deleted — watching for restoration";
        return true;
    }
    if (prev == RestorationState::ABSENT && cur == RestorationState::PRESENT) {
        // Файл восстановлен после удаления
        it->second       = RestorationState::PRESENT;
        lastEvent_[path] = "restored";
        return true;
    }

    return false;
}
