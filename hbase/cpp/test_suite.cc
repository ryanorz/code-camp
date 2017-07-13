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

#include "hbasecpp.h"
#include <assert.h>
#include <iostream>

std::string host("192.168.220.157:9876");

int main()
{
    std::cout << "Create table \"hbasecpp\"\n";
    HbaseCpp hbase(host);
    std::string tableName("hbasecpp");
    if (!hbase.TableExists(tableName)) {
        TableSchema tableSchema;
        tableSchema.set_name(tableName);
        auto family = tableSchema.add_columns();
        family->set_name("family");
        family->set_maxversions(1);
        assert(hbase.CreateTable(tableName, tableSchema));
    }

    std::cout << "Insert row to \"hbasecpp\"\n";
    CellSet cellSet;
    auto rows = cellSet.add_rows();
    rows->set_key("rowkey");
    auto mutValues = rows->mutable_values();
    auto cell = mutValues->Add();
    cell->set_column("family:a");
    cell->set_data("aaaaaaa");
    assert(hbase.PutRow(tableName, cellSet));

    std::cout << "Read row from \"hbasecpp\"\n";
    assert(hbase.GetRow(tableName, "rowkey", cellSet));

    std::cout << "Scan \"hbasecpp\"\n";
    std::string scanner;
    Scanner scannerSchema;
    scannerSchema.set_batch(10);
    assert(hbase.GetScanner(tableName, scannerSchema, scanner));
    assert(hbase.ScanByScanner(scanner, cellSet));
    assert(hbase.DeleteScanner(scanner));

    std::cout << "Delete row from \"hbasecpp\"\n";
    assert(hbase.DeleteRow(tableName, "rowkey"));

    std::cout << "Delete table \"hbasecpp\"\n";
    assert(hbase.DeleteTable(tableName));

    return 0;
}
