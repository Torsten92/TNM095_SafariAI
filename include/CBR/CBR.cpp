#include "CBR.h"

Case::Case(float m_s, float m_h, int o_t, float o_si, float o_sp, float d, int res_a, float res_s)
{
	my_size = m_s;
	my_hunger = m_h;

	other_type = o_t;
	other_size = o_si;
	other_speed = o_sp;

	resulting_action = res_a;
	resulting_speed = res_s;
	resulting_interacting_object = nullptr;
}

CBR::CBR(Object* _me)
	: me_obj{_me}	
{}


int CBR::retrieve(vector<Object*> nearby_animals)
{
	auto me = dynamic_cast<Animal*>(me_obj); // needs to re-casted here because the type Animal can not be declared before CBR at creation because it uses it.
	
	vector<Case> similar_cases;

	//Grass eating is still hardcoded
	Object* eating_object = nullptr;
	if(dynamic_cast<Grass*>(me->get_interacting_object())) {
		eating_object = me->get_interacting_object();
	}


	for(auto o : nearby_animals)
	{
		//Check for possible grass to eat
		if(i_am_herbivore) {
			if(o->get_type() == GRASS && dist(me->get_x(), me->get_y(), o->get_x(), o->get_y()) < 32.0) {
				eating_object = o;
			}
		}
		if(Animal* a = dynamic_cast<Animal*>(o)) {
			if(a->get_type() == me->get_type()) continue; // should not exist, but skip cases with the same type
			for(auto& c : cases) {
				float w_type = c.other_type == a->get_type() ? 0.5 : 0.0;
				float w_speed = c.other_speed != -1 ? (1 - min(abs(c.other_speed - a->get_speed()) / 5.0, 1.0)) * 0.2 : 0.2;
				float w_dist = c.distance != -1 ? min(abs(c.distance - dist(me->get_x(), me->get_y(), a->get_x(), a->get_y())) / 500.0, 1.0) * 0.2 : 0.2;
				float w_size = c.other_size != -1 ? (1 - min(abs(c.other_size - a->get_size()) / 3.0, 1.0)) * 0.1 : 0.1;
				
				if(w_type + w_speed + w_dist + w_size > 0.4) {
					c.resulting_interacting_object = o; // the animal this case currrently points to (not part of the case per se)
					similar_cases.push_back(c);
				}
			}
		}
	}

	// if we found no similar cases, do the hardcoded options
	if(!similar_cases.size()) {
		if(me->get_hunger() > 0.8 && me->get_interacting_object() == nullptr) {
			return IDLE;
		}
		else if(eating_object != nullptr) {
			me->set_interacting_object(eating_object);
			return EAT;
		}
		else {
			return FIND_FOOD;
		}
	}
	else {
		return i_am_herbivore ? reason_herbivore(similar_cases) : reason_carnivore(similar_cases);
	}
}

// adapt the cases to either use a modified version of one of these cases, or make a completely new 
// case, adding it to the case-base, and then returning its solution.
int CBR::reason_herbivore(vector<Case> cases)
{
	auto me = dynamic_cast<Animal*>(me_obj);
	Case* res = nullptr;
	float shortest_dist = FLT_MAX; // somehow this should be change to represent top priority case
	
	for(auto& c : cases) {
		if(auto other = dynamic_cast<Animal*>(c.resulting_interacting_object)) {
			//cout << "reasoning about case: " << c.other_type << " - " << c.other_speed << ", " << c.other_size << endl;
			if(c.resulting_action == FLEE && other->is_alive() && dist(me->get_x(), me->get_y(), other->get_x(), other->get_y()) < shortest_dist) {
				shortest_dist = dist(me->get_x(), me->get_y(), other->get_x(), other->get_y());
				res = &c;
				me->set_interacting_object(other);
			}
		}
	}

	if(res != nullptr) {
		if(res->resulting_action == FLEE) {
			return FLEE;
		}

		return res->resulting_action;
	}

	//in case we didn't find any cases (shouldn't happen)
	return IDLE;
}

// adapt the cases to either use a modified version of one of these cases, or make a completely new 
// case, adding it to the case-base, and then returning its solution.
int CBR::reason_carnivore(vector<Case> cases)
{
	auto me = dynamic_cast<Animal*>(me_obj);
	Case* res = nullptr;
	float shortest_dist = FLT_MAX; // somehow this should be change to represent top priority case

	for(auto& c : cases) {
		if(auto other = dynamic_cast<Animal*>(c.resulting_interacting_object)) {
			//cout << "reasoning about case: " << c.other_type << " - " << c.other_speed << ", " << c.other_size << endl;
			if(c.resulting_action == ATTACK && dist(me->get_x(), me->get_y(), other->get_x(), other->get_y()) < shortest_dist) {
				shortest_dist = dist(me->get_x(), me->get_y(), other->get_x(), other->get_y());
				res = &c;
				me->set_interacting_object(other);
			}
		}
	}

	if(res != nullptr) {
		if(res->resulting_action == ATTACK && shortest_dist < 32.0) {
			auto other = dynamic_cast<Animal*>(res->resulting_interacting_object);
			if(other->is_alive()) {
				return FIGHT;
			}
			else {
				return EAT;
			}
		}

		return res->resulting_action;
	}
	
	//in case we didn't find any cases (shouldn't happen)
	return IDLE;
}

void CBR::adapt_case(Case c)
{

}

void CBR::add_case(Case c)
{
	cases.push_back(c);
}

void CBR::init(int type)
{
	//auto me = dynamic_cast<Animal*>(me_obj);
	switch(type)
	{
		case DEER:
		{
			i_am_herbivore = true;

			add_case(Case(1.0, -1, WOLF, 1.0, 1.0, 300, FLEE));
			break;
		}
		case WOLF:
		{
			add_case(Case(1.0, 0.5, DEER, 1.0, 1.0, -1, ATTACK));
			break;
		}
	}
}