#include <iostream>
#include <bitset>
#include <vector>
#include <algorithm>
#include <set>
#include <array>
#include <list>
#include <time.h>

std::ostream& operator<<( std::ostream& os, const std::array<int,20>& data){
    for(int i=0;i<20;i++){
        os<<data[i]<<",";
    }
    return os;
}


std::ostream& operator<<( std::ostream& os, const std::array<int,15>& data){
    for(int i=0;i<15;i++){
        os<<data[i]<<",";
    }
    return os;
}
std::ostream& operator<<( std::ostream& os, const std::array<int,10>& data){
    for(int i=0;i<10;i++){
        os<<data[i]<<",";
    }
    return os;
}
void printArray(const int* arr,int size){
    for(int i=0;i<size;i++){
        std::cout<<arr[i]<<",";
    }
}

template<int GROUP_SIZE,int GROUP_NUM>
class BaseGrouper{
public:
    //EG:GROUP_SIZE=5,GROUP_NUM=3
    //[1,1,1,1,1,2,2,2,2,2,3,3,3,3,3]
    //static constexpr int GROUP_SIZE=5;
    //static constexpr int GROUP_NUM=3;
    static constexpr int MAX_ITEM=GROUP_SIZE*GROUP_NUM;
    using Group = std::array<int,MAX_ITEM>;
    std::list<Group> table;
    std::set<std::array<int,GROUP_NUM>> sortedSet;
public:
    bool isGroupUnique(Group group){
        std::array<std::bitset<MAX_ITEM>,GROUP_NUM> bitgroup;
        for(size_t i=0;i<MAX_ITEM;i++){
            bitgroup[group[i]][i]=1;
        }
        std::array<int,GROUP_NUM> idgroup;
        for(int i=0;i<GROUP_NUM;i++){
            idgroup[i]=bitgroup[i].to_ulong();
        }
        std::sort(idgroup.begin(),idgroup.end());
        auto it=sortedSet.find(idgroup);
        if(it==sortedSet.end()){
            sortedSet.insert(idgroup);
            return true;
        }
        return false;
    }
    void init(){
        Group vec;
        vec.fill(0);
        for(int i=0;i<MAX_ITEM;i++){
            vec[i]=i/GROUP_SIZE;
        }
        do{
            if(isGroupUnique(vec)){
                table.push_back(vec);
            }
        }while(std::next_permutation(vec.begin(),vec.end()));
        std::cout<<"GROUP_NUM="<<GROUP_NUM<<",table="<<table.size()<<"\r\n";
    }
    int test(const int* data){
        std::cout<<"grouping"<<GROUP_NUM<<"(";
        printArray(data,MAX_ITEM);
        std::cout<<"):";
        int error=10000;
        for(auto i=table.begin();i!=table.end();i++){
            int groupSum[GROUP_NUM]={0};
            bool succ=true;
            for(int j=0;j<MAX_ITEM;j++){
                int group=(*i)[j];
                groupSum[group] += data[j];
            }
            int currentError=0;
            for(int j=0;j<GROUP_NUM;j++){
                currentError += groupSum[j]-1250;
                if(groupSum[j] < 1250-20 || groupSum[j] > 1250+100){
                    succ=false;
                }
            }
            if(currentError < error){
                error=currentError;
            }
            if(succ){
                std::cout<<" succ:"<<(*i)<<"error="<<error<<"\r\n";
                return 1;
                break;
            }
        }
        std::cout<<"failed,error="<<error<<"\r\n";
        return 0;
    }
};
using Grouper3 = BaseGrouper<5,3>;
using Grouper2 = BaseGrouper<5,2>;

int test(const int* data){
    std::cout<<"grouping1"<<"(";
    printArray(data,5);
    std::cout<<"):";
    bool succ=true;
    int groupSum=0;
    for(int j=0;j<5;j++){
        groupSum += data[j];
    }
    if(groupSum < 1250-20 || groupSum > 1250+100){
        succ=false;
    }
    if(succ){
        std::cout<<" succ:"<<"\r\n";
        return 1;
    }
    std::cout<<"failed\r\n";
    return 0;
}

int main(int argc, char *argv[])
{
    std::srand(time(0));
    Grouper3 grouper3;
    Grouper2 grouper2;
    grouper2.init();
    grouper3.init();
    int total=0;
    int miss=0;
    int itemErr=10;
    int itemMid=250;
    constexpr int TEST_SIZE=1000;
    std::array<int,TEST_SIZE> data;
again:
    for(int i=0;i<TEST_SIZE;i++){
        data[i]=std::rand()%(itemErr*2)+itemMid - itemErr;
    }
    int lp=0;
    while(1){
        if(lp+15>1000){
            break;
        }
        int result;
        result=grouper3.test(data.data()+lp);
        if(result){
            lp+=grouper3.MAX_ITEM;
            total+=grouper3.MAX_ITEM;
        }else if(result==0){
            result=grouper2.test(data.data()+lp);
            if(result){
                lp+=grouper2.MAX_ITEM;
                total+=grouper2.MAX_ITEM;
            }else if(result==0){
                result=test(data.data()+lp);
                if(result==0){
                    miss++;
                    total++;
                    lp+=1;
                }else{
                    lp+=5;
                    total+=5;
                }
            }
        }
    }
    system("pause");
    goto again;
    return 0;
}
