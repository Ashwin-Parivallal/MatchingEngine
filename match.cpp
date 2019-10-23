#include<iostream>
#include<unordered_map>
#include<map>
#include<set>
#include<vector>
#include<algorithm>
#include<chrono>

//same person inserting same price for same share
//multithreading
//smart pointers

class Stock;
class Request;
class BuyOrders;
class SellOrders;


enum TransactionType{
    SELL = 1,
    BUY = 2
};


class Stock{
    std::string name;
    std::set<Request*> buyOrders;
    std::set<Request*> sellOrders;
public:
    Stock(std::string name, TransactionType orderType, Request* r){
        switch (orderType){
            case TransactionType::BUY : { buyOrders.insert(r); break; }
            case TransactionType::SELL: { sellOrders.insert(r); break; }
        }
    }
    std::set<Request*>& getBuyOrders(){ return buyOrders; }
    std::set<Request*>& getSellOrders(){ return sellOrders; }
};


class Request{
    std::string clientName;
    std::string stockName;
    std::chrono::system_clock::time_point reqTime;
    unsigned quantity;
public:
    Request(std::string clientName, std::string stockName, unsigned quantity): clientName(clientName), stockName(stockName), reqTime(std::chrono::system_clock::now()), quantity(quantity){ }
    virtual bool processRequest(std::unordered_map<std::string, Stock*>&);
    virtual unsigned getPrice()const;
    void setQuantity(const unsigned q){ quantity = q; }
    unsigned& getQuantity(){ return quantity; }
    std::string getClientName()const{ return clientName; }
    std::string getStockName()const{ return stockName; }
};

class BuyOrders: public Request{
    unsigned price;
public:
    virtual bool processRequest(std::unordered_map<std::string, Stock*>& stocks){
        auto& stockToBuy = stocks[getStockName()];
        auto& sellOrders = stockToBuy->getSellOrders();
        auto& quantity = this->getQuantity();
        while(!quantity){
            auto& minSellOrder = *std::min_element(sellOrders.begin(), sellOrders.end());
            if(minSellOrder->getPrice() > price) break;
            auto& sellingQuantity = minSellOrder->getQuantity();
            if(quantity <= sellingQuantity){
                quantity = 0;
                minSellOrder->setQuantity(sellingQuantity - quantity);
                return true;
            }
            else{
                quantity -= sellingQuantity;
            }
            if(!sellingQuantity) sellOrders.erase(minSellOrder);
        }
        return false;
    }

    BuyOrders(std::string clientName, std::string stockName, unsigned quantity, unsigned price): Request(clientName, stockName, quantity), price(price){ }
   
    virtual unsigned getPrice()const{
        return price;
    }
    
    bool operator <(const BuyOrders* rhs)const{
        return this->getPrice() < rhs->getPrice();
    }   
};

class SellOrders: public Request{
    unsigned price;
public:
    virtual bool processRequest(std::unordered_map<std::string, Stock*>& stocks){
        auto& stockToSell = stocks[getStockName()];
        auto& buyOrders = stockToBuy->getBuyOrders();
        auto& sellNum = this->getQuantity();
        while(!sellNum){
            auto& maxBuyOrder = *std::max_element(sellOrders.begin(), sellOrders.end());
            if(maxBuyOrder->getPrice() < price) break;
            auto& buyingQuantity = maxBuyOrder->getQuantity();
            if(sellNum <= buyingQuantity){
                sellNum = 0;
                //maxOrder->setQuantity(sellingQuantity - quantity);
                return true;
            }
            else{
                sellNum -= buyingQuantity;
            }
            if(!sellingQuantity) sellOrders.erase(minSellOrder);
        }
        
        return false;
    }
    
    SellOrders(std::string clientName, std::string stockName, unsigned quantity, unsigned price): Request(clientName, stockName, quantity), price(price){ }
    
    virtual unsigned getPrice()const{
        return price;
    }
    
    bool operator <(const SellOrders* rhs)const{
        return this->getPrice() < rhs->getPrice();
    }
};


class MatchingEngine{
private:
    std::unordered_map<std::string, Stock*> stocks;
    //static unsigned ID = 0;
    std::vector<Request*> requests;
public:
    void run(){
        //Listen to port
        //if there is a new request
            //create a new request object
            //add it to stocks buyorder or sellorders
        //else
            //process the currently remaining requests
    }
    void handleRequests(){
        for(auto& req : requests){
            if(req->processRequest(stocks)){
                std::remove(requests.begin(), requests.end(), req);
            }
        }
    }
};


int main(){

}
