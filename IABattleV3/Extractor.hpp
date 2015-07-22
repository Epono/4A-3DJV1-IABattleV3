#ifndef __EXTRACTOR_HPP__
#define __EXTRACTOR_HPP__

class Unit;
class Army;

template<typename T>
class Extractor
{
public:
	virtual T get(Unit& u, Army& a, Army& o) const = 0;
};

/* Constant Value Extractor */
class ExtractorValue : public Extractor< float >
{
private:
	float _value;
public:
	ExtractorValue(float value) : _value(value) {}
	float get(Unit& u, Army& a, Army& o) const{ return _value; }
};

/* Capacity value extractor */
/* Extract the Capacity value of a Unit */
class ExtractorCX : public Extractor< float >
{
private:
	Extractor< Unit& >* _eu;
	int _capacityIndex;
public:
	ExtractorCX(Extractor< Unit& >* eu, int capa_index) : _eu(eu), _capacityIndex(capa_index) {}
	float get(Unit& u, Army& a, Army& o) const
	{
		if (_capacityIndex < 0 || _capacityIndex > 6)
			throw std::invalid_argument("Capacity number not in range");
		return _eu->get(u, a, o).getCapacity(_capacityIndex)->getValue();
	}
};

/* Extract the distance between a unit and a point */
class ExtractorD : public Extractor< float >
{
private:
	Extractor < Unit& >* _eu;
	Extractor < Point& >* _ep;
public:
	ExtractorD(Extractor < Unit& >* eu, Extractor < Point& >* ep)
		: _eu(eu), _ep(ep) {}
	float get(Unit& u, Army& a, Army& o) const 
	{ 
		return _eu->get(u, a, o).getPosition().distance(_ep->get(u, a, o));
	}
};

/* Extract the maximum capacity value of an army */
class ExtractorMX : public Extractor< float >
{
private:
	Extractor< Army& >* _ea;
	int _capacityIndex;
public:
	ExtractorMX(Extractor< Army& >* ea, int capa_index) 
		: _ea(ea), _capacityIndex(capa_index) {}
	float get(Unit& u, Army& a, Army& o) const
	{
		if (_capacityIndex < 0 || _capacityIndex > 6)
			throw std::invalid_argument("Capacity number not in range");

		float result = _ea->get(u, a, o).getUnitsList().front()->getCapacity(_capacityIndex)->getValue();
		for (auto& it : _ea->get(u, a, o).getUnitsList())
		{
			float val = it->getCapacity(_capacityIndex)->getValue();
			if (val < result) 
				result = val;
		}
		return result;
	}
};

/* Extract the minimum capacity value of an army */
class ExtractormX : public Extractor< float >
{
private:
	Extractor< Army& >* _ea;
	int _capacityIndex;
public:
	ExtractormX(Extractor< Army& >* ea, int capa_index) 
		: _ea(ea), _capacityIndex(capa_index) {}
	float get(Unit& u, Army& a, Army& o) const
	{
		if (_capacityIndex < 0 || _capacityIndex > 6)
			throw std::invalid_argument("Capacity number not in range");

		float result = _ea->get(u, a, o).getUnitsList().front()->getCapacity(_capacityIndex)->getValue();
		for (auto& it : _ea->get(u, a, o).getUnitsList())
		{
			float val = it->getCapacity(_capacityIndex)->getValue();
			if (val > result)
				result = val;
		}
		return result;
	}
};

/* Extract the average capacity value of an army */
class ExtractoraX : public Extractor< float >
{
private:
	Extractor< Army& >* _ea;
	int _capacityIndex;
public:
	ExtractoraX(Extractor< Army& >* ea, int capa_index)
		: _ea(ea), _capacityIndex(capa_index) {}
	float get(Unit& u, Army& a, Army& o) const
	{
		if (_capacityIndex < 0 || _capacityIndex > 6)
			throw std::invalid_argument("Capacity number not in range");

		float result = 0.f;
		for (auto& it : _ea->get(u, a, o).getUnitsList())
			result += it->getCapacity(_capacityIndex)->getValue();

		return result / _ea->get(u, a, o).getUnitsList().size();
	}
};

