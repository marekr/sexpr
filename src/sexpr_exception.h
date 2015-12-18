#include <exception>
#include <string>

namespace SEXPR
{
	class PARSE_EXCEPTION : public std::exception
	{
	public:
		PARSE_EXCEPTION(const std::string m) :msg(m) {}
		const char* what() { return msg.c_str(); }
		virtual ~PARSE_EXCEPTION() throw() {}
	private:
		std::string msg;
	};
}