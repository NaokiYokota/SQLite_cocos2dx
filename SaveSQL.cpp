//
//  SaveSQL.cpp
//
//
//  Created by albatrus on 2014/06/10.
//
//

#include "SaveSQL.h"

#define dbName "sample.db"

USING_NS_CC;

//データベースを開く
int SaveSQL::sqliteOpen(sqlite3 **db){
    
    auto filePath = FileUtils::getInstance()->getWritablePath();
    filePath += dbName;
    
    
    return sqlite3_open(filePath.c_str(),db);
    
}


//データベースのテーブルの作成
void SaveSQL::sqliteCreateTable(){
    
    //データベースを作成
    sqlite3 *db = NULL;
    if(sqliteOpen(&db) == SQLITE_OK){
        
        // key とvalueの２つ値がある テーブル(test1)を作成
        const char *sql_createtable = "CREATE TABLE test1(key TEXT,value TEXT)";
        sqlite3_stmt *stmt = NULL;
        if (sqlite3_prepare_v2(db, sql_createtable, -1, &stmt, NULL) == SQLITE_OK) {
            
            if (sqlite3_step(stmt) == SQLITE_DONE) {
                
                CCLOG("create table done");
            }
            
            sqlite3_reset(stmt);
            
            
        }
        
        sqlite3_finalize(stmt);
        
        
        
    }
    
    sqlite3_close(db);
        
    
    
    
    
}


//keyから値を取り出す。
const char *SaveSQL::sqliteGetValueForKey(const char *key){
    
    __String *strValue = NULL;
    sqlite3 *db = NULL;
    if (sqliteOpen(&db) == SQLITE_OK) {
        
        //test1テーブルからkeyに合わせたvalueを取り出す
        const char *sql_select = "SELECT value FROM test1 WHERE key=?";
        sqlite3_stmt *stmt = NULL;
        if (sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL) == SQLITE_OK) {
            
            sqlite3_bind_text(stmt, 1, key, -1, SQLITE_TRANSIENT);
            int ret = sqlite3_step(stmt);
            if (ret == SQLITE_DONE || ret == SQLITE_ROW) {
                
                const char *val = (const char*)sqlite3_column_text(stmt, 0);
                strValue = __String::create(val);
                
            }
            
            sqlite3_reset(stmt);
            
        }
        
        sqlite3_finalize(stmt);
        
    }
    
    sqlite3_close(db);
    
    
    //valueがある場合
    if (strValue != NULL) {
        
        return strValue->getCString();
    }
    
    return NULL;
}


//keyとvalueを設定する(初期)
void SaveSQL::sqliteSetValueForKey(const char *key,const char *value){
    
    
    std::string fullpath = CCFileUtils::getInstance()->getWritablePath();
    fullpath += dbName;
    sqlite3 *db = NULL;
    
    if (sqlite3_open(fullpath.c_str(), &db) == SQLITE_OK) {
        
        const char *sql_select = "REPLACE INTO test1(key,value)VALUES(?,?)";
        sqlite3_stmt *stmt = NULL;
        
        if (sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL) == SQLITE_OK) {
            
            sqlite3_bind_text(stmt, 1, key, -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, value, -1, SQLITE_TRANSIENT);
            
            if (sqlite3_step(stmt) == SQLITE_DONE) {
                
                CCLOG("replace key:%s value:%s",key,value);
                
            }
            
            sqlite3_reset(stmt);
            
        }
        
        sqlite3_finalize(stmt);
        
    }
    
    sqlite3_close(db);
    
    
}


//keyを元にValueの値を更新する
void SaveSQL::sqliteUpdateValueForKey(const char *key,const char *value){
    
    
    std::string fullpath = CCFileUtils::getInstance()->getWritablePath();
    fullpath += dbName;
    sqlite3 *db = NULL;
    
    if (sqlite3_open(fullpath.c_str(), &db) == SQLITE_OK) {
        
        const char *sql_select = "UPDATE test1 SET value =? WHERE key =? ";
        sqlite3_stmt *stmt = NULL;
        
        if (sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL) == SQLITE_OK) {
            
            sqlite3_bind_text(stmt, 1, value, -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, key, -1, SQLITE_TRANSIENT);
            
            if (sqlite3_step(stmt) == SQLITE_DONE) {
                
                CCLOG("change key:%s value:%s",key,value);
                
            }
            
            sqlite3_reset(stmt);
            
        }
        
        sqlite3_finalize(stmt);
        
    }
    
    sqlite3_close(db);
    
    
}



