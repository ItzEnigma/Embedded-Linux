/**********************************/
/* ---------- INCLUDES ---------- */

#include "Payment.hpp"

/******************************************/
/* ---------- CLASS DEFINITION ---------- */

Payment::Payment(float Copy_Amount) : amount(Copy_Amount)
{
}

Credit::Credit(float Copy_Amount, std::string Copy_Number, std::string Copy_Type, date Copy_ExpireDate)
    : Payment(Copy_Amount), number(Copy_Number), type(Copy_Type), expireDate(Copy_ExpireDate)
{
}

Cash::Cash(float Copy_Amount, float Copy_CashTendered) : Payment(Copy_Amount), cashTendered(Copy_CashTendered)
{
}

Check::Check(float Copy_Amount, std::string Copy_Name, std::string Copy_BankID)
    : Payment(Copy_Amount), name(Copy_Name), bankID(Copy_BankID)
{
}

WireTransfer::WireTransfer(float Copy_Amount, std::string Copy_BankID, std::string Copy_BankName)
    : Payment(Copy_Amount), bankID(Copy_BankID), bankName(Copy_BankName)
{
}

/******************************************/