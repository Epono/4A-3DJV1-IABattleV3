#ifndef __TREE_HPP__
#define __TREE_HPP__

#include <sstream>

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
	Node* left_;
	Node* right_;

	Extractor< float >* leftValue_;
	Extractor< float >* rightValue_;

	Comparator* comp_;
public:
	Node(){}
	Node(Node* left,
		 Node* right,
		 Extractor< float >* leftValue,
		 Extractor< float >* rightValue,
		 Comparator* comp)
		: left_(left), right_(right), leftValue_(leftValue)
		, rightValue_(rightValue), comp_(comp) {}

	virtual std::unique_ptr<Action> getAction(Unit& unit, Army& ally, Army& opponent) = 0;
};

class DecisionNode : public Node
{ 
public:
	DecisionNode(Node* left, 
				 Node* right, 
				 Extractor< float >* leftValue, 
				 Extractor< float >* rightValue, 
				 Comparator* comp)
		: Node(left, right, leftValue, rightValue, comp) {}

	std::unique_ptr<Action> getAction(Unit& unit, Army& ally, Army& opponent)
	{
		if (comp_->compare(leftValue_->get(unit, ally, opponent), rightValue_->get(unit, ally, opponent)))
			return left_->getAction(unit, ally, opponent);
		else
			return right_->getAction(unit, ally, opponent);
	}
};

class ActionNode : public Node
{
public:
	ActionNode(){}

	virtual std::unique_ptr<Action> getAction(Unit& unit, Army& ally, Army& opponent) = 0;
};

class EmptyActionNode : public ActionNode
{
public:
	EmptyActionNode(){}

	std::unique_ptr<Action> getAction(Unit& unit, Army& ally, Army& opponent)
	{ return std::unique_ptr<Action>(new EmptyAction(unit)); }
}; 

class EscapeActionNode : public ActionNode
{
public:
	EscapeActionNode(Extractor< Point& >* ep) : ep_(ep){}

	std::unique_ptr<Action> getAction(Unit& unit, Army& ally, Army& opponent)
	{ return std::unique_ptr<Action>(new EscapeAction(unit, ep_->get(unit, ally, opponent))); }
private:
	Extractor< Point& >* ep_;
};

class MoveActionNode : public ActionNode
{
public:
	MoveActionNode(Extractor< Point& >* ep) : ep_(ep){}

	std::unique_ptr<Action> getAction(Unit& unit, Army& ally, Army& opponent)
	{ return std::unique_ptr<Action>(new MoveAction(unit, ep_->get(unit, ally, opponent))); }
private:
	Extractor< Point& >* ep_;
};

class ShotActionNode : public ActionNode
{
public:
	ShotActionNode(Extractor< Unit& >* eu) : eu_(eu){}

	std::unique_ptr<Action> getAction(Unit& unit, Army& ally, Army& opponent)
	{ return std::unique_ptr<Action>(new ShotAction(unit, eu_->get(unit, ally, opponent))); }
private:
	Extractor< Unit& >* eu_;
};

class TreeFactory
{
private:
	static Extractor< float >* buildValueExtractor(std::stringstream&);
	static Extractor< Army& >* buildArmyExtractor(std::stringstream&);
	static Extractor< Point& >* buildPointExtractor(std::stringstream&);
	static Extractor< Unit& >* buildUnitExtractor(std::stringstream&);
	static Extractor< float >* buildFloatExtractor(std::stringstream&);

	static DecisionNode* buildInternalNode(std::stringstream&);
	static ActionNode* buildActionNode(std::stringstream&);
public:
	static Node* buildTree(std::stringstream&);
};



#endif //__TREE_HPP__