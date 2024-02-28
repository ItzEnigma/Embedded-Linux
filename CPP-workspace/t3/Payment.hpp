#ifndef _PAYMENT_H_
#define _PAYMENT_H_

/**********************************/
/* ---------- INCLUDES ---------- */

#include <iostream>

/**********************************/

/******************************************/
/* ---------- TYPE DEFINITIONS ---------- */

typedef time_t date;
enum PaymentMethod
{
    CREDIT = 0,
    CASH,
    CHECK,
    WIRETRANSFER
};

/******************************************/

/******************************************/
/* ---------- CLASS PROTOTYPES ---------- */

/***** Payment Class *****/
/*
    - amount : float
*/
class Payment
{
public:
    Payment(float Copy_Amount);

private:
    float amount;
};

/***** Credit Class Inheriting Payment Class *****/
/*
    - number : String
    - type : String
    - expireDate : date
*/
class Credit : public Payment
{
public:
    Credit(float Copy_Amount, std::string Copy_Number, std::string Copy_Type, date Copy_ExpireDate);

private:
    std::string number;
    std::string type;
    date expireDate;
};

/***** Cash Class Inheriting Payment Class *****/
/*
    - cashTendered : float
*/
class Cash : public Payment
{
public:
    Cash(float Copy_Amount, float Copy_CashTendered);

private:
    float cashTendered;
};

/***** Check Class Inheriting Payment Class *****/
/*
    - name : String
    - bankID : String
    + authorized() : void?
*/
class Check : public Payment
{
public:
    Check(float Copy_Amount, std::string Copy_Name, std::string Copy_BankID);

private:
    std::string name;
    std::string bankID;
};

/***** WireTransfer Class Inheriting Payment Class *****/
/*
    - bankID : String
    - bankName : String
*/
class WireTransfer : public Payment
{
public:
    WireTransfer(float Copy_Amount, std::string Copy_BankID, std::string Copy_BankName);

private:
    std::string bankID;
    std::string bankName;
};

/******************************************/

#endif /* _PAYMENT_H_ */