#include "checkers/ExistenceChecker.h"
#include <filesystem>

namespace fs = std::filesystem;

bool ExistenceChecker::hasChanged(const std::string& path) {
    bool cur = fs::exists(path);
    auto it = prevState_.find(path);
    if (it == prevState_.end()) {
        // Первый вызов: запоминаем текущее состояние, событие не генерируем
        prevState_[path] = cur;
        return false;
    }
    bool changed = (it->second != cur);
    it->second = cur; // обновляем базу для следующей проверки
    return changed;
}

FileEvent ExistenceChecker::getEvent(const std::string& path) const {
    // Читаем текущее состояние файловой системы в момент запроса события
    return {path, fs::exists(path) ? "created" : "deleted"};
}
