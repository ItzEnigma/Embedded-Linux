#include <iostream>
#include "Order.hpp"

typedef time_t date;

/***** Customer Class *****/
/*
    - name : String
    - deliveryAddress : String
    - contact : String
    - active : boolean
*/
class Customer
{
public:
    Customer(std::string Copy_customer_name,
             std::string Copy_customer_deliveryAddress,
             std::string Copy_customer_contact,
             bool Copy_customer_active) : customer_name(Copy_customer_name),
                                          customer_deliveryAddress(Copy_customer_deliveryAddress),
                                          customer_contact(Copy_customer_contact),
                                          customer_active(Copy_customer_active)
    {
        orders = new Order *[10];
        _order_count = 0;
    }

    ~Customer()
    {
        /* Deallocate the orders dynamically allocated memory to avoid Memory Leak */
        for (int i = 0; i < _order_count; i++)
            delete orders[i];
        delete[] orders;
    }

    void addOrder()
    {
        // orders[_order_count++] = Order(order_date);
        date currTime = time(NULL); // can also be wriiten --> time(&currTime);
        orders[_order_count] = new Order(currTime);
        orders[_order_count]->updateOrderStatus(SHIPPING);
        char choice;
        int count;
        do
        {
            std::cout << "Select From The Available Items: \n";
            std::cout << "'0' --> [$2.7] \n'1' --> [$3.9] \n";
            std::cout << "'2' --> [$1.1] \n'3' --> [$0.6] \n";
            std::cout << "'4' --> [$0.46] \n'5' --> [$2.12] \n";
            std::cin >> choice;
            std::cout << "How Many? \n";
            std::cin >> count;
            orders[_order_count]->addOrderDetail(choice, count);
            std::cout << "Do you want to add another item? ";
            std::cin >> choice;
        } while (choice == 'y' || choice == 'Y');
        _order_count++;
    }

    void payOrder(int Copy_orderID)
    {
        if (Copy_orderID < _order_count && Copy_orderID >= 0)
        {
            orders[Copy_orderID]->payment();
        }
        else
        {
            std::cout << "Order Doesn't Exist !! ... [Please choose a valid order] \n";
        }
    }

private:
    // Order orders[10];
    Order **orders;
    std::string customer_name;
    std::string customer_deliveryAddress;
    std::string customer_contact;
    bool customer_active;
    int _order_count;
};

int main()
{
    Customer hosam("Hosam", "Haram", "0123124521", true);
    hosam.payOrder(0);
    hosam.addOrder();
    hosam.payOrder(0);
    hosam.payOrder(1);
    return 0;
}