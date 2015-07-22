#include "stdafx.h"

Extractor< float >* TreeFactory::buildValueExtractor(std::stringstream& code)
{
	char c;
	code >> c;
	if (c == '[')
	{
		std::string s = "";
		code >> c;
		while (c != ']')
		{
			s += c;
			code >> c;
		}
		return new ExtractorValue(atof(s.c_str()));
	}
}

Extractor< Army& >* TreeFactory::buildArmyExtractor(std::stringstream& code)
{
	char c;
	code >> c;
	switch (c)
	{
	case 'A':
		return new ExtractorA();
	case 'O':
		return new ExtractorO();
		/*case 'N':
		code >> c;
		if (c == 'L')
		{
		code >> c;
		if (c == 'D')
		{
		Extractor<float> * ev = buildValueExtractor(code);
		Extractor<Army&>* ea = buildArmyExtractor(code);
		Extractor<Point&>* ep = buildPointExtractor(code);
		return new ExtractorNLD(ev, ea, ep);
		}
		else
		{
		Extractor<float> * ev = buildValueExtractor(code);
		Extractor<Army&>* ea = buildArmyExtractor(code);
		return new ExtractorNLX(ev, ea, c);
		}
		}
		else
		{
		code >> c;
		if (c == 'D')
		{
		Extractor<float> * ev = buildValueExtractor(code);
		Extractor<Army&>* ea = buildArmyExtractor(code);
		Extractor<Point&>* ep = buildPointExtractor(code);
		return new ExtractorNHD(ev, ea, ep);
		}
		else
		{
		Extractor<float> * ev = buildValueExtractor(code);
		Extractor<Army&>* ea = buildArmyExtractor(code);
		return new ExtractorNHX(ei, ea, c - 0x30);
		}
		}
		case 'T':
		code >> c;
		if (c == 'L')
		{
		Extractor<float> * ev = buildValueExtractor(code);
		Extractor<Army&>* ea = buildArmyExtractor(code);
		return new ExtractorTLX(ev, ea, c - 0x30);
		}
		else
		{
		Extractor<float> * ev = buildValueExtractor(code);
		Extractor<Army&>* ea = buildArmyExtractor(code);
		return new ExtractorTHX(ei, ea, c - 0x30);
		}*/
	}
}

Extractor< Point& >* TreeFactory::buildPointExtractor(std::stringstream& code)
{
	char c;
	code >> c;
	switch (c)
	{
	case 'B':
		return new ExtractorB(buildArmyExtractor(code));
	case 'P':
		return new ExtractorP(buildUnitExtractor(code));
	}
}

Extractor< Unit& >* TreeFactory::buildUnitExtractor(std::stringstream& code)
{
	char c;
	code >> c;
	switch (c)
	{
	case 'U':
		return new ExtractorU();
	case 'L':
	{
		code >> c;
		if (c >= '0' && c <= '6')
			return new ExtractorLX(buildArmyExtractor(code), c - 0x30);
		else if (c == 'D')
		{
			Extractor<Army&> * ea = buildArmyExtractor(code);
			Extractor<Point&> * ep = buildPointExtractor(code);

			return new ExtractorLD(ea, ep);
		}
	}
	case 'H':
	{
		code >> c;
		if (c >= '0' && c <= '6')
			return new ExtractorHX(buildArmyExtractor(code), c - 0x30);
		else if (c == 'D')
		{
			Extractor<Army&> * ea = buildArmyExtractor(code);
			Extractor<Point&> * ep = buildPointExtractor(code);

			return new ExtractorHD(ea, ep);
		}
	}
	}
}

Extractor< float >* TreeFactory::buildFloatExtractor(std::stringstream& code)
{
	char c;
	code >> c;
	switch (c)
	{
	case 'C':
	{
		code >> c;
		if (c >= '0' && c <= '6')
			return new ExtractorCX(buildUnitExtractor(code), c - 0x30);
	}
	case 'D':
	{
		Extractor< Unit& >* eu = buildUnitExtractor(code);
		Extractor< Point& >* ep = buildPointExtractor(code);
		return new ExtractorD(eu, ep);
	}
	case 'M':
	{
		code >> c;
		if (c >= '0' && c <= '6')
			return new ExtractorMX(buildArmyExtractor(code), c - 0x30);
		else if (c == 'D')
		{
			Extractor<Army&> * ea = buildArmyExtractor(code);
			Extractor<Point&> * ep = buildPointExtractor(code);

			return new ExtractorMD(ea, ep);
		}
	}
	case 'm':
	{
		code >> c;
		if (c >= '0' && c <= '6')
			return new ExtractormX(buildArmyExtractor(code), c - 0x30);
		else if (c == 'D')
		{
			Extractor<Army&> * ea = buildArmyExtractor(code);
			Extractor<Point&> * ep = buildPointExtractor(code);

			return new ExtractormD(ea, ep);
		}
	}
	case 'a':
	{
		code >> c;
		if (c >= '0' && c <= '6')
			return new ExtractoraX(buildArmyExtractor(code), c - 0x30);
		else if (c == 'D')
		{
			Extractor<Army&> * ea = buildArmyExtractor(code);
			Extractor<Point&> * ep = buildPointExtractor(code);

			return new ExtractoraD(ea, ep);
		}
	}
	}
}

DecisionNode* TreeFactory::buildInternalNode(std::stringstream& code)
{
	Extractor<float>* leftSide = buildFloatExtractor(code);
	Comparator * cmp = nullptr;
	char c;
	code >> c;
	switch (c)
	{
	case '<':
		cmp = new LessComparator();
		break;
	case '>':
		cmp = new MoreComparator();
		break;
	}
	Extractor<float> * rightSide = buildFloatExtractor(code);
	Node* l_son = buildTree(code);
	Node* r_son = buildTree(code);
	return new DecisionNode(l_son, r_son, leftSide, rightSide, cmp);
}

ActionNode* TreeFactory::buildActionNode(std::stringstream& code)
{
	char c;
	code >> c;
	switch (c)
	{
	case 'M':
	{
		Extractor<Point&> * ep = buildPointExtractor(code);
		return new MoveActionNode(ep);
	}
	case 'E':
	{
		Extractor<Point&> * ep = buildPointExtractor(code);
		return new EscapeActionNode(ep);
	}
	case 'A':
	{
		Extractor<Unit&> * eu = buildUnitExtractor(code);
		return new ShotActionNode(eu);
	}
	case 'N':
		return new EmptyActionNode();
	}
}

Node* TreeFactory::buildTree(std::stringstream& code)
{
	char c;
	code >> c;
	switch (c)
	{
	case '?':
		return buildInternalNode(code);
	case '!':
		return buildActionNode(code);
	}
}