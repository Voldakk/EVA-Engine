#pragma once

#include <iostream>
#include <chrono>
#include <string>
#include <unordered_map>

#ifdef _DEBUG
	#define SCOPE_TIMER(MESSAGE) ScopeTimer scopeTimer(MESSAGE);
#else
	#define SCOPE_TIMER(MESSAGE);
#endif

class ScopeTimer
{
	struct Stats
	{
		size_t ns[10];
		int i = 0;
		size_t n = 0;
	};

	inline static std::unordered_map<std::string, Stats> map;
	std::chrono::steady_clock::time_point start;
	std::string message;

	inline static const char* units[4] = { "ns", "us", "ms", "s" };

public:
	ScopeTimer(const char* message) : message(message)
	{
		start = std::chrono::high_resolution_clock::now();
	}

	~ScopeTimer() 
	{
		auto end = std::chrono::high_resolution_clock::now();
		size_t ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		
		double avgNs = 0;

		auto it = map.find(message);
		if (it != map.end()) 
		{
			auto s = (*it).second;
			s.ns[s.i] = ns;
			s.i = (s.i + 1) % 10;
			s.n++;
			int n = (s.n > 10 ? 10 : s.n);
			for (size_t i = 0; i < n; i++)
			{
				avgNs += s.ns[i];
			}
			avgNs /= n;
			map[message] = s;
		}
		else
		{
			Stats s;
			s.ns[0] = ns;
			s.n = 1;
			s.i = 1;
			avgNs = ns;
			map[message] = s;
		}

		int unitI = 0;
		while (avgNs > 1000)
		{
			avgNs /= 1000;
			unitI++;
		}

		std::cout << message << ": " << avgNs << " " << units[unitI] << std::endl;
	}
};