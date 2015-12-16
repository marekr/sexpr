#include "sexpr.h"
#include "sexpr_parser.h"


int main(void)
{
	SEXPR::PARSER parser;

#if 0
	std::string data = "((data \"quoted data\" 123 -4.5)(data(!@# (4.5) \"(more\" \"data)\")))";
	SEXPR::SEXPR* result = parser.Parse(data);

	std::string test = result->AsString();


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

	SEXPR::SEXPR* result3 = parser.ParseFromFile("C:\\Users\\mroszko\\AppData\\Roaming\\kicad\\fp-lib-table");
	std::string test3 = result3->AsString();

	return 0;
}