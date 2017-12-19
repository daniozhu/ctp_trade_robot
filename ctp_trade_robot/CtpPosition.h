#pragma once

struct CThostFtdcInvestorPositionField;

class CtpPosition
{
public:

	enum class eDirection {eNet, eLong, eShort};	//1= net, 2 = long, 3= short
	enum class eDateType {eToday, eHistory};		// 1 = tody, 2 = history

	CtpPosition(CThostFtdcInvestorPositionField* pField);
	~CtpPosition() = default;

	const std::string	GetInstrumentId() const { return m_id; }
	eDirection GetDirection() const				{ return m_direction; }
	eDateType GetDateType() const				{ return m_dateType; }
	int GetVolume() const						{ return m_volume; }
	double GetCost() const						{ return m_cost; }
	double GetProfit() const					{ return m_profit; }

private:
	std::string		m_id;
	eDirection		m_direction;
	eDateType		m_dateType;
	int				m_volume;
	double			m_cost;
	double			m_profit;

};

