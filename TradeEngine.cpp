#include<iostream>
#include<fstream>
#include<map>
#include<string>
#include<vector>

using namespace std;

class Utility{
    public:
    static void splitString(string s, vector<string> &v)
    {
	 string temp = "";
	 for(int i=0;i<s.size();++i)
     {
		
		if(s[i]==','){
			v.push_back(temp);
			temp = "";
		}
		else if (s[i]!='\r' && s[i]!='\n'){
			temp.push_back(s[i]);
		}
		
	 }
	 v.push_back(temp);
    }

    // File assertion, used for tests
    static bool compareFiles(string f1, string f2)
    {
        bool areSame=true;
        ifstream in(f1);
        ifstream in2(f2);

        while ((!in.eof()) && (!in2.eof())) 
        {
            string line,line2;
            getline(in,line);
            getline(in2,line2);
            cout<<line<<"       "<<line2<<endl;
            if(line!=line2)
            {
                areSame=false;
                break;
            }
        }

        in.close();
        in2.close();
        return areSame;
    }

};

struct Trade{
    string symbol;
    long timeStamp;
    int quantity;
    int price;

    Trade(string _symbol, long _timeStamp, int _quantity, int _price)
         :symbol(_symbol),timeStamp(_timeStamp), quantity(_quantity), price(_price)
    {}
};

class StockData{
    private:
    long lastTimeStamp;
    int maxGap;
    long volume;
    float avgPrice;
    int maxPrice;

    public:
    StockData(){}
    StockData (Trade t):lastTimeStamp(t.timeStamp), maxGap(0), volume(t.quantity), avgPrice(t.price), maxPrice(t.price)
    {}

    int getMaxGap()
    {
        return maxGap;
    }
    
    long getVolume()
    {
        return volume;
    }

    int getAvgPrice()
    {
        return avgPrice;
    }

    int getMaxPrice()
    {
        return maxPrice;
    }

    void addTrade(Trade t)
    {
        if((t.timeStamp-lastTimeStamp)>maxGap)
        {
            maxGap=t.timeStamp-lastTimeStamp;
        }
        lastTimeStamp=t.timeStamp;
        
        avgPrice=((avgPrice*volume)+(t.price*t.quantity))/(volume+t.quantity);
        volume=volume+t.quantity;

        if(t.price>maxPrice)
        {
            maxPrice=t.price;
        }

    }
};

class TradeEngine{
    map<string, StockData> stockMap;
    std::ifstream inputFile;

    private:
    void processTrade(Trade t)
    {
        // Once Stock data is available, we just keep adding into it, else initialize it
        if(stockMap.find(t.symbol)==stockMap.end())
        {
            StockData s(t);
            stockMap[t.symbol]=s;
        }
        else
        {
            stockMap[t.symbol].addTrade(t);
        }
    }

    public:
    void loadInputFile(string filename)
    {
        inputFile.open(filename);
    }

    void readAndProcessInputFile()
    {
        string line;
        if(inputFile.good())
        { 
            while(std::getline(inputFile, line))
            {
                line.erase(line.size());
                vector<string> tempVector;

                // format before trade instantiation
                Utility::splitString(line,tempVector);

                if(tempVector.size()==4)
		        {
                  Trade t(tempVector[1],stol(tempVector[0]),stoi(tempVector[2]),stoi(tempVector[3]));
                  processTrade(t);
		        }
            }
        }
        inputFile.close();
    }

    void displayOutput()
    {
        cout<<"-----------------------------------------------------------"<<endl;
        cout<<"---------------------------OUTPUT--------------------------"<<endl;
        cout<<"-----------------------------------------------------------"<<endl;
        for(auto it=stockMap.begin();it!=stockMap.end();it++)
        {
            cout<<it->first<<" "<<it->second.getMaxGap()<<" "<<it->second.getVolume()<<" "<<(int)it->second.getAvgPrice()<<" "<<it->second.getMaxPrice()<<endl;
        }
    }

