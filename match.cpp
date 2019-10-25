#include<iostream>
#include<algorithm>
#include "match.h"

//same person inserting same price for same share
//multithreading
//smart pointers
//take into account time

Request::Request(std::string clientName, std::string stockName, unsigned quantity): clientName(clientName), stockName(stockName), reqTime(std::chrono::system_clock::now()), quantity(quantity){ }

Stock::Stock(std::string name, TransactionType orderType, const std::shared_ptr<Request>& r){
        switch (orderType){
            case TransactionType::BUY : { buyOrders.insert(r); break; }
            case TransactionType::SELL: { sellOrders.insert(r); break; }
        }
    }


bool BuyOrders::processRequest(std::unordered_map<std::string, std::unique_ptr<Stock>>& stocks){
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
            if(!sellingQuantity) sellOrders.erase(minSellOrder);
            return true;
        }
        else if(buyQuantity > sellingQuantity){
            buyQuantity -= sellingQuantity;
            sellOrders.erase(minSellOrder);
        }
    }
    return false;
}

BuyOrders::BuyOrders(std::string clientName, std::string stockName, unsigned quantity, unsigned price): Request(clientName, stockName, quantity), buyPrice(price){ }

unsigned BuyOrders::getPrice()const{
    return buyPrice;
}




bool SellOrders::processRequest(std::unordered_map<std::string, std::unique_ptr<Stock>>& stocks){
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
            maxBuyOrder->setQuantity(buyingQuantity - sellQuantity);
            if(!buyingQuantity) buyOrders.erase(maxBuyOrder);
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

SellOrders::SellOrders(std::string clientName, std::string stockName, unsigned quantity, unsigned price): Request(clientName, stockName, quantity), sellPrice(price){ }

unsigned SellOrders::getPrice()const{
    return sellPrice;
}

void MatchingEngine::run(std::string msg){
std::cout << msg << std::endl;
    //Listen to port
    //if there is a new request
        //create a new request object
        //add it to stocks buyorder or sellorders
    //else
        //process the currently remaining requests
}
void MatchingEngine::handleRequests(){
    for(auto& req : requests){
        if(req->processRequest(stocks)){
            std::remove(requests.begin(), requests.end(), req);
        }
    }
}

int main(){

}
