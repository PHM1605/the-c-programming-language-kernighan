// day_of_year and month_day conversion
#include <stdio.h>

static char daytab[2][13] = {
    {0,31,28,31,30,31,30,31,31,30,31,30,31},
    {0,31,29,31,30,31,30,31,31,30,31,30,31}
};

int is_leap(int year) {
    return year%4 ==0 && year%100 != 0 || year%400==0;
}

// e.g. 01.03 is 60th day of normal year (61st of leap year)
int day_of_year(int year, int month, int day) {
    int i, leap;
    
    // error checking
    if (year<1) return -1;
    if (month<1 || month>12) return -1;
    leap = is_leap(year);
    if (day<1 || day>daytab[leap][month]) return -1;

    for (i=1; i<month; i++)
        day += daytab[leap][i];
    return day;
}

// e.g. 60th day of normal year is 01.03
int month_day(int year, int yearday, int* pmonth, int* pday) {
    int i, leap, maxday;

    // error checking
    if (year<1 || pmonth==NULL || pday==NULL) return -1;
    leap = is_leap(year);
    maxday = leap ? 366 : 365;
    if (yearday < 1 || yearday > maxday) return -1;

    for (i=1; yearday > daytab[leap][i]; i++)
        yearday -= daytab[leap][i];
    *pmonth = i;
    *pday = yearday;
    return 0;
}

int main() {
    int year = 1921, month = 3, day = 1;
    int yearday = day_of_year(year, month, day);
    if (yearday < 0){
        printf("Invalid date %d-%02d-%02d\n", year, month, day);
        return 1;
    }    
    printf("Day of year %d-%02d-%02d is: %d\n", year, month, day, yearday);
    
    int m, d;
    if(month_day(year, yearday, &m, &d) < 0) {
        printf("Invalid yearday %d for year %d\n", yearday, year);
        return 1;
    }
    printf("Yearday %d is of %d is: %02d-%02d\n", yearday, year, m, d);

    return 0;
}