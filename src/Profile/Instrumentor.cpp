#include "Profile/Instrumentor.h"
#include <iostream>

namespace Profile
{
	Instrumentor* Instrumentor::sInstance = nullptr;

	long long Instrumentor::m_profileStartTime = -1;
	uint32_t Instrumentor::m_currentId = 0;

	void to_json(json& j, const ProfileResult& object)
	{
		j = json{ 
		{"id", object.Id},
		{"cat", "function"}, 
		{"dur", object.End - object.Start}, 
		{"name", object.Name}, 
		{"ph", "X"}, 
		{"pid", 0}, 
		{"tid", object.ThreadID}, 
		{"ts", object.Start}, 
		{"te", object.End} };
	}

	void from_json(const json& j, ProfileResult& object)
	{
		j.at("id").get_to(object.Id);
		j.at("name").get_to(object.Name);
		j.at("ts").get_to(object.Start);
		j.at("te").get_to(object.End);
		j.at("tid").get_to(object.ThreadID);
	}

	void to_json(json& j, const ProfileSerializeObject& object)
	{
		j["otherData"] = json(object.m_otherData);
		j["traceEvents"] = json(object.traceEvents);
	}
	
	void from_json(const json& j, ProfileSerializeObject& object)
	{
		object.m_otherData = std::vector<int>();
		object.traceEvents = std::vector<ProfileResult>();
	}

	Instrumentor::Instrumentor()
		: m_currentSession(nullptr), m_profileCount(0)
	{
	}

	Instrumentor::~Instrumentor()
	{
	}

	void Instrumentor::BeginSession(const std::string a_name, const std::string& a_filepath)
	{
		m_outputStream.open(a_filepath);
		WriteHeader();
		m_currentSession = new InstrumentationSession{ a_name };
	}

	void Instrumentor::EndSession()
	{
		WriteFooter();
		m_outputStream.close();
		delete m_currentSession;
		m_currentSession = nullptr;
		m_profileCount = 0;

		ProfileSerializeObject object;
		object.m_otherData = std::vector<int>();
		object.traceEvents = m_results;

		json saveToFile = json(object);
		m_outputStream.open("mill.json");
		m_outputStream << saveToFile.dump();
		m_outputStream.flush();
		m_outputStream.close();
	}

	Instrumentor* Instrumentor::Get()
	{
		if (sInstance == nullptr)
		{
			sInstance = new Instrumentor();
		}
		return sInstance;
	}

	void Instrumentor::ResetProfiler()
	{
		delete sInstance;
		sInstance = nullptr;
	}

	ProfileSerializeObject Instrumentor::ParseFromJson(const std::string& fileName)
	{
		std::ifstream file(fileName);
		json jsonObject = json::parse(file);

		ProfileSerializeObject object;
		jsonObject.at("otherData").get_to(object.m_otherData);
		jsonObject.at("traceEvents").get_to(object.traceEvents);
		return object;
	}

	uint32_t Instrumentor::GetNextId()
	{
		return m_currentId++;
	}

	void Instrumentor::AddProfileResult(const ProfileResult& result)
	{
		if (m_profileStartTime == -1)
		{
			m_profileStartTime = result.Start;
		}

		m_results.push_back(result);
		m_profileCount = m_results.size();

		m_results[m_profileCount - 1].Start -= m_profileStartTime;
		m_results[m_profileCount - 1].End -= m_profileStartTime;
	}

	void Instrumentor::WriteProfiles()
	{
		for (size_t i = 0; i < m_results.size(); i++)
		{
			ProfileResult currentResult = m_results[i];

			if (i > 0)
			{
				m_outputStream << ",";
			}
			
			std::string name = currentResult.Name;
			std::replace(name.begin(), name.end(), '"', '\'');
			
			m_outputStream << "{";
			m_outputStream << "\"cat\":\"function\",";
			m_outputStream << "\"dur\":" << (currentResult.End - currentResult.Start) << ',';
			m_outputStream << "\"name\":\"" << name << "\",";
			m_outputStream << "\"ph\":\"X\",";
			m_outputStream << "\"pid\":0,";
			m_outputStream << "\"tid\":" << currentResult.ThreadID << ",";
			m_outputStream << "\"ts\":" << currentResult.Start;
			m_outputStream << "}";
			
			m_outputStream.flush();
		}
	}

	void Instrumentor::WriteHeader()
	{
		m_outputStream << "{\"otherData\": {},\"traceEvents\":[";
		m_outputStream.flush();
	}

	void Instrumentor::WriteFooter()
	{
		WriteProfiles();

		m_outputStream << "]}";
		m_outputStream.flush();
	}


	InstrumentationTimer::InstrumentationTimer(const char* name, const bool& print)
		: m_Name(name), m_Stopped(false), m_print(print)
	{
		m_id = Instrumentor::Get()->GetNextId();
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	InstrumentationTimer::~InstrumentationTimer()
	{
		if (!m_Stopped)
			Stop();
	}

	void InstrumentationTimer::Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
		Instrumentor::Get()->AddProfileResult({ m_id, m_Name, start, end, threadID });//WriteProfile({ m_Name, start, end, threadID });
		
		if (m_print)
		{
			std::stringstream ss;
			ss << "Name: " << m_Name << "\n";
			ss << "Dur: " << end - start << "\n";
			std::cout << ss.str() << "\n";
		}

		m_Stopped = true;
	}

}