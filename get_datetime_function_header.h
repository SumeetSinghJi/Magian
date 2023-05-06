#ifndef GET_DATETIME_FUNCTION_HEADER_H
#define GET_DATETIME_FUNCTION_HEADER_H

#include <string> // Add this line to include the string type

std::string get_datetime_function() 
{
  time_t now = time(0);
  tm* ltm = localtime(&now);
  int year = 1900 + ltm->tm_year;
  int month = 1 + ltm->tm_mon;
  int day = ltm->tm_mday;
  std::string datetime_variable = std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
  return datetime_variable;
}

#endif
