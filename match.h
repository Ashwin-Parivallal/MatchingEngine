#include<memory>
#include<set>
#include<unordered_map>
#include<vector>
#include<chrono>
#include<string>

class Request;  
class BuyOrders;
class SellOrders;

enum TransactionType{
    SELL = 1,
    BUY = 2
};


class Stock{
    std::string name;
    std::set<std::shared_ptr<Request>> buyOrders;
    std::set<std::shared_ptr<Request>> sellOrders;
public:
    Stock(std::string, TransactionType, const std::shared_ptr<Request>&);
    std::set<std::shared_ptr<Request>>& getBuyOrders(){ return buyOrders; }
    std::set<std::shared_ptr<Request>>& getSellOrders(){ return sellOrders; }
};

class Request{
    std::string clientName;
    std::string stockName;
    std::chrono::system_clock::time_point reqTime;
    unsigned quantity;
public:
    Request(std::string, std::string, unsigned);
    virtual bool processRequest(std::unordered_map<std::string, std::unique_ptr<Stock>>&) {  }
    virtual unsigned getPrice()const {  }
    void setQuantity(const unsigned q){ quantity = q; }
    unsigned& getQuantity(){ return quantity; }
    std::string getClientName()const{ return clientName; }
    std::string getStockName()const{ return stockName; }
    virtual ~Request(){  }
};


class BuyOrders: public Request{
    unsigned buyPrice;
public:
    BuyOrders(std::string, std::string, unsigned, unsigned);
    bool processRequest(std::unordered_map<std::string, std::unique_ptr<Stock>>& stocks);
    unsigned getPrice()const;
    virtual~BuyOrders(){}
};

class SellOrders: public Request{
    unsigned sellPrice;
public:
    SellOrders(std::string clientName, std::string stockName, unsigned quantity, unsigned price);
    bool processRequest(std::unordered_map<std::string, std::unique_ptr<Stock>>& stocks);
    unsigned getPrice()const;
    virtual ~SellOrders(){}
};




class MatchingEngine{
private:
    std::unordered_map<std::string, std::unique_ptr<Stock>> stocks;
    //static unsigned ID = 0;
    std::vector<std::shared_ptr<Request>> requests;
public:
    void run(std::string msg);
    void handleRequests();
};

