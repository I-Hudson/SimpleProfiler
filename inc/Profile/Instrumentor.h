#pragma once

#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <istream>
#include <vector>
#include <thread>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Profile
{
	struct ProfileResult
	{
		uint32_t Id;
		std::string Name;
		long long Start, End;
		uint32_t ThreadID;
	};

	struct ProfileSerializeObject
	{
		std::vector<int> m_otherData;
		std::vector<ProfileResult> traceEvents;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	public:
		void BeginSession(const std::string a_name, const std::string& a_filepath = "results.json");
		void EndSession();

		static Instrumentor* Get();
		static void ResetProfiler();

		static ProfileSerializeObject ParseFromJson(const std::string& fileName);

		uint32_t GetNextId();

		void AddProfileResult(const ProfileResult& result);
		void WriteProfiles();

	private:
		Instrumentor();
		~Instrumentor();

		void WriteHeader();
		void WriteFooter();

	private:
		InstrumentationSession* m_currentSession;
		std::ofstream m_outputStream;
		int m_profileCount;

		static long long m_profileStartTime;

		std::vector<ProfileResult> m_results;

		static uint32_t m_currentId;
		static Instrumentor* sInstance;
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name, const bool& print = false);

		~InstrumentationTimer();

		void Stop();
	private:
		const char* m_Name;
		uint32_t m_id;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
		bool m_print;
	};
}

#define PROFILING 1
#if PROFILING
#define PROFILE_SCOPE(name, print) Profile::InstrumentationTimer timer##__LINE__(name, print)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCTION__, false)
#else
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()
#endif
