#include "Profile/ProfilerDataProcessor.h"

#include <nlohmann/json.hpp>

namespace Profile
{
	std::vector<std::string> Split(const std::string& a_stringToSplit, char a_splitCharacter);

	ProfilerDataProcessor::ProfilerDataProcessor()
	{
	}

	ProfilerDataProcessor::~ProfilerDataProcessor()
	{
	}

	void ProfilerDataProcessor::Load(const std::string& a_file)
	{
		PROFILE_FUNCTION();

		FILE* file;
		errno_t err; 
		if ((err = fopen_s(&file, a_file.c_str(), "r") != 0))
		{
			std::cout << "Cannot open file " << a_file << " : " << strerror(err) << "\n";
			return;
		}
		fclose(file);

		m_profileResults = Instrumentor::ParseFromJson(a_file);

		std::sort(m_profileResults.traceEvents.begin(), m_profileResults.traceEvents.end(), 
				[](Profile::ProfileResult r1, Profile::ProfileResult r2)
			{
				if (r1.Start == r2.Start)
				{
					return r1.End > r2.End;
				}

				return r1.Start < r2.Start;
			});

		m_dataReady = true;
	}

	std::vector<std::string> Split(const std::string& a_stringToSplit, char a_splitCharacter)
	{
		std::vector<std::string> strings = std::vector<std::string>();

		int startIndex = 0;

		for (int i = 0; i < a_stringToSplit.length(); ++i)
		{
			if (a_stringToSplit[i] == a_splitCharacter)
			{
				// Not being deleted. Memory leak???
				char* localString = new char[i + 1];

				size_t length = a_stringToSplit.copy(localString, i - startIndex, startIndex);
				localString[length] = '\0';
				
				strings.push_back(std::string(localString));
				
				startIndex = i;

				delete[] localString;
			}
		}

		return strings;
	}

	void ProfilerDataProcessor::Parse(const ProfileSerializeObject& a_data)
	{

	}
}