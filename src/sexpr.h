#ifndef SEXPR_H_
#define SEXPR_H_

#include <string>
#include <vector>

namespace SEXPR
{
	enum SEXPR_TYPE
	{
		SEXPR_TYPE_LIST,
		SEXPR_TYPE_ATOM_INTEGER,
		SEXPR_TYPE_ATOM_DOUBLE,
		SEXPR_TYPE_ATOM_STRING,
		SEXPR_TYPE_ATOM_SYMBOL,
	};

	typedef std::vector<class SEXPR *> SEXPR_VECTOR;

	class SEXPR
	{
	protected:
		SEXPR_TYPE m_type;
		SEXPR(SEXPR_TYPE type, size_t lineNumber);
		SEXPR(SEXPR_TYPE type);
		size_t m_lineNumber;

	public:
		bool IsList() const { return m_type == SEXPR_TYPE_LIST; }
		bool IsSymbol() const { return m_type == SEXPR_TYPE_ATOM_SYMBOL; }
		bool IsString() const { return m_type == SEXPR_TYPE_ATOM_STRING; }
		bool IsDouble() const { return m_type == SEXPR_TYPE_ATOM_DOUBLE; }
		bool IsInteger() const { return m_type == SEXPR_TYPE_ATOM_INTEGER; }
		void AddChild(SEXPR* child);
		SEXPR_VECTOR const * GetChildren() const;
		SEXPR * GetChild(size_t idx) const;
		size_t GetNumberOfChildren() const;
		long long int GetInteger() const;
		double GetDouble() const;
		std::string const & GetString() const;
		std::string const & GetSymbol() const;
		std::string AsString(size_t level = 0);
		size_t GetLineNumber() { return m_lineNumber; }
	};

	struct SEXPR_INTEGER : public SEXPR
	{
		long long int m_value;
		SEXPR_INTEGER(long long int value) : SEXPR(SEXPR_TYPE_ATOM_INTEGER), m_value(value) {};
		SEXPR_INTEGER(long long int value, int lineNumber) : SEXPR(SEXPR_TYPE_ATOM_INTEGER, lineNumber), m_value(value) {};
	};

	struct SEXPR_DOUBLE : public SEXPR
	{
		double m_value;
		SEXPR_DOUBLE(double value) : SEXPR(SEXPR_TYPE_ATOM_DOUBLE), m_value(value) {};
		SEXPR_DOUBLE(double value, int lineNumber) : SEXPR(SEXPR_TYPE_ATOM_DOUBLE, lineNumber), m_value(value) {};
	};

	struct SEXPR_STRING : public SEXPR
	{
		std::string m_value;
		SEXPR_STRING(std::string value) : SEXPR(SEXPR_TYPE_ATOM_STRING), m_value(value) {};
		SEXPR_STRING(std::string value, int lineNumber) : SEXPR(SEXPR_TYPE_ATOM_STRING, lineNumber), m_value(value) {};
	};

	struct SEXPR_SYMBOL : public SEXPR
	{
		std::string m_value;
		SEXPR_SYMBOL(std::string value) : SEXPR(SEXPR_TYPE_ATOM_SYMBOL), m_value(value) {};
		SEXPR_SYMBOL(std::string value, int lineNumber) : SEXPR(SEXPR_TYPE_ATOM_SYMBOL, lineNumber), m_value(value) {};
	};

	struct _OUT_STRING
	{
		bool _Symbol;
		const std::string& _String;
	};

	inline _OUT_STRING OutSymbol(const std::string& str)
	{
		return{ true, str };
	}

	inline _OUT_STRING OutString(const std::string& str)
	{
		return{ false, str };
	}

	struct SEXPR_LIST : SEXPR
	{
		SEXPR_VECTOR m_children;
		SEXPR_LIST() : SEXPR(SEXPR_TYPE_LIST) {};
		SEXPR_LIST(int lineNumber) : SEXPR(SEXPR_TYPE_LIST, lineNumber) {};

		friend SEXPR_LIST& operator<< (SEXPR_LIST& list, double value);
		friend SEXPR_LIST& operator<< (SEXPR_LIST& list, float value);
		friend SEXPR_LIST& operator<< (SEXPR_LIST& list, long value);
		friend SEXPR_LIST& operator<< (SEXPR_LIST& list, int value);
		friend SEXPR_LIST& operator<< (SEXPR_LIST& list, const _OUT_STRING setting);
	};
}

#endif