/* Extract the maximum distance of a point */
class ExtractorMD : public Extractor< float >
{
private:
	Extractor< Army& >* _ea;
	Extractor< Point& >* _ep;
public:
	ExtractorMD(Extractor< Army& >* ea, Extractor< Point& >* ep)
		: _ea(ea), _ep(ep) {}
	float get(Unit& u, Army& a, Army& o) const
	{
		float result = _ea->get(u, a, o).getUnitsList().front()->getPosition().distance(_ep->get(u, a, o));
		Point p = _ep->get(u, a, o);
		for (auto& it : _ea->get(u, a, o).getUnitsList())
		{
			float dist = it->getPosition().distance(p);
			if (dist < result)
				result = dist;
		}
		return result;
	}
};

/* Extract the minimum distance of a point */
class ExtractormD : public Extractor< float >
{
private:
	Extractor< Army& >* _ea;
	Extractor< Point& >* _ep;
public:
	ExtractormD(Extractor< Army& >* ea, Extractor< Point& >* ep)
		: _ea(ea), _ep(ep) {}
	float get(Unit& u, Army& a, Army& o) const
	{
		float result = _ea->get(u, a, o).getUnitsList().front()->getPosition().distance(_ep->get(u, a, o));
		Point p = _ep->get(u, a, o);
		for (auto& it : _ea->get(u, a, o).getUnitsList())
		{
			float dist = it->getPosition().distance(p);
			if (dist > result)
				result = dist;
		}
		return result;
	}
};

/* Extract the average distance of a point */
class ExtractoraD : public Extractor< float >
{
private:
	Extractor< Army& >* _ea;
	Extractor< Point& >* _ep;
public:
	ExtractoraD(Extractor< Army& >* ea, Extractor< Point& >* ep)
		: _ea(ea), _ep(ep) {}
	float get(Unit& u, Army& a, Army& o) const
	{
		float result = 0.0;
		Point p = _ep->get(u, a, o);
		for (auto& it : _ea->get(u, a, o).getUnitsList())
			result += it->getPosition().distance(p);
		
		return result / _ea->get(u, a, o).getUnitsList().size();
	}
};

/* Unit Extractor */
class ExtractorU : public Extractor< Unit& >
{
public:
	Unit& get(Unit& u, Army& a, Army& o) const { return u; }
};

/* Extract the lowest capactiy value unit of an army */
class ExtractorLX : public Extractor< Unit& >
{
private:
	Extractor< Army& >* _ea;
	int _capacityIndex;
public:
	ExtractorLX(Extractor< Army& >* ea, int capa_index) : _ea(ea), _capacityIndex(capa_index) {}
	Unit& get(Unit& u, Army& a, Army& o) const 
	{
		if (_capacityIndex < 0 || _capacityIndex > 6)
			throw std::invalid_argument("Capacity number not in range"); 
		return _ea->get(u, a, o).getLowestUnit(_capacityIndex);
	}
};

/* Extract the highest capactiy value unit of a point */
class ExtractorHX : public Extractor< Unit& >
{
private:
	Extractor< Army& >* _ea;
	int _capacityIndex;
public:
	ExtractorHX(Extractor< Army& >* ea, int capa_index) : _ea(ea), _capacityIndex(capa_index) {}
	Unit& get(Unit& u, Army& a, Army& o) const 
	{
		if (_capacityIndex < 0 || _capacityIndex > 6)
			throw std::invalid_argument("Capacity number not in range"); 
		return _ea->get(u, a, o).getHighestUnit(_capacityIndex);
	}
};

/* Extract the nearest unit of a point */
class ExtractorLD : public Extractor< Unit& >
{
private:
	Extractor< Army& >* _ea;
	Extractor< Point& >* _ep;
public:
	ExtractorLD(Extractor< Army& >* ea, Extractor< Point& >* ep)
		: _ea(ea), _ep(ep) {}
	Unit& get(Unit& u, Army& a, Army& o) const 
	{ return _ea->get(u, a, o).getNearestUnit(_ep->get(u, a, o)); }
};

