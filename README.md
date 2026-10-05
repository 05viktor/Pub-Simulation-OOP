# Viktor's Bar 🍻

A console-based **C++ bar management and simulation system** built as an Object-Oriented Programming project.

The application simulates a bar where different types of clients enter, automatically choose drinks based on their preferences and alcohol tolerance, and may eventually be removed from the bar if their alcohol level becomes too high.

The project also includes stock management, suppliers, recommendations, event logging, custom exceptions, templates, polymorphism, operator overloading, and the Singleton design pattern.

## Features

- Add clients to the bar
- Multiple client types with different drinking behavior
- Automatic drink selection based on client preferences
- Alcohol level and tolerance simulation
- Age restrictions for alcoholic drinks
- Automatic removal of clients who exceed the alcohol limit
- Drink stock management
- Restocking through a supplier
- Recommended drinks system
- Event history
- Bar statistics
- Admin menu
- Custom exception handling

## Client Types

The application contains an abstract `Client` base class and three derived client types.

### Casual Client

Prefers lighter drinks and non-alcoholic options.

- Higher alcohol tolerance modifier
- Preferred alcohol level limit: `45`
- Strong preference for non-alcoholic drinks and beer

### Regular Client

A more balanced customer.

- Normal tolerance
- Preferred alcohol level limit: `70`
- Prefers wine and beer

### Heavy Drinker

Prefers stronger alcoholic drinks.

- Lower tolerance modifier
- Preferred alcohol level limit: `95`
- Strong preference for spirits and cocktails

Each client uses polymorphic functions to determine their preferred alcohol limit and score available drinks.

## Drink System

Each drink contains:

- Name
- Category
- Alcohol impact
- Volume
- Minimum required age
- Available stock

Available drink categories are:

```text
Beer
Wine
Spirit
Cocktail
NonAlcoholic
```

The default menu contains:

```text
Draft Lager
House Wine
Vodka Shot
Gin Tonic
Still Water
Orange Juice
```

Non-alcoholic drinks can reduce a client's alcohol level, while alcoholic drinks increase it.

## Automatic Drink Selection

Clients automatically choose a suitable drink from the menu.

The selection system takes into account:

- Client type
- Drink category
- Client preferences
- Current alcohol level
- Preferred alcohol limit
- Age restrictions
- Drink availability
- Alcohol tolerance

If drinking a certain beverage would push a client significantly above their preferred limit, the drink receives a lower selection score.

When a client's alcohol level is already high, non-alcoholic drinks become more attractive.

## Alcohol & Risk System

Every client has a simulated alcohol level.

Risk levels are divided into:

| Alcohol Level | Risk |
|---|---|
| `< 20` | Low |
| `20 - 59` | Medium |
| `60 - 99` | High |
| `>= 100` | Critical |

Clients also react differently depending on their current level.

Examples include:

```text
is calm and in control
is tipsy but still behaving normally
is drunk; staff should watch them
is a risk for the bar and may need to leave
```

If the alcohol level reaches `100`, security removes the client from the bar.

Between some actions, the client's alcohol level can also decrease.

## Supplier System

The bar can be connected to a `Supplier`.

A supplier contains:

- Name
- Phone number
- Supported drink categories

A drink can only be restocked if the current supplier supports its category.

The default supplier is:

```text
Local Drinks SRL
```

and supports all available drink categories.

## Admin Menu

The application contains a separate administration menu.

Available actions:

```text
1. Add drink
2. Restock drink
3. Recommend drink
4. Show event log
5. Show heavy drinkers
6. Show supplier
0. Back
```

### Recommended Drinks

An administrator can select drinks from the menu and mark them as recommended.

Recommended drinks are displayed separately above the regular menu.

### Heavy Drinker Detection

The admin menu can display only clients of type `HeavyDrinker`.

This feature uses `dynamic_cast` to identify objects at runtime.

## Event Log

The application stores recent events such as:

```text
Alex entered the bar.
Alex ordered Draft Lager.
Order refused: Drink is out of stock.
```

The log uses the generic template:

```cpp
template <class T>
class History
```

and stores the latest five events.

## Exception Handling

The project defines its own exception hierarchy.

```text
std::exception
     |
 BarException
   /        \
StockException
OrderException
```

### `StockException`

Used for situations such as:

- Trying to sell an out-of-stock drink
- Invalid restock amount
- Missing supplier
- Supplier unable to provide a drink category

### `OrderException`

Used when an order cannot be completed, such as when a client does not meet the minimum age requirement.

