#include <iostream>
#include <vector>
#include <string>
#include<iomanip>
#include <list>
#include <algorithm>
#include <deque>
#include <stdexcept>
using namespace std;
class BarException : public exception {
    string message;
public:
    explicit BarException(const string& message) : message(message) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};
class StockException : public BarException {
public:
    explicit StockException(const string& message) : BarException(message) {}
};

class OrderException : public BarException {
public:
    explicit OrderException(const string& message) : BarException(message) {}
};
class Client;
int readInt(const string& prompt, int minimum, int maximum);
Client* createClient();
enum DrinkCategory {
    Beer,Wine,Spirit,Cocktail,NonAlcoholic
};

string categoryName(DrinkCategory c) {
    switch (c) {
        case Beer: return "Beer";
        case Wine: return "Wine";
        case Spirit: return "Spirit";
        case Cocktail: return "Cocktail";
        case NonAlcoholic: return "NonAlcoholic";
    }
    return "";
}
class Drink {
    string name;
    DrinkCategory category;
    double alcoholImpact;
    int volume;
    int minAge;
    int stock;
public:
    ~Drink() = default;
    Drink(string name = "Water",DrinkCategory c = NonAlcoholic,double i = 0,int v = 0,int a = 0,int inits = 0)
        : name(name), category(c), alcoholImpact(i), volume(v), minAge(a), stock(inits) {}
    Drink(const Drink& other): name(other.name), category(other.category), alcoholImpact(other.alcoholImpact), volume(other.volume), minAge(other.minAge), stock(other.stock) {}
    Drink& operator=(const Drink& other) {
        if (this != &other) {
            name = other.name;
            category = other.category;
            alcoholImpact = other.alcoholImpact;
            volume = other.volume;
            minAge = other.minAge;
            stock = other.stock;
        }
        return *this;
    }
    const string& getName() const { return name;}
    DrinkCategory getCategory() const {return category;}
    double getAlcoholImpact() const {return alcoholImpact;}
    int getvolume() const {return volume;}
    int getMinAge() const {return minAge;}
    int getStock() const {return stock;}
    bool isAvalible() const { return stock > 0;}
    bool isAlcohol() const { return alcoholImpact > 0;}
    void restock(int c) {
        if (c <= 0) throw StockException("Restock amount must be positive");
        stock += c;
    }
    void sell() {
        if (stock <= 0) throw StockException("Drink is out of stock");
        stock-- ;
    }
    Drink& operator+=(int c) {
        restock(c);
        return *this;
    }
    bool operator<(const Drink& other) const {
        return alcoholImpact < other.alcoholImpact;
    }
    void show(int i) const {
        cout<<i<<". "<<*this<<"\n";
    }
    friend bool operator==(const Drink& a, const Drink& b){
        return a.name == b.name && a.category == b.category;
    }
    friend ostream& operator<<(ostream& out, const Drink& drink) {
        out << left << setw(18) << drink.name
            << " | " << setw(13) << categoryName(drink.category)
            << " | " << setw(5) << drink.volume << " ml"
            << " | impact: " << setw(5) << drink.alcoholImpact
            << " | age: " << setw(2) << drink.minAge
            << " | stock: " << drink.stock;
        return out;
    }
    friend istream& operator>>(istream& in, Drink& drink) {
        int category;
        cout << "Drink name: ";
        in >> drink.name;
        cout << "Category (1 Beer, 2 Wine, 3 Spirit, 4 Cocktail, 5 Non-alcoholic): ";
        in >> category;
        if (category == 1) drink.category = Beer;
        else if (category == 2) drink.category = Wine;
        else if (category == 3) drink.category = Spirit;
        else if (category == 4) drink.category = Cocktail;
        else drink.category = NonAlcoholic;
        cout << "Alcohol impact: ";
        in >> drink.alcoholImpact;
        cout << "Volume ml: ";
        in >> drink.volume;
        cout << "Minimum age: ";
        in >> drink.minAge;
        cout << "Stock: ";
        in >> drink.stock;
        return in;
    }
};

