#include <QCoreApplication>
#include <iostream>
#include <QDebug>
#include <bitset>
#include <QBitArray>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <set>

//static_assert(GROUP_MAX_ITEM<sizeof(UInt)*8,"invalid BITNUM");
//using Bit = std::bitset<BITNUM>;

template<int GROUP_SIZE,int GROUP_NUM>
class BaseGrouper{
public:
    //EG:GROUP_SIZE=5,GROUP_NUM=3
    //[1,1,1,1,1,2,2,2,2,2,3,3,3,3,3]
    //static constexpr int GROUP_SIZE=5;
    //static constexpr int GROUP_NUM=3;
    static constexpr int MAX_ITEM=GROUP_SIZE*GROUP_NUM;
    using Int = int64_t;
    using UInt = uint32_t;
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
        std::cout<<"table="<<table.size()<<"\r\n";
    }
    UInt test(const std::array<int,MAX_ITEM> data){
        UInt maxSum=0;
        if(data.size()!=MAX_ITEM)return false;
        for(auto i=table.begin();i!=table.end();i++){
            UInt groupSum[GROUP_NUM]={0};
            for(int j=0;j<MAX_ITEM;j++){
                int group=(*i)[j];
                groupSum[group] += data[j];
            }
            if(groupSum[0] > maxSum){
                maxSum=groupSum[0];
            }
        }
        return maxSum;
    }
};
using Grouper = BaseGrouper<5,3>;

#include <QTime>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Grouper h;
    QTime timer;
    timer.restart();
    h.init();
    qDebug()<<"init cost time"<<timer.elapsed();
    timer.restart();
    std::array<int,Grouper::MAX_ITEM> data;
    for(int i=0;i<Grouper::MAX_ITEM;i++){
        data[i]=i+1;
    }
    auto maxSum=h.test(data);
    std::cout<<"maxSum="<<maxSum<<"\r\n";
    qDebug()<<"test cost time"<<timer.elapsed()<<"real table="<<h.table.size();
    return a.exec();
}
