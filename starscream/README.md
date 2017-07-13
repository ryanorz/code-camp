starscream : spiders of applications using scrapy.

# Environment

[scrapy](https://doc.scrapy.org/en/latest/intro/tutorial.html)

python 3.5

# FAQ

Q: How to create scrapy project?

A: `scrapy startproject tutorial`

Q: How to create a spider skeleton file?

A: `scrapy genspider example example.com`

Q: How to run a spider?

A: `scrapy crawl spidername`

Q: How to save output to a json file?

A: `scrapy crawl spidername -o output.json` or `scrapy crawl spidername -o output.jl`

Q: 网站数据如果使用了js生成的数据，没法获取下一页信息怎么办?

A: 抓包分析网站API, 否则只能采用其他能执行js的方案，如集成webkit.
