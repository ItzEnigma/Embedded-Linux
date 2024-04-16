#ifndef _ORDER_H_
#define _ORDER_H_

/**********************************/
/* ---------- INCLUDES ---------- */

#include <iostream>
#include <ctime>
#include "Item.hpp"
#include "Payment.hpp"

/**********************************/

/******************************************/
/* ---------- TYPE DEFINITIONS ---------- */

typedef time_t date;

/***** OrderStatus Enum *****/
/*
    - CREATE : int = 0
    - SHIPPING : int = 1
    - DELIVERED : int = 2
    - PAID : int = 3
*/
enum OrderStatus
{
    CREATE = 0,
    SHIPPING,
    DELIVERED,
    PAID
};

/******************************************/

/******************************************/
/* ---------- CLASS PROTOTYPES ---------- */

/***** OrderDetail Class *****/
/*
    - qty : int
    - taxStatus : String
    + calculateSubTotal() : float
    + calculateWeight() : float
*/
class OrderDetail
{
public:
    OrderDetail(int Copy_Qty, std::string Copy_TaxStatus);

    ~OrderDetail();

    float calculateSubTotal();

    float cauclateWeight();

    void addItem(char, int);

private:
    int qty;
    std::string taxStatus;
    Item *item;
};

/***** Order Class *****/
/*
    - createDate : date
*/
class Order
{
public:
    Order(date);

    ~Order();

    void updateOrderStatus(OrderStatus);

    void addOrderDetail(char, int);

    void payment();

private:
    date createDate;
    OrderDetail **orderDetails;
    OrderStatus order_status;
    int _orderDetails_count;
};

/******************************************/

#endif /* _ORDER_H_ */