class Supplier {
    string name;
    string phone;
    vector<DrinkCategory> suppliedCategories;
public:
    Supplier(string name = "Default Supplier", string phone = "unknown")
        : name(name), phone(phone) {}
    void addCategory(DrinkCategory category) {
        if (find(suppliedCategories.begin(), suppliedCategories.end(), category) == suppliedCategories.end()) {
            suppliedCategories.push_back(category);
        }
    }
    bool canSupply(DrinkCategory category) const {
        return find(suppliedCategories.begin(), suppliedCategories.end(), category) != suppliedCategories.end();
    }
    void show() const {
        cout << "\n=== SUPPLIER ===\n";
        cout << "Name: " << name << "\n";
        cout << "Phone: " << phone << "\n";
        cout << "Supplied categories: ";
        if (suppliedCategories.empty()) {
            cout << "none";
        } else {
            for (auto category : suppliedCategories) {
                cout << categoryName(category) << " ";
            }
        }
        cout << "\n";
    }
};

class Client {
    static int total;
    string name;
    int age;
    double alcoholLevel;
    double tolerance;
    bool present;
public:
    virtual double preferredLimit() const = 0;
    virtual double preferanceFor(const Drink& d) const  = 0;
    Client(string name="Client", int a=18,double t=1)
        :name(name), age(a), alcoholLevel(0), tolerance(t), present(true) {++total;}
    Client(const Client& other): name(other.name), age(other.age), alcoholLevel(other.alcoholLevel),tolerance(other.tolerance),present(other.present) { ++total;}
    Client& operator=(const Client& other) {
        if (this != &other) {
            name = other.name;
            age =other.age;
            alcoholLevel = other.alcoholLevel;
            tolerance = other.tolerance;
            present = other.present;
        }
        return *this;
    }
    virtual ~Client() = default;
    static int getTotalCreated() {return total;}
    const string& getName() const {return name;}
    int getAge() const {return age;}
    double getvolume() const {return alcoholLevel;}
    bool getPresent() const {return present;}
    virtual string typeName() const = 0;
    bool canBuy(const Drink& d) const {
        return present && d.isAvalible() && age >= d.getMinAge();
    }
    void drink(const Drink& d) {
        if (age < d.getMinAge()) {
            throw OrderException("Client is too young");
        }
        alcoholLevel += d.getAlcoholImpact() / tolerance;
        if (alcoholLevel < 0) {
            alcoholLevel = 0;
        }
    }
    bool shouldKickOut() const {
        return alcoholLevel >= 100;
    }
    void kickOut() {
        present = false;
    }
    void betweenRounds() {
        alcoholLevel -= 3;
        if (alcoholLevel < 0) {
            alcoholLevel = 0;
        }
    }
    static string riskName(double volume) {
        if (volume < 20) return "low";
        if (volume < 60) return "medium";
        if (volume < 100) return "high";
        return "critical";
    }
    void react() const {
        cout<< name << " (" << typeName() << ") ";
        if (alcoholLevel < 20) { cout << "is calm and in control.\n";}
        else if (alcoholLevel < 50) { cout << "is typsy but still behaving normaly.\n";}
        else if (alcoholLevel < 80) {cout << "is drunk; staff should watch them.\n";}
        else { cout<<"is a risk for the bar and may need to leave.\n";}
    }
    void show() const {
        cout << left << setw(14) << name
             << " | type: " << setw(14) << typeName()
             << " | age: " << setw(2) << age
             << " | alcohol: " << setw(6) << fixed << setprecision(1) << alcoholLevel
             << " | risk: " << setw(8) << riskName(alcoholLevel)
             << " | status: " << (present ? "inside" : "removed") << "\n";
    }
    int chooseDrink(const deque<Drink>& menu) const {
        int bestIndex = -1;
        double bestScore = -1000000;
        for (int i = 0; i < /*static_cast<int>*/(menu.size()); ++i) {
            const Drink& drink = menu[i];
            if (!canBuy(drink)) {
                continue;
            }
            double projectedLevel = alcoholLevel + drink.getAlcoholImpact() / tolerance;
            double score = preferanceFor(drink);

            if (projectedLevel > preferredLimit()) {
                score -= (projectedLevel - preferredLimit()) * 2.0;
            }
            if (drink.getCategory() == NonAlcoholic) {
                if (alcoholLevel > 45) {
                    score += 30 - drink.getAlcoholImpact();
                } else {
                    score += drink.getAlcoholImpact();
                }
            }
            if (score > bestScore) {
                bestScore = score;
                bestIndex = i;
            }
        }
        return bestIndex;
    }
};
int Client::total=0;
class CasualClient : public Client {
public:
    double preferredLimit() const { return 45;}
    double preferanceFor(const Drink& d) const {
        if (d.getCategory() == NonAlcoholic) return 65;
        if (d.getCategory() == Beer) return 45;
        if (d.getCategory() == Wine) return 35;
        if (d.getCategory() == Cocktail) return 30;
        return 5;
    }
    CasualClient(string name = "Client", int a= 18)
        : Client(name, a, 1.25) {}
    CasualClient(const CasualClient& other) : Client(other) {}
    CasualClient& operator=(const CasualClient& other) {
        Client::operator=(other);
        return *this;
    }
    string typeName() const {return "Casual";}
};
class RegularClient : public Client {
public:
    double preferredLimit() const { return 70;}
    double preferanceFor(const Drink& d) const {
        if (d.getCategory() == Wine) return 70;
        if (d.getCategory() == Beer) return 60;
        if (d.getCategory() == Cocktail) return 45;
        if (d.getCategory() == Spirit) return 25;
        return 20;
    }
    RegularClient(string name="Client", int a =  18)
        : Client(name, a , 1.0) {}
    RegularClient(const RegularClient& other)
        : Client(other) {}
    RegularClient& operator=(const RegularClient& other) {
        Client::operator=(other);
        return *this;
    }
    string typeName() const { return "Regular"; }
};
class HeavyDrinker : public Client {
public:
    double preferredLimit() const { return 95;}
    double preferanceFor(const Drink& d) const {
        if (d.getCategory() == Spirit) return 80;
        if (d.getCategory() == Cocktail) return 75;
        if (d.getCategory() == Wine) return 60;
        if (d.getCategory() == Beer) return 35;
        return 10;
    }
    HeavyDrinker(string name="Client", int a = 18)
        :Client(name,a,0.85){}
    HeavyDrinker(const HeavyDrinker& other) : Client(other) {}
    HeavyDrinker& operator=(const HeavyDrinker& other) {
        Client::operator=(other);
        return *this;
    }
    string typeName() const { return "Heavy"; }
};
int readInt(const string& prompt, int minimum, int maximum) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minimum && value <= maximum) {
            return value;
        }
        cout << "Invalid value. Try again.\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }
}
template <class T>
class History {
    list<T> items;
public:
    void add(const T& item) {
        items.push_back(item);
        if (items.size()>5) items.pop_front();
    }
    void show() const {
        if (items.empty()) {
            cout<<"Fara evenimente.\n";
            return;
        }
        for (const auto& item : items) {
            cout<<"- "<<item<<"\n";
        }
    }
};
class Bar {
    vector<Client*> clients;
    deque<Drink> menu;
    History<string> eventLog;
    Supplier* supplier;
    int totalServed;
    int totalRemoved;
    vector<Drink*> recommendedDrinks;
    Bar() : supplier(nullptr), totalServed(0), totalRemoved(0) {}
public:
    void setSupplier(Supplier* newSupplier) {
        supplier = newSupplier;
    }

