#include <string>
#include <vector>


namespace SEXPR
{
	class PARSER
	{
	public:
		PARSER();
		~PARSER();
		SEXPR* Parse(std::string &aString);
		SEXPR* ParseString(const std::string& aString, std::string::const_iterator& it);
	private:
		static const std::string whitespaceCharacters;
		static const std::string extraSymbolCharacters;
		bool isSymbolCharacter(int ch);
	};
}

