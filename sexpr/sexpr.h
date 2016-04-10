#ifndef SEXPR_H_
#define SEXPR_H_

#include <string>
#include <vector>
#include "sexpr/isexprable.h"
#include "sexpr/sexpr_exception.h"

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
		virtual ~SEXPR() {};
		bool IsList() const { return m_type == SEXPR_TYPE_LIST; }
		bool IsSymbol() const { return m_type == SEXPR_TYPE_ATOM_SYMBOL; }
		bool IsString() const { return m_type == SEXPR_TYPE_ATOM_STRING; }
		bool IsDouble() const { return m_type == SEXPR_TYPE_ATOM_DOUBLE; }
		bool IsInteger() const { return m_type == SEXPR_TYPE_ATOM_INTEGER; }
		void AddChild(SEXPR* child);
		SEXPR_VECTOR const * GetChildren() const;
		SEXPR * GetChild(size_t idx) const;
		size_t GetNumberOfChildren() const;
		long long int GetLongInteger() const;
		int GetInteger() const;
		float GetFloat() const;
		double GetDouble() const;
		std::string const & GetString() const;
		std::string const & GetSymbol() const;
		SEXPR_LIST* GetList();
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

	inline _OUT_STRING AsSymbol(const std::string& str)
	{
		struct _OUT_STRING ret = { true, str };
		return ret;
	}

	inline _OUT_STRING AsString(const std::string& str)
	{
		struct _OUT_STRING ret = { false, str };
		return ret;
	}

	struct _IN_STRING
	{
		bool _Symbol;
		std::string& _String;

	};

	inline _IN_STRING AsSymbol(std::string& str)
	{
		struct _IN_STRING ret = { true, str };
		return ret;
	}

	inline _IN_STRING AsString(std::string& str)
	{
		struct _IN_STRING ret = { false, str };
		return ret;
	}

	class SEXPR_SCAN_ARG {
		friend class SEXPR_LIST;
	public:
		SEXPR_SCAN_ARG(int* value) : type(INT) { u.int_value = value; }
		SEXPR_SCAN_ARG(long int* value) : type(LONGINT) { u.lint_value = value; }
		SEXPR_SCAN_ARG(double* value) : type(DOUBLE) { u.dbl_value = value; }
		SEXPR_SCAN_ARG(std::string* value) : type(STRING) { u.str_value = value; }
		SEXPR_SCAN_ARG(_IN_STRING& value) : type(SEXPR_STRING) { u.sexpr_str = &value; }
		SEXPR_SCAN_ARG(std::string value) : type(STRING_COMP) { str_value = value; }

	private:
		enum Type { INT, DOUBLE, STRING, LONGINT, STRING_COMP, SEXPR_STRING};
		Type type;
		union {
			long int* lint_value;
			int* int_value;
			double* dbl_value;
			std::string* str_value;
			_IN_STRING* sexpr_str;
		} u;

		std::string str_value;
	};

	class SEXPR_POPULATE_ARG {
		friend class SEXPR_LIST;
	public:
		SEXPR_POPULATE_ARG(int value) : type(INT) { u.int_value = value; }
		SEXPR_POPULATE_ARG(long int value) : type(LONGINT) { u.lint_value = value; }
		SEXPR_POPULATE_ARG(double value) : type(DOUBLE) { u.dbl_value = value; }
		SEXPR_POPULATE_ARG(std::string value) : type(STRING) { str_value = value; }
		SEXPR_POPULATE_ARG(const _OUT_STRING& value) : type(SEXPR_STRING) { sexpr_str_value = &value; }

	private:
		enum Type { INT, DOUBLE, STRING, LONGINT, SEXPR_STRING };
		Type type;
		union {
			long int lint_value;
			int int_value;
			double dbl_value;
		} u;
		const _OUT_STRING* sexpr_str_value;
		std::string str_value;
	};

	class SEXPR_LIST : public SEXPR
	{
	public:
		SEXPR_LIST() : SEXPR(SEXPR_TYPE_LIST), m_inStreamChild(0) {};
		SEXPR_LIST(int lineNumber) : SEXPR(SEXPR_TYPE_LIST, lineNumber), m_inStreamChild(0) {};
		SEXPR_VECTOR m_children;

		template <typename... Args>
		size_t Scan(const Args&... args)
		{
			SEXPR_SCAN_ARG arg_array[] = { args... };
			return doScan(arg_array, sizeof...(Args));
		}

		template <typename... Args>
		void Populate(const Args&... args)
		{
			SEXPR_POPULATE_ARG arg_array[] = { args... };
			doPopulate(arg_array, sizeof...(Args));
		}

		virtual ~SEXPR_LIST();

		friend SEXPR_LIST& operator<< (SEXPR_LIST& list, double value);
		friend SEXPR_LIST& operator<< (SEXPR_LIST& list, float value);
		friend SEXPR_LIST& operator<< (SEXPR_LIST& list, long long int value);
		friend SEXPR_LIST& operator<< (SEXPR_LIST& list, int value);
		friend SEXPR_LIST& operator<< (SEXPR_LIST& list, std::string value);
		friend SEXPR_LIST& operator<< (SEXPR_LIST& list, const _OUT_STRING setting);
		friend SEXPR_LIST& operator<< (SEXPR_LIST& list, const ISEXPRABLE& obj);
		friend SEXPR_LIST& operator<< (SEXPR_LIST& list, SEXPR_LIST* list2);
		friend SEXPR_LIST& operator<< (SEXPR_LIST& list, SEXPR* obj);
		friend SEXPR_LIST& operator>> (SEXPR_LIST& input, ISEXPRABLE& obj);
		friend SEXPR_LIST& operator>> (SEXPR_LIST& input, std::string& str);
		friend SEXPR_LIST& operator>> (SEXPR_LIST& input, int& inte);
		friend SEXPR_LIST& operator>> (SEXPR_LIST& input, long long int& inte);
		friend SEXPR_LIST& operator>> (SEXPR_LIST& input, float& inte);
		friend SEXPR_LIST& operator>> (SEXPR_LIST& input, double& inte);
		friend SEXPR_LIST& operator>> (SEXPR_LIST& input, const _IN_STRING is);
	private:
		int m_inStreamChild;
		size_t doScan(const SEXPR_SCAN_ARG *args, size_t num_args);
		void doPopulate(const SEXPR_POPULATE_ARG *args, size_t num_args);
	};
}

#endif