## Object-Oriented Programming Concepts

The project demonstrates several important C++ OOP concepts.

### Inheritance

```cpp
Client
├── CasualClient
├── RegularClient
└── HeavyDrinker
```

Custom exceptions also use inheritance.

```cpp
BarException
├── StockException
└── OrderException
```

### Polymorphism

The `Client` class contains pure virtual functions:

```cpp
virtual double preferredLimit() const = 0;
virtual double preferanceFor(const Drink& d) const = 0;
virtual string typeName() const = 0;
```

Each client type implements its own behavior.

### Abstract Classes

`Client` is an abstract class and cannot be instantiated directly.

### Dynamic Casting

`dynamic_cast` is used to detect `HeavyDrinker` objects when displaying heavy drinkers.

### Operator Overloading

The `Drink` class overloads several operators.

```cpp
Drink& operator+=(int);
bool operator<(const Drink&) const;
bool operator==(const Drink&, const Drink&);
ostream& operator<<(ostream&, const Drink&);
istream& operator>>(istream&, Drink&);
```

These operators are used for restocking, comparisons, output formatting, and user input.

### Templates

A generic history container is implemented using:

```cpp
template <class T>
class History
```

### Static Members

The `Client` class keeps track of the total number of clients created using a static variable.

```cpp
static int total;
```

### Singleton Pattern

The `Bar` class follows the **Singleton design pattern**.

Only one bar instance can exist:

```cpp
static Bar& getInstance() {
    static Bar instance;
    return instance;
}
```

Copy construction and assignment are disabled.

```cpp
Bar(const Bar&) = delete;
Bar& operator=(const Bar&) = delete;
```

## STL Containers

The project uses several containers from the C++ Standard Template Library.

| Container | Purpose |
|---|---|
| `vector<Client*>` | Stores clients |
| `deque<Drink>` | Stores the drink menu |
| `list<T>` | Stores event history |
| `vector<Drink*>` | Stores recommended drinks |
| `vector<DrinkCategory>` | Stores supplier categories |

The project also uses STL algorithms including:

```cpp
find()
remove_if()
```

## Project Structure

The current project is implemented in a single C++ source file.

```text
Viktors-Bar/
│
├── main.cpp
└── README.md
```

## Requirements

You only need a compiler with C++11 support or newer.

Examples:

- GCC / G++
- Clang
- Microsoft Visual C++

Recommended:

```text
C++17
```

No external libraries are required.

## Compilation

### GCC / G++

```bash
g++ -std=c++17 main.cpp -o viktors-bar
```

Run the program:

### Linux / macOS

```bash
./viktors-bar
```

### Windows

```powershell
.\viktors-bar.exe
```

## Main Menu

When the application starts, the following menu is displayed:

```text
Welcome to Viktor's Bar
======================

1. Add clients
2. Simulate round
3. Show bar status
4. Show menu
5. Admin menu
0. Exit
```

## Example Workflow

A typical simulation could look like this:

```text
1. Add clients

Name: Alex
Age: 21
Type:
1 Casual
2 Regular
3 Heavy

2. Simulate round

--- ROUND START ---

Alex drinks House Wine.
Alcohol level is now 19.0.

Alex (Regular) is calm and in control.

--- ROUND END ---
```

As additional rounds are simulated, clients continue selecting drinks according to their preferences and current alcohol level.

Their risk level increases until they choose safer drinks, run out of suitable drinks, or are removed by security.

## Bar Statistics

The status screen displays information such as:

```text
Drinks served
Clients removed
Clients created
Clients currently inside
```

It also displays information about every active client:

```text
Name
Client type
Age
Alcohol level
Risk level
Status
```

## Concepts Demonstrated

This project was created mainly to practice and demonstrate:

- Object-Oriented Programming
- Classes and objects
- Encapsulation
- Inheritance
- Abstract classes
- Runtime polymorphism
- Virtual functions
- Dynamic casting
- Operator overloading
- Templates
- STL containers
- STL algorithms
- Custom exceptions
- Static members
- Dynamic memory management
- Singleton design pattern
- Console input validation

## Possible Future Improvements

Some possible additions to the project:

- Save and load bar data from files
- Persistent client history
- Drink prices and client budgets
- Revenue and profit tracking
- Multiple suppliers
- Employee and bartender classes
- Random events
- Client groups
- GUI version
- Database support
- Unit tests
- Smart pointers instead of raw pointers

## Author

**Victor**

C++ Object-Oriented Programming Project
