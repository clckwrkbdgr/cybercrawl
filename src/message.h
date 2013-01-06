#pragma once
#include <string>
#include <sstream>

class Format {
public:
	Format(const std::string & format_string)
		: current_placeholder(1), result(format_string)
	{
	}
	
	template<class T>
	Format & arg(const T & value)
	{
		std::ostringstream out;
		out << '@' << current_placeholder;
		std::string placeholder = out.str();
		size_t index = result.find(placeholder);
		while(index != std::string::npos) {
			std::ostringstream out;
			out << value;
			result.replace(index, 2, out.str());
			index = result.find(placeholder);
		}
		++current_placeholder;
		return * this;
	}
	std::string str() const { return result; }
private:
	int current_placeholder;
	std::string result;
};

Format & format(const std::string & format_string);

template<class T>
Format & operator<<(Format & format, const T & value)
{
	format.arg(value);
	return format;
}

void mpr(const Format & format);
void mpr(const char * inf);
void more();
void mesclr();
void replay_messages();