    void produceOutput(string filePath)
    {
        ofstream outFile (filePath);
        if (outFile.is_open())
        {
            for(auto it=stockMap.begin();it!=stockMap.end();it++)
            {
                outFile<<it->first<<","<<it->second.getMaxGap()<<","<<it->second.getVolume()<<","<<(int)it->second.getAvgPrice()<<","<<it->second.getMaxPrice()<<"\n";
            }
            outFile.close();
        }
    }

    void clearTradeEngine()
    {
        stockMap.clear();
    }

};

int main()
{
    TradeEngine te;

    /*
    //UNCOMMENT ABOVE THIS FOR TESTS
    //Tests Start

    cout<<"Test0:"<<endl;
    te.loadInputFile("Tests/Inputs/input0.csv");
    te.readAndProcessInputFile();
    te.produceOutput("Tests/Outputs/output0.csv");
    //te.displayOutput();

    if(Utility::compareFiles("Tests/Outputs/output0.csv", "Tests/ExpectedOutputs/output0.csv"))
    cout<<" OK"<<endl;
    else cout<<" KO"<<endl;
    cout<<endl;

    te.clearTradeEngine();

    cout<<"Test1:"<<endl;
    te.loadInputFile("Tests/Inputs/input1.csv");
    te.readAndProcessInputFile();
    te.produceOutput("Tests/Outputs/output1.csv");
    
    if(Utility::compareFiles("Tests/Outputs/output1.csv", "Tests/ExpectedOutputs/output1.csv"))
    cout<<" OK"<<endl;
    else cout<<" KO"<<endl;
    cout<<endl;

    te.clearTradeEngine();

    cout<<"Test2:"<<endl;
    te.loadInputFile("Tests/Inputs/input2.csv");
    te.readAndProcessInputFile();
    te.produceOutput("Tests/Outputs/output2.csv");

    if(Utility::compareFiles("Tests/Outputs/output2.csv", "Tests/ExpectedOutputs/output2.csv"))
    cout<<" OK"<<endl;
    else cout<<" KO"<<endl;
    cout<<endl;

    te.clearTradeEngine();

    cout<<"Test3:"<<endl;
    te.loadInputFile("Tests/Inputs/input3.csv");
    te.readAndProcessInputFile();
    te.produceOutput("Tests/Outputs/output3.csv");
    if(Utility::compareFiles("Tests/Outputs/output3.csv", "Tests/ExpectedOutputs/output3.csv"))
    cout<<" OK"<<endl;
    else cout<<" KO"<<endl;
    cout<<endl;

    te.clearTradeEngine();

    cout<<"Test4:"<<endl;
    te.loadInputFile("Tests/Inputs/input4.csv");
    te.readAndProcessInputFile();
    te.produceOutput("Tests/Outputs/output4.csv");
    if(Utility::compareFiles("Tests/Outputs/output4.csv", "Tests/ExpectedOutputs/output4.csv"))
    cout<<" OK"<<endl;
    else cout<<" KO"<<endl;
    cout<<endl;

    te.clearTradeEngine();

    cout<<"Test5:"<<endl;
    te.loadInputFile("Tests/Inputs/input5.csv");
    te.readAndProcessInputFile();
    te.produceOutput("Tests/Outputs/output5.csv");
    if(Utility::compareFiles("Tests/Outputs/output5.csv", "Tests/ExpectedOutputs/output5.csv"))
    cout<<" OK"<<endl;
    else cout<<" KO"<<endl;
    cout<<endl;

    te.clearTradeEngine();

    //Tests End

    //UNCOMMENT BELOW THIS FOR TESTS
    */

    // Actual result production
    cout<<"Working with given Input"<<endl;
    te.loadInputFile("input.csv");
    te.readAndProcessInputFile();
    te.produceOutput("output.csv");
    cout<<"Output produced"<<endl;
}
