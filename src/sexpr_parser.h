#ifndef SEXPR_PARSER_H_
#define SEXPR_PARSER_H_

#include <string>
#include <vector>


namespace SEXPR
{
	class PARSER
	{
	public:
		PARSER();
		~PARSER();
		SEXPR* Parse(const std::string &aString);
		SEXPR* ParseFromFile(const std::string &filename);
	private:
		SEXPR* parseString(const std::string& aString, std::string::const_iterator& it);
		static const std::string whitespaceCharacters;
		static const std::string extraSymbolCharacters;
		bool isSymbolCharacter(int ch);
		int m_lineNumber;
		int m_lineOffset;
	};
}

#endif