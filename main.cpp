#include <QCoreApplication>
#include <iostream>
#include <QDebug>
#include <bitset>
#include <QBitArray>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

//static_assert(GROUP_MAX_ITEM<sizeof(UInt)*8,"invalid BITNUM");
//using Bit = std::bitset<BITNUM>;

class Grouper{
public:
    //EG:GROUP_SIZE=5,GROUP_NUM=3
    //[1,1,1,1,1,2,2,2,2,2,3,3,3,3,3]
    static constexpr int GROUP_SIZE=5;
    static constexpr int GROUP_NUM=3;
    static constexpr int MAX_ITEM=GROUP_SIZE*GROUP_NUM;
    using Int = int64_t;
    using UInt = uint32_t;
    std::list<std::vector<int>> table;
public:
    bool isSameGroup(std::vector<int> vec1,const std::vector<int>& vec2){
        if(vec1.size()!=vec2.size()){
            return false;
        }
        for(int j=0;j<GROUP_NUM;j++){
            int vec1key=GROUP_NUM;
            int vec2key=GROUP_NUM;
            for(size_t i=0;i<vec1.size();i++){
                if(vec1key != GROUP_NUM && vec1key==vec1[i]){
                    if(vec2key != vec2[i]){
                        return false;
                    }
                    vec1[i]=GROUP_NUM;
                }
                if(vec1key == GROUP_NUM && vec1[i]!=vec1key){
                    vec1key=vec1[i];
                    vec2key=vec2[i];
                    vec1[i]=GROUP_NUM;
                }
            }
        }
        return true;
    }
    void initGroupTableAll(){
        std::vector<int> vec;
        vec.assign(MAX_ITEM,0);
        for(int i=0;i<MAX_ITEM;i++){
            vec[i]=i/GROUP_SIZE;
        }
        do{
            table.push_back(vec);
        }while(std::next_permutation(vec.begin(),vec.end()));
        std::cout<<"table="<<table.size()<<"\r\n";
    }
    void removeDuplicated(){
        for(auto i=table.rbegin();i!=table.rend();i++){
            auto j=i;
            j++;
            for(;j!=table.rend();j++){
                if(isSameGroup(*i,*j)){
                    (*i)[0]=GROUP_NUM;
                    break;
                }
            }
        }
        std::cout<<"mark delete complete\r\n";
        for(auto i=table.begin();i!=table.end();){
            if((*i)[0]==GROUP_NUM){
                i=table.erase(i);
            }else{
                i++;
            }
        }
    }
    void init(){
        initGroupTableAll();
    }
    UInt test(const std::vector<int> data){
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
    h.removeDuplicated();
    qDebug()<<"remove duplicated cost time"<<timer.elapsed();
    /*int maxitem=h.table.size();
    for(int i=0;i<maxitem/2;i++){
        h.table.pop_back();
    }*/
    timer.restart();
    std::vector<int> data={1,2,3,4,5,6,7,8,9,10};
    auto maxSum=h.test(data);
    std::cout<<"maxSum="<<maxSum<<"\r\n";
    qDebug()<<"test cost time"<<timer.elapsed()<<"real table="<<h.table.size();
    return a.exec();
}
