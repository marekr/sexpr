#include "sexpr\sexpr_parser.h"
#include "sexpr\sexpr_exception.h"
#include <cctype>
#include <iterator>
#include <stdexcept>
#include <stdlib.h>     /* strtod */

#include <fstream>
#include <streambuf>

namespace SEXPR
{
	const std::string PARSER::whitespaceCharacters = " \t\n\r\b\f\v";

	PARSER::PARSER() : m_lineNumber(0), m_lineOffset(0)
	{
	}

	PARSER::~PARSER()
	{
	}

	SEXPR* PARSER::Parse(const std::string &aString) 
	{
		std::string::const_iterator it = aString.begin();
		return parseString(aString, it);
	}

	SEXPR* PARSER::ParseFromFile(const std::string &aFileName)
	{
		std::string str = getFileContents(aFileName);

		std::string::const_iterator it = str.begin();
		return parseString(str, it);
	}

	std::string PARSER::getFileContents(const std::string &aFileName)
	{
		std::ifstream file(aFileName.c_str(), std::ios::binary);
		std::string str;

		// Faster than automatic allocation
		file.seekg(0, std::ios::end);

		auto length = file.tellg();
		if (length < 0)
		{
			throw PARSE_EXCEPTION("Error occurred attempting to read in file");
		}

		str.resize(static_cast<size_t>(length));
		file.seekg(0, std::ios::beg);

		file.read(&str[0], str.length());

		return str;
	}

	SEXPR* PARSER::parseString(const std::string& aString, std::string::const_iterator& it)
	{
		std::string::const_iterator eit = aString.end();
		for (it; it != aString.end(); ++it)
		{
			if (*it == '\n')
			{
				m_lineNumber++;
				m_lineOffset = 0;
			}

			if (whitespaceCharacters.find(*it) != std::string::npos)
				continue;

			if (*it == '(')
			{
				std::advance(it, 1);

				SEXPR_LIST* list = new SEXPR_LIST(m_lineNumber);
				while (it != aString.end() && *it != ')')
				{
					if (whitespaceCharacters.find(*it) != std::string::npos)
					{
						std::advance(it, 1);
						continue;
					}

					SEXPR* item = parseString(aString, it);
					list->AddChild(item);
				}

				if (it != aString.end())
				{
					std::advance(it, 1);
				}

				return list;
			}
			else if (*it == ')')
			{
				return NULL;
			}
			else if (*it == '"')
			{
				size_t startPos = std::distance(aString.begin(), it) + 1;
				size_t closingPos = aString.find_first_of('"', startPos);

				if (closingPos != std::string::npos)
				{
					SEXPR_STRING* str = new SEXPR_STRING(aString.substr(startPos, closingPos - startPos),m_lineNumber);
					std::advance(it, closingPos - startPos + 2);

					return str;
				}
				else
				{
					throw PARSE_EXCEPTION("missing closing quote");
				}
			}
			else
			{
				size_t startPos = std::distance(aString.begin(), it);
				size_t closingPos = aString.find_first_of(whitespaceCharacters+"()", startPos);

				std::string tmp = aString.substr(startPos, closingPos - startPos);


				if (closingPos != std::string::npos)
				{
					if (tmp.find_first_not_of("0123456789.") == std::string::npos ||
						(tmp.size() > 1 && tmp[0] == '-' && tmp.find_first_not_of("0123456789.",1) == std::string::npos)
						)
					{
						SEXPR* res;
						if (tmp.find('.') != std::string::npos)
						{
							res = new SEXPR_DOUBLE(strtod(tmp.c_str(), NULL), m_lineNumber);
							//floating point type
						}
						else
						{
							res = new SEXPR_INTEGER(strtoll(tmp.c_str(), NULL, 0), m_lineNumber);
						}
						std::advance(it, closingPos - startPos);
						return res;
					}
					else
					{
						SEXPR_SYMBOL* str = new SEXPR_SYMBOL(tmp, m_lineNumber);
						std::advance(it, closingPos - startPos);

						return str;
					}
				}
				else
				{
					throw PARSE_EXCEPTION("format error");
				}
			}
		}

		return NULL;
	}
}

