/*
 *  Database.h
 *  sqlite3
 *
 *  Created by sscaccianoce on 10/10/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef __SQL3_DATABASE__
#define __SQL3_DATABASE__

#include <vector>
#include <string>
#include "sqlite3.h"

typedef std::vector < std::vector< std::string> > DataSet;


class Database{
	
public:
    Database(const std::string &path);
    ~Database();
    bool Query(const std::string &sql);
    int GetError() const;
	
    //std::vector < std::vector< std::string> > results;
    DataSet results;
	
	
    std::string GetValue(int row, int col);
	int GetInteger(int row, int col);
	float GetFloat(int row, int col);
	
	
	int total_rows;
	int total_cols;
	
private:
    sqlite3 *db;
    int err;
	
	
};



#endif
