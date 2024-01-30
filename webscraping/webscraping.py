import urllib.request
import re

url = "https://www.amazon.com/s?url=search-alias=aps&field-keywords=3060&rh=p_36%3A"
hdrs = {"User-Agent" : "Mozilla/5.0"}

def getHTML(url):
    request = urllib.request.Request(url, headers=hdrs)

    with urllib.request.urlopen(request) as raw_data:
        html = raw_data.read()
    return html

html = getHTML(url)

gpus = re.findall(r'class="sg-col sg-col-4-of-12 sg-col-4-of-16 sg-col-8-of-20 sg-col-8-of-24"><div class="sg-col-inner">(.*?)sg-col sg-col-4-of-12 sg-col-4-of-16 sg-col-8-of-20 sg-col-8-of-24', str(html), flags=re.DOTALL)

prices = []
for gpu in gpus:
        if "3060" in gpu and " Ti " not in gpu:
            price = re.findall(r'class="a-price-whole">(.*?)<span class="a-price-decimal"', str(gpu))
            if len(price) != 0:
                prices.append(price[0])
print(prices)