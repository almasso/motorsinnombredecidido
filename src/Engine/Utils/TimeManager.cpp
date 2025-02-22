#include "TimeManager.h"
#include "Utils/Time.h"

void TimeManager::init() {
	previous = std::chrono::system_clock::now();
}

void TimeManager::update() {
	std::chrono::system_clock::time_point current = std::chrono::system_clock::now();
	Time::deltaTime_ = std::chrono::duration<double>(current - previous).count();;
	Time::time_ += Time::deltaTime_;
	previous = current;
}
