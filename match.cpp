#include<iostream>
#include<unordered_map>
#include<map>
#include<set>
#include<vector>
#include<algorithm>
#include<chrono>
#include<memory>

//same person inserting same price for same share
//multithreading
//smart pointers
//take into account time

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
    virtual bool processRequest(std::unordered_map<std::string, std::unique_ptr<Stock>>& stocks){
        auto& stockToBuy = stocks[getStockName()];
        auto& sellOrders = stockToBuy->getSellOrders();
        auto& buyQuantity = this->getQuantity();
        while(!buyQuantity){
            auto& minSellOrder = *std::min_element(sellOrders.begin(), sellOrders.end());
            if(minSellOrder->getPrice() > buyPrice) break;
            auto& sellingQuantity = minSellOrder->getQuantity();
            if(buyQuantity <= sellingQuantity){
                buyQuantity = 0;
                minSellOrder->setQuantity(sellingQuantity - buyQuantity);
                f(!sellingQuantity) sellOrders.erase(minSellOrder);
                return true;
            }
            else if(buyQuantity > sellingQuantity){
                buyQuantity -= sellingQuantity;
                sellOrders.erase(minSellOrder);
            }
        }
        return false;
    }

    BuyOrders(std::string clientName, std::string stockName, unsigned quantity, unsigned price): Request(clientName, stockName, quantity), buyPrice(price){ }
   
    virtual unsigned getPrice()const{
        return buyPrice;
    }
    
    bool operator <(const BuyOrders* rhs)const{
        return this->getPrice() < rhs->getPrice();
    }   
};

class SellOrders: public Request{
    unsigned sellPrice;
public:
    virtual bool processRequest(std::unordered_map<std::string, std::unique_ptr<Stock>>& stocks){
        auto& stockToSell = stocks[getStockName()];
        auto& buyOrders = stockToSell->getBuyOrders();
        auto& sellQuantity = this->getQuantity();
        if(!sellQuantity) return true;
        while(!sellQuantity){
            auto& maxBuyOrder = *std::max_element(buyOrders.begin(), buyOrders.end());
            if(maxBuyOrder->getPrice() < sellPrice) break;
            auto& buyingQuantity = maxBuyOrder->getQuantity();
            if(sellQuantity <= buyingQuantity){
                sellQuantity = 0;
                maxBuyOrder->setQuantity(buyQuantity - sellQuantity);
                if(!buyQuantity) buyOrders.erase(maxBuyOrder);
                return true;
            }
            else if(sellQuantity > buyingQuantity){
                sellQuantity -= buyingQuantity;
                buyingQuantity = 0;
                buyOrders.erase(maxBuyOrder);
            }
        }
        
        return false;
    }
    
    SellOrders(std::string clientName, std::string stockName, unsigned quantity, unsigned price): Request(clientName, stockName, quantity), sellPrice(price){ }
    
    virtual unsigned getPrice()const{
        return sellPrice;
    }
    
    bool operator <(const SellOrders* rhs)const{
        return this->getPrice() < rhs->getPrice();
    }
};


class MatchingEngine{
private:
    std::unordered_map<std::string, std::unique_ptr<Stock>> stocks;
    //static unsigned ID = 0;
    std::vector<std::shared_ptr<Request>> requests;
public:
    void run(std::string msg){
    std::cout << msg << std::endl;
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
