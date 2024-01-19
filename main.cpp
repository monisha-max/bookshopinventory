/* This C++ code implements a inventory management system for a bookstore. It defines classes for general publications, books, magazines, 
customers, and a cashier. The system is operated by a StoreManager class, facilitating functionalities such as adding books and 
magazines to inventory, registering customers, selling publications with applicable discounts, updating stock, and displaying cashier 
information. The main function utilizes a menu-driven interface, allowing users to interact with the system through options like entering
publication details, displaying inventory,selling to customers, registering customers, and displaying cashier information. 
It also includes basic input validation and outputs informative messages during transactions */


#include <iostream>
#include <string>
#include <vector>
// Base class representing a generic publication
class Publication{
public:
    std::string title;
    std::string author;
    int publicationYear;
    // Constructor initializes publication details
    Publication(const std::string& t,const std::string& a,int year)
        : title(t),author(a),publicationYear(year) {}

    void displayInfo() const{
        std::cout << "Title: " << title << "\n";
        std::cout << "Author: " << author << "\n";
        std::cout << "Publication Year: " << publicationYear << "\n";
    }
};
// Derived class representing a book, inheriting from Publication
class Book : public Publication{
public:
    int numCopies;
    double pricePerCopy;
    //Constructor initializes book details and calls base class constructor
    Book(const std::string& t,const std::string& a,int year,int copies,double price)
        : Publication(t, a, year),numCopies(copies),pricePerCopy(price) {}

    void displayInfo() const{
        Publication::displayInfo();
        std::cout << "Number of Copies: " << numCopies << "\n";
        std::cout << "Price per Copy: $" << pricePerCopy << "\n";
    }
};
// Derived class representing a magazine, inheriting from Publication
class Magazine : public Publication{
public:
    int issueNumber;
    std::string genre;
    double monthlySubscriptionCost;
    //Constructor
    Magazine(const std::string& t,const std::string& a,int year,int issue,const std::string& g,double cost)
        : Publication(t, a, year),issueNumber(issue),genre(g),monthlySubscriptionCost(cost) {}

    void displayInfo() const{
        Publication::displayInfo();
        std::cout << "Issue Number: " << issueNumber << "\n";
        std::cout << "Genre: " << genre <<  "\n";
        std::cout << "Monthly Subscription Cost: $" << monthlySubscriptionCost <<"\n";
    }
};

class Customer{
private:
    int customer_id;
    std::string name;

public:
    // Constructor
    Customer(int id,const std::string& customer_name) : customer_id(id),name(customer_name) {}

    // Accessor functions
    int getCustomerId() const{
        return customer_id;
    }

    const std::string& getCustomerName() const{
        return name;
    }
};

class Cashier{
private:
    double initialMoney;
    double moneyPaidByCustomer;

public:
    //Constructor initializes the cashier with an initial amount of money i.e $1000.
    Cashier(double initial) : initialMoney(initial), moneyPaidByCustomer(0) {}

    void displayCashierInfo() const{
        std::cout << "\nCashier Information:\n";
        std::cout << "Initial Money: $" << initialMoney << std::endl;
        std::cout << "Money Paid by Customer: $" << moneyPaidByCustomer << std::endl;
        std::cout << "Total Money: $" << initialMoney + moneyPaidByCustomer << std::endl;
    }
    //Receive payment from a customer and update the total money received
    void receivePayment(double amount){
        moneyPaidByCustomer += amount;
        std::cout << "Payment received: $" << amount << std::endl;
    }
    //Reduce the initial money for a book purchase got to the store(book)
    void reduceMoneyForBook(double amount){
        initialMoney -= amount;
        std::cout << "Money reduced for book purchase: $" << amount << std::endl;
    }
    //Same for magazine
    void reduceMoneyForMagazine(double amount){
        initialMoney -= amount;
        std::cout << "Money reduced for magazine purchase: $" << amount << std::endl;
    }
};

class StoreManager{
private:
    std::vector<Book> books;
    std::vector<Magazine> magazines;
    std::vector<Customer> customers;
    Cashier& cashier;

public:
    StoreManager(Cashier& c) : cashier(c) {}

    void addBook(const Book& book){
        // Check if a book with the same title, author, and publication year already exists, if exists just add the number of copies
        for(auto& existingBook : books){
            if(existingBook.title == book.title &&
                existingBook.author == book.author &&
                existingBook.publicationYear == book.publicationYear){
                //Update the existing entry with additional copies
                existingBook.numCopies += book.numCopies;
                std::cout << "Number of copies updated for existing book.\n";
                cashier.reduceMoneyForBook(book.pricePerCopy * book.numCopies);
                return;
            }
        }

        // If the book does not exist, add it to the inventory
        books.push_back(book);
        cashier.reduceMoneyForBook(book.pricePerCopy * book.numCopies);
        std::cout << "Book added to inventory.\n";
    }

