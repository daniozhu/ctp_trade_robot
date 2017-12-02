#pragma once

#include <string>

class DataFormater
{
public:
	virtual bool Format() = 0;
};

//////////////////////////////////////////////
// Json data formater
//////////////////////////////////////////////
class JsonDataFormater : public DataFormater
{
public:
	JsonDataFormater(const std::wstring& srcFilePath, const std::wstring& formatedFilePath);
	virtual ~JsonDataFormater() = default;

	virtual bool Format() override;

private:
	std::wstring		m_srcFilePath;
	std::wstring		m_formatedPath;
};

