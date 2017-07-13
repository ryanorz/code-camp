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
#include <restbed>

using namespace restbed;

struct HbaseCpp::Impl {
    Impl()
        : host()
    {}
    Impl(const std::string& host_)
        : host(host_)
    {}

    std::string host;

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
};

bool HbaseCpp::Impl::TableExists(const std::string& table)
{
    auto request = std::make_shared<Request>(Uri(
        "http://" + host + "/" + table + "/schema"
    ));
    request->set_header("Host", host);
    auto response = Http::sync(request);
    auto stat = response->get_status_code();
    return (200 == stat);
}

bool HbaseCpp::Impl::CreateTable(const std::string& table, const TableSchema& tableSchema)
{
    if (TableExists(table))
        return true;

    auto request = std::make_shared<Request>(Uri(
        "http://" + host + "/" + table + "/schema"
    ));
    auto body = tableSchema.SerializeAsString();
    request->set_method("POST");
    request->set_header("Host", host);
    request->set_header("Content-Type", "application/x-protobuf");
    request->set_header("Content-Length", std::to_string(body.size()));
    request->set_body(body);
    auto response = Http::sync(request);
    auto stat = response->get_status_code();
    return (201 == stat);
}

bool HbaseCpp::Impl::UpdateTable(const std::string& table, const TableSchema& tableSchema)
{
    if (!TableExists(table))
        return false;

    auto request = std::make_shared<Request>(Uri(
        "http://" + host + "/" + table + "/schema"
    ));
    auto body = tableSchema.SerializeAsString();
    request->set_method("UPDATE");
    request->set_header("Host", host);
    request->set_header("Content-Type", "application/x-protobuf");
    request->set_header("Content-Length", std::to_string(body.size()));
    request->set_body(body);
    auto response = Http::sync(request);
    auto stat = response->get_status_code();
    return (200 == stat);
}

bool HbaseCpp::Impl::DeleteTable(const std::string& table)
{
    if (!TableExists(table))
        return true;

    auto request = std::make_shared<Request>(Uri(
        "http://" + host + "/" + table + "/schema"
    ));
    request->set_method("DELETE");
    request->set_header("Host", host);
    auto response = Http::sync(request);
    auto stat = response->get_status_code();
    return (200 == stat);
}

bool HbaseCpp::Impl::ListTable(TableList& tableList)
{
    auto request = std::make_shared<Request>(Uri(
        "http://" + host + "/"
    ));
    request->set_header("Host", host);
    request->set_header("Accept", "application/x-protobuf");
    auto response = Http::sync(request);
    auto stat = response->get_status_code();
    if (200 != stat) {
        return false;
    }
    int length;
    response->get_header("Content-Length", length);
    Http::fetch(length, response);
    return tableList.ParseFromArray(response->get_body().data(), response->get_body().size());
}

bool HbaseCpp::Impl::PutRow(const std::string& table, const CellSet& cellSet)
{
    auto request = std::make_shared<Request>(Uri(
        "http://" + host + "/" + table + "/fakerow"
    ));

    auto body = cellSet.SerializeAsString();
    request->set_method("PUT");
    request->set_header("Host", host);
    request->set_header("Content-Type", "application/x-protobuf");
    request->set_header("Content-Length", std::to_string(body.size()));
    request->set_body(body);
    auto response = Http::sync(request);
    auto stat = response->get_status_code();
    return (200 == stat);
}

bool HbaseCpp::Impl::GetRow(const std::string& table, const std::string& row, CellSet& cellSet)
{
    auto request = std::make_shared<Request>(Uri(
        "http://" + host + "/" + table + "/" + row
    ));
    request->set_header("Host", host);
    request->set_header("Accept", "application/x-protobuf");
    auto response = Http::sync(request);
    auto stat = response->get_status_code();
    if (200 != stat) {
        return false;
    }
    int length;
    response->get_header("Content-Length", length);
    Http::fetch(length, response);
    return cellSet.ParseFromArray(response->get_body().data(), response->get_body().size());
}

bool HbaseCpp::Impl::GetRowVersions(const std::string& table, const std::string& row, int versions, CellSet& cellSet)
{
    auto request = std::make_shared<Request>(Uri(
        "http://" + host + "/" + table + "/" + row + "/?v=" + std::to_string(versions)
    ));
    request->set_header("Host", host);
    request->set_header("Accept", "application/x-protobuf");
    auto response = Http::sync(request);
    auto stat = response->get_status_code();
    if (200 != stat) {
        return false;
    }
    int length;
    response->get_header("Content-Length", length);
    Http::fetch(length, response);
    return cellSet.ParseFromArray(response->get_body().data(), response->get_body().size());
}

