# -*- coding: utf-8 -*-
import scrapy
import json
import requests


class MiSpider(scrapy.Spider):
    """Spider to crawl XiaoMi appstore all applications."""
    name = 'mi'
    start_urls = ['http://app.mi.com/']


    def parse(self, response):
        for sidebar_mod in response.css("div.sidebar div.sidebar-mod"):
            sidebar_str = sidebar_mod.css("h1.sidebar-h::text").extract_first()
            if sidebar_str != "应用分类":
                continue

            # 小米商店API通过抓包分析获得
            # 获取每个分类的应用数量，从而确定请求页数
            for li in sidebar_mod.css("ul.category-list li"):
                categoryId = li.css("a::attr(href)").re(r'/category/(.*)')[0]
                categoryUri = "http://app.mi.com/categotyAllListApi?categoryId=%s" % (categoryId)
                resp = requests.get(url=categoryUri)
                data = json.loads(resp.text)
                appCount = data["count"]
                pageSize = 30
                pageNumber = appCount // pageSize + 1
                # 分别获取每页数据
                for i in range(pageNumber):
                    pageUri = "http://app.mi.com/categotyAllListApi?page=%d&categoryId=%s&pageSize=%d" %(i, categoryId, pageSize)
                    resp = requests.get(url=pageUri)
                    data = json.loads(resp.text)

                    # 获取每个应用的详情
                    for app in data["data"]:
                        appUri = "http://app.mi.com/details?id=%s" %(app["packageName"])
                        yield response.follow(appUri, callback=self.parse_app)


    def parse_app(self, response):
        detail = response.css("div.details.preventDefault")
        cf = detail.css("ul.cf li::text").extract()
        second = detail.css("ul.second-ul li::text").extract()
        yield {
            "app": response.css("div.intro-titles h3::text").extract_first(),
            "compnay": response.css("div.intro-titles p::text").extract_first(),
            "category": response.css("div.intro-titles p.special-font.action::text").extract_first(),
            "size": cf[1],
            "version": cf[3],
            "time": cf[5],
            "permits" : second
        }
