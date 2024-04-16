/**********************************/
/* ---------- INCLUDES ---------- */

#include "Order.hpp"

/******************************************/
/* ---------- CLASS DEFINITION ---------- */

Order::Order(date Copy_Date = 0) : createDate(Copy_Date)
{
    // std::cout << "Is it called?" << std::endl;
    orderDetails = new OrderDetail *[5]; // can contain upto 5 items
    order_status = CREATE;
    _orderDetails_count = 0;
}

Order::~Order()
{
    /* Deallocate the orders dynamically allocated memory to avoid Memory Leak */
    for (int i = 0; i < _orderDetails_count; i++)
        delete orderDetails[i];
    delete[] orderDetails;
}

void Order::updateOrderStatus(OrderStatus Copy_Status)
{
    order_status = Copy_Status;
}

void Order::addOrderDetail(char Copy_ItemID, int Copy_ItemCount)
{
    orderDetails[_orderDetails_count] = new OrderDetail(Copy_ItemCount, "0");
    orderDetails[_orderDetails_count]->addItem(Copy_ItemID, Copy_ItemCount);
    _orderDetails_count++;
}

void Order::payment()
{
    char payment_method;
    float total;
    for (int i = 0; i < _orderDetails_count; i++)
    {
        total += orderDetails[i]->calculateSubTotal();
    }
    std::cout << "Total Amount : " << total << '\n';
    std::cout << "Please Specify Payment Method\n";
    std::cout << "'1' --> Credit \n'2' --> Cash \n'3' --> Check \n'4' --> Wire Transfer \n";
    std::cin >> payment_method;
    switch (payment_method)
    {
    case '1': /* Credit */
    {
        std::string number;
        std::string type;
        date expireDate;

        std::cout << "Please Enter your Credit [Number], [Type], [Expire Date] \n";
        std::cin >> number;
        std::cin >> type;
        std::cin >> expireDate;

        Credit custom_creditPayment(total, number, type, expireDate);
        order_status = PAID;
        break;
    }
    case '2':
    {
        float tendered;

        std::cin >> tendered;
        Cash custom_cashPayment(total, tendered);
        order_status = PAID;
        break;
    }
    case '3':
    {
        std::string name;
        std::string bankID;

        std::cout << "Please Enter your [Name], [Bank ID] \n";

        std::cin >> name;
        std::cin >> bankID;

        Check custom_checkPayment(total, name, bankID);
        order_status = PAID;
        break;
    }
    case '4':
    {
        std::string bankID;
        std::string bankName;

        std::cout << "Please Enter your [Bank ID], [Bank Name] \n";

        std::cin >> bankID;
        std::cin >> bankName;

        WireTransfer custom_wirePayment(total, bankID, bankName);
        order_status = PAID;
        break;
    }
    default:
        std::cout << "Error! ... Payment Method Does Not Exist! \n";
    }
}

OrderDetail::OrderDetail(int Copy_Qty, std::string Copy_TaxStatus)
    : qty(Copy_Qty), taxStatus(Copy_TaxStatus)
{
}

OrderDetail::~OrderDetail()
{
    /* Deallocate the item dynamically allocated memory to avoid Memory Leak */
    if (item != NULL)
        delete item;
}

float OrderDetail::calculateSubTotal()
{
    return (item->getPriceForQuantity() * (float)qty);
}

float OrderDetail::cauclateWeight()
{
    return item->getWeight();
}

void OrderDetail::addItem(char Copy_ItemID, int Copy_ItemCount)
{
    qty = Copy_ItemCount;
    switch (Copy_ItemID)
    {
    case '0':
    {
        item = new Item(23.5f, "Item 0");
        break;
    }
    case '1':
    {
        item = new Item(44.2f, "Item 1");
        break;
    }
    case '2':
    {
        item = new Item(14.8f, "Item 2");
        break;
    }
    case '3':
    {
        item = new Item(8.8f, "Item 3");
        break;
    }
    case '4':
    {
        item = new Item(10, "Item 4");
        break;
    }
    case '5':
    {
        item = new Item(48.2f, "Item 5");
        break;
    }
    default:
        std::cout << "Error! ... Item Does Not Exist! \n";
    }
}

/******************************************/