/* Extract the furthest unit of a point */
class ExtractorHD : public Extractor< Unit& >
{
private:
	Extractor< Army& >* _ea;
	Extractor< Point& >* _ep;
public:
	ExtractorHD(Extractor< Army& >* ea, Extractor< Point& >* ep)
		: _ea(ea), _ep(ep) {}
	Unit& get(Unit& u, Army& a, Army& o) const
	{ return _ea->get(u, a, o).getFurthestUnit(_ep->get(u, a, o)); }
};

/* Point Extractor */
/* Extract the Barycenter of an army */
class ExtractorB : public Extractor< Point& >
{
private:
	Extractor< Army& >* _ea;
public:
	ExtractorB(Extractor< Army& >* ea) : _ea(ea) {}
	Point& get(Unit& u, Army& a, Army& o) const
	{
		float x = 0.f, y = 0.f;
		std::vector< std::shared_ptr< Unit > > army = _ea->get(u, a, o).getUnitsList();

		for (auto& it : army)
		{
			x += it->getPosition().getX();
			y += it->getPosition().getY();
		}

		return *(std::unique_ptr< Point > (new Point(x / static_cast<float>(army.size())
													, y / static_cast<float>(army.size()))));
	}
};

/* Extract the Position of a Unit */
class ExtractorP : public Extractor< Point& >
{
private:
	Extractor< Unit& >* _eu;
public:
	ExtractorP(Extractor< Unit& >* eu) : _eu(eu) {}
	Point& get(Unit& u, Army& a, Army& o) const { return _eu->get(u, a, o).getPosition(); }
};

/* Army Extractor */
/* Extract the ally army */
class ExtractorA : public Extractor< Army& >
{
public:
	Army& get(Unit& u, Army& a, Army& o) const { return a; }
};

/* Extract the opponent army */
class ExtractorO : public Extractor< Army& >
{
public:
	Army& get(Unit& u, Army& a, Army& o) const { return o; }
};

/* SubArmy Extractor */
/* Extract N units having the lowest capacity value */
class ExtractorNLX : public Extractor< Army >
{
private:
	Extractor< Army& >* _ea;
	int _capacityIndex;
	unsigned int _size;
public:
	ExtractorNLX(Extractor< Army& >* ea, int capa_index, int size)
		: _ea(ea), _capacityIndex(capa_index), _size(size) {}

	Army get(Unit& u, Army& a, Army& o) const
	{
		std::vector< std::shared_ptr< Unit > > subArmy;
		Army army = _ea->get(u, a, o);
		
		if (_size > army.getUnitsList().size())
			throw std::invalid_argument("Subarmy size too high");

		std::sort(army.getUnitsList().begin(), army.getUnitsList().end(),
			[this](std::shared_ptr< Unit > u1, std::shared_ptr< Unit > u2)
			{ return u1->getCapacity(_capacityIndex)->getValue() < u2->getCapacity(_capacityIndex)->getValue(); });
		std::copy(army.getUnitsList().begin(), army.getUnitsList().begin() + _size, subArmy.begin());

		return Army(subArmy);
	}
};

/* Extract _size units having the highest capacity value */
class ExtractorNHX : public Extractor< Army >
{
private:
	Extractor< Army& >* _ea;
	int _capacityIndex;
	unsigned int _size;
public:
	ExtractorNHX(Extractor< Army& >* ea, int capa_index, int size)
		: _ea(ea), _capacityIndex(capa_index), _size(size) {}

	Army get(Unit& u, Army& a, Army& o) const
	{
		std::vector< std::shared_ptr< Unit > > subArmy;
		Army army = _ea->get(u, a, o);

		if (_size > army.getUnitsList().size())
			throw std::invalid_argument("Subarmy size too high");

		std::sort(army.getUnitsList().begin(), army.getUnitsList().end(),
			[this](std::shared_ptr< Unit > u1, std::shared_ptr< Unit > u2)
			{ return u1->getCapacity(_capacityIndex)->getValue() > u2->getCapacity(_capacityIndex)->getValue(); });
		std::copy(army.getUnitsList().begin(), army.getUnitsList().begin() + _size, subArmy.begin());

		return Army(subArmy);
	}
};

