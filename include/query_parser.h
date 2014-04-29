#ifndef _query_parser_h_
#define _query_parser_h_
#include <string>

class script_file_parser;

struct query_interface
{
	typedef script_file_parser key_list;

	virtual ~query_interface() { }
	virtual bool evaluate(const key_list & vec) const = 0;

	static query_interface * parse(int & begin, int argc, const char * args[]);
};

class query_before : public query_interface
{
private:
	int   _date;

public:
	query_before(int d);
	bool evaluate(const key_list & vec) const;
};

class query_after : public query_interface
{
private:
	int   _date;

public:
	query_after(int d);
	bool evaluate(const key_list & vec) const;
};

class query_and : public query_interface
{
private:
	query_interface * lhs;
	query_interface * rhs;

public:
	query_and(query_interface * i, query_interface * j);
	~query_and();

	bool evaluate(const key_list & vec) const;
	static
	query_interface * parse(int & begin, int end, const char * args[]);
};

class query_xor : public query_interface
{
private:
	query_interface * lhs;
	query_interface * rhs;

public:
	query_xor(query_interface * i, query_interface * j);
	~query_xor();

	bool evaluate(const key_list & vec) const;
	static
	query_interface * parse(int & begin, int end, const char * args[]);
};

class query_or : public query_interface
{
private:
	query_interface * lhs;
	query_interface * rhs;

public:
	query_or(query_interface * i, query_interface * j);
	~query_or();
	bool evaluate(const key_list & vec) const;

	static
	query_interface * parse(int & begin, int end, const char * args[]);
};

class query_not : public query_interface
{
private:
	query_interface * _self;

public:
	query_not(query_interface * it);
	~query_not();

	bool evaluate(const key_list & vec) const;

	static
	bool is_digit(char a) {	return '0' <= a || a <= '9'; }
	static
	bool is_separator(char a) {	return a == '\\' || a == '/' || a == '-'; }
	static
	int date_parse(const char * arg);
	static
	query_interface * parse(int & begin, int end, const char * args[]);
};

class query_keyword : public virtual query_interface
{
private:
	std::string _keyword;

public:
	query_keyword(const char * k);
	bool evaluate(const key_list & vec) const;
};

#endif
