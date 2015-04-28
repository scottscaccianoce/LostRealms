

/*
 *  Database.cpp
 *  sqlite3
 *
 *  Created by sscaccianoce on 10/10/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Database.h"

Database::Database(const std::string &path)
{
    err = sqlite3_open(path.c_str(),&db);
}

Database::~Database()
{
    sqlite3_close(db);
}

int Database::GetError() const
{
    return err;
}

bool Database::Query(const std::string &sql)
{
    //Reset error
    err = 0;
    int rc;
    sqlite3_stmt *statement;
    if((rc = sqlite3_prepare(db,sql.c_str(),-1,&statement,0)) != SQLITE_OK){
        err = rc;
        return false;
    }
	
    //run though and fill out results
    results.clear();
    int cols = sqlite3_column_count(statement);
	total_cols = cols;
    int result;
    while((result = sqlite3_step(statement)) == SQLITE_ROW){
		
        std::vector<std::string> values;
		
        for(int col = 0; col < cols; ++col){
            std::string val;
            char * ptr = (char*)sqlite3_column_text(statement,col);
            val = (ptr) ? ptr: "";
            values.push_back(val);
        }
		
        results.push_back(values);
    }
	
	total_rows = results.size();
    sqlite3_finalize(statement);
    return true;
}

std::string Database::GetValue(int row, int col) {
	return results[row][col];
}

int Database::GetInteger(int row, int col) {
	return atoi(GetValue(row,col).c_str());
}

float Database::GetFloat(int row, int col) {
	return atof(GetValue(row,col).c_str());
}