/* Extract _size units having the lowest distance to a point */
class ExtractorNLD : public Extractor< Army >
{
private:
	Extractor< Army& >* _ea;
	Extractor< Point& >* _ep;
	unsigned int _size;
public:
	ExtractorNLD(Extractor< Army& >* ea, Extractor< Point& >* ep, int size)
		: _ea(ea), _ep(ep), _size(size) {}

	Army get(Unit& u, Army& a, Army& o) const
	{
		std::vector< std::shared_ptr< Unit > > subArmy;
		Army army = _ea->get(u, a, o);

		if (_size > army.getUnitsList().size())
			throw std::invalid_argument("Subarmy size too high");

		Point point(_ep->get(u, a, o));
		std::sort(army.getUnitsList().begin(), army.getUnitsList().end(),
			[this, &point](std::shared_ptr< Unit > u1, std::shared_ptr< Unit > u2)
			{ return u1->getPosition().distance(point) < u2->getPosition().distance(point); });
		std::copy(army.getUnitsList().begin(), army.getUnitsList().begin() + _size, subArmy.begin());

		return Army(subArmy);
	}
};

/* Extract _size units having the highest distance to a point */
class ExtractorNHD : public Extractor< Army >
{
private:
	Extractor< Army& >* _ea;
	Extractor< Point& >* _ep;
	unsigned int _size;
public:
	ExtractorNHD(Extractor< Army& >* ea, Extractor< Point& >* ep, int size)
		: _ea(ea), _ep(ep), _size(size) {}

	Army get(Unit& u, Army& a, Army& o) const
	{
		std::vector< std::shared_ptr< Unit > > subArmy;
		Army army = _ea->get(u, a, o);

		if (_size > army.getUnitsList().size())
			throw std::invalid_argument("Subarmy size too high");

		Point point(_ep->get(u, a, o));
		std::sort(army.getUnitsList().begin(), army.getUnitsList().end(),
			[this, &point](std::shared_ptr< Unit > u1, std::shared_ptr< Unit > u2)
			{ return u1->getPosition().distance(point) > u2->getPosition().distance(point); });
		std::copy(army.getUnitsList().begin(), army.getUnitsList().begin() + _size, subArmy.begin());

		return Army(subArmy);
	}
};

/* Extract all Unit where Capacity value is lower than the threshold */
class ExtractorTLX : public Extractor< Army >
{
private:
	Extractor< Army& >* _ea;
	int _capacityIndex;
	float _value;
public:
	ExtractorTLX(Extractor< Army& >* ea, int capa_index, float value)
		: _ea(ea), _capacityIndex(capa_index), _value(value) {}

	Army get(Unit& u, Army& a, Army& o) const
	{
		std::vector< std::shared_ptr< Unit > > subArmy;
		Army army = _ea->get(u, a, o);

		std::copy_if(_ea->get(u, a, o).getUnitsList().begin(), 
					_ea->get(u, a, o).getUnitsList().end(), 
					subArmy.begin(),
					[this](std::shared_ptr< Unit > u)
					{ return u->getCapacity(_capacityIndex)->getValue() < _value; }
					);

		return Army(subArmy);
	}
};

/* Extract all Unit where Capacity value is higher than the threshold */
class ExtractorTHX : public Extractor< Army >
{
private:
	Extractor< Army& >* _ea;
	int _capacityIndex;
	float _value;
public:
	ExtractorTHX(Extractor< Army& >* ea, int capa_index, float value)
		: _ea(ea), _capacityIndex(capa_index), _value(value) {}

	Army get(Unit& u, Army& a, Army& o) const
	{
		std::vector< std::shared_ptr< Unit > > subArmy;

		std::copy_if(_ea->get(u, a, o).getUnitsList().begin(), 
					_ea->get(u, a, o).getUnitsList().end(), 
					subArmy.begin(),
					[this](std::shared_ptr< Unit > u)
					{ return u->getCapacity(_capacityIndex)->getValue() > _value; }
					);

		return Army(subArmy);
	}
};

#endif //__EXTRACTOR_HPP__