bool HbaseCpp::Impl::DeleteRow(const std::string& table, const std::string& row)
{
    auto request = std::make_shared<Request>(Uri(
        "http://" + host + "/" + table + "/" + row
    ));
    request->set_method("DELETE");
    request->set_header("Host", host);
    auto response = Http::sync(request);
    auto stat = response->get_status_code();
    return (200 == stat);
}

bool HbaseCpp::Impl::GetScanner(const std::string &table, const Scanner& scanner, std::string &location)
{
    auto request = std::make_shared<Request>(Uri(
        "http://" + host + "/" + table + "/scanner"
    ));
    auto body = scanner.SerializeAsString();
    request->set_method("PUT");
    request->set_header("Host", host);
    request->set_header("Content-Type", "application/x-protobuf");
    request->set_header("Content-Length", std::to_string(body.size()));
    request->set_body(body);
    auto response = Http::sync(request);
    auto stat = response->get_status_code();
    if (201 == stat)
        location = response->get_header("Location");
    return (201 == stat);
}

bool HbaseCpp::Impl::ScanByScanner(const std::string& location, CellSet& cellSet)
{
    auto request = std::make_shared<Request>(Uri(location));
    request->set_header("Host", host);
    request->set_header("Accept", "application/x-protobuf");
    auto response = Http::sync(request);
    auto stat = response->get_status_code();
    if (200 != stat) {
        return false;
    }
    int length;
    response->get_header("Content-Length", length);
    Http::fetch(length, response);
    return cellSet.ParseFromArray(response->get_body().data(), response->get_body().size());
}

bool HbaseCpp::Impl::DeleteScanner(const std::string& location)
{
    auto request = std::make_shared<Request>(Uri(location));
    request->set_method("DELETE");
    request->set_header("Host", host);
    auto response = Http::sync(request);
    auto stat = response->get_status_code();
    return (200 == stat);
}

HbaseCpp::HbaseCpp()
    : pImpl(std::make_shared<Impl>())
{}

HbaseCpp::HbaseCpp(const std::string& host)
    : pImpl(std::make_shared<Impl>(host))
{}

HbaseCpp::~HbaseCpp() = default;

HbaseCpp::HbaseCpp(const HbaseCpp &rhs)
    : pImpl(std::make_shared<Impl>(*rhs.pImpl))
{}

HbaseCpp& HbaseCpp::operator=(const HbaseCpp &rhs)
{
    *pImpl = *rhs.pImpl;
    return *this;
}

void HbaseCpp::set_host(const std::string &host)
{
    pImpl->host = host;
}

bool HbaseCpp::TableExists(const std::string &table)
{
    return pImpl->TableExists(table);
}

bool HbaseCpp::CreateTable(const std::string &table, const TableSchema &tableSchema)
{
    return pImpl->CreateTable(table, tableSchema);
}

bool HbaseCpp::UpdateTable(const std::string &table, const TableSchema &tableSchema)
{
    return pImpl->UpdateTable(table, tableSchema);
}

bool HbaseCpp::DeleteTable(const std::string& table)
{
    return pImpl->DeleteTable(table);
}

bool HbaseCpp::ListTable(TableList &tableList)
{
    return pImpl->ListTable(tableList);
}

bool HbaseCpp::PutRow(const std::string& table, const CellSet& cellSet)
{
    return pImpl->PutRow(table, cellSet);
}

bool HbaseCpp::GetRow(const std::string& table, const std::string& row, CellSet& cellSet)
{
    return pImpl->GetRow(table, row, cellSet);
}

bool HbaseCpp::GetRowVersions(const std::string& table, const std::string& row, int versions, CellSet& cellSet)
{
    return pImpl->GetRowVersions(table, row, versions, cellSet);
}

bool HbaseCpp::DeleteRow(const std::string& table, const std::string& row)
{
    return pImpl->DeleteRow(table, row);
}

bool HbaseCpp::GetScanner(const std::string& table, const Scanner& scanner, std::string& location)
{
    return pImpl->GetScanner(table, scanner, location);
}

bool HbaseCpp::ScanByScanner(const std::string& location, CellSet& cellSet)
{
    return pImpl->ScanByScanner(location, cellSet);
}

bool HbaseCpp::DeleteScanner(const std::string& location)
{
    return pImpl->DeleteScanner(location);
}
