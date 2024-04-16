/**********************************/
/* ---------- INCLUDES ---------- */

#include "Item.hpp"

/******************************************/
/* ---------- CLASS DEFINITION ---------- */

Item::Item(float Copy_Weight, std::string Copy_Description)
    : weight(Copy_Weight), description(Copy_Description)
{
    if (Copy_Weight == 23.5f)
    {
        price = 2.7f;
    }
    else if (Copy_Weight == 44.2f)
    {
        price = 3.9f;
    }
    else if (Copy_Weight == 14.8f)
    {
        price = 1.1f;
    }
    else if (Copy_Weight == 8.8f)
    {
        price = 0.6f;
    }
    else if (Copy_Weight == 10)
    {
        price = 0.46f;
    }
    else if (Copy_Weight == 48.2f)
    {
        price = 2.12f;
    }
    else
    {
        /* Item Does Not Exist ! */
    }
}

float Item::getPriceForQuantity()
{
    return price;
}

float Item::getWeight()
{
    return weight;
}

/******************************************/