    void showSupplier() const {
        if (supplier == nullptr) {
            cout << "\nNo supplier assigned.\n";
            return;
        }
        supplier->show();
    }

    void recommendDrink() {
        showMenu();
        int index = readInt("Drink number to recommend: ", 1, static_cast<int>(menu.size()));
        recommendedDrinks.push_back(&menu[index - 1]);
        cout << menu[index - 1].getName() << " is now recommended.\n";
    }

    void logEvent(const string& event) {
        eventLog.add(event);
    }
    void showEventLog() const {
        cout << "\n=== EVENT LOG ===\n";
        eventLog.show();
    }
    void showHeavyDrinkers() const {
        cout << "\n=== HEAVY DRINKERS ===\n";
        bool found = false;
        for (auto client : clients) {
            HeavyDrinker* heavy = dynamic_cast<HeavyDrinker*>(client);
            if (heavy) {
                heavy->show();
                found = true;
            }
        }
        if (!found) {
            cout << "No heavy drinkers in the bar.\n";
        }
    }
    Bar(const Bar&) = delete;
    Bar& operator=(const Bar&) = delete;

    static Bar& getInstance() {
        static Bar instance;
        return instance;
    }
    ~Bar() {
        for (auto client : clients) {
            delete client;
        }
    }
    void addDrink(const Drink& drink) {
        auto found = find(menu.begin(), menu.end(), drink);
        if (found == menu.end()) {
            menu.push_back(drink);
        } else {
            *found += drink.getStock();
        }
    }
    void addClient(Client* client) {
        logEvent(client->getName() + " entered the bar.");
        clients.push_back(client);
    }
    void addClientsFromInput() {
        int clientCount = readInt("Number of clients to add: ", 0, 100);
        for (int i = 0; i < clientCount; ++i) {
            cout << "Client " << i + 1 << "\n";
            addClient(createClient());
        }
    }
    void addDrinkFromInput() {
        Drink drink;
        cin >> drink;
        addDrink(drink);
        cout << drink.getName() << " added to menu.\n";
    }
    void showMenu() const {
        cout << "\n=== RECOMMENDED DRINKS ===\n";
        if (recommendedDrinks.empty()) {
            cout << "No recommended drinks.\n";
        } else {
            for (auto drink : recommendedDrinks) {
                cout << "- " << drink->getName() << "\n";
            }
        }

        cout << "\n=== MENU ===\n";
        for (int i = 0; i < static_cast<int>(menu.size()); ++i) {
            menu[i].show(i + 1);
        }
    }
    void showStatus() const {
        cout << "\n=== BAR STATUS ===\n";
        cout << "Drinks served: " << totalServed << "\n";
        cout << "Clients removed: " << totalRemoved << "\n";
        cout << "Clients created: " << Client::getTotalCreated() << "\n";
        cout << "Clients inside: " << clients.size() << "\n\n";
        if (clients.empty()) {
            cout << "No clients are currently inside.\n";
            return;
        }
        for (auto client : clients) {
            client->show();
        }
    }
    void simulateRound() {
        if (clients.empty()) {
            cout << "\nNo clients left in the bar.\n";
            return;
        }
        cout << "\n--- ROUND START ---\n";

        for (auto client : clients) {
            int choice = client->chooseDrink(menu);

            if (choice == -1) {
                cout << client->getName()
                     << " cannot buy anything suitable right now.\n";
                client->betweenRounds();
                continue;
            }

            try {
                Drink& drink = menu[choice];
                drink.sell();
                client->drink(drink);
                ++totalServed;
                logEvent(client->getName() + " ordered " + drink.getName() + ".");

                cout << client->getName() << " drinks " << drink.getName()
                     << ". Alcohol level is now "
                     << fixed << setprecision(1) << client->getvolume() << ".\n";

                client->react();
                if (client->shouldKickOut()) {
                    cout << "Security removes " << client->getName()
                         << " from the bar.\n";
                    client->kickOut();
                    ++totalRemoved;
                }
            } catch (BarException& error) {
                cout << "Order refused: " << error.what() << "\n";
                logEvent(string("Order refused: ") + error.what());
            }
        }
        clients.erase(
            remove_if(clients.begin(), clients.end(),
                      [](Client* client) {
                          if (!client->getPresent()) {
                              delete client;
                              return true;
                          }
                          return false;
                      }),
            clients.end());
        cout << "--- ROUND END ---\n";
    }
    void restockDrink() {
        showMenu();
        int index = readInt("Drink number to restock: ", 1, static_cast<int>(menu.size()));
        int amount = readInt("Amount to add: ", 1, 1000);
        try {
            if (supplier == nullptr) {
                throw StockException("No supplier assigned");
            }
            if (!supplier->canSupply(menu[index - 1].getCategory())) {
                throw StockException("Supplier cannot provide this drink category");
            }
            menu[index - 1] += amount;
            cout << menu[index - 1].getName() << " restocked by " << amount << ".\n";
        } catch (const BarException& error) {
            cout << "Restock failed: " << error.what() << "\n";
        }
    }
    void loadDefaultBar() {
        addDrink(Drink("Draft Lager", Beer, 13.0, 500, 18, 12));
        addDrink(Drink("House Wine", Wine, 19.0, 150, 18, 8));
        addDrink(Drink("Vodka Shot", Spirit, 32.0, 50, 18, 10));
        addDrink(Drink("Gin Tonic", Cocktail, 24.0, 250, 18, 6));
        addDrink(Drink("Still Water", NonAlcoholic, -9.0, 500, 0, 20));
        addDrink(Drink("Orange Juice", NonAlcoholic, -6.0, 330, 0, 10));
    }
};
Client* createClient() {
    string name;
    cout << "Name: ";
    cin >> name;
    int age = readInt("Age: ", 1, 120);
    cout << "Type (1 Casual, 2 Regular, 3 Heavy): ";
    int type = readInt("", 1, 3);
    if (type == 1) {
        return new CasualClient(name, age);
    }
    if (type == 2) {
        return new RegularClient(name, age);
    }
    return new HeavyDrinker(name, age);
}

