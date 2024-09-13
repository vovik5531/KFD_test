#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<random>
double rand_double(double low, double high);
class Logger
{
public: 
    void GetError(std::string Error_Type)
    {
        std::cout<<"Error. " << Error_Type << std::endl;
    }
    template<typename T, typename U>
    void ShowBill(std::map<T, U>& dictionary)
    {
        for(auto& i : dictionary)
        {
            std::cout << i.first << ": " <<  i.second << std::endl; 
        } 
    }
};

class CurrencyPair
{
private:
    std::string First_Currency; 
    std::string Second_Currency; 
    double exchange_coef;
public:
    CurrencyPair(const std::string& First, const std::string& Second, double coef)
    {
        First_Currency = First; 
        Second_Currency = Second; 
        exchange_coef = coef; 
    }
    std::string GetFirst() const
    {
        return First_Currency; 
    }
    std::string GetSecond() const
    {
        return Second_Currency;
    }
    double GetCoef() const 
    {
        return exchange_coef;
    }
    void UpdateCoef()
    {
        double exponenta = rand_double(-0.05, 0.05);
        exchange_coef = (1.0+exponenta)*exchange_coef;
    }
};

class Bank
{
private:
    std::vector<CurrencyPair> Market;
    
public:
    Bank(std::vector<CurrencyPair> & new_market)
    {
        Market = new_market;
    }
    bool FindCurrencies(std::string& FirstCurr, std::string& SecondCurr) const 
    {
        bool ans = 0;
        for( auto& coins : Market )
        {
            if( (coins.GetFirst() == FirstCurr) && (coins.GetSecond()==SecondCurr) ) 
            {
                ans = 1;
                break;
            }
            if( (coins.GetFirst() == SecondCurr) && (coins.GetSecond()==FirstCurr) ) 
            {
                ans = 1;
                break;
            }
        }
        return ans; 
    }
    double FindCoef(std::string& FirstCurr, std::string& SecondCurr) const 
    {
        for( auto& coins : Market )
        {
            if( (coins.GetFirst() == FirstCurr) && (coins.GetSecond()==SecondCurr) ) 
            {
                return coins.GetCoef();
                break;
            }
            else if( (coins.GetFirst() == SecondCurr) && (coins.GetSecond()==FirstCurr) ) 
            {
                return coins.GetCoef();
                break;
            }
        }
        return 0.0; 
    }
    void UpdateCurrencyPairs()
    {
        for( auto& coins : Market )
        {
            coins.UpdateCoef();
        }
    }
    CurrencyPair* GetCurrencyPair(size_t index)
    {
        return &Market[index]; 
    }
    int GetMarketSize()
    {
        return Market.size(); 
    }
};


class ATM : public Bank
{
private:
    std::map<std::string, double> ATM_Storage;
public:
    ATM(std::vector<CurrencyPair> market, std::map<std::string, double>ATM_storage) : Bank(market)
    {
        this->ATM_Storage = ATM_storage;
    }
    bool ExchangeMoney(std::string FromCurr, std::string ToCurr, double Amount)
    {
        Logger Log; 
        if(!FindCurrencies(FromCurr, ToCurr))
        {
            Log.GetError("Non-existing currency or exchange pair.");
            return 0; 
        }
        if(ATM_Storage[ToCurr]< Amount*FindCoef(FromCurr, ToCurr))
        {
            Log.GetError("Not enough cash. Try another ATM"); 
            return 0; 
        }
    
        ATM_Storage[FromCurr] += Amount; 
        ATM_Storage[ToCurr] -= Amount*FindCoef(FromCurr, ToCurr);
        UpdateCurrencyPairs(); 
        return 1;
        
    }
    void Check_Storage()
    {
        Logger Log; 
        Log.ShowBill(ATM_Storage);
    }
};

