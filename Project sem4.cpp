// Project sem4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <map>

class Category;

class Product {
private:
    std::string name;
    std::string description; // Опис товару
    double price;
    Category* category;

public:
    Product(const std::string& name, const std::string& description, double price, Category* category)
        : name(name), description(description), price(price), category(category) {}

    const std::string& getName() const {
        return name;
    }

    const std::string& getDescription() const {
        return description;
    }

    double getPrice() const {
        return price;
    }

    Category* getCategory() const;
};

class Category {
private:
    std::string name;
    std::vector<Product*> products;

public:
    Category(const std::string& name) : name(name) {}

    void addProduct(Product* product) {
        products.push_back(product);
    }

    const std::string& getName() const {
        return name;
    }

    const std::vector<Product*>& getProducts() const {
        return products;
    }
};

Category* Product::getCategory() const {
    return category;
}

class AbstractFactory {
public:
    virtual std::vector<Product*> getProductsByCategory(const Category* category) const = 0;
};

class ConcreteFactory : public AbstractFactory {
public:
    std::vector<Product*> getProductsByCategory(const Category* category) const override {
        return category->getProducts();
    }
};

class Catalog {
private:
    std::vector<Category*> categories;
    AbstractFactory* factory;

public:
    Catalog() {
        factory = new ConcreteFactory();
    }

    ~Catalog() {
        delete factory;
        for (Category* category : categories) {
            delete category;
        }
    }

    void addCategory(Category* category) {
        categories.push_back(category);
    }

    void addProductToCategory(Product* product, Category* category) {
        category->addProduct(product);
    }

    std::vector<Category*> getCategories() const {
        return categories;
    }

    std::vector<Product*> getProductsByCategory(const Category* category) const {
        return factory->getProductsByCategory(category);
    }
};

class Cart {
protected:
    std::map<Product*, int> products;  // Карта для відстеження товарів та їх кількості

public:
    virtual ~Cart() = default;

    virtual void addProduct(Product* product) {
        if (products.find(product) != products.end()) {
            products[product] += 1;  // Збільшуємо кількість товару
        }
        else {
            products[product] = 1;   // Додаємо товар у кошик
        }
    }

    virtual void removeProduct(Product* product) {
        if (products.find(product) != products.end()) {
            if (products[product] > 1) {
                products[product] -= 1;  // Зменшуємо кількість товару
            }
            else {
                products.erase(product);  // Видаляємо товар з кошика
            }
        }
    }

    virtual void printCart() const {
        std::cout << "Cart:" << std::endl;
        double totalPrice = 0.0;
        for (const auto& item : products) {
            Product* product = item.first;
            int quantity = item.second;
            double productPrice = product->getPrice() * quantity;

            std::cout << "Product: " << product->getName() << ", Description: " << product->getDescription() << ", Quantity: " << quantity << ", Price: " << productPrice << std::endl;
            totalPrice += productPrice;
        }
        std::cout << "Total Price: " << totalPrice << std::endl;
    }
};

class CartDecorator : public Cart {
public:
    void printCart() const override {
        std::cout << "Cart with discount:" << std::endl;
        double totalPrice = 0.0;
        int count = 1;
        for (const auto& item : products) {
            Product* product = item.first;
            int quantity = item.second;
            double productPrice = product->getPrice() * quantity;

            if (count % 3 == 0) {
                productPrice *= 0.9;  // Застосовуємо знижку 10% до кожного третього товару
            }

            std::cout << "Product: " << product->getName() << ", Description: " << product->getDescription() << ", Quantity: " << quantity << ", Price: " << productPrice << std::endl;
            totalPrice += productPrice;
            count++;
        }
        std::cout << "Total Price: " << totalPrice << std::endl;
    }
};

int main() {
    // Створення категорій
    Category* category1 = new Category("Electronics");
    Category* category2 = new Category("Clothing");

    // Створення товарів
    Product* product1 = new Product("Smartphone", "A high-end smartphone with advanced features", 1000.0, category1);
    Product* product2 = new Product("Laptop", "A powerful laptop for professional use", 1500.0, category1);
    Product* product3 = new Product("T-Shirt", "A comfortable cotton t-shirt", 20.0, category2);
    Product* product4 = new Product("Jeans", "Stylish jeans made from premium denim", 50.0, category2);

    // Створення каталогу
    Catalog catalog;
    catalog.addCategory(category1);
    catalog.addCategory(category2);

    // Додавання товарів до категорій
    catalog.addProductToCategory(product1, category1);
    catalog.addProductToCategory(product2, category1);
    catalog.addProductToCategory(product3, category2);
    catalog.addProductToCategory(product4, category2);

    // Створення користувачів та кошиків
    std::map<std::string, Cart*> users;
    users["Alice"] = new CartDecorator();
    users["Bob"] = new Cart();

    // Додавання товарів до кошиків
    users["Alice"]->addProduct(product1);
    users["Alice"]->addProduct(product3);
    users["Alice"]->addProduct(product2);
    users["Alice"]->addProduct(product4);
    users["Bob"]->addProduct(product2);
    users["Bob"]->addProduct(product3);
    users["Bob"]->addProduct(product4);

    // Виведення замовлень та загальної ціни для кожного користувача
    for (const auto& user : users) {
        std::cout << "User: " << user.first << std::endl;
        user.second->printCart();
        std::cout << std::endl;
    }

    // Видалення товарів з кошиків
    users["Alice"]->removeProduct(product1);
    users["Bob"]->removeProduct(product2);

    // Виведення оновлених замовлень та загальної ціни для кожного користувача
    std::cout << "Updated Carts:" << std::endl;
    for (const auto& user : users) {
        std::cout << "User: " << user.first << std::endl;
        user.second->printCart();
        std::cout << std::endl;
    }

    // Звільнення пам'яті
    for (const auto& user : users) {
        delete user.second;
    }

    return 0;
}
// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
