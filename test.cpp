#include <iostream>
#include <vector>
#include <string>

using namespace std;

class item_class 
{
public:
  string name;
  string description;
  int cost;
  int effect;
  item_class(string name, string description)
  {
    this->name = name;
    this->description = description;
  }
  virtual void use(int& value)
  {
    /*
    This section deliberately left blank
    This is Virtual function that makes this item_class polymorphic.
    This virtual function will be overwritten by the subclass e.g. potion_item_subclass
    */
  }
};

class potion_item_subclass : public item_class
{
public:
  potion_item_subclass() : item_class("Potion", "When used increases players life by 1 by regenerating their body.") {}
  void use(int& lives)
  {
    cout << "You used a Potion. 1 life gained" << endl;
    lives++;
  }
};

class leather_boots_item_subclass : public item_class
{
public:
  leather_boots_item_subclass() : item_class("Leather boots", "When used increases players speed by 1 by cushioning their steps.") {}
  void use(int& speed)
  {
    cout << "You wore leather boots. Speed permanently increased by 1" << endl;
    speed++;
  }
};

/* Global Vectors*/
vector<item_class*> inventory_vector;

/* Global variables */
int lives = 3;
int speed = 1;

void check_items()
{
  cout << "INVENTORY" <<endl;
  for (int i=0;i<inventory_vector.size(); i++)
  {
    cout << i << ": " << inventory_vector[i]->name << endl;
  }

  // using an item
  int item_select_variable;
  cout << "Select item you wish to use: " << endl;
  cin >> item_select_variable;

  if (item_select_variable >= 0 && item_select_variable < inventory_vector.size())
  {
    cout << "Using item: " << inventory_vector[item_select_variable]->name << endl;

    if (inventory_vector[item_select_variable]->name == "Potion")
    {
      potion_item_subclass* potion = dynamic_cast<potion_item_subclass*>(inventory_vector[item_select_variable]);
      potion->use(lives);
    }
    else if (inventory_vector[item_select_variable]->name == "Leather boots")
    {
      leather_boots_item_subclass* boots = dynamic_cast<leather_boots_item_subclass*>(inventory_vector[item_select_variable]);
      boots->use(speed);
    }

    inventory_vector.erase(inventory_vector.begin() + item_select_variable);
  }
  else
  {
    cout << "Invalid item index option" << endl;
  }
}

int main() 
{
    // Adding healing medicine item to players inventory vector
    inventory_vector.push_back(new potion_item_subclass());
    inventory_vector.push_back(new leather_boots_item_subclass());

    cout << "Lives are currently: " << lives << endl;
    cout << "Speed is currently: " << speed << endl;

    check_items();
    
    cout << "Lives are now: " << lives << endl;
    cout << "Speed is now: " << speed << endl;

    return 0;
}
