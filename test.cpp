#include <iostream>
#include <memory>
#include <vector>

using namespace std;

// Item class
class item_class 
{
public:
  string name;
  string description;
  int cost;
  int effect;
  
  item_class(string name, string description)
    : name(name), description(description)
  {
  }

  virtual void use(int& value)
  {
    // This section deliberately left blank
    // This is a virtual function that makes item_class polymorphic.
    // This virtual function will be overridden by the subclass, e.g., potion_item_subclass.
    // The overridden use function in the subclass will be used in check_item() to apply the specific effect of that item.
  }
};

class potion_item_subclass : public item_class
{
public:
  potion_item_subclass(string name, string description)
    : item_class(name, description)
  {
  }
  
  void use(int& value) override
  {
    // Apply the effect of the potion item on the value
    // ...
  }
};

int main()
{
    vector<unique_ptr<item_class>> inventory_vector;
    int lives = 0;
    int item_select_variable = 0;

    inventory_vector.push_back(make_unique<potion_item_subclass>("Health Potion", "Restores health."));
    unique_ptr<item_class>& item = inventory_vector[item_select_variable];
    potion_item_subclass* potion = dynamic_cast<potion_item_subclass*>(item.get());
    if (potion)
    {
        potion->use(lives);
    }
    else
    {
        cout << "Selected item is not a potion." << endl;
    }
    cout << "test" << endl;

    return 0;
}