    void addMagazine(const Magazine& magazine){
        // Check if a magazine with the same title, author, and publication year already exists, if exists just add the number of copies
        for(auto& existingMagazine : magazines){
            if(existingMagazine.title == magazine.title &&
                existingMagazine.author == magazine.author &&
                existingMagazine.publicationYear == magazine.publicationYear){
                //Update the existing entry with additional issues
                existingMagazine.issueNumber += magazine.issueNumber;
                std::cout << "Issue number updated for existing magazine.\n";
                cashier.reduceMoneyForMagazine(magazine.monthlySubscriptionCost);
                return;
            }
        }

        // If the magazine does not exist, add it to the inventory
        magazines.push_back(magazine);
        cashier.reduceMoneyForMagazine(magazine.monthlySubscriptionCost);
        std::cout << "Magazine added to inventory.\n";
    }
    //Registering customers
    void registerCustomer(Customer& customer){
        customers.push_back(customer);
    }
    //Displaying customers name and id
    void displayRegisteredCustomers() const{
    std::cout << "\nRegistered Customers:\n";
    for (const auto& customer : customers) {
        std::cout << "Customer ID: " << customer.getCustomerId() << ", Customer Name: " << customer.getCustomerName() << std::endl;
    }
    }
    //Displaying details of each and every book and magazine
    void displayInventory() const{
        std::cout << "\nBook Inventory:\n";
        for(const auto& book : books){
            book.displayInfo();
            std::cout << "----------------------\n";
        }

        std::cout << "\nMagazine Inventory:\n";
        for(const auto& magazine : magazines){
            magazine.displayInfo();
            std::cout << "----------------------\n";
        }
    }

    void sellBookToCustomer(const std::string& requestedTitle,int requestedYear,bool isOldCustomer){
        for(auto& book : books){
            //If the book is found, then decrease a copy of it
            if (book.title == requestedTitle && book.publicationYear == requestedYear){
                if(book.numCopies >0){
                    std::cout << "Book sold to customer:\n";
                    book.displayInfo();
                    book.numCopies--;

                    //Applying discount for old customers
                    double discount = (isOldCustomer) ? 0.10 * book.pricePerCopy : 0.0;
                    double discountedPrice = book.pricePerCopy - discount;
                    std::cout << "Customer Type: " << (isOldCustomer ? "Old, so u get 10% discount" : "New") <<"\n";
                    std::cout << "Discount Applied: $" << discount << "\n";

                    cashier.receivePayment(discountedPrice);
                    return;
                } else {
                    //If the number of copies is equal to 0
                    std::cout <<"Sorry, the requested book is out of stock.\n";
                    return;
                }
            }
        }
        //If the book itself is not matching with any of the entered book
        std::cout << "Sorry,the requested book is not in the inventory.\n";
    }
    //The same logic as sellBookToCustomer
    void sellMagazineToCustomer(const std::string& requestedTitle, int requestedYear, bool isOldCustomer) {
        for (auto& magazine : magazines) {
            if (magazine.title == requestedTitle && magazine.publicationYear == requestedYear) {
                if (magazine.issueNumber > 0) {
                    std::cout << "Magazine sold to customer:\n";
                    magazine.displayInfo();
                    magazine.issueNumber--;

                    
                    double discount = (isOldCustomer) ? 0.10 * magazine.monthlySubscriptionCost : 0.0;
                    double discountedPrice = magazine.monthlySubscriptionCost - discount;
                    std::cout << "Customer Type: " << (isOldCustomer ? "Old, so u get 10% discount" : "New") << std::endl;
                  
                    std::cout << "Discount Applied: $" << discount << std::endl;

                    cashier.receivePayment(discountedPrice);
                    return;
                } else {
                    std::cout << "Sorry, the requested magazine is out of stock.\n";
                    return;
                }
            }
        }
        std::cout << "Sorry, the requested magazine is not in the inventory.\n";
    }
};

