#ifndef _ITEM_H_
#define _ITEM_H_

/**********************************/
/* ---------- INCLUDES ---------- */

#include <iostream>

/**********************************/

/******************************************/
/* ---------- TYPE DEFINITIONS ---------- */

/******************************************/

/******************************************/
/* ---------- CLASS PROTOTYPES ---------- */

/***** Item Class *****/
/*
    - weight : float
    - description : String
    + getPriceForQuantity() : float
    + getWeight() : float
*/
class Item
{
public:
    Item(float Copy_Weight, std::string Copy_Description);

    float getPriceForQuantity();
    float getWeight();

private:
    float weight;
    float price;
    std::string description;
};

/******************************************/

#endif /* _ITEM_H_ */