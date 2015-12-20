#include "sexpr\sexpr.h"
#include <cctype>
#include <iterator>
#include <stdexcept>
#include <sstream>

namespace SEXPR
{
	SEXPR::SEXPR(SEXPR_TYPE type, size_t lineNumber) :
		m_type(type), m_lineNumber(lineNumber)
	{
	}

	SEXPR::SEXPR(SEXPR_TYPE type) :
		m_type(type), m_lineNumber(0)
	{
	}

	SEXPR_VECTOR const * SEXPR::GetChildren() const
	{
		if (m_type != SEXPR_TYPE_LIST)
		{
			throw new std::invalid_argument("SEXPR is not a list type!");
		}

		return &static_cast<SEXPR_LIST const *>(this)->m_children;
	}
	
	SEXPR* SEXPR::GetChild(size_t idx) const 
	{
		if (m_type != SEXPR_TYPE_LIST)
		{
			throw new std::invalid_argument("SEXPR is not a list type!");
		}

		return static_cast<SEXPR_LIST const *>(this)->m_children[idx];
	}

	void SEXPR::AddChild(SEXPR* child)
	{
		if (m_type != SEXPR_TYPE_LIST)
		{
			throw new std::invalid_argument("SEXPR is not a list type!");
		}

		SEXPR_LIST* list = static_cast<SEXPR_LIST *>(this);

		list->m_children.push_back(child);
	}

	size_t SEXPR::GetNumberOfChildren() const
	{
		if (m_type != SEXPR_TYPE_LIST)
		{
			throw new std::invalid_argument("SEXPR is not a list type!");
		}

		return static_cast<SEXPR_LIST const *>(this)->m_children.size();
	}

	std::string const & SEXPR::GetString() const
	{
		if (m_type != SEXPR_TYPE_ATOM_STRING)
		{
			throw new std::invalid_argument("SEXPR is not a string type!");
		}

		return static_cast<SEXPR_STRING const *>(this)->m_value;
	}

	long long int SEXPR::GetInteger() const
	{
		if (m_type != SEXPR_TYPE_ATOM_INTEGER)
		{
			throw new std::invalid_argument("SEXPR is not a integer type!");
		}

		return static_cast<SEXPR_INTEGER const *>(this)->m_value;
	}

	double SEXPR::GetDouble() const
	{
		if (m_type != SEXPR_TYPE_ATOM_DOUBLE)
		{
			throw new std::invalid_argument("SEXPR is not a double type!");
		}

		return static_cast<SEXPR_DOUBLE const *>(this)->m_value;
	}

	std::string const & SEXPR::GetSymbol() const
	{
		if (m_type != SEXPR_TYPE_ATOM_SYMBOL)
		{
			throw new std::invalid_argument("SEXPR is not a symbol type!");
		}

		return static_cast<SEXPR_SYMBOL const *>(this)->m_value;
	}

	std::string SEXPR::AsString(size_t level)
	{
		std::string result;

		if (IsList())
		{
			if (level != 0)
			{
				result = "\n";
			}
			result.append(level * 4, ' ');
			level++;
			result += "(";

			SEXPR_VECTOR const* list = GetChildren();

			for (std::vector<SEXPR *>::const_iterator it = list->begin(); it != list->end(); ++it)
			{
				result += (*it)->AsString(level);
				if (it != list->end()-1)
				{
					result += " ";
				}
			}
			result += ")";

			level--;
		}
		else if (IsString())
		{
			result += "\"" + GetString() + "\"";
		}
		else if (IsSymbol())
		{
			result += GetSymbol();
		}
		else if (IsInteger())
		{
			std::stringstream out;
			out << GetInteger();
			result += out.str();
		}
		else if (IsDouble())
		{
			std::stringstream out;
			out << GetDouble();
			result += out.str();
		}

		return result;
	}

	SEXPR_LIST& operator<< (SEXPR_LIST& list, long value)
	{
		list.AddChild(new SEXPR_INTEGER(value));
		return list;
	}

	SEXPR_LIST& operator<< (SEXPR_LIST& list, int value)
	{
		list.AddChild(new SEXPR_INTEGER(value));
		return list;
	}

	SEXPR_LIST& operator<< (SEXPR_LIST& list, float value)
	{
		list.AddChild(new SEXPR_DOUBLE(value));
		return list;
	}

	SEXPR_LIST& operator<< (SEXPR_LIST& list, double value)
	{
		list.AddChild(new SEXPR_DOUBLE(value));
		return list;
	}

	SEXPR_LIST& operator<< (SEXPR_LIST& list, const _OUT_STRING setting)
	{
		SEXPR *res;
		if (setting._Symbol)
		{
			res = new SEXPR_SYMBOL(setting._String);
		}
		else
		{
			res = new SEXPR_STRING(setting._String);
		}
		list.AddChild(res);

		return list;
	}
}