int main(){
    char choice;
    int numBooks = 0;
    int numMagazines = 0;
    Cashier cashier(1000.0);//initial cash with the cashier
    StoreManager storeManager(cashier);
    
   do{
        std::cout << "Menu:\n";
        std::cout << "1. Enter details for a book\n";
        std::cout << "2. Enter details for a magazine\n";
        std::cout << "3. Display total number of books and magazines\n";
        std::cout << "4. Display inventory\n";
        std::cout << "5. Sell publication to customer\n";
        std::cout << "6. Display cashier information\n";
        std::cout << "7. Register a customer\n";
        std::cout << "8. Display registered customers\n";
        std::cout << "9. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch(choice){
            case '1':{
                //Enter details for a Book
                std::string title,author;
                int publicationYear,numCopies;
                double pricePerCopy;

                std::cout << "Enter the title of the book: ";
                std::getline(std::cin,title);

                std::cout << "Enter the author of the book: ";
                std::getline(std::cin,author);

                std::cout << "Enter the publication year of the book: ";
                std::cin >> publicationYear;

                std::cout << "Enter the number of copies: ";
                std::cin >> numCopies;

                std::cout << "Enter the price per copy: $";
                std::cin >> pricePerCopy;

                Book myBook(title, author, publicationYear, numCopies, pricePerCopy);

                //Add the book to the inventory
                storeManager.addBook(myBook);

                //Increment the number of books
                numBooks++;
                break;
            }
            case '2': {
                // Enter details for a Magazine
                std::string title, author;
                int publicationYear, issueNumber;
                std::string genre;
                double monthlySubscriptionCost;

                std::cout << "Enter the title of the magazine: ";
                std::getline(std::cin,title);

                std::cout << "Enter the author of the magazine: ";
                std::getline(std::cin,author);

                std::cout << "Enter the publication year of the magazine: ";
                std::cin >> publicationYear;

                std::cout << "Enter the issue number of the magazine: ";
                std::cin >> issueNumber;

                std::cin.ignore();

                std::cout << "Enter the genre of the magazine: ";
                std::getline(std::cin, genre);

                std::cout << "Enter the monthly subscription cost of the magazine: $";
                std::cin >> monthlySubscriptionCost;

                Magazine myMagazine(title, author, publicationYear, issueNumber, genre, monthlySubscriptionCost);

                // Add the magazine to the inventory
                storeManager.addMagazine(myMagazine);

                // Increment the number of magazines
                numMagazines++;
                break;
            }
            case '3':
                // Display the total number of books and magazines
                std::cout << "\nTotal Number of Books: " << numBooks << std::endl;
                std::cout << "Total Number of Magazines: " << numMagazines << std::endl;
                break;
            case '4':
                // Display the inventory
                storeManager.displayInventory();
                break;
            case '5':{
                // Sell a publication to the customer
                char publicationType;
                //What type of publication is the customer buying?
                std::cout << "Is the customer buying a book or a magazine? (Enter 'b' for book, 'm' for magazine): ";
                std::cin >> publicationType;

                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                //If it's a book
                if(publicationType == 'b' || publicationType == 'B'){
                    // Sell a book to the customer
                    std::cout << "Is the customer old or new? (Enter 'o' for old, 'n' for new): ";
                    char customerType;
                    std::cin >> customerType;

                    bool isOldCustomer;

                    switch (customerType){
                        //If old customer
                        case 'o':
                        case 'O':
                            isOldCustomer = true;//If old customer, setting the  flag as true
                            break;
                        case 'n':
                        case 'N':
                            isOldCustomer = false;
                            break;
                        default:
                            std::cout <<"Invalid customer type. Assuming the customer is new.\n";
                            isOldCustomer = false;
                    }
                    std::string  requestedTitle;
                    int  requestedYear;
                    std::cin.ignore();
                    std::cout << "Enter the requested book title: ";
                    std::getline(std::cin, requestedTitle);

                    std::cout << "Enter the requested publication year: ";
                    std::cin >> requestedYear;

                
                    storeManager.sellBookToCustomer(requestedTitle, requestedYear, isOldCustomer);
                }else if(publicationType == 'm' || publicationType == 'M'){
                    // Sell a magazine to the customer
                    std::cout << "Is the customer old or new? (Enter 'o' for old, 'n' for new): ";
                    char customerType;
                    std::cin >> customerType;
                    

                    bool isOldCustomer;

                    switch(customerType){
                        case 'o':
                        case 'O':
                            isOldCustomer = true;
                            
                            break;
                        case 'n':
                        case 'N':
                            isOldCustomer = false;
                            break;
                        default:
                            std::cout << "Invalid customer type. Assuming the customer is new.\n";
                            isOldCustomer = false;
                    }
                    if (isOldCustomer){
                        std::cout << "Since you are an old customer, you get a 10% discount.\n";
                    }

                    std::string  requestedTitle;
                    int  requestedYear;
                    std::cin.ignore();
                    std::cout << "Enter the requested magazine title: ";
                    std::getline(std::cin, requestedTitle);

                    std::cout << "Enter the requested publication year: ";
                    std::cin >> requestedYear;
                    
                    storeManager.sellMagazineToCustomer(requestedTitle, requestedYear, isOldCustomer);
                }else{
                    std::cout << "Invalid publication type.\n";
                }
                break;
            }
            case '6':
                // Display cashier information that is the initial money, money paid by customer, and total money
                cashier.displayCashierInfo();
                break;

            case '7':{
                //Registering a customer
                int customerId;
                std::string customerName;

                std::cout << "Enter customer ID: ";
                std::cin >> customerId;

                std::cin.ignore(); 
                std::cout << "Enter customer name: ";
                std::getline(std::cin, customerName);
                Customer customer(customerId, customerName);
                storeManager.registerCustomer(customer);
                break;
                
            }

            case '8':
                // Display registered customers
                storeManager.displayRegisteredCustomers();
                break;

            default:
                std::cout << "Invalid choice. Please enter a valid option.\n";
        }

    } while (choice != '9'); // Update the exit condition

    return 0;
}
