#include<memory>
#include<set>
#include<unordered_map>
#include<vector>


enum TransactionType{
    SELL = 1,
    BUY = 2
};


class Stock{
    std::string name;
    std::set<std::shared_ptr<Request>> buyOrders;
    std::set<std::shared_ptr<Request>> sellOrders;
public:
    Stock(std::string name, TransactionType orderType, const std::shared_ptr<Request>& r){
        switch (orderType){
            case TransactionType::BUY : { buyOrders.insert(r); break; }
            case TransactionType::SELL: { sellOrders.insert(r); break; }
        }
    }
    std::set<std::shared_ptr<Request>>& getBuyOrders(){ return buyOrders; }
    std::set<std::shared_ptr<Request>>& getSellOrders(){ return sellOrders; }
};


class Request{
    std::string clientName;
    std::string stockName;
    std::chrono::system_clock::time_point reqTime;
    unsigned quantity;
public:
    Request(std::string clientName, std::string stockName, unsigned quantity): clientName(clientName), stockName(stockName), reqTime(std::chrono::system_clock::now()), quantity(quantity){ }
    virtual bool processRequest(std::unordered_map<std::string, std::unique_ptr<Stock>>&);
    virtual unsigned getPrice()const;
    void setQuantity(const unsigned q){ quantity = q; }
    unsigned& getQuantity(){ return quantity; }
    std::string getClientName()const{ return clientName; }
    std::string getStockName()const{ return stockName; }
};

class BuyOrders: public Request{
    unsigned buyPrice;
public:
    virtual bool processRequest(std::unordered_map<std::string, std::unique_ptr<Stock>>& stocks);
    BuyOrders(std::string clientName, std::string stockName, unsigned quantity, unsigned price): Request(clientName, stockName, quantity), buyPrice(price){ }
   
    virtual unsigned getPrice()const;
    bool operator <(const BuyOrders* rhs)const; 
};

class SellOrders: public Request{
    unsigned sellPrice;
public:
    virtual bool processRequest(std::unordered_map<std::string, std::unique_ptr<Stock>>& stocks);
    bool operator <(const SellOrders* rhs)const;
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

