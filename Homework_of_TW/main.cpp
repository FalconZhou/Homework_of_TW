#include <iostream>
#include <string>
#include <sstream>
using namespace std;
enum e_WeekDay
{
	Monday = 1, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday
};
enum e_WEEK {
	WEEK, WEEKEND
};
const int g_StartTime = 9;
const int g_EndTime = 22;
const int g_WeekPriceList[] = { 30,30,30,50,50,50,50,50,50,80,80,60,60 };
const int g_WeekendPriceList[] = { 40,40,40,50,50,50,50,50,50,60,60,60,60 };

class CDate {
	string m_szDate; // 日期
/*
protected:
	void SetDate(const string &szDate)
	{
		this->m_szDate = szDate;
	}*/
public:
	CDate() :m_szDate(""){};
	CDate(const string &szDate)
	{
		m_szDate = szDate;
		//m_szTime = szTime;
	}
	CDate(const CDate &D)
	{
		this->m_szDate = D.m_szDate;
	}

	int GetYear()const
	{
		if (m_szDate.empty())
		{
			cout << "Date is empty";
			return -1;
		}
		int Year = 0;
		for (int i = 0; i < 4; i++)
		{
			Year = Year * 10 + (m_szDate.at(i) - '0');
		}
		return Year;
	}
	int GetMonth()const
	{
		if (m_szDate.empty())
		{
			cout << "Date is empty";
			return -1;
		}
		int Month = 0;
		for (int i = 5; i < 7; i++)
		{
			Month = Month * 10 + (m_szDate.at(i) - '0');
		}
		return Month;
	}
	int GetDay()const
	{
		if (m_szDate.empty())
		{
			cout << "Date is empty";
			return -1;
		}
		int Day = 0;
		for (int i = 8; i < 10; i++)
		{
			Day = Day * 10 + (m_szDate.at(i) - '0');
		}
		return Day;
	}
	string GetDate()const {
		return m_szDate;
	}

	CDate &operator=(const CDate &D)
	{
		this->m_szDate = D.m_szDate;
		return *this;
	}
	int GetWeekday()const;

};

class CClock {
	string m_szClock;
	/*
protected:
	void SetClock(const string &szClock)
	{
		m_szClock = szClock;
	}
	*/
public:
	CClock() :m_szClock("") {};
	CClock(const string szClock)
	{
		m_szClock = szClock;
	}
	CClock(const CClock &C)
	{
		this->m_szClock = C.m_szClock;
	}

	int GetStartHour()const
	{
		if (m_szClock.empty())
		{
			cout << "Clock is empty";
		}
		int StartHour;
		StartHour = (m_szClock.at(0) - '0') * 10 + m_szClock.at(1) - '0';
		return StartHour;
	}
	int GetEndHour()const
	{
		if (m_szClock.empty())
		{
			cout << "Clock is empty";
		}
		int EndHour;
		EndHour = (m_szClock.at(6) - '0') * 10 + m_szClock.at(7) - '0';
		return EndHour;
	}
	//起始的分钟数
	//int GetStartMinite()const;
	//int GetEndMinite()const;

	string GetClock()const
	{
		return m_szClock;
	}

	CClock &operator=(const CClock &C)
	{
		this->m_szClock = C.m_szClock;
		return *this;
	}
};

/*
class CTime:public CDate,public CClock
{
	CDate m_Date;
	CClock m_Clock;
public:
	CTime():m_Date(),m_Clock(){};
	CTime(const string &szDate, const string &szClock) :m_Date(szDate), m_Clock(szClock) {};
	CTime(const CDate &Date, const CClock &Clock) :m_Date(Date), m_Clock(Clock) {};

};
*/
class CActivity {
	CDate m_Date;
	CClock m_Clock;
	int m_members;
public:
	CActivity() {};
	CActivity(const string &szDate, const string &szClock, const int &members)
		:m_Date(szDate), m_Clock(szClock), m_members(members) {};
	CActivity(const CDate &Date, const CClock &Clock, const int &members)
		:m_Date(Date), m_Clock(Clock), m_members(members) {};

	int GetGroundNum()const; //获取场次
	int GetIncome()const //获取收入
	{
		return m_members * 30;
	}
	int GetExpense()const; //获取支出

	string PrintSummary()const;//打印账单
};

//Kim larsson calculation formula
//W = ( d + 2*m + 3*( m + 1 ) / 5 + y + y/4 - y/100 + y/400 + 1 )%7
//确定这一天是周几
int CDate::GetWeekday()const
{
	if (m_szDate.empty())
	{
		cout << "Date is empty";
		return -1;
	}
	int Year = this->GetYear();
	int Month = this->GetMonth();
	int Day = this->GetDay();
	int weekFlag;

	if (Month == 1 || Month == 2)
	{
		Year--;
		Month += 12;
	}
	weekFlag = (Day + 2 * Month + 3 * (Month + 1) / 5 + Year + Year / 4 - Year / 100 + Year / 400) % 7 + 1;
	switch (weekFlag)
	{
		case Monday:
		case Tuesday:
		case Wednesday: 
		case Thursday:
		case Friday: return WEEK;
		case Saturday:;
		case Sunday: return WEEKEND;
		default:
			return -1;
	}
}

//获取场次
int CActivity::GetGroundNum()const
{
	if (m_members < 0) {
		cout << "the Numbers of people is wrong!" << endl;
		return -1;
	}

	int T = m_members / 6; //立即确定的场数
	int X = m_members % 6; //多出来的人
	switch (T)
	{
		case 0:
			if (X < 4) {
				//cancel the activity
				return 0;
			}
			else return T + 1;
		case 1:
			return T + 1;
		case 2:
		case 3:
			if (X >= 4) return T + 1;
			else return T;
		default:
			return T;
	}
}

//获取支出
int CActivity::GetExpense()const
{
	int week = m_Date.GetWeekday();
	int startHour = m_Clock.GetStartHour();
	int endHour = m_Clock.GetEndHour();
	int Price = 0;
	if (week == WEEK)
	{
		for (int i = startHour - g_StartTime; i < endHour - g_StartTime; i++)
		{
			Price += g_WeekPriceList[i];
		}
	}
	else if(week == WEEKEND)
	{
		for (int i = startHour - g_StartTime; i < endHour - g_StartTime; i++)
		{
			Price += g_WeekendPriceList[i];
		}
	}
	else
	{
		cout << "Error!" << endl;
		return -1;
	}

	Price *= this->GetGroundNum();
	return Price;
}

string CActivity::PrintSummary()const
{
	string summary;
	string income;
	string expense;
	string total;
	summary = m_Date.GetDate() + " " + m_Clock.GetClock() + " +";
		
}


int main()
{
	
	string szDay("2015-02-02"), szTime("20:00~22:00");
	int members=3;



	system("pause");

	return 0;
}