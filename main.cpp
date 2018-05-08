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
    using Group = UInt[GROUP_SIZE];
    using GroupItem = Group[GROUP_NUM];
    //using GroupTable = QList<GroupItem>;

    //GroupTable table;
    std::list<std::vector<int>> table;
public:
    //select m from n,get arrange situation of m
    Int arrange(Int n,Int m){
        Int sum=1;
        m=n-m;
        for(;n>m;n--){
            sum*=n;
        }
        return sum;
    }
    //select m from n,get combine situation of m
    Int combine(Int n,Int m){
        Int sum1=1;
        Int sum2=1;
        Int sum3=1;
        Int mmax=m;
        if(mmax < n/2){
            mmax = n-m;
            m = n - mmax;
        }
        for(;n>mmax;n--){
            sum1*=n;
        }
        for(;m>1;m--){
            sum2*=m;
        }
        sum3 = sum1/sum2;
        return sum3;
    }
    bool isSameGroup(std::vector<int> vec1,std::vector<int> vec2){
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
                    vec2[i]=GROUP_NUM;
                }
                if(vec1key == GROUP_NUM && vec1[i]!=vec1key){
                    vec1key=vec1[i];
                    vec2key=vec2[i];
                    vec1[i]=GROUP_NUM;
                    vec2[i]=GROUP_NUM;
                }
            }
        }
        return true;
    }
    void initGroupTable(){
        std::vector<int> vec;
        std::vector<int> vec0;
        vec.assign({0,0,1,1,2,2});
        /*vec.assign(MAX_ITEM,0);
        for(int i=0;i<MAX_ITEM;i++){
            vec[i]=i/GROUP_SIZE;
        }*/
        /*Int cnum=combine(15,5)*combine(10,5)/arrange(3,3);
        std::cout<<"c(15,5)="<<combine(15,5)<<"\r\n";
        std::cout<<"c(10,5)="<<combine(10,5)<<"\r\n";
        std::cout<<"c(3,3) ="<<arrange(3,3)<<"\r\n";
        std::cout<<"cnum ="<<cnum<<"\r\n";*/

        /*for(int i=MAX_ITEM-5;i<MAX_ITEM;i++){
            vec[i]=1;
        }*/
        //table.push_back(vec);
        //for(Int i=0;i<cnum;i++){
        do{
            bool duliplcated=false;
            for(auto i=table.begin();i!=table.end();i++){
                if(isSameGroup(*i,vec)){
                    duliplcated=true;
                    break;
                }
            }
            if(duliplcated==false){
                table.push_back(vec);
                std::cout<<"vec:";
                for(const auto& i:vec){
                  std::cout<<i<<',';
                }
                std::cout<<std::endl;
            }else{
                std::cout<<"dulipciated vec:";
                for(const auto& i:vec){
                  std::cout<<i<<',';
                }
                std::cout<<std::endl;
                //std::cout<<"size="<<table.size()<<std::endl;
            }
            /*std::cout<<"vec:";
            for(const auto& i:vec){
              std::cout<<i<<',';
            }
            std::cout<<std::endl;*/
            //table.push_back(vec);
        }while(std::next_permutation(vec.begin(),vec.end()));
        /*for(auto i=table.begin();table)
        for(int i=0;i<table.size();i++){
            if(isSameGroup(vec0,vec)){
                std::cout<<"testing vec:";
                for(const auto& i:vec){
                  std::cout<<i<<',';
                }
                std::cout<<"    result="<<isSameGroup(vec0,vec)<<std::endl;
            }
        }*/
        std::cout<<"table="<<table.size()<<"\r\n";
        /*FILE *fp=fopen("table.txt","w+");
        for(const std::vector<int>& vec:table){
            for(const auto& i:vec){
               fprintf(fp,"%d,",i);
            }
            fprintf(fp,"\n");
        }
        fclose(fp);*/
    }
    void init(){
        initGroupTable();
    }
    UInt test(const std::vector<int> data){
        UInt maxSum=0;
        if(data.size()!=MAX_ITEM)return false;
        for(auto i=table.begin();i!=table.end();i++){
            UInt groupSum[GROUP_NUM]={0};
            for(int j=0;j<MAX_ITEM;j++){
                groupSum[(*i)[j]] += data[j];
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
    /*std::vector<int> data={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    timer.restart();
    auto maxSum=h.test(data);
    std::cout<<"maxSum="<<maxSum<<"\r\n";
    qDebug()<<"test cost time"<<timer.elapsed();*/
    return a.exec();
}