class Bank_Client
{
private:
    std::string Name; 
    std::string Surname;
    std::map<std::string, double> Bank_Account;
public:
    Bank_Client(const std::string& name, const std::string& surname, std::map<std::string, double> account)
    {
        Name = name; 
        Surname = surname; 
        Bank_Account = account;
        std::cout << "Created client: " << name << " " << surname << std::endl;    
    }
    std::string GetName() const 
    {
        return Name; 
    }
    std::string GetSurname() const 
    {
        return Surname; 
    }
    void Show_Bill() 
    {
        Logger Log; 
        Log.ShowBill(Bank_Account);
    }
    void ExchangeMoney(ATM& terminal, std::string FromCurr, std::string ToCurr, double Amount)
    {
        Logger Log;
        
        if (terminal.ExchangeMoney(FromCurr, ToCurr, Amount) )
        {
            if(Amount > Bank_Account[FromCurr] )
            {
                Log.GetError("Not enough funds.");
            }
            if(Amount < 0 )
            {
                Log.GetError("Try to enter positive number.");
            }
            else
            { 
                Bank_Account[FromCurr] -= Amount; 
                Bank_Account[ToCurr] += Amount*terminal.FindCoef(FromCurr, ToCurr);
            }
        }
    }
};
double rand_double(double low, double high)
	{
		static std::default_random_engine re {};
		using Dist = std::uniform_real_distribution<double>;
  		static Dist uid {};
  		return uid(re, Dist::param_type{low,high});
	}
int main()
{
    CurrencyPair ur = {"USD", "RUB", 89.9};
    CurrencyPair er = {"EUR", "RUB", 111.0};
    CurrencyPair p3 = {"EUR", "USD", 2.0};
    CurrencyPair p4 = {"USD", "USDT" ,200.1};
    CurrencyPair p5 = {"USD", "BTC" ,1110.0};
    std::vector<CurrencyPair> Moscow_Market{ur, er, p3, p4, p5};
    std::map<std::string, double> Sber_ATM_storage{ {"USD", 1'000}, {"RUB", 10'000}, {"EUR", 1'000}, {"USDT", 1'000}, {"BTC", 1.5}};
    std::map<std::string, double> User_test_bill{ {"RUB", 1'000'000}};
    Bank_Client a = {"Vladimir", "Avdeyev", User_test_bill};
    a.Show_Bill();
    ATM Sber_ATM = { Moscow_Market, Sber_ATM_storage};
    // Sber_ATM.Check_Storage();
    // std::cout<< rand_double(-5.0, 5.0);

    // a.ExchangeMoney(Sber_ATM, "RUB", "EUR", 1);
    // std::cout<< rand_double(-0.05, 0.05) << std::endl;
    // a.Show_Bill();
    // Sber_ATM.Check_Storage();
    // std::cout<< rand_double(-5.0, 5.0);
      // Меню
    int choice;
    do {
        std::cout << "--------------------------------" << std::endl;
        std::cout << "Валютный терминал" << std::endl;
        std::cout << "--------------------------------" << std::endl;
        std::cout << "1. Обмен валюты" << std::endl;
        std::cout << "2. Просмотр баланса" << std::endl;
        std::cout << "3. Выход" << std::endl;
        std::cout << "Введите ваш выбор: ";
        std::cin >> choice;
        switch (choice) {
            case 1: {
                std::cout << "Валютные пары:" << std::endl;
                for (int i = 0; i < Sber_ATM.GetMarketSize(); i++) {
                    CurrencyPair* temp = Sber_ATM.GetCurrencyPair(i);
                    std::cout.setf(std::ios::fixed);  // вывод в фиксированном формате 
                    std::cout.precision(2);
                    std::cout << i + 1 << ". " << (*temp).GetFirst() << " / " << (*temp).GetSecond() << " (курс: " << (*temp).GetCoef() << ")" << std::endl;
                }

                int pairIndex;
                std::string fromCurrency, toCurrency;
                std::cout << "Введите валюту для обмена: ";
                std::cin >> fromCurrency;
                std::cout << "Введите валюту, на которую нужно обменять: ";
                std::cin >> toCurrency;
                double amount;
                std::cout << "Введите сумму для обмена: ";
                std::cin >> amount;
                // Обмен валюты
                a.ExchangeMoney(Sber_ATM, fromCurrency, toCurrency, amount);
                break;
            }
            case 2: {
                // Просмотр баланса
                a.Show_Bill();
                break;
            }
            case 3:
                std::cout << "До свидания!" << std::endl;
                break;
            default:
                std::cout << "Неверный выбор." << std::endl;
        }
    } while (choice != 3);

    return 0; 
}
