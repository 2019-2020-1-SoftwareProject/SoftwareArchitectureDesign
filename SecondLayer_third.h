//
//  SecondLayer_third.hpp
//  SoftwareArchitecture
//
//  Created by 曹梓睿 on 2020/3/31.
//  Copyright © 2020 曹梓睿. All rights reserved.
//

#ifndef SecondLayer_third_h
#define SecondLayer_third_h

#include "p_DataBaseOp.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace DBHelper;

class SecondLayer_third:p_DataBaseOp {
private:
    p_DataBaseOp databaseOp;
    string nameTemp, idTemp, placeTemp, phoneTemp;
    unsigned yearTemp, monthTemp, dayTemp;
    
public:
    SecondLayer_third(){
        nameTemp = "";
        idTemp = "";
        placeTemp = "";
        phoneTemp = "";
        yearTemp = 2020;
        monthTemp = 1;
        dayTemp = 1;
    }
    
    //提供信息以预约订单
    //_name 预约人姓名
    //_id 预约人身份证号
    //_place 预约人预约购买地
    //_phone 预约人电话号码
    //_rec 是否需要预约凭证
    //_year,_month,_day 预约购买时间
    //path 凭证保存的路径
    void newOrder(string _name, string _id, string _place, string _phone, bool _rec,
                  unsigned _year,unsigned _month, unsigned _day, string path = ""){
        nameTemp = _name;
        idTemp = _id;
        placeTemp = _place;
        phoneTemp = _phone;
        yearTemp = _year;
        monthTemp = _month;
        dayTemp = _day;
        Date _date(_year, _month, _day);
        OrderInfo _orderInfo(_name, _id, _place, _phone, _rec, _date);
        databaseOp.addOrder(_orderInfo);
        
        if (_rec) {
            getCertificate(path);
        }
    }
    
    //生成预约信息凭证 txt
    void getCertificate(string path){
        ofstream fout(path);
        if (fout) {
            fout << "预约凭证" << endl;
            fout << "预约人姓名: " << nameTemp << endl;
            fout << "预约人身份证号: " << idTemp << endl;
            fout << "预约人预约购买地: " << placeTemp << endl;
            fout << "预约人电话号码: " << phoneTemp << endl;
            fout << "预约购买时间: " << yearTemp << "年" << monthTemp << "月" << dayTemp <<"日";
        }
        fout.close();
    }
    
    //预约后若购买完成
    //只提供身份证号或电话号码即可
    //true提供id，false提供phone
    void purchased(const string& ipOrid, bool isId){
        databaseOp.deleteOrder(ipOrid, isId);
    }
    
};

#endif /* SecondLayer_third_hpp */
