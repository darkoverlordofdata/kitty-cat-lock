#pragma once
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "polyfill.h"

typedef struct VCalendar VCalendar;
typedef struct VEvent VEvent;

struct VCalendar 
{
    char* version;      //  VERSION:2.0
    char* prodid;       //  PRODID:icalendar-ruby
    char* calscale;     //  CALSCALE:GREGORIAN
    char* calname;      //  X-WR-CALNAME:US Holidays
    char* language;     //  X-APPLE-LANGUAGE:en-US
    char* region;       //  X-APPLE-REGION:US
    long nevents;       //  size of event[]
    char* data;         //  text data from file
    VEvent** event;     //  array of VEvent
    // VEvent* event[];     //  array of VEvent
};  

struct VEvent 
{
    char* dtstamp;      //  DTSTAMP:20190531T224632Z
    char* uid;          //  UID:c5bbb4ae-8dfd-33db-b3e0-4862c55e3ce7
    char* dtstart;      //  DTSTART;VALUE=DATE:20180704
    char* klass;        //  CLASS:PUBLIC
    char* summary;      //  SUMMARY;LANGUAGE=en-US:Independence Day
    char* transp;       //  TRANSP:TRANSPARENT
    char* rrule;        //  RRULE:FREQ=YEARLY;COUNT=5
    char* categories;   //  CATEGORIES:Holidays
    char* id;           //  X-APPLE-UNIVERSAL-ID:f42c6443-488d-9756-925b-bf42f63a3348

};

typedef struct Holiday Holiday;
struct Holiday 
{
   Holiday* next;       //  next => in list
   int date;            //  vevent dtstart
   char* description;   //  vevent summary
};

typedef struct Holidays Holidays;
struct Holidays 
{
   Holiday* head;       //  next => in list
   int today;           //  today's date
   int count;           //  count of holidays
};

void vcalendar_dispose(VCalendar* this);
VCalendar* vcalendar_new(char* path);
int vcalendar_date(VCalendar* this, int event);

void holidays_dispose(Holidays* holidays);
Holidays* holidays_new(char* path);
typedef void (*HolidayIter) (Holiday* this);
void holidays_filter(Holidays* this, int for_date, HolidayIter iterate) ;
