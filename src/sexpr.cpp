
#include "sexpr.h"
#include <cctype>
#include <iterator>
#include <stdexcept>
#include <sstream>

namespace SEXPR
{
	SEXPR::SEXPR(SEXPR_TYPE type) :
		m_type(type)
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

	std::string SEXPR::AsString()
	{
		std::string result;

		if (IsList())
		{
			result += "(";

			SEXPR_VECTOR const* list = GetChildren();

			for (std::vector<SEXPR *>::const_iterator it = list->begin(); it != list->end(); ++it)
			{
				result += (*it)->AsString();
				if (it != list->end()-1)
				{
					result += " ";
				}
			}
			result += ")";

			return result;
		}
		else if (IsString())
		{
			return "\"" + GetString() + "\"";
		}
		else if (IsSymbol())
		{
			return GetSymbol();
		}
		else if (IsInteger())
		{
			std::stringstream out;
			out << GetInteger();
			return out.str();
		}
		else if (IsDouble())
		{
			std::stringstream out;
			out << GetDouble();
			return out.str();
		}
		else
		{
			return result;
		}
	}
}

