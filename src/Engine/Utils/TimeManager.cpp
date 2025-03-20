#include "TimeManager.h"
#include "Utils/Time.h"

void TimeManager::init() {
	_previous = std::chrono::system_clock::now();
}

void TimeManager::update() {
	std::chrono::system_clock::time_point current = std::chrono::system_clock::now();
	Time::_deltaTime = std::chrono::duration<double>(current - _previous).count();;
	Time::_time += Time::_deltaTime;
	_previous = current;
}
