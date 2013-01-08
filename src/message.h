#pragma once
#include <cassert>
#include <string>
#include <sstream>

// TODO Move all messaging from MPR to MSG.
void mpr(const char * inf);
void more();
void mesclr();
void replay_messages();

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
template<class T>
Format & operator<<(Format & format, const T & value)
{
	format.arg(value);
	return format;
}

struct MessageData {
	MessageData(const std::string & format_string) : format(format_string), refCount(1) {}
	Format format;
	int refCount;
};
class Message {
public:
	Message(const std::string & format_string) : data(new MessageData(format_string)) { }
	Message(const Message & other) : data(other.data) { data->refCount++; }
	~Message()
	{
		data->refCount--;
		assert(data->refCount >= 0);
		if(data->refCount == 0) {
			mpr(data->format.str().c_str());
			delete data;
		}
	}
	template<class T>
	void arg(const T & value) {
		data->format.arg(value);
	}
private:
	Message & operator=(const Message &) { return * this; }
	MessageData * data;
};
template<class T>
Message operator<<(Message message, const T & value)
{
	message.arg(value);
	return message;
}
Message msg(const std::string & format_string);

