#include "sexpr.h"
#include "sexpr_parser.h"
#include <windows.h>
#include <iostream>


class TEST_OBJECT : public SEXPR::ISEXPRABLE
{
public:
	TEST_OBJECT::TEST_OBJECT(int val1, std::string val2)
	{
		value1 = val1;
		value2 = val2;
	}

	SEXPR::SEXPR* SerializeSEXPR() override
	{
		SEXPR::SEXPR_LIST* list = new SEXPR::SEXPR_LIST();
		*list << 0;
		*list << "test";

		return list;
	}

	void DeserializeSEXPR(SEXPR::SEXPR& sexp)
	{
		if (sexp.IsList())
		{
			SEXPR::SEXPR_LIST* list = sexp.GetList();
			*list >> value1;
			*list >> value2;
		}
	}
private:
	int value1;
	std::string value2;
};

int main(void)
{
	SEXPR::PARSER parser;

#if EXAMPLE1
	std::string data = "((data \"quoted data\" 123 -4.5)(data(!@# (4.5) \"(more\" \"data)\")))";
	SEXPR::SEXPR* result = parser.Parse(data);

	std::string test = result->AsString();
#endif

#if EXAMPLE2
	int kicadVersion = 0;
	std::string host = "";
	std::string hostExtra = "";
	std::string data2 = "(kicad_pcb (version 4) (host pcbnew \"(2015 - 05 - 13 BZR 5653) - product\")"
		"\n"
		"(general"
			"(links 20)\n"
			"(no_connects 0)\n"
			"(area 123.761499 90.741499 168.338501 132.778501)\n"
			"(thickness 1.6002)\n"
			"(drawings 4)\n"
			"(tracks 57)\n"
			"(zones 0)"
			"(modules 15)"
			"(nets 14)"
			")"
		"\n"
		"(page A4)"
		"(layers\n"
		"	(0 Composant signal)"
		"	(31 Cuivre signal)"
		"	(34 B.Paste user)"
		"	(35 F.Paste user)"
		"	(36 B.SilkS user)"
		"	(37 F.SilkS user)"
		"	(38 B.Mask user)"
		"	(39 F.Mask user)"
		"(40 Dwgs.User user)"
		"(41 Cmts.User user)"
		"(42 Eco1.User user)"
		"	(43 Eco2.User user)"
		"(44 Edge.Cuts user)"
		"))";
	SEXPR::SEXPR* result2 = parser.Parse(data2);
	std::string test2 = result2->AsString();

	if (result2->IsList())
	{
		SEXPR::SEXPR* kicadtitle = result2->GetChild(0);
		if (kicadtitle->GetSymbol() == "kicad_pcb")
		{
			const SEXPR::SEXPR_VECTOR* children = result2->GetChildren();

			//shitty lazy loop(i know!)
			for (int i = 1; i < 3; i++)
			{
				SEXPR::SEXPR* kicadelem = result2->GetChild(i);
				if (kicadelem->IsList())
				{
					SEXPR::SEXPR* kicadsubelemn = kicadelem->GetChild(0);
					if (kicadsubelemn->IsSymbol())
					{
						if (kicadsubelemn->GetSymbol() == "version")
						{
							kicadVersion = kicadelem->GetChild(1)->GetInteger();
						}
						else if (kicadsubelemn->GetSymbol() == "host")
						{
							host = kicadelem->GetChild(1)->GetSymbol();
							hostExtra = kicadelem->GetChild(2)->GetString();
						}
					}
				}
			}
		}
	}
#endif

	TEST_OBJECT test(1,"test");
	TEST_OBJECT test2(300, "test3");

	SEXPR::SEXPR_LIST* list = new SEXPR::SEXPR_LIST();
	*list << SEXPR::OutSymbol("test") << SEXPR::OutString("kicad");
	*list << test;

	SEXPR::SEXPR* tstobjexp = list->GetChild(2);
	test2.DeserializeSEXPR(*tstobjexp);

	std::string testty = list->AsString();
	

	DWORD dw1 = GetTickCount();

	SEXPR::SEXPR* result3 = parser.ParseFromFile("C:\\Users\\mroszko\\AppData\\Roaming\\kicad\\fp-lib-table");
//	SEXPR::SEXPR* result3 = parser.ParseFromFile("C:\\Users\\mroszko\\Projects\\wcp52.kicad_pcb");
//	SEXPR::SEXPR* result3 = parser.ParseFromFile("C:\\Program Files\\KiCad\\share\\kicad\\demos\\video\\video.kicad_pcb");
	DWORD dw2 = GetTickCount();
	std::string test3 = result3->AsString();

	//Do something 


	std::cout << "Time difference is " << (dw2 - dw1) << " milliSeconds" << std::endl;
	//std::cout << test3;
	return 0;
}