int main() {
    Bar& bar = Bar::getInstance();
    bar.loadDefaultBar();
    Supplier supplier("Local Drinks SRL", "0722123456");
    supplier.addCategory(Beer);
    supplier.addCategory(Wine);
    supplier.addCategory(Spirit);
    supplier.addCategory(Cocktail);
    supplier.addCategory(NonAlcoholic);
    bar.setSupplier(&supplier);

    cout << "Welcome to Viktor's Bar\n";
    cout << "======================\n";
    int choice;
    int round = 0;
    do {
        cout << "\n1. Add clients\n";
        cout << "2. Simulate round\n";
        cout << "3. Show bar status\n";
        cout << "4. Show menu\n";
        cout << "5. Admin menu\n";
        cout << "0. Exit\n";
        choice = readInt("Choice: ", 0,5 );
        if (choice == 1)
            bar.addClientsFromInput();
        else if (choice == 2) {
            bar.simulateRound();
            round++;
        }
        else if (choice == 3)
            bar.showStatus();
        else if (choice == 4)
            bar.showMenu();
        else if (choice == 5) {
                int adminChoice;
                do {
                    cout << "\n=== ADMIN MENU ===\n";
                    cout << "1. Add drink\n";
                    cout << "2. Restock drink\n";
                    cout << "3. Recommend drink\n";
                    cout << "4. Show event log\n";
                    cout << "5. Show heavy drinkers\n";
                    cout << "6. Show supplier\n";
                    cout << "0. Back\n";
                    adminChoice = readInt("Choice: ", 0, 6);

                    if (adminChoice == 1)
                        bar.addDrinkFromInput();
                    else if (adminChoice == 2)
                        bar.restockDrink();
                    else if (adminChoice == 3)
                        bar.recommendDrink();
                    else if (adminChoice == 4)
                        bar.showEventLog();
                    else if (adminChoice == 5)
                        bar.showHeavyDrinkers();
                    else if (adminChoice == 6)
                        bar.showSupplier();
                } while (adminChoice != 0);
            }
    } while (choice != 0 && round <= 10);
    cout << "======================\n";
    cout << "Closing bar.\n";
    return 0;
}


