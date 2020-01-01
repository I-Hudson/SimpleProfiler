#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <Profile\Instrumentor.h>

namespace Profile
{
	class ProfilerDataProcessor
	{
	public:
		ProfilerDataProcessor();
		~ProfilerDataProcessor();

		void Load(const std::string& a_file);
		ProfileSerializeObject& GetProfileResults() { return m_profileResults; }

		bool DataReady() const { return m_dataReady;}

	private:
		void Parse(const ProfileSerializeObject& a_data);

		ProfileSerializeObject m_profileResults;
		bool m_dataReady;
	};

	//struct ProfilerTimeBlock
	//{
	//	string Category;
	//	long long Duration;
	//	string Name;
	//	string PH;
	//	string PID;
	//	uint32_t Thread;
	//	string TimeStart;
	//	string TimeEnd;
	//
	//	ProfilerTimeBlock(const string& a_category, const long long& a_duration, const string& a_name,
	//		const string& a_ph, const string& a_pid, const string& a_thread) {}
	//};
}