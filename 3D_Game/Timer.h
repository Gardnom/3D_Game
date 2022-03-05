#pragma once
#include <chrono>
#include <vector>
#include <tuple>
#include "app.h"
#include "Gui.h"

typedef std::pair<std::string, long long> Benchmark;

class Timer {
public:
	Timer(std::string name) {
		m_Name = name;
		m_StartTimePoint = std::chrono::high_resolution_clock::now();
	}
	~Timer() {
		Stop();
	}

	void Stop() {
		auto endTimePoint = std::chrono::high_resolution_clock::now();
		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch();

		auto duration = (end - start).count();
		m_Timings.push_back(std::pair<std::string, long long>(m_Name, duration));
		//printf("%s Took %f ms\n", m_Name.c_str(), (float)duration / 1000);
	}

	static const std::vector<Benchmark>& GetTimings() {
		return m_Timings;
	}

	friend class App;
	//friend class Gui;

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimePoint;
	std::string m_Name;
	// 1. Name 2.duration
	static std::vector<std::pair<std::string, long long>> m_Timings;
	static void ClearTimings() {
		m_Timings.clear();
	}
};