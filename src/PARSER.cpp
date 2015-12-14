
#include "SEXPR.h"
#include "PARSER.h"
#include <cctype>
#include <iterator>
#include <stdexcept>
#include <stdlib.h>     /* strtod */

namespace SEXPR
{
	const std::string PARSER::whitespaceCharacters = " \t\n\r\b\f\v";
	const std::string PARSER::extraSymbolCharacters = "!@#:\\/";

	PARSER::PARSER()
	{
	}

	PARSER::~PARSER()
	{
	}

	SEXPR* PARSER::Parse(std::string &aString)
	{
		return ParseString(aString, aString.begin());
	}

	SEXPR* PARSER::ParseString(const std::string& aString, std::string::const_iterator& it)
	{
		for (; it != aString.end(); ++it)
		{
			if (*it == '(')
			{
				std::advance(it, 1);

				SEXPR_LIST* list = new SEXPR_LIST();
				while (it != aString.end() && *it != ')')
				{
					SEXPR* item = ParseString(aString, it);
					list->AddChild(item);
				}

				std::advance(it, 1);

				return list;
			}
			else if (*it == ')')
			{
			}
			else if (*it == '"')
			{
				size_t startPos = std::distance(aString.begin(), it) + 1;
				size_t closingPos = aString.find_first_of('"', startPos);

				if (closingPos != std::string::npos)
				{
					SEXPR_STRING* str = new SEXPR_STRING(aString.substr(startPos, closingPos - startPos));
					std::advance(it, closingPos - startPos + 2);

					return str;
				}
				else
				{
					throw std::invalid_argument("missing closing quote");
				}
			}
			else if (std::isdigit(*it) ||
				(*it == '-' && it != aString.end()-1 && std::isdigit(*(it+1))) 
				)
			{
				size_t startPos = std::distance(aString.begin(), it);
				size_t closingPos = aString.find_first_not_of("-0123456789.", startPos);

				std::string tmp = aString.substr(startPos, closingPos - startPos);

				SEXPR* res;
				if (tmp.find('.') != std::string::npos)
				{
					res = new SEXPR_DOUBLE(strtod(tmp.c_str(), NULL));
					//floating point type
				}
				else
				{
					res = new SEXPR_INTEGER(strtoll(tmp.c_str(), NULL, 0));
				}
				std::advance(it, closingPos - startPos);

				return res;
			}
			else if (isSymbolCharacter(*it))
			{
				size_t startPos = std::distance(aString.begin(), it);
				size_t closingPos = aString.find_first_of(whitespaceCharacters+"()", startPos);

				if (closingPos != std::string::npos)
				{
					SEXPR_SYMBOL* str = new SEXPR_SYMBOL(aString.substr(startPos, closingPos - startPos));
					std::advance(it, closingPos - startPos + 1);

					return str;
				}
				else
				{
					throw std::invalid_argument("missing closing quote");
				}
			}
		}
	}


	bool PARSER::isSymbolCharacter(int ch)
	{
		if (std::isalpha(ch))
			return true;

		return extraSymbolCharacters.find(ch) != std::string::npos;
	}
}

