#include<iostream>
using namespace std;

class Date
{
private:
    int _year;
    int _month;
    int _day;

public:
    int Getmonthday(int year, int month) const
    {
        // 采用静态数组，维护月份对应的天数
        static int monthDays[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};  
        if((month==2) && ((year%4==0 && year%100!=0) || year%400==0)) // 闰年且2月
            return 29;
        return monthDays[month];
    } 

    Date(int year=0, int month=1, int day=1)
    {
        if(year>=0 && month>=1 && month<=12 && day>=1 && day<=Getmonthday(year, month))
        {
            _year = year;
            _month = month;
            _day = day;
        }
        else
            cout<<"invalid date"<<endl;
    }

    void Print()
    {
        cout<<_year<<'-'<<_month<<'-'<<_day<<endl;
    }

    bool operator>(const Date& d) const
    {
        return _year>d._year || (_year==d._year&&_month>d._month)
            || (_year==d._year && _month==d._month && _day>d._day);
    }
    bool operator==(const Date& d) const
    {
        return _year==d._year && _month==d._month && _day==d._day ;
            
    }
    bool operator>=(const Date& d) const
    {
        return *this > d || *this == d;
    }
    bool operator<(const Date& d) const
    {
        return !(*this>=d);
    }

    Date operator+(int day)  
    {
       Date ret(*this);  // 拷贝构造
       ret._day += day;
        return ret;
    }
    Date& operator+=(int day) // 需要修改当前对象，使用引用返回
    {
        if(day < 0)
            return *this-= -day;
        _day += day;
        while(_day > Getmonthday(_year, _month))
        {
            _day -= Getmonthday(_year, _month);
            _month++;
            if(_month > 12)
            {
                _year++;
                _month = 1;
            }
        }
        return *this;  // this出函数仍存在，故返回一个引用
    }
    Date& operator++()
    {
        *this += 1;
        return *this;
    }

    Date operator-(int day)
    {
        Date ret(*this);
        ret._day -= day;
        return ret;
    }
    Date& operator-=(int day)
    {
        if(day < 0)
            return *this += -day;
        _day -= day;
        while(_day < 0)
        {
            if(_month == 1)
            {
                _month = 12;
                _year--;
            }
            else
            {
                _month--;
            }    
            _day += Getmonthday(_year, _month);
                
        }
        return *this;
    }

    int operator-(const Date& d) const// 日期相减
    {
        int count = 0, sign = 1;
        Date max = *this;
        Date min = d;
        if(*this < d)
        {
            max = d;
            min = *this;
            sign = -1;
        }
        while(max > min)
        {
            ++min;
            count++;
        }
        return count*sign;
    }

};

int main()
{
    int year1, month1, day1;
    int year2, month2, day2;
    cin>>year1>>month1>>day1;
    cin>>year2>>month2>>day2;

    Date d1(year1, month1, day1);
    Date d2(year2, month2, day2);
    cout<<(d1-d2)<<endl;
    return 0;
}