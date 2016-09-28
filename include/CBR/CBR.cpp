#include "CBR.h"

CBR::CBR(Object* _me)
	: me_obj{_me}	
{}

int CBR::retrieve(WorldState s)
{
	auto me = dynamic_cast<Animal*>(me_obj); // neccessary because Animal can not be declared before CBR at creation because it uses it.
	for(auto o : s.nearby_animals)
	{
		if(o->get_type() == GRASS && dist(me->get_x(), me->get_y(), o->get_x(), o->get_y()) < 10.0) {
			me->set_interacting_object(o);
			return EAT; //TODO: this is too hardcoded, but animal may only enter eat state if distance is this close
		}
	}

	if(me->get_hunger() > 0.8)
		return IDLE;

	return FIND_FOOD;
}

void CBR::add_case(Case c)
{
	cases.push_back(c);
}

void CBR::init(int type)
{
	if(type == DEER)
	{
		add_case(Case(WorldState(vector<Object*>()), FIND_FOOD));
		add_case(Case(WorldState(vector<Object*>()), FIND_FOOD));
		add_case(Case(WorldState(vector<Object*>()), IDLE));
	}
}