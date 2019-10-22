#include<iostream>
#include<unordered_map>
#include<map>
#include<vector>
#include<algorithm>
#include<chrono>

//same person inserting same price for same share
//multithreading
//smart pointers

class Request{
    std::string clientName;
    std::string stockName;
    std::chrono::system_clock::time_point reqTime;
    unsigned quantity;
public:
    Request(std::string clientName, std::string stockName, unsigned quantity): clientName(clientName), stockName(stockName), reqTime(std::chrono::system_clock::now()), quantity(quantity){ }
    virtual bool processRequest(std::unordered_map<Stock>&);
};

class BuyOrders: public Request{
    std::map<unsigned> prices;
public:
    virtual bool processRequest(std::unordered_map<Stock>& stocks){
        auto& stockToBuy = stocks[stockName];
        auto& sellOrders = stockToBuy.getSellOrders();
        for(auto& sellOrder : sellOrders){
            auto& sellPrices = sellOrders.getPrices();
            if( >=
        }
        
    }
    BuyOrders(std::string clientName, std::string::stockName, unsigned quantity, unsigned price): Request(clientName, stockName, quantity){
        prices.insert(price);  
    }
};

class SellOrders: public Request{
    std::map<unsigned> prices;
public:
    virtual bool processRequest(std::unordered_map<Stock>& stocks){
        //if matcching candidate is found, do the needful
            //print to matchingengine console
            //print to buyer and seller's console
            //return true;
        //return false;
    }
    
    SellOrders(std::string clientName, std::string::stockName, unsigned quantity, unsigned price): Request(clientName, stockName, quantity){
        prices.insert(price);  
    }
};

class Stock{
    std::string name;
    std::map<BuyOrders*> buyOrders;
    std::map<SellOrders*> sellOrders;
    Stock(std::string name, std::string orderType, Request* r){
        switch (orderType){
            case "buy" : { buyOrders.insert(r); break; }
            case "sell": { sellOrders.insert(r); break; }
        }
    }
};


class MatchingEngine{
private:
    std::unordered_map<Stock> stocks;
    static unsigned ID = 0;
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
            if(req.processRequest(stocks)){
                //delete from unorder_map
            }
        }
    }
};