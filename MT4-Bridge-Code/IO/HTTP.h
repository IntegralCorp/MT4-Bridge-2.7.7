#pragma once
class HTTP
{
public:
	HTTP(std::string& Host, int Port, std::string& Path, int Login, std::string& Org, std::string& PrimeBroker, std::string& Broker, std::string& Stream);
	~HTTP();

	bool Succeeded();

private:
	bool hasFailed = true;
};

