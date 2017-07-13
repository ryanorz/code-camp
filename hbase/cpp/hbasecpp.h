/*
 * Copyright 2017 石印 <ryanorz@126.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef HBASECPP_H
#define HBASECPP_H

#include <string>
#include <memory>
#include "hbase_rest.pb.h"

class HbaseCpp
{
public:
    HbaseCpp();
    explicit HbaseCpp(const std::string &host);
    ~HbaseCpp();

    HbaseCpp(const HbaseCpp &rhs);
    HbaseCpp& operator=(const HbaseCpp &rhs);

    void set_host(const std::string &host);

    bool TableExists(const std::string &table);
    bool CreateTable(const std::string &table, const TableSchema &tableSchema);
    bool UpdateTable(const std::string &table, const TableSchema &tableSchema);
    bool DeleteTable(const std::string &table);
    bool ListTable(TableList &tableList);

    bool PutRow(const std::string& table, const CellSet& cellSet);
    bool GetRow(const std::string& table, const std::string& row, CellSet& cellSet);
    bool GetRowVersions(const std::string& table, const std::string& row, int versions, CellSet& cellSet);
    bool DeleteRow(const std::string& table, const std::string& row);

    bool GetScanner(const std::string &table, const Scanner& scanner, std::string &location);
    bool ScanByScanner(const std::string& location, CellSet &cellSet);
    bool DeleteScanner(const std::string& location);
private:
    struct Impl;
    std::shared_ptr<Impl> pImpl;
};

#endif // HBASECPP_H
