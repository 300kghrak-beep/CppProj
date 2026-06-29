#include <iostream>
#include "Gorilla.h"
#define abstract class

class Character;

abstract Item {
protected:
    MyString name;
    int weigth;
    int value;
    int iconId;
public:
    virtual bool use(Character& user) = 0;
    virtual void onEquip(Character& owner) {}
    virtual void onUnequip(Character& owner) {}
    [[nodiscard]] virtual Item* clone() const = 0;


    int Weigth() const {
        return weigth;
    };
    int Value() const {
        return value;
    }
    const MyString& Name() const {
        return name;
    }

    virtual ~Item() = default;
};

class Inventory {
private:
    Item** items;
    int size;
    int capacity;
public:
    Inventory() {
        size = 0;
        capacity = 20;
        items = new Item*[capacity] {};
    }

    Inventory(Inventory&& other) noexcept {
        items = other.items;
        size = other.size;
        capacity = other.capacity;

        other.items = nullptr;
        other.size = 0;
        other.capacity = 0;
    }


    Inventory& operator=(Inventory&& other) noexcept {
        if (this == &other) return *this;

        if (items) {
            for (int i = 0; i < size; i++) {
                delete items[i];
            }
            delete[] items;
        }


        items = other.items;
        size = other.size;
        capacity = other.capacity;
        other.items = nullptr;
        other.size = 0;
        other.capacity = 0;
        return *this;
    }

    void add(Item *item) {
        if (item == nullptr) return;
        if (size >= capacity) return;
        items[size++] = item;
    }
    void remove(int slot, Character& owner);

    void useItem(int slot, Character&user) {
        if (slot < 0 || slot >= size) return;
        Item* item = items[slot];
        if (item == nullptr) return;
        if (item->use(user)) {
            remove(slot, user);
        }
    }


    void sortByWeight() {
        for (int i = 0; i < size - 1; i++) {
            for (int  j = 0; j < size - i - 1; j++) {
                if (items[j]->Weigth() > items[j+1]->Weigth()) {
                    Item* temp = items[j];
                    items[j] = items[j+1];
                    items[j+1] = temp;
                }
            }
        }
    }
    void sortByName() {
      for (int i = 0; i < size - 1; i++) {
          for (int  j = 0; j < size - i - 1; j++) {
              if (items[j]->Name() > items[j+1]->Name()) {
                  Item* temp = items[j];
                  items[j] = items[j+1];
                  items[j+1] = temp;
              }
          }
      }
    };
    int getTotalWeight() {
        int weight = 0;
        for (int i = 0; i < size; i++) {
            weight += items[i]->Weigth();
        }
        return weight;
    };
    int getTotalValue() {
        int value = 0;
        for (int i = 0; i < size; i++) {
            value += items[i]->Value();
        }
        return value;
    }


    Item* operator[](int slot) {
        if (slot < 0 || slot >= size) return nullptr;
        return items[slot];
    }

    const Item* operator[](int slot) const {
        if (slot < 0 || slot >= size) return nullptr;
        return items[slot];
    }

    [[nodiscard]] int Size() const {
        return size;
    }

    Inventory(const Inventory&) = delete;
    Inventory& operator=(const Inventory&) = delete;

    ~Inventory() {
        for (int i = 0; i < size; i++) {
            delete items[i];
        }
        delete[] items;
    }
};


class Weapon;
class HealthPotion;
class Armor;

class Character {
private:
    int health;
    int maxHealth;
    int strength;
    Inventory inventory;
    Weapon* currentweapon;
    Armor* currentarmor;
    int defence;
public:
    Character(): health(0), maxHealth(0), strength(0), defence(0) {
        currentweapon = nullptr;
        currentarmor = nullptr;
    };

    Character(
    int health,
    int maxHealth,
    int strength,
    Inventory&& inventory,
    Weapon* currentweapon,
    Armor* currentarmor,
    int defence)
    : health(health),
      maxHealth(maxHealth),
      strength(strength),
      inventory(std::move(inventory)),
      currentweapon(currentweapon),
      currentarmor(currentarmor),
      defence(defence)
    {}


    Character(Character&& character) noexcept
    : health(character.health),
      maxHealth(character.maxHealth),
      strength(character.strength),
      inventory(std::move(character.inventory)),
      currentweapon(character.currentweapon),
      currentarmor(character.currentarmor),
      defence(character.defence)
    {
        character.currentweapon = nullptr;
        character.currentarmor = nullptr;
    }

