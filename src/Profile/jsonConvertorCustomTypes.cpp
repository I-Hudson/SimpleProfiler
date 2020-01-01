#include <json.hpp>
#include "Profile/Instrumentor.h"

using json = nlohmann::json;

namespace Profile
{
	void to_json(json& a_json, const ProfileResult& a_profileresult)
	{
		a_json = json{
						{"cat", a_profileresult.Category},
						{"dur", a_profileresult.Duration},
						{"ph", a_profileresult.ph},
						{"pid", a_profileresult.pid},
						{"name", a_profileresult.Name},
						{"ts", a_profileresult.Start},
						{"te", a_profileresult.End},
						{"thr", a_profileresult.ThreadID}
		};
	}

	void from_json(const json& a_json, ProfileResult& a_profileresult)
	{
		a_json.at("cat").get_to(a_profileresult.Category);
		a_json.at("dur").get_to(a_profileresult.Duration);
		a_json.at("ph").get_to(a_profileresult.ph);
		a_json.at("pid").get_to(a_profileresult.pid);
		a_json.at("name").get_to(a_profileresult.Name);
		a_json.at("ts").get_to(a_profileresult.Start);
		a_json.at("te").get_to(a_profileresult.End);
		a_json.at("thr").get_to(a_profileresult.ThreadID);
	}
}