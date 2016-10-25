#include "CBR.h"

Case::Case(float m_s, float m_h, int o_t, float o_si, float o_sp, float d, int res_a, float i, float res_s)
{
	my_size = m_s;
	my_hunger = m_h;

	other_type = o_t;
	other_size = o_si;
	other_speed = o_sp;
	
	distance = d;
	importance = i;

	resulting_action = res_a;
	resulting_speed = res_s;
	resulting_interacting_object = nullptr;
	resulting_weight = 0.0;
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
			if(a->get_type() == me->get_type() || (i_am_herbivore && !a->is_alive())) continue; // skip cases with the same type, or with dead animals
			for(auto& c : cases) {
				c.resulting_weight = 0.0; // reset previous results
				float w_myhunger = c.my_hunger != -1 ? (1.0 - min(abs(c.my_hunger - me->get_hunger()), 1.0f) * 2.0) * 0.2 : 0.2;
				float w_type = c.other_type == a->get_type() ? 0.4 : 0.0;
				w_type += (!i_am_herbivore && !a->is_alive()) ? 0.2 : 0.0; // carnivores automatically considers cases with dead animals
				float w_speed = c.other_speed != -1 ? (1.0 - min(abs(c.other_speed - a->get_speed()) / 2.0, 1.0)) * 0.15 : 0.15;
				float w_dist = c.distance != -1 ? (1 - min(abs(c.distance - dist(me->get_x(), me->get_y(), a->get_x(), a->get_y())) / 200.0f, 1.0f)) * 0.15 : 0.15;
				float w_size = c.other_size != -1 ? (1 - min(abs(c.other_size - a->get_size()) / 3.0, 1.0)) * 0.1 : 0.1;
				c.resulting_weight = (w_myhunger + w_type + w_speed + w_dist + w_size) * c.importance;

				if(c.resulting_action == FLEE) c.resulting_weight *= me->get_fear_factor();

				//if(!i_am_herbivore)
				//cout << "case = " << c.distance << " - " << (abs(c.distance - dist(me->get_x(), me->get_y(), a->get_x(), a->get_y()))) << ": h=" << w_myhunger << ", t=" << w_type << ", sp=" << w_speed << ", si=" << 
				//	w_size << ", d=" << w_dist << " : " << w_sum << endl;
				//cout << "case distance: " << c.distance << ", " << (dist(me->get_x(), me->get_y(), a->get_x(), a->get_y())) << endl;

				if(c.resulting_weight > 0.4 || (!i_am_herbivore && !a->is_alive() && (dist(me->get_x(), me->get_y(), a->get_x(), a->get_y()) < 32.0 || me->get_hunger() < 0.8))) {
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
		me->set_case_speed(res->resulting_speed);
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
	
	float highest_weight = (me->get_action() != ATTACK) ? 0.0 : 0.9; // if we are already attacking something, it will take a pretty high value to change state

	for(auto& c : cases) {
		if(auto other = dynamic_cast<Animal*>(c.resulting_interacting_object)) {
			//cout << "reasoning about case: " << c.other_type << " - " << c.other_speed << ", " << c.other_size << endl;
			if(c.resulting_action == ATTACK && c.resulting_weight > highest_weight /*dist(me->get_x(), me->get_y(), other->get_x(), other->get_y()) < shortest_dist*/) {
				shortest_dist = dist(me->get_x(), me->get_y(), other->get_x(), other->get_y());
				highest_weight = c.resulting_weight;
				res = &c;
				me->set_interacting_object(other);
			}
		}
	}

	if(res != nullptr) {
		//adapt or add new case based on res if neccessary
		adapt_case(res);

		me->set_case_speed(res->resulting_speed);
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
	else if(me->get_action() == ATTACK) {
		return ATTACK;
	}
	
	//in case we didn't find any cases (shouldn't happen)
	return IDLE;
}

void CBR::adapt_case(Case* c)
{
	
	// On average how close will the retrieved case be to the query case?
	// How many characteristics will differ between the cases in the usual situation?
	// Are there commonsense or otherwise known rules that can be applied to do the adaptation?
	if(c->resulting_interacting_object == nullptr) return;
	auto me = dynamic_cast<Animal*>(me_obj);
	auto a = dynamic_cast<Animal*>(c->resulting_interacting_object);

	//recalculate weights, now in scale 0-1 for all
	float w_myhunger = c->my_hunger != -1 ? (1.0 - min(abs(c->my_hunger - me->get_hunger()), 1.0f) * 2.0) : 0.0;
	float w_speed = c->other_speed != -1 ? (1.0 - min(abs(c->other_speed - a->get_speed()) / 2.0, 1.0)) : 0.0;
	float w_dist = c->distance != -1 ? (1 - min(abs(c->distance - dist(me->get_x(), me->get_y(), a->get_x(), a->get_y())) / 200.0f, 1.0f)) : 0.0;
	float w_size = c->other_size != -1 ? (1 - min(abs(c->other_size - a->get_size()) / 3.0, 1.0)) : 0.0;

	if(c->resulting_weight < 0.9) {
		add_case(Case( -1,
			w_myhunger < 0.1 ? -1 : me->get_hunger(),
			a->get_type(),
			w_speed < 0.1 ? -1 : a->get_speed(),
			w_size < 0.1 ? -1 : a->get_size(),
			w_dist < 0.1 ? -1 : dist(me->get_x(), me->get_y(), a->get_x(), a->get_y()),
			c->resulting_action
		));
	}
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

			add_case(Case(-1, -1, -1, 1.2, 5.0, 300, FLEE)); // flee from anything that is close and moves fast
			add_case(Case(-1, -1, -1, 1.2, -1, 100, FLEE)); // flee from anything that is very close
			add_case(Case(-1, -1, -1, 1.2, 5.0, 100, FLEE, 1.5, 0.8)); // flee from anything that is very close and moves fast (high priority)
			break;
		}
		case WOLF:
		{
			add_case(Case(1.0, 0.5, -1, 1.0, 1.0, -1, ATTACK, 1.0, 0.5)); // hunger medium, other speed medium
			add_case(Case(1.0, 0.2, -1, 1.0, 1.0, -1, ATTACK, 1.0, 0.5)); // hunger low, other speed medium
			add_case(Case(1.0, 0.5, -1, 1.0, 0.5, -1, ATTACK, 1.0, 0.5)); // hunger medium, other speed low
			add_case(Case(1.0, 0.2, -1, 1.0, 0.5, -1, ATTACK, 1.0, 0.75)); // hunger low, other speed low
			add_case(Case(1.0, 0.2, -1, 1.0, -1, 100, ATTACK, 1.5, 0.75)); // hunger low, other nearby (high priority)
			break;
		}

		case BEAR:
		{
			add_case(Case(1.0, 0.5, -1, -1, 1.0, -1, ATTACK, 1.0, 0.5)); // hunger medium, other speed medium
			add_case(Case(1.0, 0.2, -1, -1, 1.0, -1, ATTACK, 1.0, 0.5)); // hunger low, other speed medium
			add_case(Case(1.0, 0.5, -1, -1, 0.5, -1, ATTACK, 1.0, 0.5)); // hunger medium, other speed low
			add_case(Case(1.0, 0.2, -1, -1, 0.5, -1, ATTACK, 1.0, 0.75)); // hunger low, other speed low
			add_case(Case(1.0, 0.2, -1, 1.0, -1, 50, ATTACK, 1.5, 0.75)); // hunger low, other nearby (high priority)
			break;
		}
	}
}

int CBR::get_num_cases()
{
	return cases.size();
}