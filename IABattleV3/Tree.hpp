#ifndef __TREE_HPP__
#define __TREE_HPP__

#include <sstream>

#include "Unit.hpp"
#include "Army.hpp"
#include "Extractor.hpp"
#include "Comparator.hpp"
#include "Action.hpp"
#include "EmptyAction.hpp"
#include "MoveAction.hpp"
#include "ShotAction.hpp"
#include "EscapeAction.hpp"

class Node
{
protected:
	Unit* unit_;
	Army* ally_;
	Army* opponent_;

	Node* left_;
	Node* right_;

	Extractor< float >* leftValue_;
	Extractor< float >* rightValue_;

	Comparator* comp_;
public:
	Node(Unit* unit,
		 Army* ally,
		 Army* opponent,
		 Node* left,
		 Node* right,
		 Extractor< float >* leftValue,
		 Extractor< float >* rightValue,
		 Comparator* comp)
		: unit_(unit), ally_(ally), opponent_(opponent)
		, left_(left), right_(right), leftValue_(leftValue)
		, rightValue_(rightValue), comp_(comp) {}
	virtual Action* getAction() = 0;
};

class DecisionNode : public Node
{ 
public:
	DecisionNode(Unit* unit, 
				 Army* ally, 
				 Army* opponent, 
				 Node* left, 
				 Node* right, 
				 Extractor< float >* leftValue, 
				 Extractor< float >* rightValue, 
				 Comparator* comp)
		: Node(unit, ally, opponent, left, right, leftValue, rightValue, comp) {}
	Action* getAction()
	{
		if (comp_->compare(leftValue_->get(*unit_, *ally_, *opponent_), rightValue_->get(*unit_, *ally_, *opponent_)))
			return left_->getAction();
		else
			return right_->getAction();
	}
};

class ActionNode : public Node
{
private:
	Action* action_;
public:
	Action* getAction()	{ return action_; }
};

class TreeFactory
{
private:
	static Extractor< float >* buildValueExtractor(std::stringstream& code)
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

	static Extractor< Army& >* buildArmyExtractor(std::stringstream& code)
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
			return new ExtractorNHX(ei, ea, static_cast<int>(c));
			}
			}
			case 'T':
			code >> c;
			if (c == 'L')
			{
			Extractor<float> * ev = buildValueExtractor(code);
			Extractor<Army&>* ea = buildArmyExtractor(code);
			return new ExtractorTLX(ev, ea, static_cast<int>(c));
			}
			else
			{
			Extractor<float> * ev = buildValueExtractor(code);
			Extractor<Army&>* ea = buildArmyExtractor(code);
			return new ExtractorTHX(ei, ea, static_cast<int>(c));
			}*/
		}
	}

	static Extractor<Point&>* buildPointExtractor(std::stringstream& code)
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

	static Extractor< Unit& >* buildUnitExtractor(std::stringstream& code)
	{
		char c;
		code >> c;
		switch (c)
		{
		case 'U':
			return new ExtractorU();
		case 'L':
			code >> c;
			if (c >= '0' && c <= '6')
				return new ExtractorLX(buildArmyExtractor(code), static_cast<int>(c));
			else if (c == 'D')
			{
				Extractor<Army&> * ea = buildArmyExtractor(code);
				Extractor<Point&> * ep = buildPointExtractor(code);

				return new ExtractorLD(ea, ep);
			}
		case 'H':
			code >> c;
			if (c >= '0' && c <= '6')
				return new ExtractorHX(buildArmyExtractor(code), static_cast<int>(c));
			else if (c == 'D')
			{
				Extractor<Army&> * ea = buildArmyExtractor(code);
				Extractor<Point&> * ep = buildPointExtractor(code);

				return new ExtractorHD(ea, ep);
			}
		}
	}

	static Extractor<float>* buildFloatExtractor(std::stringstream& code)
	{
		char c;
		code >> c;
		switch (c)
		{
		case 'C':
			code >> c;
			if (c >= '0' && c <= '6')
				return new ExtractorCX(buildUnitExtractor(code), static_cast<int>(c));
		case 'D':
			Extractor< Unit& >* eu = buildUnitExtractor(code);
			Extractor< Point& >* ep = buildPointExtractor(code);
			return new ExtractorD(eu, ep);
		case 'M':
			code >> c;
			if (c >= '0' && c <= '6')
				return new ExtractorMX(buildArmyExtractor(code), static_cast<int>(c));
			else if (c == 'D')
			{
				Extractor<Army&> * ea = buildArmyExtractor(code);
				Extractor<Point&> * ep = buildPointExtractor(code);

				return new ExtractorMD(ea, ep);
			}
		case 'm':
			code >> c;
			if (c >= '0' && c <= '6')
				return new ExtractormX(buildArmyExtractor(code), static_cast<int>(c));
			else if (c == 'D')
			{
				Extractor<Army&> * ea = buildArmyExtractor(code);
				Extractor<Point&> * ep = buildPointExtractor(code);

				return new ExtractormD(ea, ep);
			}
		case 'a':
			code >> c;
			if (c >= '0' && c <= '6')
				return new ExtractoraX(buildArmyExtractor(code), static_cast<int>(c));
			else if (c == 'D')
			{
				Extractor<Army&> * ea = buildArmyExtractor(code);
				Extractor<Point&> * ep = buildPointExtractor(code);

				return new ExtractoraD(ea, ep);
			}
		}
	}

	static DecisionNode* buildInternalNode(std::stringstream& code, Unit* u, Army* a, Army* o)
	{
		Extractor<float>* leftSide = buildFloatExtractor(code);
		Comparator * cmp;
		char c;
		code >> c;
		switch (c)
		{
		case '<':
			cmp = new LessComparator();
		case '>':
			cmp = new MoreComparator();
		}
		Extractor<float> * rightSide = buildFloatExtractor(code);
		Node* l_son = buildTree(code, u, a, o);
		Node* r_son = buildTree(code, u, a, o);
		return new DecisionNode(u, a, o, l_son, r_son, leftSide, rightSide, cmp);
	}

	static ActionNode* buildActionNode(std::stringstream& code, Unit* u, Army* a, Army* o)
	{
		Action * action;
		char c;
		code >> c;
		switch (c)
		{
		case 'M':
			Extractor<Point&> * ep = buildPointExtractor(code);
			action = new MoveAction(*u, ep->get(*u, *a, *o));
		case 'E':
			Extractor<Point&> * ep = buildPointExtractor(code);
			action = new EscapeAction(*u, ep->get(*u, *a, *o));
		case 'A':
			Extractor<Unit&> * eu = buildUnitExtractor(code);
			action = new ShotAction(*u, eu->get(*u, *a, *o));
		case 'N':
			action = new EmptyAction(*u);
		}
	}

public:
	static Node* buildTree(std::stringstream& code, Unit* u, Army* a, Army* o)
	{
		char c;
		code >> c;
		switch (c)
		{
		case '?':
			return buildInternalNode(code, u, a, o);
		case '!':
			return buildActionNode(code, u, a, o);
		}
	}
};



#endif //__TREE_HPP__