    void takeDamage(int damage) {
        if (damage < 0 || health == 0) return;

        int incoming_damage = damage;
        incoming_damage -= this->defence;
        if (incoming_damage < 0) incoming_damage = 0;

        health -= incoming_damage;
        if (health < 0) health = 0;
    }

    void heal(int heal) {
        if (heal < 0) {
            return;
        }
        if (heal + health > maxHealth) {
            health = maxHealth;
        }
        else {
            health += heal;
        }
    }

    void addDamage(int damage) {
        this->strength += damage;
    }

    void removeDamage(int damage) {
        this->strength -= damage;
    }

    void addDefence(int defence) {
        this->defence += defence;
    }

    void removeDefence(int defence) {
        this->defence -= defence;
    }

    Armor* getCurrentArmor() {
        return currentarmor;
    }
    Weapon* getCurrentweapon() {
        return currentweapon;
    }

    Inventory& getInventory() {
        return inventory;
    }

    void unequip();
    void unequip_armor();

    void equip(Weapon* weapon);
    void equip_armor(Armor* armor);
    ~Character() = default;
};


class Weapon: public Item {
private:
    int damage;
    int durability;
    MyString weaponType;
public:
    Weapon(): damage(0), durability(0), weaponType("") {}
    Weapon(int damage, int durability, const MyString &weaponType) {
        this->damage = damage;
        this->durability = durability;
        this->weaponType = weaponType;
    }

    [[nodiscard]] Item* clone() const override {
        return new Weapon(*this);
    }

    bool use(Character& user) override {
        user.equip(this);
        return false;
    }

    void onEquip(Character& owner) override {
        owner.addDamage(damage);
    }

    void onUnequip(Character& owner) override {
        owner.removeDamage(damage);
    }

    [[nodiscard]] int GetDamage() const {
        return damage;
    }
};


void Character::equip(Weapon* weapon) {
    if (!weapon) return;

    if (currentweapon != nullptr)
    currentweapon->onUnequip(*this);

    currentweapon = weapon;

    currentweapon->onEquip(*this);
}


void Character::unequip() {
    if (!currentweapon) return;
    currentweapon->onUnequip(*this);

    currentweapon = nullptr;
}



class HealthPotion: public Item {
private:
    int healAmount;
public:
    HealthPotion(): healAmount(0) {}
    explicit HealthPotion(const int healAmount): healAmount(healAmount) {}
    [[nodiscard]] Item* clone() const override {
       return new HealthPotion(*this);
    }

    bool use(Character& user) override {
        user.heal(healAmount);
        return true;
    }
};



class Armor: public Item {
private:
    int defence;
    MyString armorType;
public:
    Armor(): defence(0), armorType("") {}
    Armor(int defence, const MyString& armorType) {
        this->defence = defence;
        this->armorType = armorType;
    }

    bool use(Character& user) override {
        user.equip_armor(this);
        return false;
    }

    [[nodiscard]] Item* clone() const override {
        return new Armor(*this);
    }

    void onEquip(Character& owner) override {
        owner.addDefence(defence);
    }

    void onUnequip(Character& owner) override {
        owner.removeDefence(defence);
    }
};



void Character::equip_armor(Armor *armor) {
    if (!armor) return;
    if (currentarmor != nullptr) {
        currentarmor->onUnequip(*this);
    }
    currentarmor = armor;
    currentarmor->onEquip(*this);
}


void Character::unequip_armor() {
    if (!currentarmor) return;
    currentarmor->onUnequip(*this);
    currentarmor = nullptr;
}


void Inventory::remove(int slot, Character &owner) {
    if (slot < 0 || slot >= size) return;
    if (items[slot] == nullptr) return;

    if (items[slot] == owner.getCurrentArmor() || items[slot] == owner.getCurrentweapon()) return;

    delete items[slot];

    for (int i = slot; i < size-1; i++) {
        items[i] = items[i + 1];
    }
    items[size-1] = nullptr;
    size--;
}


class KeyItem : public Item {
private:
    int questId;
    bool isConsumable;
public:
    KeyItem(): questId(0), isConsumable(false) {};
    KeyItem(const int quest,const bool isConsumable): questId(quest), isConsumable(isConsumable) {};

    bool use(Character& user) override {
        std::cout << "Quest " << questId << " completed\n";
        return isConsumable;
    }


    [[nodiscard]] Item* clone() const override {
        return new KeyItem(*this);
    }
};